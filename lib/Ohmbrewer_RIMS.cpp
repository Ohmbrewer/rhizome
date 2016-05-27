#include "Ohmbrewer_RIMS.h"
#include "Ohmbrewer_Screen.h"
#include "Ohmbrewer_Publisher.h"
#include "Ohmbrewer_Onewire.h"
#include "Ohmbrewer_Temperature.h"


/**
 * Constructor - creating a RIMS will use 7 equipment IDs
 * @param thermPins list with formatting of: [ temp busPin ;  heating controlPin ; heating powerPin ]
 * @param pumpPin - Single speed pump will only have PowerPin
 * @param safetyIndex - onewire index of the probe attached for safetySensor (RIMS tube)
 * @param elementPins - controlPin always first in <list>
 */
Ohmbrewer::RIMS::RIMS(std::list<int>* thermPins, int pumpPin, int safetyIndex ) {
    initRIMS(thermPins, pumpPin, safetyIndex);
//    registerUpdateFunction();
}

/**
 * Constructor
 * @param thermPins list with formatting of: [ temp busPin ;  heating controlPin ; heating powerPin ]
 * @param pumpPin - Single speed pump will only have PowerPin
 * @param safetyIndex - onewire index of the probe attached for safetySensor (RIMS tube)
 * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
 * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
 * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
 */
Ohmbrewer::RIMS::RIMS(std::list<int>* thermPins, int pumpPin, int safetyIndex, int stopTime,
                      bool state, String currentTask) : Ohmbrewer::Equipment(stopTime, state, currentTask) {
    initRIMS(thermPins, pumpPin, safetyIndex);
//    registerUpdateFunction();
}

/**
 * Constructor
 * @param thermPins list with formatting of: [ temp busPin ;  heating controlPin ; heating powerPin ]
 * @param pumpPin - Single speed pump will only have PowerPin
 * @param safetyIndex - onewire index of the probe attached for safetySensor (RIMS tube)
 * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
 * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
 * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
 * @param targetTemp The new target temperature in Celsius
 */
Ohmbrewer::RIMS::RIMS(std::list<int>* thermPins, int pumpPin, int safetyIndex, int stopTime,
                      bool state, String currentTask, const double targetTemp) : Ohmbrewer::Equipment(stopTime, state, currentTask) {
    initRIMS(thermPins, pumpPin, safetyIndex);
    getTube()->setTargetTemp(targetTemp);
//    registerUpdateFunction();
}

/**
 * Copy constructor
 * FIXME: Copy constructors should probably reset the pins and ID's, no? Depends why we are copying it
 * @param clonee The Equipment object to copy
 */
Ohmbrewer::RIMS::RIMS(const Ohmbrewer::RIMS& clonee) : Ohmbrewer::Equipment(clonee) {
    _tube = clonee.getTube();
    _recirc = clonee.getRecirculator();

//    registerUpdateFunction();
}

/**
 * Destructor
 */
Ohmbrewer::RIMS::~RIMS() {
    delete _recirc;
    delete _tube;
    delete _safetySensor;
    delete _safetyTemp;
}

/**
 * Initializes the members of the RIMS class
 * @param thermPins list with formatting of: [ temp busPin ; onewire index ; heating controlPin ; heating powerPin ]
 * @param pumpPin - Single speed pump will only have PowerPin
 * @param safetyIndex - onewire index of the probe attached for safetySensor (RIMS tube)
 */
void Ohmbrewer::RIMS::initRIMS(std::list<int>* thermPins, int pumpPin, int safetyIndex){
    int size = thermPins->size();
    if ( (size == 4) ){

        //busPin unused for now
        _safetySensor = new TemperatureSensor(new Onewire(safetyIndex));
        _tube = new Thermostat(thermPins);
        //init therm timer?
        //set therm timer?
    }else{//incorrect number of pins supplied
        //publish error
        Ohmbrewer::Publisher::publish_map_t pMap;
        Ohmbrewer::Publisher* pub = new Ohmbrewer::Publisher(new String("error_log"), &pMap);
        pMap[String("list_check_rims")] = String("improperly formed array - RIMS(int, int<list>, int");
        pub->publish();
        delete pub;
    }
    _recirc = new Pump(pumpPin);
    _safetyTemp = new Temperature();
}

/**
 * The Equipment ID
 * @returns The Sprout ID to use for this piece of Equipment
 */
int Ohmbrewer::RIMS::getID() const {
    return _tube->getID();
}

/**
 * The Tube thermostat
 * @returns The Thermostat object representing the RIMS tube elements
 */
Ohmbrewer::Thermostat* Ohmbrewer::RIMS::getTube() const {
    return _tube;
}

