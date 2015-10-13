#include "Ohmbrewer_Thermostat.h"
#include "Ohmbrewer_Screen.h"
#include "Ohmbrewer_Publisher.h"


/**
 * Constructor
 * @param id The Sprout ID to use for this piece of Thermostat
 * @param tubePins[ temp busPin ; heating powerPin ; heating controlPin ]
 */
Ohmbrewer::Thermostat::Thermostat(int id, int (&thermPins)[3]) : Ohmbrewer::Equipment(id) {
    int n = sizeof(thermPins) / sizeof(int);
    if ( n > 2){
        _heatingElm = new HeatingElement(id+2, thermPins[1], thermPins[2]);
        _tempSensor = new TemperatureSensor(id+1, thermPins[0]);
    }else if (n == 2 ){                     // no switch, only power% (SSR in parallel or always on other leg)
        _heatingElm = new HeatingElement(id+2, thermPins[1], -1);
        _tempSensor = new TemperatureSensor(id+1, thermPins[0]);
    }else{
        //publish error

        Ohmbrewer::Publisher::publish_map_t pMap;
        Ohmbrewer::Publisher* pub = new Ohmbrewer::Publisher(new String("error_log"), &pMap);

        pMap[String("array_check_thermostat")] = String("improperly formed array - Thermostat(int id, int[])");
        pub->publish();
    }

    _targetTemp = new Temperature(0);
    registerUpdateFunction();
}

/**
 * Constructor
 * @param id The Sprout ID to use for this piece of Thermostat
 * @param tubePins[ temp busPin ; heating powerPin ; heating controlPin ]
 * @param targetTemp The new target temperature in Celsius
 */
Ohmbrewer::Thermostat::Thermostat(int id, int (&thermPins)[3], const double targetTemp) : Ohmbrewer::Equipment(id) {
    int n = sizeof(thermPins) / sizeof(int);
    if ( n > 2){
        _heatingElm = new HeatingElement(id+2, thermPins[1], thermPins[2]);
        _tempSensor = new TemperatureSensor(id+1, thermPins[0]);
    }else if (n == 2 ){                     // no switch, only power% (SSR in parallel or always on other leg)
        _heatingElm = new HeatingElement(id+2, thermPins[1], -1);
        _tempSensor = new TemperatureSensor(id+1, thermPins[0]);
    }else{
        //publish error
        Ohmbrewer::Publisher::publish_map_t pMap;
        Ohmbrewer::Publisher* pub = new Ohmbrewer::Publisher(new String("error_log"), &pMap);

        pMap[String("array_check_thermostat")] = String("improperly formed array - Thermostat(int, int[], double)");
        pub->publish();
    }
    _targetTemp = new Temperature(targetTemp);
    registerUpdateFunction();
}

/**
 * Constructor
 * @param id The Sprout ID to use for this piece of Thermostat
 * @param tubePins[ temp busPin ; heating powerPin ; heating controlPin ]
 * @param stopTime The time at which the Thermostat should shut off, assuming it isn't otherwise interrupted
 * @param state Whether the Thermostat is ON (or OFF). True => ON, False => OFF
 * @param currentTask The unique identifier of the task that the Thermostat believes it should be processing
 */
Ohmbrewer::Thermostat::Thermostat(int id, int (&thermPins)[3], int stopTime,
                                  bool state, String currentTask) : Ohmbrewer::Equipment(id, stopTime, state, currentTask) {
    int n = sizeof(thermPins) / sizeof(int);
    if ( n > 2){
        _heatingElm = new HeatingElement(id+2, thermPins[1], thermPins[2]);
        _tempSensor = new TemperatureSensor(id+1, thermPins[0]);
    }else if (n == 2 ){                     // no switch, only power% (SSR in parallel or always on other leg)
        _heatingElm = new HeatingElement(id+2, thermPins[1], -1);
        _tempSensor = new TemperatureSensor(id+1, thermPins[0]);
    }else{
        //publish error
        Ohmbrewer::Publisher::publish_map_t pMap;
        Ohmbrewer::Publisher* pub = new Ohmbrewer::Publisher(new String("error_log"), &pMap);

        pMap[String("array_check_thermostat")] = String("improperly formed array - Thermostat(int, int[], int, bool, String)");
        pub->publish();
    }
    _targetTemp = new Temperature(0);
    registerUpdateFunction();
}

/**
 * Constructor
 * @param id The Sprout ID to use for this piece of Thermostat
 * @param tubePins[ temp busPin ; heating powerPin ; heating controlPin ]
 * @param stopTime The time at which the Thermostat should shut off, assuming it isn't otherwise interrupted
 * @param state Whether the Thermostat is ON (or OFF). True => ON, False => OFF
 * @param currentTask The unique identifier of the task that the Thermostat believes it should be processing
 * @param targetTemp The new target temperature in Celsius
 */
