#include "Ohmbrewer_Pump.h"
#include "Ohmbrewer_Publisher.h"
#include "Ohmbrewer_Screen.h"

/**
 * Constructor
 * @param id The Sprout ID to use for this piece of Equipment
 * @param pumpPin - Single speed pump will only have PowerPin
 */
Ohmbrewer::Pump::Pump(int id, int pumpPin) : Ohmbrewer::Relay(id, pumpPin) {

    registerUpdateFunction();
}

/**FIXME
 * Constructor
 * @param id The Sprout ID to use for this piece of Equipment
 * @param pumpPin - Single speed pump will only have PowerPin
 * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
 * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
 * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
 */
Ohmbrewer::Pump::Pump(int id, int pumpPin, int stopTime,
                      bool state, String currentTask) : Ohmbrewer::Relay(id, pumpPin, stopTime, state, currentTask) {

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
 * Draws information to the Rhizome's display.
 * This function is called by display().
 * @returns The time taken to run the method
 */
int Ohmbrewer::Pump::doDisplay(Ohmbrewer::Screen *screen) {
    screen->print("Pump");
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
