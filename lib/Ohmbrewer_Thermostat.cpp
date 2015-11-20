#include "Ohmbrewer_Thermostat.h"
#include "Ohmbrewer_Screen.h"
#include "Ohmbrewer_Publisher.h"
#include "Ohmbrewer_Onewire.h"


/**
 * Constructor - creating a Thermostat will use up 3 equipment IDs
 * @param id The Sprout ID to use for this piece of Thermostat
 * @param thermPins list with formatting of: [ temp busPin ;  heating controlPin ; heating powerPin ]
 * TODO add UID?
 */
Ohmbrewer::Thermostat::Thermostat(int id, std::list<int>* thermPins) : Ohmbrewer::Equipment(id) {
    initThermostat(id, thermPins);
//    _timer = pidTime;
//    registerUpdateFunction();
}

/**
 * Constructor
 * @param id The Sprout ID to use for this piece of Thermostat
 * @param thermPins list with formatting of: [ temp busPin ;  heating controlPin ; heating powerPin ]
 * @param targetTemp The new target temperature in Celsius
 */
Ohmbrewer::Thermostat::Thermostat(int id, std::list<int>* thermPins, const double targetTemp) : Ohmbrewer::Equipment(id) {
    initThermostat(id, thermPins);
    _targetTemp->fromC(targetTemp);
//    _timer = pidTime;
//    registerUpdateFunction();
}

/**
 * Constructor
 * @param id The Sprout ID to use for this piece of Thermostat
 * @param thermPins list with formatting of: [ temp busPin ;  heating controlPin ; heating powerPin ]
 * @param stopTime The time at which the Thermostat should shut off, assuming it isn't otherwise interrupted
 * @param state Whether the Thermostat is ON (or OFF). True => ON, False => OFF
 * @param currentTask The unique identifier of the task that the Thermostat believes it should be processing
 */
Ohmbrewer::Thermostat::Thermostat(int id, std::list<int>* thermPins, int stopTime,
                                  bool state, String currentTask) : Ohmbrewer::Equipment(id, stopTime, state, currentTask) {
    initThermostat(id, thermPins);
//    _timer = pidTime;
//    registerUpdateFunction();
}

/**
 * Constructor
 * @param id The Sprout ID to use for this piece of Thermostat
 * @param thermPins list with formatting of: [ temp busPin ;  heating controlPin ; heating powerPin ]
 * @param stopTime The time at which the Thermostat should shut off, assuming it isn't otherwise interrupted
 * @param state Whether the Thermostat is ON (or OFF). True => ON, False => OFF
 * @param currentTask The unique identifier of the task that the Thermostat believes it should be processing
 * @param targetTemp The new target temperature in Celsius
 */
Ohmbrewer::Thermostat::Thermostat(int id, std::list<int>* thermPins, int stopTime,
                                  bool state, String currentTask,
                                  const double targetTemp) : Ohmbrewer::Equipment(id, stopTime, state, currentTask) {
    initThermostat(id, thermPins);
    _targetTemp->fromC(targetTemp);
//    _timer = pidTime;
//    registerUpdateFunction();
}

/**
 * Copy Constructor
 * @param clonee The Thermostat object to copy
 */
Ohmbrewer::Thermostat::Thermostat(const Ohmbrewer::Thermostat& clonee) : Ohmbrewer::Equipment(clonee) {
    _heatingElm = clonee.getElement();
    _tempSensor = clonee.getSensor();
    _targetTemp = clonee.getTargetTemp();
//    registerUpdateFunction();
}

/**
 * Destructor
 */
Ohmbrewer::Thermostat::~Thermostat() {
    delete _heatingElm;
    delete _tempSensor;
    delete _targetTemp;
    //delete _timer;
}

/**
 * logic for initializing equipment and PID in the constructors
 * @param id The Sprout ID to use for this piece of Thermostat
 * @param thermPins list with formatting of: [ temp busPin ;  heating controlPin ; heating powerPin ]
 */
