#include "Ohmbrewer_RIMS.h"

/**
 * The Tube thermostat
 */
Ohmbrewer::Thermostat* Ohmbrewer::RIMS::getTube() const {
    return _tube;
}

/**
 * The temperature sensor located in the tun
 */
Ohmbrewer::TemperatureSensor* Ohmbrewer::RIMS::getTunSensor() const {
    return _tunSensor;
}

/**
 * The recirculation pump between the tun and the tube
 */
Ohmbrewer::Pump* Ohmbrewer::RIMS::getRecirculator() const {
    return _recirc;
}


/**
 * Constructors
 */
Ohmbrewer::RIMS::RIMS(int id, int* pins) : Ohmbrewer::Equipment(id, pins) {
    // TODO: Figure out how to properly set the components of the RIMS the constructors
    int fakePins[2] = {1,2};
    _tube = new Thermostat(1, fakePins);
    _tunSensor = new TemperatureSensor(1, fakePins);
    _recirc = new Pump(1,fakePins);
}

Ohmbrewer::RIMS::RIMS(int id, int* pins, int stopTime,
                      bool state, char* currentTask) : Ohmbrewer::Equipment(id, pins, stopTime, state, currentTask) {
    // TODO: Figure out how to properly set the components of the RIMS the constructors
    int fakePins[2] = {1,2};
    _tube = new Thermostat(1, fakePins);
    _tunSensor = new TemperatureSensor(1, fakePins);
    _recirc = new Pump(1,fakePins);
}

Ohmbrewer::RIMS::RIMS(int id, int* pins, int stopTime,
                      bool state, char* currentTask, const double targetTemp) : Ohmbrewer::Equipment(id, pins, stopTime, state, currentTask) {
    // TODO: Figure out how to properly set the components of the RIMS the constructors
    int fakePins[2] = {1,2};
    _tube = new Thermostat(1, fakePins, targetTemp);
    _tunSensor = new TemperatureSensor(1, fakePins);
    _recirc = new Pump(1,fakePins);
}

/**
 * Copy constructor
 */
Ohmbrewer::RIMS::RIMS(const Ohmbrewer::RIMS& clonee) : Ohmbrewer::Equipment((Equipment)clonee) {
    _tube = clonee.getTube();
    _tunSensor = clonee.getTunSensor();
    _recirc = clonee.getRecirculator();
}

/**
 * Destructor
 */
Ohmbrewer::RIMS::~RIMS() {
    delete _recirc;
    delete _tube;
    delete _tunSensor;
}

/**
 * Overloaded << operator.
 */
// friend std::ostream& operator<<( std::ostream& os, RIMS const& rims);

/**
 * Specifies the interface for arguments sent to this Equipment's associated function.
 * Parses the supplied string into an array of strings for setting the Equipment's values.
 * Most likely will be called during update().
 */
Ohmbrewer::Equipment::args_map_t Ohmbrewer::RIMS::parseArgs(const char* argsStr) {
    // TODO: Implement RIMS::parseArgs
    args_map_t placeholder;
    placeholder[String("fixme")] = String("nonononononono");
    return placeholder;
}

/**
 * Sets the Equipment state. True => On, False => Off
 * This turns *EVERYTHING* on, so watch out. You may want to turn the components on individually instead.
 */
const int Ohmbrewer::RIMS::setState(const bool state) {
    getTube()->setState(state);
    getTunSensor()->setState(state);
    getRecirculator()->setState(state);
    return 0;
}

/**
 * The Equipment state. True => On, False => Off
 */
bool Ohmbrewer::RIMS::getState() const {
    return getRecirculator()->getState() || getTunSensor()->getState() || getTube()->getState();
}

/**
 * True if the Equipment state is On.
 */
bool Ohmbrewer::RIMS::isOn() const {
    return getState();
}

/**
 * True if the Equipment state is Off.
 */
bool Ohmbrewer::RIMS::isOff() const {
    return !getState();
}

/**
 * Performs the Equipment's current task. Expect to use this during loop().
 * This function is called by work().
 */
int Ohmbrewer::RIMS::doWork() {
    // TODO: Implement RIMS::doWork
    return -1;
}

/**
 * Draws information to the Rhizome's display.
 * This function is called by display().
 */
int Ohmbrewer::RIMS::doDisplay() {
    // TODO: Implement RIMS::doDisplay
    return -1;
}

/**
 * Publishes updates to Ohmbrewer, etc.
 * This function is called by update().
 */
int Ohmbrewer::RIMS::doUpdate() {
    // TODO: Implement RIMS::doUpdate
    return -1;
}

/**
 * Reports which of the Rhizome's pins are occupied by the
 * Equipment, forming a logical Sprout.
 */
int* Ohmbrewer::RIMS::whichPins() const {
    // TODO: Implement RIMS::whichPins
    //return {&_pins, &getTube()->whichPins(), &getTunSensor()->whichPins(), &getRecirculator()->whichPins()};
    return NULL;
}

