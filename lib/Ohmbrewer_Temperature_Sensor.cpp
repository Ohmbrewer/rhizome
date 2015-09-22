#include "Ohmbrewer_Temperature_Sensor.h"
#include "Ohmbrewer_Screen.h"

/**
 * The last temperature read by the sensor. Currently returns in Celsius.
 * @returns The last temperature reading
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
 * @param pins The list of physical pins this Equipment is attached to
 */
Ohmbrewer::TemperatureSensor::TemperatureSensor(int id, std::list<int>* pins) : Ohmbrewer::Equipment(id, pins) {
    _lastReading = new Temperature(0);
    _lastReadTime = Time.now();
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
Ohmbrewer::TemperatureSensor::TemperatureSensor(int id, std::list<int>* pins, int stopTime, bool state, String currentTask) : Ohmbrewer::Equipment(id, pins, stopTime, state, currentTask) {
    _lastReading = new Temperature(0);
    _lastReadTime = Time.now();
    registerUpdateFunction();
}

/**
 * Copy Constructor
 * @param clonee The Equipment object to copy
 */
Ohmbrewer::TemperatureSensor::TemperatureSensor(const TemperatureSensor& clonee) : Ohmbrewer::Equipment(clonee) {
    _lastReading = clonee.getTemp();
    _lastReadTime = Time.now();
    registerUpdateFunction();
}

/**
 * Destructor
 */
Ohmbrewer::TemperatureSensor::~TemperatureSensor() {
    delete _lastReading;
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
Ohmbrewer::Equipment::args_map_t Ohmbrewer::TemperatureSensor::parseArgs(const String argsStr) {
    return Equipment::parseArgs(argsStr);
}

/**
 * Sets the Equipment state. True => On, False => Off
 * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
 * @returns The time taken to run the method
 */
const int Ohmbrewer::TemperatureSensor::setState(const bool state) {
    unsigned long start = millis();

    _state = state;

    return start - millis();
}

/**
 * The Equipment state. True => On, False => Off
 * @returns True => On, False => Off
 */
bool Ohmbrewer::TemperatureSensor::getState() const {
    return _state;
}

/**
 * True if the Equipment state is On.
 * @returns Whether the Equipment is turned ON
 */
bool Ohmbrewer::TemperatureSensor::isOn() const {
    return _state;
}

/**
 * True if the Equipment state is Off.
 * @returns Whether the Equipment is turned OFF
 */
bool Ohmbrewer::TemperatureSensor::isOff() const {
    return !_state;
}

/**
 * Performs the Equipment's current task. Expect to use this during loop().
 * This function is called by work().
 *
 * This analyzes the connected DS18B20 probes and updates temperature.from_c with the Celsius value.
 *
 *
 * @returns The time taken to run the method
 */
int Ohmbrewer::TemperatureSensor::doWork() {
    char uid[8];                                //need the unique ID of the probe to search for.
    int start_time = millis();        //starting time of reading temperature data
    uint8_t subzero, cel, cel_frac_bits;        //local vars
    char msg[100];
    double temp_c;
    log("Starting measurement");
    //Asks all DS18x20 devices to start temperature measurement, takes up to 750ms at max resolution
    DS18X20_start_meas( DS18X20_POWER_PARASITE, NULL );
    //If your code has other tasks, you can store the timestamp instead and return when a second has passed.
    delay(1000);

    /*
     * This section is where we can filter for the desired probe UID and only report that one. for now we will simply
     * state that there is only one and use sensors[0] as the probe reading.
     */
    uint8_t numsensors = ow_search_sensors(10, sensors);
    sprintf(msg, "Found %i sensors", numsensors);
    log(msg);
    if ( DS18X20_read_meas( &sensors[0], &subzero, &cel, &cel_frac_bits) == DS18X20_OK ) {
        char sign = (subzero) ? '-' : '+';
        int frac = cel_frac_bits*DS18X20_FRACCONV;
        temp_c = (double)cel;
       // temp_c = temp_c + (10**(-4)*(double)frac);  //TODO integrate fraction to end of temp_c ??
        Temperature::fromC(temp_c); //TODO still need access to a specific temperature object to update
    }
    /*//scanning code

    uint8_t numsensors = ow_search_sensors(10, sensors);
    sprintf(msg, "Found %i sensors", numsensors);
    log(msg);


    for (uint8_t i=0; i<numsensors; i++){
     //THIS NEEDS WORK STILL
        //current probe ID
        char probe_id[8] = {
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
            if (uid == probe_id){
                if ( DS18X20_read_meas( &sensors[0], &subzero, &cel, &cel_frac_bits) == DS18X20_OK ) {
                    char sign = (subzero) ? '-' : '+';
                    int frac = cel_frac_bits*DS18X20_FRACCONV;
                    temp_c = (double)cel;
                   // temp_c = temp_c + (10**(-4)*(double)frac);
                    Temperature::fromC(temp_c);
                }
                else
                {
                    // Do Nothing, error?
                }
            }
        }
    }


     */
    return (millis()-start_time);


}

/**
 * Draws information to the Rhizome's display.
 * This function is called by display().
 * @returns The time taken to run the method
 */
int Ohmbrewer::TemperatureSensor::doDisplay(Ohmbrewer::Screen *screen) {
    unsigned long start = micros();
    char relay_id[2];
    char tempStr [24];

    sprintf(relay_id,"%d", _id);
    sprintf(tempStr, "%2.2f", getTemp()->c());

    // Print a fancy identifier
    screen->print(" [");

    screen->setTextColor(screen->WHITE, screen->DEFAULT_BG_COLOR);
    screen->print(relay_id);
    screen->resetTextColor();

    screen->print("]: ");

    // Print the temperature
    screen->setTextColor(screen->YELLOW, screen->DEFAULT_BG_COLOR);
    screen->println(tempStr);

    screen->resetTextColor();

    return micros() - start;
}

/**
 * Publishes updates to Ohmbrewer, etc.
 * This function is called by update().
 * @param args The argument string passed into the Particle Cloud
 * @returns The time taken to run the method
 */
int Ohmbrewer::TemperatureSensor::doUpdate(String* args) {
    // TODO: Implement TemperatureSensor::doUpdate
    return -1;
}

/**
 * Reports which of the Rhizome's pins are occupied by the
 * Equipment, forming a logical Sprout.
 * @returns The list of physical pins that the Equipment is connected to.
 */
std::list<int>* Ohmbrewer::TemperatureSensor::whichPins() const {
    return _pins;
}