void Ohmbrewer::Thermostat::initThermostat(int id, std::list<int>* thermPins){

    // Initialize equipment components
    int size = thermPins->size();
    if ( (size == 2) || (size == 3) ){
        _tempSensor = new TemperatureSensor(id+1, new Onewire());
        thermPins->pop_front();
        _heatingElm = new HeatingElement(id+3, thermPins);
    } else {
        // Publish error
        Ohmbrewer::Publisher::publish_map_t pMap;
        Ohmbrewer::Publisher pub(new String("error_log"), &pMap);
        pMap[String("list_check_thermostat")] = String("improperly formed input - Thermostat(int id, int<list>)");
        pub.publish();

        // Disable and GTFO
        setState(false);
        return;
    }
    _targetTemp = new Temperature(-69);

    // PID set up
    _thermPID = new PID(&input, &output, &setPoint,
                        cons.kP(), cons.kI(), cons.kD(),
                        PID::DIRECT);

    // Initialize the variables we're linked to
    windowStartTime = millis();
    setPoint = _targetTemp->c();
    input = _tempSensor->getTemp()->c();

    // Tell the PID to range between 0 and the full window size
    _thermPID->SetOutputLimits(0, windowSize);

    // Turn the PID on
    _thermPID->SetMode(PID::AUTOMATIC);

    //Timer timer(5000, doPID);
    //_timer = &timer;

}

/**
 * The desired target temperature. Defaults to Celsius
 * @returns The target temperature in Celsius, as a Temperature object pointer
 */
Ohmbrewer::Temperature* Ohmbrewer::Thermostat::getTargetTemp() const {
    return _targetTemp;
}

/**
 * Sets the target temperature
 * @param targetTemp The new target temperature in Celsius
 * @returns The time taken to run the method
 */
const int Ohmbrewer::Thermostat::setTargetTemp(const double targetTemp) {
    unsigned long start = millis();
    _targetTemp->set(targetTemp);
    return start - millis();
}

/**
 * The Thermostat's heating element
 * @returns The heating element
 */
Ohmbrewer::HeatingElement* Ohmbrewer::Thermostat::getElement() const {
    return _heatingElm;
}

/**
 * The Thermostat's temperature sensor
 * @returns The temperature sensor
 */
Ohmbrewer::TemperatureSensor* Ohmbrewer::Thermostat::getSensor() const {
    return _tempSensor;
}

/**
 * Sets the Thermostat's temperature sensor
 * @param sensor -  The temperature sensor
 * @returns The time taken to run the method
 */
const int Ohmbrewer::Thermostat::setSensor(Ohmbrewer::TemperatureSensor* sensor){
    unsigned long start = millis();
    _tempSensor = sensor;
    return start - millis();
}

/**
 * Specifies the interface for arguments sent to this Thermostat's associated function.
 * Parses the supplied string into an array of strings for setting the Thermostat's values.
 * Most likely will be called during update().
 * @param argsStr The arguments supplied as an update to the Rhizome.
 * @param result A map representing the key/value pairs for the update
 */
void Ohmbrewer::Thermostat::parseArgs(const String &argsStr, Ohmbrewer::Equipment::args_map_t &result) {

    if(argsStr.length() > 0) {
        char* params = new char[argsStr.length() + 1];
        strcpy(params, argsStr.c_str());

        // Parse the parameters
        String targetTemp  = String(strtok(params, ","));
        String sensorState = String(strtok(NULL, ","));
        String elmState    = String(strtok(NULL, ","));

        result[String("target_temp")] = targetTemp;

        // Save them to the map
        if(sensorState.length() > 0) {
            result[String("sensor_state")] = sensorState;
        }
        if(sensorState.length() > 0) {
            result[String("element_state")] = elmState;
        }

        // Serial.println("Got these additional Thermostat results: ");
        // Serial.println(targetTemp);
        // Serial.println(sensorState);
        // Serial.println(elmState);

        // Clear out that dynamically allocated buffer
        delete params;
    }

}

/**
 * Sets the Thermostat state. True => On, False => Off
 * @param state Whether the Thermostat is ON (or OFF). True => ON, False => OFF
 * @returns The time taken to run the method
 */
const int Ohmbrewer::Thermostat::setState(const bool state) {
    unsigned long start = millis();
    _state = state;
//    getElement()->setState(state);
//    getSensor()->setState(state);

    return start - millis();
}

/**
 * The Thermostat state. True => On, False => Off
 * @returns True => On, False => Off
 */
bool Ohmbrewer::Thermostat::getState() const {
    return _state;
    //return (getElement()->getState() || getSensor()->getState() );
}

/**
 * True if the Thermostat state is On.
 * @returns Whether the Thermostat is turned ON
 */
bool Ohmbrewer::Thermostat::isOn() const {
    return getState();
}

