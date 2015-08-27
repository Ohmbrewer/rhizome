#include "Ohmbrewer_Temperature_Sensor.h"

/**
 * The last temperature read by the sensor. Currently returns in Celsius.
 */
double Ohmbrewer::TemperatureSensor::getTemp() const {
    return _lastReading;
}

/**
 * The last temperature read by the sensor in Fahrenheit
 */
double Ohmbrewer::TemperatureSensor::toFahrenheit() const {
    return (_lastReading * 1.8) + 32;
}

/**
 * The last temperature read by the sensor in Celsius
 */
double Ohmbrewer::TemperatureSensor::toCelsius() const {
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
}

/**
 * Constructor
 */
Ohmbrewer::TemperatureSensor::TemperatureSensor(int id, int* pins) : Ohmbrewer::Equipment(id, pins) {
    _lastReading = 0;
    _lastReadTime = Time.now();
    _type = "temp";
}

/**
 * Constructor
 */
Ohmbrewer::TemperatureSensor::TemperatureSensor(int id, int* pins, int stopTime, bool state, char* currentTask) : Ohmbrewer::Equipment(id, pins, stopTime, state, currentTask) {
    _lastReading = 0;
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
    // Nothing to do here...
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
char** Ohmbrewer::TemperatureSensor::parseArgs(const char* argsStr) {
    // TODO: Implement TemperatureSensor::parseArgs
}

/**
 * Sets the Equipment state. True => On, False => Off
 */
const int Ohmbrewer::TemperatureSensor::setState(const bool state) {
    _state = state;
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
}

/**
 * Draws information to the Rhizome's display.
 * This function is called by display().
 */
int Ohmbrewer::TemperatureSensor::doDisplay() {
    // TODO: Implement TemperatureSensor::doDisplay
}

/**
 * Publishes updates to Ohmbrewer, etc.
 * This function is called by update().
 */
int Ohmbrewer::TemperatureSensor::doUpdate() {
    // TODO: Implement TemperatureSensor::doUpdate
}

/**
 * Reports which of the Rhizome's pins are occupied by the
 * Equipment, forming a logical Sprout.
 */
int* Ohmbrewer::TemperatureSensor::whichPins() const {
    return _pins;
}

