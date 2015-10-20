
#include "Ohmbrewer_Still.h"

#include "Ohmbrewer_Screen.h"
#include "Ohmbrewer_Publisher.h"


/**
 * Constructor - standard still
 * @param id The Sprout ID to use for this piece of Equipment
 * @param elementPins[ heating powerPin ; heating controlPin ]
 * @param tempBus - onewire temperature pin
 */
Ohmbrewer::Still::Still(int id, int (&elementPins)[2], int tempBus ) : Ohmbrewer::Equipment(id) {


    //Thermostat
    //Temp therm constructor with no array checking
    int thermPins[] = {0, 2, 3};
//    memcpy(&thermPins+sizeof(int), elementPins, 2*sizeof(int));
//    _therm = new Thermostat(id+4, thermPins);

    _therm = new Thermostat(id+4, thermPins );
    /*
    int n = sizeof(elementPins) / sizeof(int);
    if (elementPins[1]==NULL){      //publish error
        Ohmbrewer::Publisher::publish_map_t pMap;
        Ohmbrewer::Publisher* pub = new Ohmbrewer::Publisher(new String("error_log"), &pMap);
        pMap[String("array_check_still")] = String("improperly formed array - Still(int, int[], int)");
        pub->publish();
    }
    if (elementPins[1]==NULL){      //no switch leg on pump
        _therm = new Thermostat(id+4, {tempBus, elementPins[0], NULL} );
    }else{                          //switch and SSR
        _therm = new Thermostat(id+4, {tempBus, elementPins[0], elementPins[1]} );
    }*/
    //Temperature Sensors (DS18b20)
    _kettleSensor = new TemperatureSensor(id+1, tempBus);
    _columnSensor = new TemperatureSensor(id+2, tempBus);
    _coolantSensor = new TemperatureSensor(id+3, tempBus); //optional

    registerUpdateFunction();
}

/**TODO additional constuctors need work USE CONSTRUCTOR 1
 * Constructor
 * @param id The Sprout ID to use for this piece of Equipment
 * @param thermPins[ temp busPin ; heating powerPin ; heating controlPin ]
 * @param tempBus - mash tun temperature pin
 * @param pumpPins[powerPin ; controlPin ]
 */
Ohmbrewer::Still::Still(int id, int (&thermPins)[3], int tempBus, int (&pumpPins)[2] ) : Ohmbrewer::Equipment(id) {
//    //Thermostat
//    _therm = new Thermostat(id+3, thermPins);
//    //Temperature Sensors (DS18b20)
//    _kettleSensor = new TemperatureSensor(id+1, tempBus);
//    _columnSensor = new TemperatureSensor(id+1, tempBus);
//    _coolantSensor = new TemperatureSensor(id+1, tempBus);
//    //Pump
//    int n = sizeof(pumpPins) / sizeof(int);
//    if ( n > 1){
//        _coolantPump = new Pump(id+4, pumpPins[0], pumpPins[1]);
//    }else if (n == 1){
//        _coolantPump = new Pump(id+4, pumpPins[0], -1);//Single speed pump
//    }else{
//        //publish error
//        Ohmbrewer::Publisher::publish_map_t pMap;
//        Ohmbrewer::Publisher* pub = new Ohmbrewer::Publisher(new String("error_log"), &pMap);
//
//
//        pMap[String("array_check_rims")] = String("improperly formed array - RIMS(int, int[], int , int[])");
//        pub->publish();
//    }
//    registerUpdateFunction();
}

/**FIXME
 * Constructor
 * @param id The Sprout ID to use for this piece of Equipment
 * @param thermPins[ temp busPin ; heating powerPin ; heating controlPin ]
 * @param tempBus - mash tun temperature pin
 * @param pumpPins[powerPin ; controlPin ]
 * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
 * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
 * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
 */