/**
 * True if the Thermostat state is Off.
 * @returns Whether the Thermostat is turned OFF
 */
bool Ohmbrewer::Thermostat::isOff() const {
    return !getState();
}

/**
 * Performs the Thermostat's current task. Expect to use this during loop().
 * This function is called by work().
 *
 *
 * @returns The time taken to run the method
 */
int Ohmbrewer::Thermostat::doWork() {
    unsigned long start = micros();

    if (getState()){
        //enable timer
//        _timer->start();
        doPID();
    }else{
        //Shut down procedure
        //if thermostat is turned off then turn off element too.
        getElement()->setState(false);
        getElement()->work();//reset element
        //stop Timer
//        _timer->stop();
        //if PID is off then update temp sensor
        getSensor()->work();
    }

    return micros() - start;
}

/**
 * Controls all the inner workings of the PID functionality
 * Should be called by _timer
 *
 * Controls the heating element Relays manually, overriding the standard relay
 * functionality
 *
 * The pid is designed to Output an analog value, but the relay can only be On/Off.
 *
 * "time proportioning control"  it's essentially a really slow version of PWM.
 * first we decide on a window size. Then set the pid to adjust its output between 0 and that window size.
 * lastly, we add some logic that translates the PID output into "Relay On Time" with the remainder of the
 * window being "Relay Off Time"
 *
 * PID Adaptive Tuning
 * You can change the tuning parameters.  this can be
 * helpful if we want the controller to be agressive at some
 * times, and conservative at others.
 *
 */
void Ohmbrewer::Thermostat::doPID(){
    getSensor()->work();

    setPoint = getTargetTemp()->c();        //targetTemp
    input = getSensor()->getTemp()->c();//currentTemp
    double gap = abs(setPoint-input);   //distance away from target temp
    //SET TUNING PARAMETERS
    if (gap<10) {  //we're close to targetTemp, use conservative tuning parameters
        _thermPID->SetTunings(cons.kP(), cons.kI(), cons.kD());
    }else {//we're far from targetTemp, use aggressive tuning parameters
        _thermPID->SetTunings(agg.kP(), agg.kI(), agg.kD());
    }
    //COMPUTATIONS
    _thermPID->Compute();
    if (millis() - windowStartTime>windowSize) { //time to shift the Relay Window
        windowStartTime += windowSize;
    }
    //TURN ON
    if (getState() && gap!=0) {//if we want to turn on the element (thermostat is ON)
        //TURN ON state and powerPin
        if (!(getElement()->getState())) {//if heating element is off
            getElement()->setState(true);//turn it on
            if (getElement()->getPowerPin() != -1) { // if powerPin enabled
                digitalWrite(getElement()->getPowerPin(), HIGH); //turn it on (only once each time you switch state)
            }
        }
        //RELAY MODULATION
        if (output < millis() - windowStartTime) {
            digitalWrite(getElement()->getControlPin(), HIGH);
        } else {
            digitalWrite(getElement()->getControlPin(), LOW);
        }
    }
    //TURN OFF
    if (gap == 0 || getTargetTemp()->c() <= getSensor()->getTemp()->c() ) {//once reached target temp
        getElement()->setState(false); //turn off element
        getElement()->work();
//        if (getElement()->getPowerPin() != -1) { // if powerPin enabled
//            digitalWrite(getElement()->getPowerPin(), LOW); //turn it off too TODO check this
//        }

        // Notify Ohmbrewer that the target temperature has been reached.
        Publisher pub = Publisher(new String(getStream()),
                                  String("msg"),
                                  String("Target Temperature Reached."));
        pub.add(String("last_read_time"),
                String(getSensor()->getLastReadTime()));
        pub.add(String("temperature"),
                String(getSensor()->getTemp()->c()));
        pub.publish();
    }
}

/**
 * Draws information to the Rhizome's display.
 * This function is called by display().
 * @returns The time taken to run the method
 */
int Ohmbrewer::Thermostat::doDisplay(Ohmbrewer::Screen *screen) {
    unsigned long start = micros();

    screen->resetTextSize();
    screen->resetTextColor();

    // Print the section title
    screen->print("== Thermostat #");
    screen->print(getID());
    screen->print("  ==");

    // Add a wee margin
    screen->printMargin(2);

    //print out the therm info
    displayThermTemp(screen);
    screen->printMargin(2);         //if we are hurting for space this can go
    displayRelay(screen);
    // Add another wee margin
    screen->printMargin(2);
//
//    screen->resetTextSize();
    screen->resetTextColor();

    return micros() - start;
}

