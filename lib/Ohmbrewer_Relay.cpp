#include "Ohmbrewer_Relay.h"
#include "Ohmbrewer_Screen.h"

/**
 * Adds the update function for the instance.
 */
void Ohmbrewer::Relay::addUpdateFunction() {
    String updateFunction;
    getUpdateFunctionName(&updateFunction);
    Spark.function<Ohmbrewer::Relay>(updateFunction.c_str(), &Ohmbrewer::Relay::update, this);
}

/**
 * Constructor
 * @param id The Sprout ID to use for this piece of Equipment
 * @param pins The list of physical pins this Equipment is attached to
 */
Ohmbrewer::Relay::Relay(int id, std::list<int>* pins) : Ohmbrewer::Equipment(id, pins) {
    _type = "relay";
    // For now, we will not automatically add a Spark.function to Relays as
    // it's used mostly as a base class and our subclasses call the Relay constructor. If we can find a safe way to
    // determine if a function for actual Relay types should be added, then we'll change that.
}

/**
 * Constructor
 * @param id The Sprout ID to use for this piece of Equipment
 * @param pins The list of physical pins this Equipment is attached to
 * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
 * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
 * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
 */
Ohmbrewer::Relay::Relay(int id, std::list<int>* pins, int stopTime,
                      bool state, String currentTask) : Ohmbrewer::Equipment(id, pins, stopTime, state, currentTask) {
    _type = "relay";
    // For now, we will not automatically add a Spark.function to Relays as
    // it's used mostly as a base class and our subclasses call the Relay constructor. If we can find a safe way to
    // determine if a function for actual Relay types should be added, then we'll change that.
}

/**
 * Copy Constructor
 * @param clonee The Equipment object to copy
 */
Ohmbrewer::Relay::Relay(const Relay& clonee) : Ohmbrewer::Equipment(clonee) {
    // This has probably already been set, but maybe clonee is a more complicated child class...
    _type = "relay";
    // For now, we will not automatically add a Spark.function to Relays as
    // it's used mostly as a base class and our subclasses call the Relay constructor. If we can find a safe way to
    // determine if a function for actual Relay types should be added, then we'll change that.
}

/**
 * Destructor
 */
Ohmbrewer::Relay::~Relay() {
    // Nothing to do here...
}

/**
 * Overloaded << operator.
 */
// friend std::ostream& operator<<( std::ostream& os, Relay const& relay);

/**
 * Specifies the interface for arguments sent to this Equipment's associated function. 
 * Parses the supplied string into an array of strings for setting the Equipment's values.
 * Most likely will be called during update().
 * @param argsStr The arguments supplied as an update to the Rhizome.
 * @returns A map representing the key/value pairs for the update
 */
Ohmbrewer::Equipment::args_map_t Ohmbrewer::Relay::parseArgs(const String argsStr) {
    args_map_t result;
    char* params = new char[argsStr.length() + 1];
    strcpy(params, argsStr.c_str());

    // Parse the parameters
    String id          = String(strtok(params, ","));
    String currentTask = String(strtok(params, ","));
    String state       = String(strtok(NULL, ","));
    String stopTime    = String(strtok(NULL, ","));

    // Save them to the map
    result[String("id")] = id;
    result[String("current_task")] = currentTask;
    result[String("state")] = state;
    result[String("stopTime")] = stopTime;

    // Clear out that dynamically allocated buffer
    delete params;

    return result;
}

/**
 * Sets the Equipment state. True => On, False => Off
 * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
 * @returns The time taken to run the method
 */
const int Ohmbrewer::Relay::setState(const bool state) {
    unsigned long start = millis();

    _state = state;

    return start - millis();
}

/**
 * The Equipment state. True => On, False => Off
 * @returns True => On, False => Off
 */
bool Ohmbrewer::Relay::getState() const {
    return _state;
}

/**
 * True if the Equipment state is On.
 * @returns Whether the Equipment is turned ON
 */
bool Ohmbrewer::Relay::isOn() const {
    return _state;
}

/**
 * True if the Equipment state is Off.
 * @returns Whether the Equipment is turned OFF
 */
bool Ohmbrewer::Relay::isOff() const {
    return !_state;
}

/**
 * Performs the Equipment's current task. Expect to use this during loop().
 * This function is called by work().
 * @returns The time taken to run the method
 */
int Ohmbrewer::Relay::doWork() {
    // TODO: Implement Relay::doWork
    return -1;
}

/**
 * Draws information to the Rhizome's display.
 * This function is called by display().
 * @returns The time taken to run the method
 */
int Ohmbrewer::Relay::doDisplay(Ohmbrewer::Screen *screen) {
    unsigned long start = micros();
    char relay_id[2];

    // Print a fancy identifier
    screen->print(" [");
    screen->setTextColor(screen->WHITE, screen->DEFAULT_BG_COLOR);

    sprintf(relay_id,"%d", _id);
    screen->print(relay_id);

    screen->resetTextColor();
    screen->print("]:");

    // Print the state
    if (_state){
        screen->setTextColor(screen->YELLOW, screen->DEFAULT_BG_COLOR);
        screen->println(" ON ");
    } else {
        screen->setTextColor(screen->RED, screen->DEFAULT_BG_COLOR);
        screen->println(" OFF");
    }

    screen->resetTextColor();

    return micros() - start;
}

/**
 * Publishes updates to Ohmbrewer, etc.
 * This function is called by update().
 * @param args The argument string passed into the Particle Cloud
 * @returns The time taken to run the method
 */
int Ohmbrewer::Relay::doUpdate(String args) {
    // TODO: Implement Relay::doUpdate
    return -1;
}

/**
 * Reports which of the Rhizome's pins are occupied by the
 * Equipment, forming a logical Sprout.
 * @returns The list of physical pins that the Equipment is connected to.
 */
std::list<int>* Ohmbrewer::Relay::whichPins() const {
    return _pins;
}

