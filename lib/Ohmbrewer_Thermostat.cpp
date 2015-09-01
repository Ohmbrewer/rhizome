#include "Ohmbrewer_Thermostat.h"

/**
 * The desired target temperature. Defaults to Celsius
 */
double Ohmbrewer::Thermostat::getTargetTemp() const {
    return getTargetTempC();
}

/**
 * The last temperature read by the sensor in Fahrenheit
 */
double Ohmbrewer::Thermostat::getTargetTempF() const {
    return (getTargetTempC() * 1.8) + 32;
}

/**
 * The last temperature read by the sensor in Celsius
 */
double Ohmbrewer::Thermostat::getTargetTempC() const {
    return _targetTemp;
}

/**
 * Sets the target temperature
 */
const int Ohmbrewer::Thermostat::setTargetTemp(const int targetTemp) {
    _targetTemp = targetTemp;
    return 0;
}

/**
 * The heating element
 */
Ohmbrewer::HeatingElement* Ohmbrewer::Thermostat::getElement() const {
    return _heatingElm;
}

/**
 * The temperature sensor
 */
Ohmbrewer::TemperatureSensor* Ohmbrewer::Thermostat::getSensor() const {
    return _tempSensor;
}

/**
 * Constructor
 */
Ohmbrewer::Thermostat::Thermostat(int id, int* pins) : Ohmbrewer::Equipment(id, pins) {
    // TODO: Figure out how to properly set the HeatingElement and TemperatureSensor in the constructors
    int fakePins[2] = {1,2};
    _heatingElm = new HeatingElement(1,fakePins); // This isn't right
    _tempSensor = new TemperatureSensor(1, fakePins); // Neither is this
}

/**
 * Constructor
 */
Ohmbrewer::Thermostat::Thermostat(int id, int* pins, const double targetTemp) : Ohmbrewer::Equipment(id, pins) {
    // TODO: Figure out how to properly set the HeatingElement and TemperatureSensor in the constructors
    int fakePins[2] = {1,2};
    _heatingElm = new HeatingElement(1,fakePins); // This isn't right
    _tempSensor = new TemperatureSensor(1, fakePins); // Neither is this
    _targetTemp = targetTemp;
}

/**
 * Constructor
 */
Ohmbrewer::Thermostat::Thermostat(int id, int* pins, int stopTime,
                                  bool state, char* currentTask) : Ohmbrewer::Equipment(id, pins, stopTime, state, currentTask) {
    // TODO: Figure out how to properly set the HeatingElement and TemperatureSensor in the constructors
    int fakePins[2] = {1,2};
    _heatingElm = new HeatingElement(1,fakePins); // This isn't right
    _tempSensor = new TemperatureSensor(1, fakePins); // Neither is this
}

/**
 * Constructor
 */
Ohmbrewer::Thermostat::Thermostat(int id, int* pins, int stopTime,
                                  bool state, char* currentTask,
                                  const double targetTemp) : Ohmbrewer::Equipment(id, pins, stopTime, state, currentTask) {
    // TODO: Figure out how to properly set the HeatingElement and TemperatureSensor in the constructors
    int fakePins[2] = {1,2};
    _heatingElm = new HeatingElement(1,fakePins); // This isn't right
    _tempSensor = new TemperatureSensor(1, fakePins); // Neither is this
    _targetTemp = targetTemp;
}

/**
 * Copy Constructor
 */
Ohmbrewer::Thermostat::Thermostat(const Ohmbrewer::Thermostat& clonee) : Ohmbrewer::Equipment((Equipment)clonee) {
    _heatingElm = clonee.getElement();
    _tempSensor = clonee.getSensor();
    _targetTemp = clonee.getTargetTemp();
}

/**
 * Destructor
 */
Ohmbrewer::Thermostat::~Thermostat() {
    delete _heatingElm;
    delete _tempSensor;
}

/**
 * Overloaded << operator.
 */
// friend std::ostream& Ohmbrewer::Thermostat::operator<<( std::ostream& os, Thermostat const& thermostat);

/**
 * Specifies the interface for arguments sent to this Equipment's associated function.
 * Parses the supplied string into an array of strings for setting the Equipment's values.
 * Most likely will be called during update().
 */
char** Ohmbrewer::Thermostat::parseArgs(const char* argsStr) {
    // TODO: Implement Thermostat::parseArgs
    char* placeholder[] = {"fixme"};
    return placeholder;
}

/**
 * Sets the Equipment state. True => On, False => Off
 * This turns *EVERYTHING* on, so watch out. You may want to turn the element and sensor on individually instead.
 */
const int Ohmbrewer::Thermostat::setState(const bool state) {
    getElement()->setState(state);
    getSensor()->setState(state);
    return 0;
}

/**
 * The Equipment state. True => On, False => Off
 */
bool Ohmbrewer::Thermostat::getState() const {
    return (getElement()->getState() || getSensor()->getState());
}

/**
 * True if the Equipment state is On.
 */
bool Ohmbrewer::Thermostat::isOn() const {
    return getState();
}

/**
 * True if the Equipment state is Off.
 */
bool Ohmbrewer::Thermostat::isOff() const {
    return !getState();
}

/**
 * Performs the Equipment's current task. Expect to use this during loop().
 * This function is called by work().
 */
int Ohmbrewer::Thermostat::doWork() {
    // TODO: Implement Thermostat::doWork
    return -1;
}

/**
 * Draws information to the Rhizome's display.
 * This function is called by display().
 */
int Ohmbrewer::Thermostat::doDisplay() {
    // TODO: Implement Thermostat::doDisplay
    return -1;
}

/**
 * Publishes updates to Ohmbrewer, etc.
 * This function is called by update().
 */
int Ohmbrewer::Thermostat::doUpdate() {
    // TODO: Implement Thermostat::doUpdate
    return -1;
}

/**
 * Reports which of the Rhizome's pins are occupied by the
 * Equipment, forming a logical Sprout.
 */
int* Ohmbrewer::Thermostat::whichPins() const {
    // TODO: Implement Thermostat::whichPins
    //return {&_pins, &getElement(), &getSensor()};
    return NULL;
}

