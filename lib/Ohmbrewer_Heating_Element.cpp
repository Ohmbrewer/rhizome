#include "Ohmbrewer_Heating_Element.h"

/**
 * The voltage at which the Heating Element runs..
 */
int Ohmbrewer::HeatingElement::getVoltage() const {
    return _voltage;
}

/**
 * Sets the voltage at which the Heating Element runs.
 */
const int Ohmbrewer::HeatingElement::setVoltage(const int voltage) {
    _voltage = voltage;
    return 0;
}

/**
 * Constructor
 */
Ohmbrewer::HeatingElement::HeatingElement(int id, int* pins) : Ohmbrewer::Equipment(id, pins) {
    _voltage = 0;
    _type = "heat";
}

/**
 * Constructor
 */
Ohmbrewer::HeatingElement::HeatingElement(int id, int* pins, int stopTime, bool state, char* currentTask) : Ohmbrewer::Equipment(id, pins, stopTime, state, currentTask) {
    _voltage = 0;
    _type = "heat";
}

/**
 * Constructor
 */
Ohmbrewer::HeatingElement::HeatingElement(int id, int* pins, int stopTime, bool state, char* currentTask, int voltage) : Ohmbrewer::Equipment(id, pins, stopTime, state, currentTask)  {
    _voltage = voltage;
    _type = "heat";
}

/**
 * Copy Constructor
 */
Ohmbrewer::HeatingElement::HeatingElement(const HeatingElement& clonee) : Ohmbrewer::Equipment((Equipment)clonee) {
    _voltage = clonee.getVoltage();
}

/**
 * Destructor
 */
Ohmbrewer::HeatingElement::~HeatingElement() {
    // Nothing to do here...
}

/**
 * Overloaded << operator.
 */
// friend std::ostream& operator<<( std::ostream& os, HeatingElement const& HeatingElement);

/**
 * Specifies the interface for arguments sent to this Equipment's associated function. 
 * Parses the supplied string into an array of strings for setting the Equipment's values.
 * Most likely will be called during update().
 */
Ohmbrewer::Equipment::args_map_t Ohmbrewer::HeatingElement::parseArgs(const char* argsStr) {
    // TODO: Implement HeatingElement::parseArgs
    args_map_t placeholder;
    placeholder[String("fixme")] = String("nonononononono");
    return placeholder;
}

/**
 * Sets the Equipment state. True => On, False => Off
 */
const int Ohmbrewer::HeatingElement::setState(const bool state) {
    _state = state;
    return 0;
}

/**
 * The Equipment state. True => On, False => Off
 */
bool Ohmbrewer::HeatingElement::getState() const {
    return _state;
}

/**
 * True if the Equipment state is On.
 */
bool Ohmbrewer::HeatingElement::isOn() const {
    return _state;
}

/**
 * True if the Equipment state is Off.
 */
bool Ohmbrewer::HeatingElement::isOff() const {
    return !_state;
}

/**
 * Performs the Equipment's current task. Expect to use this during loop().
 * This function is called by work().
 */
int Ohmbrewer::HeatingElement::doWork() {
    // TODO: Implement HeatingElement::doWork
    return -1;
}

/**
 * Draws information to the Rhizome's display.
 * This function is called by display().
 */
int Ohmbrewer::HeatingElement::doDisplay() {
    // TODO: Implement HeatingElement::doDisplay
    return -1;
}

/**
 * Publishes updates to Ohmbrewer, etc.
 * This function is called by update().
 */
int Ohmbrewer::HeatingElement::doUpdate() {
    // TODO: Implement HeatingElement::doUpdate
    return -1;
}

/**
 * Reports which of the Rhizome's pins are occupied by the
 * Equipment, forming a logical Sprout.
 */
int* Ohmbrewer::HeatingElement::whichPins() const {
    return _pins;
}

