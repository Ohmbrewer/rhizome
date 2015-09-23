#include "Ohmbrewer_Thermostat.h"
#include "Ohmbrewer_Screen.h"

/**
 * The desired target temperature. Defaults to Celsius
 * @returns The target temperature in Celsius
 */
Ohmbrewer::Temperature* Ohmbrewer::Thermostat::getTargetTemp() const {
    return _targetTemp;
}

/**
 * Sets the target temperature
 * @param targetTemp The new target temperature in Celsius
 * @returns The time taken to run the method
 */
const int Ohmbrewer::Thermostat::setTargetTemp(const double targetTemp) {
    unsigned long start = millis();

    _targetTemp->set(targetTemp);

    return start - millis();
}

/**
 * The Thermostat's heating element
 * @returns The heating element
 */
Ohmbrewer::HeatingElement* Ohmbrewer::Thermostat::getElement() const {
    return _heatingElm;
}

/**
 * The Thermostat's temperature sensor
 * @returns The temperature sensor
 */
Ohmbrewer::TemperatureSensor* Ohmbrewer::Thermostat::getSensor() const {
    return _tempSensor;
}

/**
 * Constructor
 * @param id The Sprout ID to use for this piece of Equipment
 * @param pins The list of physical pins this Equipment is attached to
 */
Ohmbrewer::Thermostat::Thermostat(int id, std::list<int>* pins) : Ohmbrewer::Equipment(id, pins) {
    // TODO: Figure out how to properly set the HeatingElement and TemperatureSensor in the constructors
    std::list<int>* fakePins = new std::list<int>(1,-1);
    _heatingElm = new HeatingElement(1,fakePins); // This isn't right
    _tempSensor = new TemperatureSensor(1, fakePins); // Neither is this
    _targetTemp = new Temperature(0);
    registerUpdateFunction();
}

/**
 * Constructor
 * @param id The Sprout ID to use for this piece of Equipment
 * @param pins The list of physical pins this Equipment is attached to
 * @param targetTemp The new target temperature in Celsius
 */
Ohmbrewer::Thermostat::Thermostat(int id, std::list<int>* pins, const double targetTemp) : Ohmbrewer::Equipment(id, pins) {
    // TODO: Figure out how to properly set the HeatingElement and TemperatureSensor in the constructors
    std::list<int>* fakePins = new std::list<int>(1,-1);
    _heatingElm = new HeatingElement(1,fakePins); // This isn't right
    _tempSensor = new TemperatureSensor(1, fakePins); // Neither is this
    _targetTemp = new Temperature(targetTemp);
    registerUpdateFunction();
}

/**
 * Constructor
 * @param id The Sprout ID to use for this piece of Equipment
 * @param pins The list of physical pins this Equipment is attached to
 * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
 * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
 * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
 */