/**
 * The temperature sensor located in the tun
 * @returns The Temperature Sensor object representing the sensor located in the mash tun
 */
Ohmbrewer::TemperatureSensor* Ohmbrewer::RIMS::getTunSensor() const {
    return _tube->getSensor();
}

/**
 * The desired safety temperature. Defaults to Celsius
 * @returns The safety temperature in Celsius
 */
Ohmbrewer::Temperature* Ohmbrewer::RIMS::getSafetyTemp() const{
    return _safetyTemp;
}

/**
 * Sets the safety temperature
 * @param safetyTemp The new safety temperature in Celsius
 * @returns The time taken to run the method
 */
const int Ohmbrewer::RIMS::setSafetyTemp(const double safetyTemp){
    unsigned long start = millis();
    _safetyTemp->set(safetyTemp);
    return start - millis();
}
/**
 * The Thermostat's safety temperature sensor
 * @returns The temperature sensor
 */
Ohmbrewer::TemperatureSensor* Ohmbrewer::RIMS::getSafetySensor() const{
    return _safetySensor;
}

/**
 * Sets the Thermostat's temperature sensor
 * @param sensor -  The temperature sensor
 * @returns The time taken to run the method
 */
const int Ohmbrewer::RIMS::setSafetySensor(Ohmbrewer::TemperatureSensor* sensor){
    unsigned long start = millis();
    _safetySensor = sensor;
    return start - millis();
}

/**
 * The recirculation pump between the tun and the tube
 * @returns The Pump object representing the recirculation pump
 */
Ohmbrewer::Pump* Ohmbrewer::RIMS::getRecirculator() const {
    return _recirc;
}

/**
 * Specifies the interface for arguments sent to this Equipment's associated function.
 * Parses the supplied string into an array of strings for setting the Equipment's values.
 * Most likely will be called during update().
 * @param argsStr The arguments supplied as an update to the Rhizome.
 * @param result A map reprsenting the key/value pairs for the update
 */
void Ohmbrewer::RIMS::parseArgs(const String &argsStr, Ohmbrewer::Equipment::args_map_t &result) {

    if(argsStr.length() > 0) {
        // FIXME These should probably be converted into private const variables
        String safetySensorKey = String("safety_sensor_state");
        String rPumpKey = String("r_pump_state");
        String thermKey = String("therm_params");
        char* params = new char[argsStr.length() + 1];
        strcpy(params, argsStr.c_str());

        // Parse the parameters
        String tunSensorState = String(strtok(params, ","));
        String rPumpState     = String(strtok(NULL, ","));

        // Save them to the map
        if(tunSensorState.length() > 0) {
            result[safetySensorKey] = tunSensorState;
        }
        if(rPumpState.length() > 0) {
            result[rPumpKey] = rPumpState;
        }

        result[thermKey] = argsStr.substring(tunSensorState.length() + rPumpState.length() + 2);

        // Serial.println("Got these additional RIMS results: ");
        // Serial.println(tunSensorState);
        // Serial.println(rPumpState);
        // Serial.println(result[thermKey]);

        // Clear out that dynamically allocated buffer
        delete params;
    }

}

/**
 * Sets the Equipment state. True => On, False => Off
 * This turns *EVERYTHING* on, so watch out. You may want to turn the components on individually instead.
 * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
 * @returns The time taken to run the method
 */
const int Ohmbrewer::RIMS::setState(const bool state) {
    unsigned long start = millis();
    _state = state;
    getTube()->setState(state);
    getTunSensor()->setState(state);
    getSafetySensor()->setState(state);
    getRecirculator()->setState(state);

    return start - millis();
}

/**
 * The Equipment state. True => On, False => Off
 * @returns True => On, False => Off
 */
bool Ohmbrewer::RIMS::getState() const {
    return _state;
//    return ( getRecirculator()->getState() || getTunSensor()->getState() || getTube()->getState()
//             || getSafetySensor()->getState() );
}

/**
 * True if the Equipment state is On.
 * @returns Whether the Equipment is turned ON
 */
bool Ohmbrewer::RIMS::isOn() const {
    return getState();
}

/**
 * True if the Equipment state is Off.
 * @returns Whether the Equipment is turned OFF
 */
bool Ohmbrewer::RIMS::isOff() const {
    return !getState();
}

/**
 * Performs the Equipment's current task. Expect to use this during loop().
 * This function is called by work().
 * @returns The time taken to run the method
 */
