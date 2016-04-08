#include "Ohmbrewer_Temperature_Sensor.h"
#include "Ohmbrewer_Screen.h"
#include "Ohmbrewer_Onewire.h"
#include "Ohmbrewer_Publisher.h"


/**
 * Constructor
 * @param busPin The Digital Pin that the temp probes are attached to. NOTE: for ds18b20 should always be D0
 */
Ohmbrewer::TemperatureSensor::TemperatureSensor(Probe* probe) {
    _probe = probe;                 //For now all probes are all onewire
    _lastReading = new Temperature(-69);
    _lastReadTime = Time.now();
//    registerUpdateFunction();
}

/**
 * Constructor
 * @param busPin The Digital Pin that the temp probes are attached to. NOTE: for ds18b20 should always be D0
 * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
 * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
 * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
 */
Ohmbrewer::TemperatureSensor::TemperatureSensor(Probe* probe, int stopTime, bool state, String currentTask) : Ohmbrewer::Equipment(stopTime, state, currentTask) {
    _probe = probe;
    _lastReading = new Temperature(-69);
    _lastReadTime = Time.now();
//    registerUpdateFunction();
}

/**
 * Copy Constructor
 * @param clonee The TemperatureSensor object to copy
 */
Ohmbrewer::TemperatureSensor::TemperatureSensor(const TemperatureSensor& clonee) : Ohmbrewer::Equipment(clonee) {
    _probe = clonee.getProbe();
    _lastReading = clonee.getTemp();
    _lastReadTime = Time.now();
//    registerUpdateFunction();
}

/**
 * Destructor
 */
Ohmbrewer::TemperatureSensor::~TemperatureSensor() {
    delete _lastReading;
    delete _probe;
}

/**
 * The Equipment ID
 * @returns The Sprout ID to use for this piece of Equipment
 */
int Ohmbrewer::TemperatureSensor::getID() const {
    return _probe->getID();
}

/**
 * The last temperature read by the sensor. Currently returns in Celsius.
 * @returns A pointer to the Temperature object representing the last temperature reading
 */
Ohmbrewer::Temperature* Ohmbrewer::TemperatureSensor::getTemp() const {
    return _lastReading;
}

/**
 * The last time the temperature was read by the sensor
 * @returns The last temperature reading time
 */
int Ohmbrewer::TemperatureSensor::getLastReadTime() const {
    return _lastReadTime;
}

/**
 * Sets the last time the temperature was read by the sensor
 * @param lastReadTime The last temperature reading time
 */
const int Ohmbrewer::TemperatureSensor::setLastReadTime(const int lastReadTime) {
    _lastReadTime = lastReadTime;
    return 0;
}

/**
 * @returns the probe for this sensor
 */
Ohmbrewer::Probe* Ohmbrewer::TemperatureSensor::getProbe() const{
    return _probe;
}

/**
 * The Bus pin - Data input line
 * onewire protocol input location for DS18b20
 * @returns The pin number in use for this piece of Equipment
 */
int Ohmbrewer::TemperatureSensor::getBusPin() const{
    return _probe->getPin();
    //TODO will need work once more probe subclasses are added
}

/**
 * Specifies the interface for arguments sent to this TemperatureSensor's associated function.
 * Parses the supplied string into an array of strings for setting the TemperatureSensor's values.
 * Most likely will be called during update().
 * @param args The arguments supplied as an update to the Rhizome.
 * @param argsMap A map representing the key/value pairs for the update
 */
void Ohmbrewer::TemperatureSensor::parseArgs(const String &args, Ohmbrewer::Equipment::args_map_t &argsMap) {
    // Nothing special to parse out for this class.
}

/**
 * Sets the TemperatureSensor state. True => On, False => Off
 * @param state Whether the TemperatureSensor is ON (or OFF). True => ON, False => OFF
 * @returns The time taken to run the method
 */
const int Ohmbrewer::TemperatureSensor::setState(const bool state) {
    unsigned long start = millis();

    _state = state;

    return start - millis();
}

/**
 * The TemperatureSensor state. True => On, False => Off
 * @returns True => On, False => Off
 */
bool Ohmbrewer::TemperatureSensor::getState() const {
    return _state;
}

/**
 * True if the TemperatureSensor state is On.
 * @returns Whether the TemperatureSensor is turned ON
 */
bool Ohmbrewer::TemperatureSensor::isOn() const {
    return _state;
}

/**
 * True if the TemperatureSensor state is Off.
 * @returns Whether the TemperatureSensor is turned OFF
 */
bool Ohmbrewer::TemperatureSensor::isOff() const {
    return !_state;
}

/**
 * Performs the TemperatureSensor's current task. Expect to use this during loop().
 * This function is called by work().
 *
 * This analyzes the connected DS18B20 probes and updates temperature.from_c with the Celsius value.
 *
 *
 * @returns The time taken to run the method
 */
int Ohmbrewer::TemperatureSensor::doWork() {
    int startTime = millis();
    getTemp()->fromC(_probe->getReading());
    return (millis()-startTime);
}

/**
 * Draws information to the Rhizome's display.
 * This function is called by display().
 * @returns The time taken to run the method
 */
int Ohmbrewer::TemperatureSensor::doDisplay(Ohmbrewer::Screen *screen) {
    unsigned long start = micros();
    char relay_id[2];
    char tempStr [10];

    sprintf(relay_id,"%d", getId());
    if(screen->getSettings()->isTempUnitCelsius()){
        getTemp()->toStrC(tempStr);
    } else {
        getTemp()->toStrF(tempStr);
    }


    screen->resetTextColor();
    // Print a fancy identifier
    screen->print("[");

    screen->setTextColor(screen->WHITE, screen->DEFAULT_BG_COLOR);
    screen->print(relay_id);
    screen->resetTextColor();

    screen->print("]: ");

    // Print the temperature
    screen->setTextColor(screen->WHITE, screen->DEFAULT_BG_COLOR);
    screen->println(tempStr);

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
int Ohmbrewer::TemperatureSensor::doUpdate(String &args, Ohmbrewer::Equipment::args_map_t &argsMap) {
    unsigned long start = millis();
    // Nothing to do as it is...
    return millis() - start;
}

/**
 * Reports which of the Rhizome's pins are occupied by the
 * Equipment, forming a logical Sprout.
 * @param pins The list of physical pins that the TemperatureSensor is connected to.
 */
void Ohmbrewer::TemperatureSensor::whichPins(std::list<int>* pins) {
    pins->push_back(_probe->getPin());
}

/**
 * Publishes the latest reading
 */
void Ohmbrewer::TemperatureSensor::publishSensorReading() {
    Publisher pub = Publisher(new String(getStream()),
                              String("temperature"),
                              String(getTemp()->c()));
    pub.publish();
}