Ohmbrewer::Thermostat::Thermostat(int id, int (&thermPins)[3], int stopTime,
                                  bool state, String currentTask,
                                  const double targetTemp) : Ohmbrewer::Equipment(id, stopTime, state, currentTask) {

    int n = sizeof(thermPins) / sizeof(int);
    if ( n > 2){
        _heatingElm = new HeatingElement(id+2, thermPins[1], thermPins[2]);
        _tempSensor = new TemperatureSensor(id+1, thermPins[0]);
    }else if (n == 2 ){                     // no switch, only power% (SSR in parallel or always on other leg)
        _heatingElm = new HeatingElement(id+2, thermPins[1], -1);
        _tempSensor = new TemperatureSensor(id+1, thermPins[0]);
    }else{
        //publish error
        Ohmbrewer::Publisher::publish_map_t pMap;
        Ohmbrewer::Publisher* pub = new Ohmbrewer::Publisher(new String("error_log"), &pMap);

        pMap[String("array_check_thermostat")] = String("improperly formed array - Thermostat(int, int [], int, bool, String, double)");
        pub->publish();
    }
    _targetTemp = new Temperature(targetTemp);
    registerUpdateFunction();
}

/**
 * Copy Constructor
 * @param clonee The Thermostat object to copy
 */
Ohmbrewer::Thermostat::Thermostat(const Ohmbrewer::Thermostat& clonee) : Ohmbrewer::Equipment(clonee) {
    _heatingElm = clonee.getElement();
    _tempSensor = clonee.getSensor();
    _targetTemp = clonee.getTargetTemp();
    registerUpdateFunction();
}

/**
 * Destructor
 */
Ohmbrewer::Thermostat::~Thermostat() {
    delete _heatingElm;
    delete _tempSensor;
    delete _targetTemp;
}

/**
 * Overloaded << operator.
 */
// friend std::ostream& Ohmbrewer::Thermostat::operator<<( std::ostream& os, Thermostat const& thermostat);

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
 * This turns *EVERYTHING* on, so watch out. You may want to turn the components on individually instead.
 * @param state Whether the Thermostat is ON (or OFF). True => ON, False => OFF
 * @returns The time taken to run the method
 */
const int Ohmbrewer::Thermostat::setState(const bool state) {
    unsigned long start = millis();

    getElement()->setState(state);
    getSensor()->setState(state);

    return start - millis();
}

/**
 * The Thermostat state. True => On, False => Off
 * @returns True => On, False => Off
 */
bool Ohmbrewer::Thermostat::getState() const {
    return (getElement()->getState() || getSensor()->getState());
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
 * @returns The time taken to run the method
 */
int Ohmbrewer::Thermostat::doWork() {
    // TODO: Implement Thermostat::doWork PID stuff here.
    return -1;
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
    screen->print("== Therm. #");
    screen->print(getID());
    screen->print(" (");
    screen->writeDegree(); // Degree symbol
    screen->print("C) ==");

    // Add a wee margin
    screen->printMargin(2);

    // Print out the current temp
    displayCurrentTemp(screen);

    // Print out the target temp
    displayTargetTemp(screen);

    // Add another wee margin
    screen->printMargin(2);

    return micros() - start;
}

/**
 * Prints the temperature information for our sensors onto the touchscreen.
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Thermostat::displayCurrentTemp(Ohmbrewer::Screen *screen) {
    unsigned long start = micros();

    // If current == target, we'll default to yellow, 'cause we're golden...
    uint16_t color = screen->YELLOW;

    if(getSensor()->getTemp()->c() > getTargetTemp()->c()) {
        // Too hot
        color = screen->RED;
    } else if(getSensor()->getTemp()->c() < getTargetTemp()->c()) {
        // Too cold
        color = screen->CYAN;
    }

    displayTemp(getSensor()->getTemp(), "Current: ", color, screen);

    // Show a warning if the Heating Element is active
    if(getElement()->isOn()) {
        screen->setTextColor(screen->RED, screen->DEFAULT_BG_COLOR);
        screen->print(" ON");
    } else {
        screen->setTextColor(screen->BLACK, screen->DEFAULT_BG_COLOR);
        screen->print(" ");
        screen->writeBlock();
        screen->writeBlock();
    }
    screen->resetTextColor();

    screen->println("");

    return micros() - start;
}

/**
 * Prints the temperature information for our sensors onto the touchscreen.
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Thermostat::displayTargetTemp(Ohmbrewer::Screen *screen) {
    unsigned long start = micros();
    displayTemp(getTargetTemp(), "Target:  ", screen->YELLOW, screen);
    screen->println("");
    return micros() - start;
}

/**
 * Prints the temperature information for our sensors onto the touchscreen.
 * @param temp The temperature to display
 * @param label The text label to print to the left of the temperature
 * @param color The color of the temperature text
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Thermostat::displayTemp(const Temperature *temp, char* label, uint16_t color, Ohmbrewer::Screen *screen) {
    unsigned long start = micros();
    char tempStr [10];
    temp->toStrC(tempStr);

    // Print the label
    screen->resetTextColor();
    screen->print(" "); // We want a little margin
    screen->print(label);

    // Print out the target temp
    screen->setTextColor(color, screen->DEFAULT_BG_COLOR);
    screen->print(tempStr);

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