int Ohmbrewer::RIMS::doWork() {
    unsigned long start = micros();

    //FANCY RIMS, turns the pump off to rest when tun temp is good and safety temp is good.
    if (getState()) {//IF RIMS ON

        // make sure R. PUMP is ON if tube temp > tun temp +3(margin)
        if (getSafetySensor()->getTemp()->c() > (getTunSensor()->getTemp()->c() + 3) &&
                !(getRecirculator()->getState()) ){
            getRecirculator()->setState(true); // turn on pump
        }else if ( getSafetySensor()->getTemp()->c() <= (getTunSensor()->getTemp()->c() + 3) &&
                getRecirculator()->getState() ){
            getRecirculator()->setState(false); // turn off pump
        }

        // safetySensor guard on Therm (if: tube temp > safety setting, then: NO heat )
        if ( getSafetyTemp()->c() > getSafetySensor()->getTemp()->c() &&
                !getTube()->getState() ) { //safety temp > tube temp(safe) and therm==OFF
            //tube temperature < safetyTemp
            getTube()->setState(true); // turn on therm

            // Notify Ohmbrewer that the Thermostat has been turned on.
            Publisher pub = Publisher(new String(getStream()),
                                      String("thermostat"),
                                      String("ON"));
            pub.add(String("last_read_time"),
                    String(getTube()->getSensor()->getLastReadTime()));
            pub.add(String("temperature"),
                    String(getTube()->getSensor()->getTemp()->c()));
            pub.publish();

        }else if ( getSafetyTemp()->c() <= getSafetySensor()->getTemp()->c() &&
                getTube()->getState() ) {// tube temp >= safety temp and therm == ON
            //shut therm off
            getTube()->setState(false);//PID should be able to handle this.
            //TODO  with timers simply stop the timer

            // Notify Ohmbrewer that the Thermostat has been turned off.
            Publisher pub = Publisher(new String(getStream()),
                                      String("thermostat"),
                                      String("OFF"));
            pub.add(String("last_read_time"),
                    String(getTube()->getSensor()->getLastReadTime()));
            pub.add(String("temperature"),
                    String(getTube()->getSensor()->getTemp()->c()));
            pub.publish();
        }
    }else{//IF RIMS OFF

        // make sure R. PUMP is OFF
        getRecirculator()->setState(false);
        // turn OFF therm
        getTube()->setState(false);
    }
        
    getSafetySensor()->work();
    getRecirculator()->work();
    getTube()->work();
    return micros() - start;
}

/**
 * Draws information to the Rhizome's display.
 * This function is called by display().
 * @param screen The Rhizome's touchscreen
 * @returns The time taken to run the method
 */
int Ohmbrewer::RIMS::doDisplay(Ohmbrewer::Screen *screen) {
    unsigned long start = micros();

    screen->resetTextSize();
    screen->resetTextColor();

    // Print the section title
    screen->print("===== RIMS #");
    screen->print(getID());
    screen->print("  =====");

    // Add a wee margin
    screen->printMargin(2);

    // Print out the temperature from the Tube
    displayTunTemp(screen);

    displaySafetyTemp(screen);

    screen->printMargin(2);
    // Print the HE and pump status
    displayHeatElmStatus(screen);
    screen->print(" ");
    displayRecircStatus(screen);
    screen->printMargin(2);

    return micros() - start;
}

