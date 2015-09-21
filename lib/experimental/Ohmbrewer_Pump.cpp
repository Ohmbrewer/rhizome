/*
 * FIXME: Convert this to VariablePump
 */

#include "Ohmbrewer_Pump.h"

/**
 * The speed at which the Pump runs.
 * @returns The current Pump speed
 */
int Ohmbrewer::Pump::getSpeed() const {
    return _speed;
}

/**
 * Sets the speed at which the Pump runs.
 * @param speed The new pump speed
 */
const int Ohmbrewer::Pump::setSpeed(const int speed) {
    _speed = speed;
    return 0;
}

/**
 * Constructor
 * @param id The Sprout ID to use for this piece of Equipment
 * @param pins The list of physical pins this Equipment is attached to
 */
Ohmbrewer::Pump::Pump(int id, std::list<int>* pins) : Ohmbrewer::Equipment(id, pins) {
    _speed = 0;
}

/**
 * Constructor
 * @param id The Sprout ID to use for this piece of Equipment
 * @param pins The list of physical pins this Equipment is attached to
 * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
 * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
 * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
 */
Ohmbrewer::Pump::Pump(int id, std::list<int>* pins, int stopTime,
                      bool state, String currentTask) : Ohmbrewer::Equipment(id, pins, stopTime, state, currentTask) {
    _speed = 0;
}

/**
 * Constructor
 * @param id The Sprout ID to use for this piece of Equipment
 * @param pins The list of physical pins this Equipment is attached to
 * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
 * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
 * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
 * @param speed The new pump speed
 */
Ohmbrewer::Pump::Pump(int id, std::list<int>* pins, int stopTime,
                      bool state, String currentTask, int speed) : Ohmbrewer::Equipment(id, pins, stopTime, state, currentTask)  {
    _speed = speed;
}

/**
 * Copy Constructor
 * @param clonee The Equipment object to copy
 */
Ohmbrewer::Pump::Pump(const Pump& clonee) : Ohmbrewer::Equipment(clonee) {
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
 * @param argsStr The arguments supplied as an update to the Rhizome.
 * @returns A map representing the key/value pairs for the update
 */
Ohmbrewer::Equipment::args_map_t Ohmbrewer::Pump::parseArgs(const String argsStr) {
    args_map_t result;
    char* params = new char[argsStr.length() + 1];
    strcpy(params, argsStr.c_str());

    // Parse the parameters
    String id          = String(strtok(params, ","));
    String currentTask = String(strtok(params, ","));
    String state       = String(strtok(NULL, ","));
    String stopTime    = String(strtok(NULL, ","));
    String speed       = "";

    // Parse the optional parameters
    if(stopTime != NULL && stopTime.toInt() > 0) {
        speed = "" + String(strtok(NULL, ","));
    }

    // Save them to the map
    result[String("id")] = id;
    result[String("current_task")] = currentTask;
    result[String("state")] = state;
    result[String("stopTime")] = stopTime;
    result[String("speed")] = speed;

    // Clear out that dynamically allocated buffer
    delete params;

    return result;
}

/**
 * Sets the Equipment state. True => On, False => Off
 * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
 * @returns The time taken to run the method
 */
const int Ohmbrewer::Pump::setState(const bool state) {
    unsigned long start = millis();

    _state = state;

    return start - millis();
}

/**
 * The Equipment state. True => On, False => Off
 * @returns True => On, False => Off
 */
bool Ohmbrewer::Pump::getState() const {
    return _state;
}

/**
 * True if the Equipment state is On.
 * @returns Whether the Equipment is turned ON
 */
bool Ohmbrewer::Pump::isOn() const {
    return _state;
}

/**
 * True if the Equipment state is Off.
 * @returns Whether the Equipment is turned OFF
 */
bool Ohmbrewer::Pump::isOff() const {
    return !_state;
}

/**
 * Performs the Equipment's current task. Expect to use this during loop().
 * This function is called by work().
 * @returns The time taken to run the method
 */
int Ohmbrewer::Pump::doWork() {
    // TODO: Implement Pump::doWork
    return -1;
}

/**
 * Draws information to the Rhizome's display.
 * This function is called by display().
 * @returns The time taken to run the method
 */
int Ohmbrewer::Pump::doDisplay(Ohmbrewer::Screen *screen) {
    // TODO: Implement Pump::doDisplay
    return -1;
}

/**
 * Publishes updates to Ohmbrewer, etc.
 * This function is called by update().
 * @returns The time taken to run the method
 */
int Ohmbrewer::Pump::doUpdate(String args) {
    // TODO: Implement Pump::doUpdate
    return -1;
}

/**
 * Reports which of the Rhizome's pins are occupied by the
 * Equipment, forming a logical Sprout.
 * @returns The list of physical pins that the Equipment is connected to.
 */
std::list<int>* Ohmbrewer::Pump::whichPins() const {
    return _pins;
}