/**
 * Prints the temperature information for our sensors onto the touchscreen.
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Thermostat::displayThermTemp(Ohmbrewer::Screen *screen) {
    unsigned long start = micros();
    //"Temp °C:  88.0  90.0"
    //         current target
    // If current == target, we'll default to yellow, 'cause we're golden...
    uint16_t color = screen->YELLOW;

    if(getSensor()->getTemp()->c() > getTargetTemp()->c()) {
        // above target temp
        color = screen->RED;
    } else if(getSensor()->getTemp()->c() < getTargetTemp()->c()) {
        // below target temp
        color = screen->CYAN;
    }
    //Label
    screen->setTextColor(screen->WHITE, screen->DEFAULT_BG_COLOR);
    screen->print("Temp ");
    screen->writeDegree();
    screen->print("C:");
    //Temps
    getSensor()->getTemp()->displayTempC(color, screen);
    screen->print(" "); //margin
    getTargetTemp()->displayTempC(screen->YELLOW, screen);

    screen->resetTextColor();

    screen->println("");

    return micros() - start;
}

/**
 * Draws information to the Rhizome's display.
 * This function is called by display().
 * @returns The time taken to run the method
 */
int Ohmbrewer::Thermostat::displayRelay(Ohmbrewer::Screen *screen) {
    unsigned long start = micros();
    char relay_id[2];
    screen->setTextColor(screen->WHITE, screen->DEFAULT_BG_COLOR);

    // Print a fancy identifier
    screen->print("Heat[");

    // Print the state
    if (getElement()->getState()){
        screen->setTextColor(screen->RED, screen->DEFAULT_BG_COLOR);
        screen->print("ON!");
    } else {
        screen->setTextColor(screen->GREEN, screen->DEFAULT_BG_COLOR);
        screen->print("OFF");
    }
    screen->setTextColor(screen->WHITE, screen->DEFAULT_BG_COLOR);
    screen->print("]");

    // TODO add? screen->print(" Cool [");
    screen->println("");

    return micros() - start;
}

/**
 * Publishes updates to Ohmbrewer, etc.
 * This function is called by update().
 * @param args The argument string passed into the Particle Cloud
 * @param argsMap A map representing the key/value pairs for the update
 * @returns The time taken to run the method
 */
int Ohmbrewer::Thermostat::doUpdate(String &args, Ohmbrewer::Equipment::args_map_t &argsMap) {
    unsigned long start = millis();

    // If there are any remaining parameters
    if(args.length() > 0) {
        String targetKey = String("target_temp");
        String sensorKey = String("sensor_state");
        String elmKey = String("element_state");

        parseArgs(args, argsMap);

        // If there are any arguments, the will be a new Target Temperature value
        setTargetTemp(argsMap[targetKey].toFloat());

        // The remaining settings are optional/convenience parameters
        if(argsMap.count(sensorKey) != 0) {
            if(argsMap[sensorKey].equalsIgnoreCase("ON")) {
                getSensor()->setState(true);
            } else if(argsMap[sensorKey].equalsIgnoreCase("OFF")) {
                getSensor()->setState(false);
            } else if(argsMap[sensorKey].equalsIgnoreCase("--")) {
                // Do nothing. Intentional.
            } else {
                // Do nothing. TODO: Should probably raise an error code...
            }
        }

        if(argsMap.count(elmKey) != 0) {
            if(argsMap[elmKey].equalsIgnoreCase("ON")) {
                getElement()->setState(true);
            } else if(argsMap[elmKey].equalsIgnoreCase("OFF")) {
                getElement()->setState(false);
            } else if(argsMap[elmKey].equalsIgnoreCase("--")) {
                // Do nothing. Intentional.
            } else {
                // Do nothing. TODO: Should probably raise an error code...
            }
        }

    }


    return millis() - start;
}

/**
 * Reports which of the Rhizome's pins are occupied by the
 * Equipment, forming a logical Sprout.
 * @param pins The list of physical pins that the Thermostat is connected to.
 */
void Ohmbrewer::Thermostat::whichPins(std::list<int>* pins) {

    pins->push_back(_tempSensor->getBusPin());
    _heatingElm->whichPins(pins);

}