/**
 * Prints the temperature information for our sensors onto the touchscreen.
 * @param screen The Rhizome's touchscreen
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::RIMS::displayTunTemp(Ohmbrewer::Screen *screen) {
    unsigned long start = micros();
    //target, we'll default to yellow,
    //Tun °C:   70.0  68.0
    //       current target


    uint16_t color = screen->YELLOW;

    if(getTunSensor()->getTemp()->c() > getTube()->getTargetTemp()->c()) {
        // Too hot
        color = screen->RED;
    } else if(getTunSensor()->getTemp()->c() < getTube()->getTargetTemp()->c()) {
        // Too cold
        color = screen->CYAN;
    }
    screen->setTextColor(screen->WHITE, screen->DEFAULT_BG_COLOR);
    screen->print("Tun ");
    screen->writeDegree();
    screen->print("C: ");
    //current temp
    getTunSensor()->getTemp()->displayTempC(color, screen);

    screen->print(" ");
    //print out target temp in yellow
    getTube()->getTargetTemp()->displayTempC(screen->YELLOW, screen);
    screen->println("");

    screen->resetTextColor();

    return micros() - start;
}

/**
 * Prints the temperature information for our safety sensor onto the touchscreen.
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::RIMS::displaySafetyTemp(Ohmbrewer::Screen *screen) {
    unsigned long start = micros();
    //  target, we'll default to yellow
    // Tube °C:  88.0  90.0
    uint16_t color = screen->YELLOW;

    if(getSafetySensor()->getTemp()->c() >= getSafetyTemp()->c()) {
        // Too hot
        color = screen->RED;
    } else if(getSafetySensor()->getTemp()->c() < getSafetyTemp()->c()) {
        // Too cold ... ie ok
        color = screen->CYAN;
    }
    screen->setTextColor(screen->WHITE, screen->DEFAULT_BG_COLOR);
    screen->print("Tube ");
    screen->writeDegree();
    screen->print("C:");
    //current temp
    getSafetySensor()->getTemp()->displayTempC(color, screen);

    screen->print(" ");
    //print out safety target temp in yellow
    getSafetyTemp()->displayTempC(screen->YELLOW, screen);
    screen->println("");

    screen->resetTextColor();

    return micros() - start;
}

/**
 * Prints the recirculation pump status onto the touchscreen.
 * @param screen The Rhizome's touchscreen
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::RIMS::displayHeatElmStatus(Ohmbrewer::Screen *screen) {
    unsigned long start = micros();

    // Print the label
    screen->setTextColor(screen->WHITE, screen->DEFAULT_BG_COLOR);
    screen->print("Heat["); // We want a little margin

    // Print the state
    if (getTube()->getElement()->getState()){
        screen->setTextColor(screen->RED, screen->DEFAULT_BG_COLOR);
        screen->print("ON!");
    } else {
        screen->setTextColor(screen->GREEN, screen->DEFAULT_BG_COLOR);
        screen->print("OFF");
    }

    screen->setTextColor(screen->WHITE, screen->DEFAULT_BG_COLOR);
    screen->print("]");

    return micros() - start;
}

/**
 * Prints the recirculation pump status onto the touchscreen.
 * @param screen The Rhizome's touchscreen
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::RIMS::displayRecircStatus(Ohmbrewer::Screen *screen) {
    unsigned long start = micros();

    // Print the label
    screen->setTextColor(screen->WHITE, screen->DEFAULT_BG_COLOR);
    screen->print("Pump["); // We want a little margin

    // Print the state
    if (getRecirculator()->getState()){
        screen->setTextColor(screen->RED, screen->DEFAULT_BG_COLOR);
        screen->print("ON!");
    } else {
        screen->setTextColor(screen->GREEN, screen->DEFAULT_BG_COLOR);
        screen->print("OFF");
    }
    screen->setTextColor(screen->WHITE, screen->DEFAULT_BG_COLOR);
    screen->print("]");

    return micros() - start;
}

/**
 * Publishes updates to Ohmbrewer, etc.
 * This function is called by update().
 * @param args The argument string passed into the Particle Cloud
 * @param argsMap A map representing the key/value pairs for the update
 * @returns The time taken to run the method
 */
int Ohmbrewer::RIMS::doUpdate(String &args, Ohmbrewer::Equipment::args_map_t &argsMap) {
    unsigned long start = millis();

    // If there are any remaining parameters
    if(args.length() > 0) {
        String safetySensorKey = String("safety_sensor_state");
        String rPumpKey = String("r_pump_state");
        String thermKey = String("therm_params");

        parseArgs(args, argsMap);

        // The remaining settings are optional/convenience parameters
        if(argsMap.count(safetySensorKey) != 0) {
            if(argsMap[safetySensorKey].equalsIgnoreCase("ON")) {
                getSafetySensor()->setState(true);
            } else if(argsMap[safetySensorKey].equalsIgnoreCase("OFF")) {
                getSafetySensor()->setState(false);
            } else if(argsMap[safetySensorKey].equalsIgnoreCase("--")) {
                // Do nothing. Intentional.
            } else {
                // Do nothing. TODO: Should probably raise an error code...
            }
        }

        if(argsMap.count(rPumpKey) != 0) {
            if(argsMap[rPumpKey].equalsIgnoreCase("ON")) {
                getRecirculator()->setState(true);
            } else if(argsMap[rPumpKey].equalsIgnoreCase("OFF")) {
                getRecirculator()->setState(false);
            } else if(argsMap[rPumpKey].equalsIgnoreCase("--")) {
                // Do nothing. Intentional.
            } else {
                // Do nothing. TODO: Should probably raise an error code...
            }
        }


        if(argsMap.count(thermKey) != 0) {
            // Pass the remaining parameters down to the Tube (a Thermostat)
            getTube()->doUpdate(argsMap[thermKey], argsMap);
        }

    }


    return millis() - start;
}

/**
 * Reports which of the Rhizome's pins are occupied by the
 * Equipment, forming a logical Sprout.
 * @param pins The list of physical pins that the Equipment is connected to.
 */
void Ohmbrewer::RIMS::whichPins(std::list<int>* pins){

    pins->push_back(getSafetySensor()->getBusPin());
    _tube->whichPins(pins);
    _recirc->whichPins(pins);

}

