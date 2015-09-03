#include "Ohmbrewer_Temperature_Sensor.h"

/**
 * The last temperature read by the sensor. Currently returns in Celsius.
 */
Ohmbrewer::Temperature* Ohmbrewer::TemperatureSensor::getTemp() const {
    return _lastReading;
}

/**
 * The last time the temperature was read by the sensor
 */
int Ohmbrewer::TemperatureSensor::getLastReadTime() const {
    return _lastReadTime;
}

/**
 * Sets the last time the temperature was read by the sensor
 */
const int Ohmbrewer::TemperatureSensor::setLastReadTime(const int lastReadTime) {
    _lastReadTime = lastReadTime;
    return 0;
}

/**
 * Constructor
 */
Ohmbrewer::TemperatureSensor::TemperatureSensor(int id, int* pins) : Ohmbrewer::Equipment(id, pins) {
    _lastReading = new Temperature(0);
    _lastReadTime = Time.now();
    _type = "temp";
}

/**
 * Constructor
 */
Ohmbrewer::TemperatureSensor::TemperatureSensor(int id, int* pins, int stopTime, bool state, char* currentTask) : Ohmbrewer::Equipment(id, pins, stopTime, state, currentTask) {
    _lastReading = new Temperature(0);
    _lastReadTime = Time.now();
    _type = "temp";
}

/**
 * Copy Constructor
 */
Ohmbrewer::TemperatureSensor::TemperatureSensor(const TemperatureSensor& clonee) : Ohmbrewer::Equipment((Equipment)clonee) {
    _lastReading = clonee.getTemp();
    _lastReadTime = Time.now();
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
 * Specifies the interface for arguments sent to this Equipment's associated function. 
 * Parses the supplied string into an array of strings for setting the Equipment's values.
 * Most likely will be called during update().
 */
Ohmbrewer::Equipment::args_map_t Ohmbrewer::TemperatureSensor::parseArgs(const char* argsStr) {
    // TODO: Implement TemperatureSensor::parseArgs
    args_map_t placeholder;
    placeholder[String("fixme")] = String("nonononononono");
    return placeholder;
}

/**
 * Sets the Equipment state. True => On, False => Off
 */
const int Ohmbrewer::TemperatureSensor::setState(const bool state) {
    _state = state;
    return 0;
}

/**
 * The Equipment state. True => On, False => Off
 */
bool Ohmbrewer::TemperatureSensor::getState() const {
    return _state;
}

/**
 * True if the Equipment state is On.
 */
bool Ohmbrewer::TemperatureSensor::isOn() const {
    return _state;
}

/**
 * True if the Equipment state is Off.
 */
bool Ohmbrewer::TemperatureSensor::isOff() const {
    return !_state;
}

/**
 * Performs the Equipment's current task. Expect to use this during loop().
 * This function is called by work().
 */
int Ohmbrewer::TemperatureSensor::doWork() {
    // TODO: Implement TemperatureSensor::doWork
    return -1;
}

/**
 * Draws information to the Rhizome's display.
 * This function is called by display().
 */
int Ohmbrewer::TemperatureSensor::doDisplay() {
    // TODO: Implement TemperatureSensor::doDisplay
    return -1;
}

/**
 * Publishes updates to Ohmbrewer, etc.
 * This function is called by update().
 */
int Ohmbrewer::TemperatureSensor::doUpdate() {
    // TODO: Implement TemperatureSensor::doUpdate
    return -1;
}

/**
 * Reports which of the Rhizome's pins are occupied by the
 * Equipment, forming a logical Sprout.
 */
int* Ohmbrewer::TemperatureSensor::whichPins() const {
    return _pins;
}