Ohmbrewer::Still::Still(int id, int (&thermPins)[3], int tempBus, int (&pumpPins)[2], int stopTime,
                      bool state, String currentTask) : Ohmbrewer::Equipment(id, stopTime, state, currentTask) {
//    _therm = new Thermostat(id+2, thermPins);
//    _kettleSensor = new TemperatureSensor(id+1, tempBus);
//    int n = sizeof(pumpPins) / sizeof(int);
//    Ohmbrewer::Publisher::publish_map_t pMap;
//    Ohmbrewer::Publisher* pub = new Ohmbrewer::Publisher(new String("error_log"), &pMap);
//    if ( n > 1){
//        _coolantPump = new Pump(1, pumpPins[0], pumpPins[1]);
//    }else if (n == 1){
//        _coolantPump = new Pump(1, pumpPins[0], -1);//Single speed pump
//    }else{
//        //publish error
//        Ohmbrewer::Publisher::publish_map_t pMap;
//        Ohmbrewer::Publisher* pub = new Ohmbrewer::Publisher(new String("error_log"), &pMap);
//
//        pMap[String("array_check_rims")] = String("improperly formed array - RIMS(int, int[], int, int[], int, bool, String)");
//        pub->publish();
//    }
//
//    registerUpdateFunction();
}

/**FIXME
 * Constructor
 * @param id The Sprout ID to use for this piece of Equipment
 * @param thermPins[ temp busPin ; heating powerPin ; heating controlPin ]
 * @param tempBus - mash tun temperature pin
 * @param pumpPins[powerPin ; controlPin ]
 * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
 * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
 * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
 * @param targetTemp The new target temperature in Celsius
 */
Ohmbrewer::Still::Still(int id, int (&thermPins)[3], int tempBus, int (&pumpPins)[2], int stopTime,
                      bool state, String currentTask, const double targetTemp) : Ohmbrewer::Equipment(id, stopTime, state, currentTask) {
//
//    _therm = new Thermostat(id+2, thermPins, targetTemp);
//    _kettleSensor = new TemperatureSensor(id+1, tempBus);
//    int n = sizeof(pumpPins) / sizeof(int);
//    Ohmbrewer::Publisher::publish_map_t pMap;
//    Ohmbrewer::Publisher* pub = new Ohmbrewer::Publisher(new String("error_log"), &pMap);
//    if ( n > 1){
//        _coolantPump = new Pump(1, pumpPins[0], pumpPins[1]);
//    }else if (n == 1){
//        _coolantPump = new Pump(1, pumpPins[0], -1);//Single speed pump
//    }else{
//        //publish error
//        Ohmbrewer::Publisher::publish_map_t pMap;
//        Ohmbrewer::Publisher* pub = new Ohmbrewer::Publisher(new String("error_log"), &pMap);
//
//        pMap[String("array_check_rims")] = String("improperly formed array - RIMS(int , int[], int, int[], int, bool, String, double)");
//        pub->publish();
//    }
//    registerUpdateFunction();
}

/**
 * Copy constructor
 * FIXME: Copy constructors should probably reset the pins and ID's, no? Depends why we are copying it
 * @param clonee The Equipment object to copy
 */
Ohmbrewer::Still::Still(const Ohmbrewer::Still& clonee) : Ohmbrewer::Equipment(clonee) {
    _therm = clonee.getTherm();
    _kettleSensor = clonee.getKettleSensor();
    _columnSensor = clonee.getColumnSensor();
    _coolantSensor = clonee.getCoolantSensor();
    _coolantPump = clonee.getCoolantPump();

    registerUpdateFunction();
}

/**
 * Destructor
 */
Ohmbrewer::Still::~Still() {
    delete _coolantPump;
    delete _therm;
    delete _kettleSensor;
    delete _columnSensor;
    delete _coolantSensor;
}

/**
 * Overloaded << operator.
 */
// friend std::ostream& operator<<( std::ostream& os, RIMS const& rims);

/**
 * The Tube thermostat
 * @returns The Thermostat object representing the RIMS tube elements
 */
Ohmbrewer::Thermostat* Ohmbrewer::Still::getTherm() const {
    return _therm;
}

/**
 * The temperature sensor located in the tun
 * @returns The Temperature Sensor object representing the sensor located in the mash tun
 */
Ohmbrewer::TemperatureSensor* Ohmbrewer::Still::getKettleSensor() const {
    return _kettleSensor;
}

/**
 * The temperature sensor located in the tun
 * @returns The Temperature Sensor object representing the sensor located in the mash tun
 */
Ohmbrewer::TemperatureSensor* Ohmbrewer::Still::getColumnSensor() const {
    return _columnSensor;
}

/**
 * The temperature sensor located in the tun
 * @returns The Temperature Sensor object representing the sensor located in the mash tun
 */
Ohmbrewer::TemperatureSensor* Ohmbrewer::Still::getCoolantSensor() const {
    return _coolantSensor;
}

/**
 * The recirculation pump between the tun and the tube
 * @returns The Pump object representing the recirculation pump
 */
