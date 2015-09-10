#include "Ohmbrewer_Heating_Element.h"

/**
 * The voltage at which the Heating Element runs.
 * @returns The current voltage setting
 */
int Ohmbrewer::HeatingElement::getVoltage() const {
    return _voltage;
}

/**
 * Sets the voltage at which the Heating Element runs.
 * @returns The time taken to run the method
 */
const int Ohmbrewer::HeatingElement::setVoltage(const int voltage) {
    unsigned long start = millis();

    _voltage = voltage;

    return start - millis();
}

/**
 * Constructor
 * @param id The Sprout ID to use for this piece of Equipment
 * @param pins The list of physical pins this Equipment is attached to
 */
Ohmbrewer::HeatingElement::HeatingElement(int id, std::list<int>* pins) : Ohmbrewer::Equipment(id, pins) {
    _voltage = 0;
    _type = "heat";
}

/**
 * Constructor
 * @param id The Sprout ID to use for this piece of Equipment
 * @param pins The list of physical pins this Equipment is attached to
 * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
 * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
 * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
 */
Ohmbrewer::HeatingElement::HeatingElement(int id, std::list<int>* pins, int stopTime,
                                          bool state, char* currentTask) : Ohmbrewer::Equipment(id, pins, stopTime, state, currentTask) {
    _voltage = 0;
    _type = "heat";
}

/**
 * Constructor
 * @param id The Sprout ID to use for this piece of Equipment
 * @param pins The list of physical pins this Equipment is attached to
 * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
 * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
 * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
 * @param voltage The current voltage setting
 */
Ohmbrewer::HeatingElement::HeatingElement(int id, std::list<int>* pins, int stopTime,
                                          bool state, char* currentTask, int voltage) : Ohmbrewer::Equipment(id, pins, stopTime, state, currentTask)  {
    _voltage = voltage;
    _type = "heat";
}

/**
 * Copy Constructor
 * @param clonee The Equipment object to copy
 */
Ohmbrewer::HeatingElement::HeatingElement(const HeatingElement& clonee) : Ohmbrewer::Equipment(clonee) {
    _voltage = clonee.getVoltage();
    _type = "heat";
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
 * @param argsStr The arguments supplied as an update to the Rhizome.
 * @returns A map representing the key/value pairs for the update
 */
Ohmbrewer::Equipment::args_map_t Ohmbrewer::HeatingElement::parseArgs(const String argsStr) {
    args_map_t result;
    char* params = new char[argsStr.length() + 1];
    strcpy(params, argsStr.c_str());

    // Parse the parameters
    String id          = String(strtok(params, ","));
    String currentTask = String(strtok(params, ","));
    String state       = String(strtok(NULL, ","));
    String stopTime    = String(strtok(NULL, ","));
    String voltage     = String(strtok(NULL, ","));

    // Save them to the map
    result[String("id")] = id;
    result[String("current_task")] = currentTask;
    result[String("state")] = state;
    result[String("stopTime")] = stopTime;
    result[String("voltage")] = voltage;

    // Clear out that dynamically allocated buffer
    delete params;

    return result;
}

/**
 * Sets the Equipment state. True => On, False => Off
 * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
 * @returns The time taken to run the method
 */
const int Ohmbrewer::HeatingElement::setState(const bool state) {
    unsigned long start = millis();

    _state = state;

    return start - millis();
}

/**
 * The Equipment state.
 * @returns True => On, False => Off
 */
bool Ohmbrewer::HeatingElement::getState() const {
    return _state;
}

/**
 * True if the Equipment state is On.
 * @returns Whether the Equipment is turned ON
 */
bool Ohmbrewer::HeatingElement::isOn() const {
    return _state;
}

/**
 * True if the Equipment state is Off.
 * @returns Whether the Equipment is turned OFF
 */
bool Ohmbrewer::HeatingElement::isOff() const {
    return !_state;
}

/**
 * Performs the Equipment's current task. Expect to use this during loop().
 * This function is called by work().
 * @returns The time taken to run the method
 */
int Ohmbrewer::HeatingElement::doWork() {
    // TODO: Implement HeatingElement::doWork
    return -1;
}

/**
 * Draws information to the Rhizome's display.
 * This function is called by display().
 * @returns The time taken to run the method
 */
int Ohmbrewer::HeatingElement::doDisplay() {
    // TODO: Implement HeatingElement::doDisplay
    return -1;
}

/**
 * Publishes updates to Ohmbrewer, etc.
 * This function is called by update().
 * @returns The time taken to run the method
 */
int Ohmbrewer::HeatingElement::doUpdate() {
    // TODO: Implement HeatingElement::doUpdate
    return -1;
}

/**
 * Reports which of the Rhizome's pins are occupied by the
 * Equipment, forming a logical Sprout.
 * @returns The list of physical pins that the Equipment is connected to.
 */
std::list<int>* Ohmbrewer::HeatingElement::whichPins() const {
    return _pins;
}