Ohmbrewer::Thermostat::Thermostat(int id, std::list<int>* pins, int stopTime,
                                  bool state, String currentTask) : Ohmbrewer::Equipment(id, pins, stopTime, state, currentTask) {
    // TODO: Figure out how to properly set the HeatingElement and TemperatureSensor in the constructors
    std::list<int>* fakePins = new std::list<int>(1,-1);
    _heatingElm = new HeatingElement(1,fakePins); // This isn't right
    _tempSensor = new TemperatureSensor(1, fakePins); // Neither is this
    _targetTemp = new Temperature(0);
    registerUpdateFunction();
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
Ohmbrewer::Thermostat::Thermostat(int id, std::list<int>* pins, int stopTime,
                                  bool state, String currentTask,
                                  const double targetTemp) : Ohmbrewer::Equipment(id, pins, stopTime, state, currentTask) {
    // TODO: Figure out how to properly set the HeatingElement and TemperatureSensor in the constructors
    std::list<int>* fakePins = new std::list<int>(1,-1);
    _heatingElm = new HeatingElement(1,fakePins); // This isn't right
    _tempSensor = new TemperatureSensor(1, fakePins); // Neither is this
    _targetTemp = new Temperature(targetTemp);
    registerUpdateFunction();
}

/**
 * Copy Constructor
 * @param clonee The Equipment object to copy
 */
Ohmbrewer::Thermostat::Thermostat(const Ohmbrewer::Thermostat& clonee) : Ohmbrewer::Equipment(clonee) {
    _heatingElm = clonee.getElement();
    _tempSensor = clonee.getSensor();
    _targetTemp = clonee.getTargetTemp();
    registerUpdateFunction();
}

/**
 * Destructor
 */
Ohmbrewer::Thermostat::~Thermostat() {
    delete _heatingElm;
    delete _tempSensor;
    delete _targetTemp;
}

/**
 * Overloaded << operator.
 */
// friend std::ostream& Ohmbrewer::Thermostat::operator<<( std::ostream& os, Thermostat const& thermostat);

/**
 * Specifies the interface for arguments sent to this Equipment's associated function.
 * Parses the supplied string into an array of strings for setting the Equipment's values.
 * Most likely will be called during update().
 * @param argsStr The arguments supplied as an update to the Rhizome.
 * @returns A map representing the key/value pairs for the update
 */
Ohmbrewer::Equipment::args_map_t Ohmbrewer::Thermostat::parseArgs(const String argsStr) {
    // TODO: Implement Thermostat::parseArgs
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
const int Ohmbrewer::Thermostat::setState(const bool state) {
    unsigned long start = millis();

    getElement()->setState(state);
    getSensor()->setState(state);

    return start - millis();
}

/**
 * The Equipment state. True => On, False => Off
 * @returns True => On, False => Off
 */
bool Ohmbrewer::Thermostat::getState() const {
    return (getElement()->getState() || getSensor()->getState());
}

/**
 * True if the Equipment state is On.
 * @returns Whether the Equipment is turned ON
 */
bool Ohmbrewer::Thermostat::isOn() const {
    return getState();
}

/**
 * True if the Equipment state is Off.
 * @returns Whether the Equipment is turned OFF
 */
bool Ohmbrewer::Thermostat::isOff() const {
    return !getState();
}

/**
 * Performs the Equipment's current task. Expect to use this during loop().
 * This function is called by work().
 * @returns The time taken to run the method
 */
int Ohmbrewer::Thermostat::doWork() {
    // TODO: Implement Thermostat::doWork
    return -1;
}

/**
 * Draws information to the Rhizome's display.
 * This function is called by display().
 * @returns The time taken to run the method
 */
int Ohmbrewer::Thermostat::doDisplay(Ohmbrewer::Screen *screen) {
    unsigned long start = micros();

    screen->resetTextSize();
    screen->resetTextColor();

    // Print the section title
    screen->print("== Therm. #");
    screen->print(getID());
    screen->print(" (");
    screen->writeDegree(); // Degree symbol
    screen->print("C) ==");

    // Add a wee margin
    screen->printMargin(2);

    // Print out the current temp
    displayCurrentTemp(screen);

    // Print out the target temp
    displayTargetTemp(screen);

    // Add another wee margin
    screen->printMargin(2);

    return micros() - start;
}

/**
 * Prints the temperature information for our sensors onto the touchscreen.
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Thermostat::displayCurrentTemp(Ohmbrewer::Screen *screen) {
    unsigned long start = micros();

    // If current == target, we'll default to yellow, 'cause we're golden...
    uint16_t color = screen->YELLOW;

    if(getSensor()->getTemp()->c() > getTargetTemp()->c()) {
        // Too hot
        color = screen->RED;
    } else if(getSensor()->getTemp()->c() < getTargetTemp()->c()) {
        // Too cold
        color = screen->CYAN;
    }

    displayTemp(getSensor()->getTemp(), "Current: ", color, screen);

    // Show a warning if the Heating Element is active
    if(getElement()->isOn()) {
        screen->setTextColor(screen->RED, screen->DEFAULT_BG_COLOR);
        screen->print(" ON");
    } else {
        screen->setTextColor(screen->BLACK, screen->DEFAULT_BG_COLOR);
        screen->print(" ");
        screen->writeBlock();
        screen->writeBlock();
    }
    screen->resetTextColor();

    screen->println("");

    return micros() - start;
}

/**
 * Prints the temperature information for our sensors onto the touchscreen.
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Thermostat::displayTargetTemp(Ohmbrewer::Screen *screen) {
    unsigned long start = micros();
    displayTemp(getTargetTemp(), "Target:  ", screen->YELLOW, screen);
    screen->println("");
    return micros() - start;
}

/**
 * Prints the temperature information for our sensors onto the touchscreen.
 * @param temp The temperature to display
 * @param label The text label to print to the left of the temperature
 * @param color The color of the temperature text
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Thermostat::displayTemp(const Temperature *temp, char* label, uint16_t color, Ohmbrewer::Screen *screen) {
    unsigned long start = micros();
    char tempStr [10];
    temp->toStrC(tempStr);

    // Print the label
    screen->resetTextColor();
    screen->print(" "); // We want a little margin
    screen->print(label);

    // Print out the target temp
    screen->setTextColor(color, screen->DEFAULT_BG_COLOR);
    screen->print(tempStr);

    screen->resetTextColor();

    return micros() - start;
}

/**
 * Publishes updates to Ohmbrewer, etc.
 * This function is called by update().
 * @param args The argument string passed into the Particle Cloud
 * @returns The time taken to run the method
 */
int Ohmbrewer::Thermostat::doUpdate(String* args) {
    // TODO: Implement Thermostat::doUpdate
    return -1;
}

/**
 * Reports which of the Rhizome's pins are occupied by the
 * Equipment, forming a logical Sprout.
 * @returns The list of physical pins that the Equipment is connected to.
 */
std::list<int>* Ohmbrewer::Thermostat::whichPins() const {
    // TODO: Implement Thermostat::whichPins
    //return {&_pins, &getElement(), &getSensor()};
    return _pins;
}

