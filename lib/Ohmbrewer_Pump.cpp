#include "Ohmbrewer_Pump.h"

/**
 * The speed at which the Pump runs..
 */
int Ohmbrewer::Pump::getSpeed() const {
    return _speed;
}

/**
 * Sets the speed at which the Pump runs.
 */
const int Ohmbrewer::Pump::setSpeed(const int speed) {
    _speed = speed;
    return 0;
}

/**
 * Constructor
 */
Ohmbrewer::Pump::Pump(int id, int* pins) : Ohmbrewer::Equipment(id, pins) {
    _speed = 0;
    _type = "pump";
}

/**
 * Constructor
 */
Ohmbrewer::Pump::Pump(int id, int* pins, int stopTime, bool state, char* currentTask) : Ohmbrewer::Equipment(id, pins, stopTime, state, currentTask) {
    _speed = 0;
    _type = "pump";
}

/**
 * Constructor
 */
Ohmbrewer::Pump::Pump(int id, int* pins, int stopTime, bool state, char* currentTask, int speed) : Ohmbrewer::Equipment(id, pins, stopTime, state, currentTask)  {
    _speed = speed;
    _type = "pump";
}

/**
 * Copy Constructor
 */
Ohmbrewer::Pump::Pump(const Pump& clonee) : Ohmbrewer::Equipment((Equipment)clonee) {
    _speed = clonee.getSpeed();
}

/**
 * Destructor
 */
Ohmbrewer::Pump::~Pump() {
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
char** Ohmbrewer::Pump::parseArgs(const char* argsStr) {
    // TODO: Implement Pump::parseArgs
    char* placeholder[] = {"fixme"};
    return placeholder;
}

/**
 * Sets the Equipment state. True => On, False => Off
 */
const int Ohmbrewer::Pump::setState(const bool state) {
    _state = state;
    return 0;
}

/**
 * The Equipment state. True => On, False => Off
 */
bool Ohmbrewer::Pump::getState() const {
    return _state;
}

/**
 * True if the Equipment state is On.
 */
bool Ohmbrewer::Pump::isOn() const {
    return _state;
}

/**
 * True if the Equipment state is Off.
 */
bool Ohmbrewer::Pump::isOff() const {
    return !_state;
}

/**
 * Performs the Equipment's current task. Expect to use this during loop().
 * This function is called by work().
 */
int Ohmbrewer::Pump::doWork() {
    // TODO: Implement Pump::doWork
    return -1;
}

/**
 * Draws information to the Rhizome's display.
 * This function is called by display().
 */
int Ohmbrewer::Pump::doDisplay() {
    // TODO: Implement Pump::doDisplay
    return -1;
}

/**
 * Publishes updates to Ohmbrewer, etc.
 * This function is called by update().
 */
int Ohmbrewer::Pump::doUpdate() {
    // TODO: Implement Pump::doUpdate
    return -1;
}

/**
 * Reports which of the Rhizome's pins are occupied by the
 * Equipment, forming a logical Sprout.
 */
int* Ohmbrewer::Pump::whichPins() const {
    return _pins;
}

