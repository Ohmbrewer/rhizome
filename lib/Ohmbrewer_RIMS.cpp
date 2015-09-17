#include "Ohmbrewer_RIMS.h"

/**
 * The Tube thermostat
 * @returns The Thermostat object representing the RIMS tube elements
 */
Ohmbrewer::Thermostat* Ohmbrewer::RIMS::getTube() const {
    return _tube;
}

/**
 * The temperature sensor located in the tun
 * @returns The Temperature Sensor object representing the sensor located in the mash tun
 */
Ohmbrewer::TemperatureSensor* Ohmbrewer::RIMS::getTunSensor() const {
    return _tunSensor;
}

/**
 * The recirculation pump between the tun and the tube
 * @returns The Pump object representing the recirculation pump
 */
Ohmbrewer::Pump* Ohmbrewer::RIMS::getRecirculator() const {
    return _recirc;
}

/**
 * Constructor
 * @param id The Sprout ID to use for this piece of Equipment
 * @param pins The list of physical pins this Equipment is attached to
 */
Ohmbrewer::RIMS::RIMS(int id, std::list<int>* pins) : Ohmbrewer::Equipment(id, pins) {
    // TODO: Figure out how to properly set the components of the RIMS the constructors
    std::list<int>* fakePins = new std::list<int>(1,-1);
    _tube = new Thermostat(1, fakePins);
    _tunSensor = new TemperatureSensor(1, fakePins);
    _recirc = new Pump(1,fakePins);
    _type = "rims";
}

/**
 * Constructor
 * @param id The Sprout ID to use for this piece of Equipment
 * @param pins The list of physical pins this Equipment is attached to
 * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
 * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
 * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
 */
Ohmbrewer::RIMS::RIMS(int id, std::list<int>* pins, int stopTime,
                      bool state, String currentTask) : Ohmbrewer::Equipment(id, pins, stopTime, state, currentTask) {
    // TODO: Figure out how to properly set the components of the RIMS the constructors
    std::list<int>* fakePins = new std::list<int>(1,-1);
    _tube = new Thermostat(1, fakePins);
    _tunSensor = new TemperatureSensor(1, fakePins);
    _recirc = new Pump(1,fakePins);
    _type = "rims";
}

/**
 * Constructor
 * @param id The Sprout ID to use for this piece of Equipment
 * @param pins The list of physical pins this Equipment is attached to
 * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
 * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
 * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
 * @param targetTemp The new target temperature in Celsius
 */
Ohmbrewer::RIMS::RIMS(int id, std::list<int>* pins, int stopTime,
                      bool state, String currentTask, const double targetTemp) : Ohmbrewer::Equipment(id, pins, stopTime, state, currentTask) {
    // TODO: Figure out how to properly set the components of the RIMS the constructors
    std::list<int>* fakePins = new std::list<int>(1,-1);
    _tube = new Thermostat(1, fakePins, targetTemp);
    _tunSensor = new TemperatureSensor(1, fakePins);
    _recirc = new Pump(1,fakePins);
    _type = "rims";
}

/**
 * Copy constructor
 * @param clonee The Equipment object to copy
 */
Ohmbrewer::RIMS::RIMS(const Ohmbrewer::RIMS& clonee) : Ohmbrewer::Equipment(clonee) {
    _tube = clonee.getTube();
    _tunSensor = clonee.getTunSensor();
    _recirc = clonee.getRecirculator();
    _type = "rims";
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
 * @param argsStr The arguments supplied as an update to the Rhizome.
 * @returns A map representing the key/value pairs for the update
 */
Ohmbrewer::Equipment::args_map_t Ohmbrewer::RIMS::parseArgs(const String argsStr) {
    // TODO: Implement RIMS::parseArgs
    args_map_t placeholder;
    placeholder[String("fixme")] = String("nonononononono");
    return placeholder;
}

/**
 * Sets the Equipment state. True => On, False => Off
 * This turns *EVERYTHING* on, so watch out. You may want to turn the components on individually instead.
 * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
 * @returns The time taken to run the method
 */
const int Ohmbrewer::RIMS::setState(const bool state) {
    unsigned long start = millis();

    getTube()->setState(state);
    getTunSensor()->setState(state);
    getRecirculator()->setState(state);

    return start - millis();
}

/**
 * The Equipment state. True => On, False => Off
 * @returns True => On, False => Off
 */
bool Ohmbrewer::RIMS::getState() const {
    return getRecirculator()->getState() || getTunSensor()->getState() || getTube()->getState();
}

/**
 * True if the Equipment state is On.
 * @returns Whether the Equipment is turned ON
 */
bool Ohmbrewer::RIMS::isOn() const {
    return getState();
}

/**
 * True if the Equipment state is Off.
 * @returns Whether the Equipment is turned OFF
 */
bool Ohmbrewer::RIMS::isOff() const {
    return !getState();
}

/**
 * Performs the Equipment's current task. Expect to use this during loop().
 * This function is called by work().
 * @returns The time taken to run the method
 */
int Ohmbrewer::RIMS::doWork() {
    // TODO: Implement RIMS::doWork
    return -1;
}

/**
 * Draws information to the Rhizome's display.
 * This function is called by display().
 * @returns The time taken to run the method
 */
int Ohmbrewer::RIMS::doDisplay(Ohmbrewer::Screen *screen) {
    // TODO: Implement RIMS::doDisplay
    return -1;
}

/**
 * Publishes updates to Ohmbrewer, etc.
 * This function is called by update().
 * @returns The time taken to run the method
 */
int Ohmbrewer::RIMS::doUpdate() {
    // TODO: Implement RIMS::doUpdate
    return -1;
}

/**
 * Reports which of the Rhizome's pins are occupied by the
 * Equipment, forming a logical Sprout.
 * @returns The list of physical pins that the Equipment is connected to.
 */
std::list<int>* Ohmbrewer::RIMS::whichPins() const {
    // TODO: Implement RIMS::whichPins
    //return {&_pins, &getTube()->whichPins(), &getTunSensor()->whichPins(), &getRecirculator()->whichPins()};
    return _pins;
}

