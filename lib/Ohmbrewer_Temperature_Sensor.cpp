#include "Ohmbrewer_Temperature_Sensor.h"
#include "Ohmbrewer_Screen.h"

/**
 * The last temperature read by the sensor. Currently returns in Celsius.
 * @returns A pointer to the Temperature object representing the last temperature reading
 */
Ohmbrewer::Temperature* Ohmbrewer::TemperatureSensor::getTemp() const {
    return _lastReading;
}

/**
 * The last time the temperature was read by the sensor
 * @returns The last temperature reading time
 */
int Ohmbrewer::TemperatureSensor::getLastReadTime() const {
    return _lastReadTime;
}

/**
 * Sets the last time the temperature was read by the sensor
 * @param lastReadTime The last temperature reading time
 */
const int Ohmbrewer::TemperatureSensor::setLastReadTime(const int lastReadTime) {
    _lastReadTime = lastReadTime;
    return 0;
}

/**
 * Constructor
 * @param id The Sprout ID to use for this TemperatureSensor
 * @param pins The list of physical pins this TemperatureSensor is attached to
 */
Ohmbrewer::TemperatureSensor::TemperatureSensor(int id, std::list<int>* pins) : Ohmbrewer::Equipment(id, pins) {
    _lastReading = new Temperature(0);
    _lastReadTime = Time.now();
    registerUpdateFunction();
}

/**
 * Constructor
 * @param id The Sprout ID to use for this piece of TemperatureSensor
 * @param pins The list of physical pins this TemperatureSensor is attached to
 * @param stopTime The time at which the TemperatureSensor should shut off, assuming it isn't otherwise interrupted
 * @param state Whether the TemperatureSensor is ON (or OFF). True => ON, False => OFF
 * @param currentTask The unique identifier of the task that the TemperatureSensor believes it should be processing
 */
Ohmbrewer::TemperatureSensor::TemperatureSensor(int id, std::list<int>* pins, int stopTime, bool state, String currentTask) : Ohmbrewer::Equipment(id, pins, stopTime, state, currentTask) {
    _lastReading = new Temperature(0);
    _lastReadTime = Time.now();
    registerUpdateFunction();
}

/**
 * Copy Constructor
 * @param clonee The TemperatureSensor object to copy
 */
Ohmbrewer::TemperatureSensor::TemperatureSensor(const TemperatureSensor& clonee) : Ohmbrewer::Equipment(clonee) {
    _lastReading = clonee.getTemp();
    _lastReadTime = Time.now();
    registerUpdateFunction();
}

/**
 * Destructor
 */
Ohmbrewer::TemperatureSensor::~TemperatureSensor() {
    delete _lastReading;
}

/**
 * Overloaded << operator.
 */
// friend std::ostream& operator<<( std::ostream& os, Pump const& pump);

/**
 * Specifies the interface for arguments sent to this TemperatureSensor's associated function.
 * Parses the supplied string into an array of strings for setting the TemperatureSensor's values.
 * Most likely will be called during update().
 * @param args The arguments supplied as an update to the Rhizome.
 * @param argsMap A map representing the key/value pairs for the update
 */
void Ohmbrewer::TemperatureSensor::parseArgs(const String &args, Ohmbrewer::Equipment::args_map_t &argsMap) {
    // Nothing special to parse out for this class.
}

/**
 * Sets the TemperatureSensor state. True => On, False => Off
 * @param state Whether the TemperatureSensor is ON (or OFF). True => ON, False => OFF
 * @returns The time taken to run the method
 */
const int Ohmbrewer::TemperatureSensor::setState(const bool state) {
    unsigned long start = millis();

    _state = state;

    return start - millis();
}

/**
 * The TemperatureSensor state. True => On, False => Off
 * @returns True => On, False => Off
 */
bool Ohmbrewer::TemperatureSensor::getState() const {
    return _state;
}

/**
 * True if the TemperatureSensor state is On.
 * @returns Whether the TemperatureSensor is turned ON
 */
bool Ohmbrewer::TemperatureSensor::isOn() const {
    return _state;
}

/**
 * True if the TemperatureSensor state is Off.
 * @returns Whether the TemperatureSensor is turned OFF
 */
bool Ohmbrewer::TemperatureSensor::isOff() const {
    return !_state;
}

/**
 * Performs the TemperatureSensor's current task. Expect to use this during loop().
 * This function is called by work().
 * @returns The time taken to run the method
 */
int Ohmbrewer::TemperatureSensor::doWork() {
    // TODO: Implement TemperatureSensor::doWork
    return -1;
}

/**
 * Draws information to the Rhizome's display.
 * This function is called by display().
 * @returns The time taken to run the method
 */
int Ohmbrewer::TemperatureSensor::doDisplay(Ohmbrewer::Screen *screen) {
    unsigned long start = micros();
    char relay_id[2];
    char tempStr [10];

    sprintf(relay_id,"%d", _id);
    getTemp()->toStrC(tempStr);

    // Print a fancy identifier
    screen->print(" [");

    screen->setTextColor(screen->WHITE, screen->DEFAULT_BG_COLOR);
    screen->print(relay_id);
    screen->resetTextColor();

    screen->print("]: ");

    // Print the temperature
    screen->setTextColor(screen->WHITE, screen->DEFAULT_BG_COLOR);
    screen->println(tempStr);

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
int Ohmbrewer::TemperatureSensor::doUpdate(String &args, Ohmbrewer::Equipment::args_map_t &argsMap) {
    unsigned long start = millis();
    // Nothing to do as it is...
    return millis() - start;
}

/**
 * Reports which of the Rhizome's pins are occupied by the
 * Equipment, forming a logical Sprout.
 * @returns The list of physical pins that the TemperatureSensor is connected to.
 */
std::list<int>* Ohmbrewer::TemperatureSensor::whichPins() const {
    return _pins;
}

