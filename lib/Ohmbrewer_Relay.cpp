#include "Ohmbrewer_Relay.h"
#include "Ohmbrewer_Screen.h"

/**
 * Constructor
 * @param id The Sprout ID to use for this piece of Equipment
 * @param powerPin - The power pin - on/off line. Digital pin number X.
 * @param controlPin - The Control pin - Data/speed/power level Digital pin number X.
 */
Ohmbrewer::Relay::Relay(int id, int powerPin, int controlPin) : Ohmbrewer::Equipment(id) {
    _powerPin = powerPin;
    _controlPin = controlPin;
    // For now, we will not automatically add a Spark.function to Relays as
    // it's used mostly as a base class and our subclasses call the Relay constructor. If we can find a safe way to
    // determine if a function for actual Relay types should be added, then we'll change that.
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
Ohmbrewer::Relay::Relay(int id, int powerPin, int controlPin, int stopTime,
                      bool state, String currentTask) : Ohmbrewer::Equipment(id, stopTime, state, currentTask) {
    _powerPin = powerPin;
    _controlPin = controlPin;
    // For now, we will not automatically add a Spark.function to Relays as
    // it's used mostly as a base class and our subclasses call the Relay constructor. If we can find a safe way to
    // determine if a function for actual Relay types should be added, then we'll change that.
}

/**
 * Copy Constructor
 * @param clonee The Equipment object to copy
 */
Ohmbrewer::Relay::Relay(const Relay& clonee) : Ohmbrewer::Equipment(clonee) {
    _powerPin = clonee.getPowerPin();
    _controlPin = clonee.getControlPin();
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
 * The power pin - on/off line
 * hardware destinations (hardware switches - DPDT, main power)
 * @returns The pin number in use for this piece of Equipment
 */
int Ohmbrewer::Relay::getPowerPin() const{
    return _powerPin;
}

/**
 * Sets the Digital pin for the power pin.
 * @param pinNum Dx
 * @returns The time taken to run the method
 */
const int Ohmbrewer::Relay::setPowerPin(const int pinNum) {
    unsigned long start = millis();

    _powerPin = pinNum;

    return start - millis();
}


/**
 * The Control pin - Data/speed/power level
 * hardware destinations (SSR, databus(temp sensor), pump speed control)
 * @returns The pin number in use for this piece of Equipment
 */
int Ohmbrewer::Relay::getControlPin() const {
    return _controlPin;
}

/**
 * Sets the Digital pin for the control pin.
 * @param pinNum Dx
 * @returns The time taken to run the method
 */
const int Ohmbrewer::Relay::setControlPin(const int pinNum) {
    unsigned long start = millis();

    _controlPin = pinNum;

    return start - millis();
}

/**
 * Specifies the interface for arguments sent to this Equipment's associated function.
 * Parses the supplied string into an array of strings for setting the Equipment's values.
 * Most likely will be called during update().
 * @param argsStr The arguments supplied as an update to the Rhizome.
 * @param result A map representing the key/value pairs for the update
 */
void Ohmbrewer::Relay::parseArgs(const String &argsStr, Ohmbrewer::Equipment::args_map_t &result) {
    // Nothing special to parse out for this class.
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
 * Switches the power pin on or off for a generic relay, subclasses can utilize the controlPin
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
 * @param argsMap A map representing the key/value pairs for the update
 * @returns The time taken to run the method
 */
int Ohmbrewer::Relay::doUpdate(String &args, Ohmbrewer::Equipment::args_map_t &argsMap) {
    unsigned long start = millis();
    // Nothing to do as it is...
    return millis() - start;
}

/**
 * Reports which of the Rhizome's pins are occupied by the
 * Equipment, forming a logical Sprout.
 * @param pins The list of physical pins that the Equipment is connected to.
 */
void Ohmbrewer::Relay::whichPins(std::list<int>* pins){

    pins->push_back(getPowerPin());
    pins->push_back(getControlPin());

}

