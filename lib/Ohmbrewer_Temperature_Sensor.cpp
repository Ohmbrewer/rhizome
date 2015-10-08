#include "Ohmbrewer_Temperature_Sensor.h"
#include "Ohmbrewer_Screen.h"

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
 * Constructor
 * @param id The Sprout ID to use for this piece of Equipment
 * @param busPin The Digital Pin that the temp probes are attached to. NOTE: for ds18b20 should always be D0
 */
Ohmbrewer::TemperatureSensor::TemperatureSensor(int id, int busPin) : Ohmbrewer::Equipment(id) {
    _busPin = busPin;
    ow_setPin(D0);
    _lastReading = new Temperature(0);
    _lastReadTime = Time.now();
    registerUpdateFunction();
//    _probeId;
}

/**
 * Constructor
 * @param id The Sprout ID to use for this piece of Equipment
 * @param busPin The Digital Pin that the temp probes are attached to. NOTE: for ds18b20 should always be D0
 * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
 * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
 * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
 */
Ohmbrewer::TemperatureSensor::TemperatureSensor(int id,  int busPin, int stopTime, bool state, String currentTask) : Ohmbrewer::Equipment(id, stopTime, state, currentTask) {
    _busPin = busPin;
    ow_setPin(D0);
    _lastReading = new Temperature(0);
    _lastReadTime = Time.now();
//    _probeId;
    registerUpdateFunction();

}

/**
 * Copy Constructor
 * @param clonee The TemperatureSensor object to copy
 */
Ohmbrewer::TemperatureSensor::TemperatureSensor(const TemperatureSensor& clonee) : Ohmbrewer::Equipment(clonee) {
    _busPin = clonee.getBusPin();
    ow_setPin(D0);                      //needed?
    _lastReading = clonee.getTemp();
    _lastReadTime = Time.now();
//    _probeId;
    registerUpdateFunction();
}

/**
 * Destructor
 */
Ohmbrewer::TemperatureSensor::~TemperatureSensor() {
    delete _lastReading;
}

/**
 * Sets the probeID
 * @param - id char[] of the ds18b20 probe
 */
void Ohmbrewer::TemperatureSensor::setProbeId(char id[8]){
    _probeId = id;
}
/**
 * reads current probes connected to busPin
 * @param - sensors array to update with ID values of all(max10) the connected ds18b20 probes
 * @returns - number of sensors discovered
 */
int Ohmbrewer::TemperatureSensor::findProbeIds(uint8_t sensors[80]){
    return (int)ow_search_sensors(10, sensors);
}

/**
 * The Bus pin - Data input line
 * onewire protocol input location for DS18b20
 * @returns The pin number in use for this piece of Equipment
 */
int Ohmbrewer::TemperatureSensor::getBusPin() const{
    return _busPin;
}

/**
 * Sets the Digital pin for the data Bus.
 * @param pinNum Dx
 * @returns The time taken to run the method
 */
const int Ohmbrewer::TemperatureSensor::setBusPin(const int pinNum) {
    unsigned long start = millis();

    _busPin = pinNum;

    return start - millis();
}

/**
 * Overloaded << operator.
 */
// friend std::ostream& operator<<( std::ostream& os, Pump const& pump);

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
    //char uid[8];                   //need the unique ID of the probe to search for.
    uint8_t sensors[80];

    int startTime = millis();        //starting time of reading temperature data
    uint8_t subzero, cel, celFracBits;        //local vars
    char msg[100];
    double tempC;
    //log("Starting measurement");
    //Asks all DS18x20 devices to start temperature measurement, takes up to 750ms at max resolution
    DS18X20_start_meas( DS18X20_POWER_PARASITE, NULL );
    //If your code has other tasks, you can store the timestamp instead and return when a second has passed.
    delay(1000);                                        //TODO change / manage this delay
    uint8_t numSensors = ow_search_sensors(10, sensors);
    //sprintf(msg, "Found %i sensors", numSensors);
    //log(msg);

    /*
     * This section is where we can filter for the desired probe UID and only report that one. for now we will simply
     * state that there is only one and use sensors[0] as the probe reading.
     */

    if ( DS18X20_read_meas( &sensors[0], &subzero, &cel, &celFracBits) == DS18X20_OK ) {
        char sign = (subzero) ? '-' : '+';
        int frac = celFracBits*DS18X20_FRACCONV;
        tempC = (double)cel;
        if (sign == '-'){
            tempC = tempC * -1;
        }
        tempC = tempC + (.0001*(double)frac);
        getTemp()->fromC(tempC);
    }

    ///////////////Code below replaces above block when infrastructure is ready/////////////////
/*
    // filters for a desired probe.
    for (uint8_t i=0; i<numSensors; i++){
        //current probe ID
        char probeId[8] = {
				sensors[(i*OW_ROMCODE_SIZE)+0],
				sensors[(i*OW_ROMCODE_SIZE)+1],
				sensors[(i*OW_ROMCODE_SIZE)+2],
				sensors[(i*OW_ROMCODE_SIZE)+3],
				sensors[(i*OW_ROMCODE_SIZE)+4],
				sensors[(i*OW_ROMCODE_SIZE)+5],
				sensors[(i*OW_ROMCODE_SIZE)+6],
				sensors[(i*OW_ROMCODE_SIZE)+7]};

        if (sensors[i*OW_ROMCODE_SIZE+0] == 0x10 || sensors[i*OW_ROMCODE_SIZE+0] == 0x28) //0x10=DS18S20, 0x28=DS18B20
        {
            // if current probe matches the probe we are looking for
            if (_probeId == probeId){
                if ( DS18X20_read_meas( &sensors[0], &subzero, &cel, &celFracBits) == DS18X20_OK ) {
                    char sign = (subzero) ? '-' : '+';
                    int frac = celFracBits*DS18X20_FRACCONV;
                    tempC = (double)cel;
                    tempC = tempC + (.0001*(double)frac);
                    getTemp()->fromC(tempC);
                }
                else {
                    if (i == numSensors - 1) {
                        sprintf(msg, "Sensor %s not connected", _probeId); //TODO inject probe ID
                        //log(msg);
                        //getTemp()->fromC(-69);
                    }
                }
            }
        }
    }
*/


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

    sprintf(relay_id,"%d", _id);
    getTemp()->toStrC(tempStr);

    // Print a fancy identifier
    screen->print(" [");

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

    pins->push_back(getBusPin());


}

/*
 * publish to the log
 */
void log(char* msg)
{
    Spark.publish("log", msg);
    delay(500);
}