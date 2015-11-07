#include "Ohmbrewer_RIMS.h"
#include "Ohmbrewer_Screen.h"
#include "Ohmbrewer_Publisher.h"
#include "Ohmbrewer_Onewire.h"


/**
 * Constructor - creating a RIMS will use 7 equipment IDs
 * @param id The Sprout ID to use for this piece of Equipment
 * @param thermPins list with formatting of: [ temp busPin ;  heating controlPin ; heating powerPin ]
 * @param pumpPin - Single speed pump will only have PowerPin
 * @param elementPins - controlPin always first in <list>
 * TODO add UID?
 */
Ohmbrewer::RIMS::RIMS(int id, std::list<int>* thermPins, int pumpPin ) : Ohmbrewer::Equipment(id) {
    //TODO currently only one probe at a time working.
    initRIMS(id, thermPins, pumpPin);
    registerUpdateFunction();
}

/**
 * Constructor
 * @param id The Sprout ID to use for this piece of Equipment
 * @param thermPins list with formatting of: [ temp busPin ;  heating controlPin ; heating powerPin ]
 * @param pumpPin - Single speed pump will only have PowerPin
 * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
 * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
 * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
 */
Ohmbrewer::RIMS::RIMS(int id, std::list<int>* thermPins, int pumpPin, int stopTime,
                      bool state, String currentTask) : Ohmbrewer::Equipment(id, stopTime, state, currentTask) {
    initRIMS(id, thermPins, pumpPin);
    registerUpdateFunction();
}

/**
 * Constructor
 * @param id The Sprout ID to use for this piece of Equipment
 * @param thermPins list with formatting of: [ temp busPin ;  heating controlPin ; heating powerPin ]
 * @param pumpPin - Single speed pump will only have PowerPin
 * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
 * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
 * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
 * @param targetTemp The new target temperature in Celsius
 */
Ohmbrewer::RIMS::RIMS(int id, std::list<int>* thermPins, int pumpPin, int stopTime,
                      bool state, String currentTask, const double targetTemp) : Ohmbrewer::Equipment(id, stopTime, state, currentTask) {
    initRIMS(id, thermPins, pumpPin);
    getTube()->setTargetTemp(targetTemp);
    registerUpdateFunction();
}

/**
 * Copy constructor
 * FIXME: Copy constructors should probably reset the pins and ID's, no? Depends why we are copying it
 * @param clonee The Equipment object to copy
 */