Ohmbrewer::Pump* Ohmbrewer::Still::getCoolantPump() const {
    return _coolantPump;
}

/**
 * Specifies the interface for arguments sent to this Equipment's associated function.
 * Parses the supplied string into an array of strings for setting the Equipment's values.
 * Most likely will be called during update().
 * @param argsStr The arguments supplied as an update to the Rhizome.
 * @param result A map reprsenting the key/value pairs for the update
 */
void Ohmbrewer::Still::parseArgs(const String &argsStr, Ohmbrewer::Equipment::args_map_t &result) {

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
const int Ohmbrewer::Still::setState(const bool state) {
    unsigned long start = millis();

    getTherm()->setState(state);
    getKettleSensor()->setState(state);
    getColumnSensor()->setState(state);
    getCoolantSensor()->setState(state);
    getCoolantPump()->setState(state);

    return start - millis();
}

/**
 * The Equipment state. True => On, False => Off
 * @returns True => On, False => Off
 */
bool Ohmbrewer::Still::getState() const {
    return getCoolantPump()->getState() || getKettleSensor()->getState() || getTherm()->getState()
            || getCoolantSensor()->getState() || getColumnSensor()->getState();
}

/**
 * True if the Equipment state is On.
 * @returns Whether the Equipment is turned ON
 */
bool Ohmbrewer::Still::isOn() const {
    return getState();
}

/**
 * True if the Equipment state is Off.
 * @returns Whether the Equipment is turned OFF
 */
bool Ohmbrewer::Still::isOff() const {
    return !getState();
}

/**
 * Performs the Equipment's current task. Expect to use this during loop().
 * This function is called by work().
 * @returns The time taken to run the method
 */
int Ohmbrewer::Still::doWork() {
    // TODO: Implement STILL::doWork
    //need to call the PID functionality
    //set the column temp as the therm sensor someplace
    //call PID on column temp
    //call pID on coolant if enabled. TODO


    return -1;
}

/**
 * Draws information to the Rhizome's display.
 * This function is called by display().
 * @param screen The Rhizome's touchscreen
 * @returns The time taken to run the method
 */
int Ohmbrewer::Still::doDisplay(Ohmbrewer::Screen *screen) {
    unsigned long start = micros();

    screen->resetTextSize();
    screen->resetTextColor();

    // Print the section title
    screen->print("===== STILL #");
    screen->print(getID());
    screen->print(" =====");

    // Add a wee margin
    screen->printMargin(2);

    // Print out the current temp from the Tun
    displayKettleTemp(screen);
    displayColumnTemp(screen);
    displayCoolantTemp(screen);

    screen->printMargin(2);
//    screen->print("------ status ------");
//    screen->printMargin(2);

    // Print out the temperature from the Tube
    getTherm()->displayCurrentTemp(screen);

    // Print out the target temp
    getTherm()->displayTargetTemp(screen);

    // Print the pump status
    displayCoolantPumpStatus(screen);

    return micros() - start;
}

/**
 * Prints the temperature information for our sensors onto the touchscreen.
 * @param screen The Rhizome's touchscreen
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Still::displayKettleTemp(Ohmbrewer::Screen *screen) {
    unsigned long start = micros();
    char tempStr [24];
    // If current == target, we'll default to yellow, 'cause we're golden...
    uint16_t color = screen->YELLOW;

    if(getKettleSensor()->getTemp()->c() > getTherm()->getTargetTemp()->c()) {
        // Too hot
        color = screen->RED;
    } else if(getKettleSensor()->getTemp()->c() < getTherm()->getTargetTemp()->c()) {
        // Too cold
        color = screen->CYAN;
    }

    sprintf(tempStr, "%2.2f", getKettleSensor()->getTemp()->c());

    // Print the label
    screen->resetTextColor();
    screen->print(" Kettle (");
    screen->writeDegree();
    screen->print("C): ");

    // Print out the temp
    screen->setTextColor(color, screen->DEFAULT_BG_COLOR);
    screen->println(tempStr);

    screen->resetTextColor();

    return micros() - start;
}

/**
 * Prints the temperature information for our sensors onto the touchscreen.
 * @param screen The Rhizome's touchscreen
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Still::displayColumnTemp(Ohmbrewer::Screen *screen) {
    unsigned long start = micros();
    char tempStr [24];
    // If current == target, we'll default to yellow, 'cause we're golden...
    uint16_t color = screen->YELLOW;

    if(getColumnSensor()->getTemp()->c() > getTherm()->getTargetTemp()->c()) {
        // Too hot
        color = screen->RED;
    } else if(getColumnSensor()->getTemp()->c() < getTherm()->getTargetTemp()->c()) {
        // Too cold
        color = screen->CYAN;
    }

    sprintf(tempStr, "%2.2f", getColumnSensor()->getTemp()->c());

    // Print the label
    screen->resetTextColor();
    screen->print(" column (");
    screen->writeDegree();
    screen->print("C): ");

    // Print out the temp
    screen->setTextColor(color, screen->DEFAULT_BG_COLOR);
    screen->println(tempStr);

    screen->resetTextColor();

    return micros() - start;
}

/**
 * Prints the temperature information for our sensors onto the touchscreen.
 * @param screen The Rhizome's touchscreen
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Still::displayCoolantTemp(Ohmbrewer::Screen *screen) {
    unsigned long start = micros();
    char tempStr [24];
    // If current == target, we'll default to yellow, 'cause we're golden...
    uint16_t color = screen->YELLOW;

    if(getCoolantSensor()->getTemp()->c() > getTherm()->getTargetTemp()->c()) {
        // Too hot
        color = screen->RED;
    } else if(getCoolantSensor()->getTemp()->c() < getTherm()->getTargetTemp()->c()) {
        // Too cold
        color = screen->CYAN;
    }

    sprintf(tempStr, "%2.2f", getCoolantSensor()->getTemp()->c());

    // Print the label
    screen->resetTextColor();
    screen->print(" coolant (");
    screen->writeDegree();
    screen->print("C): ");

    // Print out the temp
    screen->setTextColor(color, screen->DEFAULT_BG_COLOR);
    screen->println(tempStr);

    screen->resetTextColor();

    return micros() - start;
}

/**
 * Prints the recirculation pump status onto the touchscreen.
 * @param screen The Rhizome's touchscreen
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Still::displayCoolantPumpStatus(Ohmbrewer::Screen *screen) {
    unsigned long start = micros();

    // Print the label
    screen->resetTextColor();
    screen->print(" C. Pump: "); // We want a little margin

    // Print the state
    if (getCoolantPump()->getState()){
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
int Ohmbrewer::Still::doUpdate(String &args, Ohmbrewer::Equipment::args_map_t &argsMap) {
    unsigned long start = millis();

    // If there are any remaining parameters
    if(args.length() > 0) {
        String tunSensorKey = String("kettle_sensor_state");
        String rPumpKey = String("c_pump_state");
        String tubeKey = String("therm_params");

        parseArgs(args, argsMap);

        //TODO flush out with all vars
        // The remaining settings are optional/convenience parameters
        if(argsMap.count(tunSensorKey) != 0) {
            if(argsMap[tunSensorKey].equalsIgnoreCase("ON")) {
                getKettleSensor()->setState(true);
            } else if(argsMap[tunSensorKey].equalsIgnoreCase("OFF")) {
                getKettleSensor()->setState(false);
            } else if(argsMap[tunSensorKey].equalsIgnoreCase("--")) {
                // Do nothing. Intentional.
            } else {
                // Do nothing. TODO: Should probably raise an error code...
            }
        }

        if(argsMap.count(rPumpKey) != 0) {
            if(argsMap[rPumpKey].equalsIgnoreCase("ON")) {
                getCoolantPump()->setState(true);
            } else if(argsMap[rPumpKey].equalsIgnoreCase("OFF")) {
                getCoolantPump()->setState(false);
            } else if(argsMap[rPumpKey].equalsIgnoreCase("--")) {
                // Do nothing. Intentional.
            } else {
                // Do nothing. TODO: Should probably raise an error code...
            }
        }


        if(argsMap.count(tubeKey) != 0) {
            // Pass the remaining parameters down to the Tube (a Thermostat)
            getTherm()->doUpdate(argsMap[tubeKey], argsMap);
        }

    }


    return millis() - start;
}

/**
 * Reports which of the Rhizome's pins are occupied by the
 * Equipment, forming a logical Sprout.
 * @param pins The list of physical pins that the Equipment is connected to.
 */
void Ohmbrewer::Still::whichPins(std::list<int>* pins){

    pins->push_back(_kettleSensor->getBusPin()); //assumes onewire protocol
    _therm->whichPins(pins);
    _coolantPump->whichPins(pins);

}

