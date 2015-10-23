#include "Ohmbrewer_Pump.h"

/**
 * Constructor
 * @param id The Sprout ID to use for this piece of Equipment
 * @param powerPin - The power pin - on/off line. Digital pin number X.
 * @param controlPin - The Control pin - Data/speed/power level Digital pin number X.
 */
Ohmbrewer::Pump::Pump(int id, int powerPin, int controlPin) : Ohmbrewer::Relay(id, powerPin, controlPin) {
    registerUpdateFunction();
}

/**
 * Constructor
 * @param id The Sprout ID to use for this piece of Equipment
 * @param powerPin - The power pin - on/off line. Digital pin number X.
 * @param controlPin - The Control pin - Data/speed/power level Digital pin number X.
 * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
 * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
 * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
 */
Ohmbrewer::Pump::Pump(int id, int powerPin, int controlPin, int stopTime,
                      bool state, String currentTask) : Ohmbrewer::Relay(id, powerPin, controlPin, stopTime, state, currentTask) {
    registerUpdateFunction();
}

/**
 * Copy Constructor
 * @param clonee The Equipment object to copy
 */
Ohmbrewer::Pump::Pump(const Pump& clonee) : Ohmbrewer::Relay(clonee) {
    // This has probably already been set, but maybe clonee is a more complicated child class...
    registerUpdateFunction();
}

/**
 * Destructor
 */
Ohmbrewer::Pump::~Pump() {
    // Nothing to do here...
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
    return Relay::doDisplay(screen);
}

/**
 * Publishes updates to Ohmbrewer, etc.
 * This function is called by update().
 * @param args The argument string passed into the Particle Cloud
 * @param argsMap A map representing the key/value pairs for the update
 * @returns The time taken to run the method
 */
int Ohmbrewer::Pump::doUpdate(String &args, Ohmbrewer::Equipment::args_map_t &argsMap) {
    return Relay::doUpdate(args, argsMap);
}

/**
 * Specifies the interface for arguments sent to this Equipment's associated function.
 * Parses the supplied string into an array of strings for setting the Equipment's values.
 * Most likely will be called during update().
 * @param argsStr The arguments supplied as an update to the Rhizome.
 * @param result A map representing the key/value pairs for the update
 */
void Ohmbrewer::Pump::parseArgs(const String &argsStr, Ohmbrewer::Equipment::args_map_t &result) {
    // Nothing special to parse out for this class.
}