Ohmbrewer::RIMS::RIMS(const Ohmbrewer::RIMS& clonee) : Ohmbrewer::Equipment(clonee) {
    _tube = clonee.getTube();
    _recirc = clonee.getRecirculator();

    registerUpdateFunction();
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
 * @param id The Sprout ID to use for this piece of Equipment
 * @param thermPins list with formatting of: [ temp busPin ;  heating controlPin ; heating powerPin ]
 * @param pumpPin - Single speed pump will only have PowerPin
 */
void Ohmbrewer::RIMS::initRIMS(int id, std::list<int>* thermPins, int pumpPin){
    int size = thermPins->size();
    if ( (size == 2) || (size == 3) ){
        _safetySensor = new TemperatureSensor(id+2, new Onewire());  //TODO need UIDS
        _tube = new Thermostat(id+3, thermPins);
    }else{
        //publish error
        Ohmbrewer::Publisher::publish_map_t pMap;
        Ohmbrewer::Publisher* pub = new Ohmbrewer::Publisher(new String("error_log"), &pMap);
        pMap[String("list_check_rims")] = String("improperly formed array - RIMS(int, int<list>, int");
        pub->publish();
        delete pub;
    }
    _recirc = new Pump(id+2, pumpPin);
    _safetyTemp = new Temperature(-69);
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
        String tunSensorKey = String("tun_sensor_state");
        String rPumpKey = String("r_pump_state");
        String tubeKey = String("tube_params");
        char* params = new char[argsStr.length() + 1];
        strcpy(params, argsStr.c_str());

        // Parse the parameters
        String tunSensorState = String(strtok(params, ","));
        String rPumpState     = String(strtok(NULL, ","));

        // Save them to the map
        if(tunSensorState.length() > 0) {
            result[tunSensorKey] = tunSensorState;
        }
        if(rPumpState.length() > 0) {
            result[rPumpKey] = rPumpState;
        }

        result[tubeKey] = argsStr.substring(tunSensorState.length() + rPumpState.length() + 2);

        // Serial.println("Got these additional RIMS results: ");
        // Serial.println(tunSensorState);
        // Serial.println(rPumpState);
        // Serial.println(result[tubeKey]);

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
    return ( getRecirculator()->getState() || getTunSensor()->getState() || getTube()->getState()
             || getSafetySensor()->getState() );
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
    // TODO: Implement RIMS::doWork
    // enable thermotat?
    //disable is safetysensor is on alert
    //call them dowork()
    //futz with pump a bit too.
    return -1;
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

    // Print out the current temp from the Tun
    displayTunTemp(screen);

    screen->printMargin(2);
    screen->print("------  Tube  ------");
    screen->printMargin(2);

    // Print out the temperature from the Tube
    displaySafetyTemp(screen);

    // Print out the target temp
    getTube()->displayTargetTemp(screen);

    // Print the pump status
    displayRecircStatus(screen);

    return micros() - start;
}

/**
 * Prints the temperature information for our sensors onto the touchscreen.
 * @param screen The Rhizome's touchscreen
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::RIMS::displayTunTemp(Ohmbrewer::Screen *screen) {
    unsigned long start = micros();
    // If current == target, we'll default to yellow, 'cause we're golden...
    uint16_t color = screen->YELLOW;

    if(getTunSensor()->getTemp()->c() > getTube()->getTargetTemp()->c()) {
        // Too hot
        color = screen->RED;
    } else if(getTunSensor()->getTemp()->c() < getTube()->getTargetTemp()->c()) {
        // Too cold
        color = screen->CYAN;
    }

    getTube()->displayTemp(getTunSensor()->getTemp(), "Tun", color, screen);

    screen->resetTextColor();
    screen->println("");

    return micros() - start;
}

/**
 * Prints the temperature information for our safety sensor onto the touchscreen.
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::RIMS::displaySafetyTemp(Ohmbrewer::Screen *screen) {
    unsigned long start = micros();
    // If current == target, we'll default to yellow, 'cause we're golden...
    uint16_t color = screen->YELLOW;

    if(getSafetySensor()->getTemp()->c() >= getSafetyTemp()->c()) {
        // Too hot
        color = screen->RED;
    } else if(getSafetySensor()->getTemp()->c() < getSafetyTemp()->c()) {
        // Too cold ... ie ok
        color = screen->CYAN;
    }

    getTube()->displayTemp(getSafetySensor()->getTemp(), "Tube", color, screen);

    screen->resetTextColor();
    screen->println("");

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
    screen->resetTextColor();
    screen->print(" R. Pump: "); // We want a little margin

    // Print the state
    if (getRecirculator()->getState()){
        screen->setTextColor(screen->YELLOW, screen->DEFAULT_BG_COLOR);
        screen->print("ON ");
    } else {
        screen->setTextColor(screen->RED, screen->DEFAULT_BG_COLOR);
        screen->print("OFF");
    }

    screen->printMargin(2);
    screen->resetTextColor();

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
        String tunSensorKey = String("tun_sensor_state");
        String rPumpKey = String("r_pump_state");
        String tubeKey = String("tube_params");

        parseArgs(args, argsMap);

        // The remaining settings are optional/convenience parameters
        if(argsMap.count(tunSensorKey) != 0) {
            if(argsMap[tunSensorKey].equalsIgnoreCase("ON")) {
                getTunSensor()->setState(true);
            } else if(argsMap[tunSensorKey].equalsIgnoreCase("OFF")) {
                getTunSensor()->setState(false);
            } else if(argsMap[tunSensorKey].equalsIgnoreCase("--")) {
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


        if(argsMap.count(tubeKey) != 0) {
            // Pass the remaining parameters down to the Tube (a Thermostat)
            getTube()->doUpdate(argsMap[tubeKey], argsMap);
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

