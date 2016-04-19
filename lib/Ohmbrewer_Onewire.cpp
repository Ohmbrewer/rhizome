
#include "Ohmbrewer_Onewire.h"
#include "ds18x20.h"
#include "onewire.h"


/**
 * Constructors
 * @param busPin Digital pin that serves as the bus for all onewire
 * @param probeId Unique ID for the temperature probe [8] char array ID code
 */
Ohmbrewer::Onewire::Onewire() : Ohmbrewer::Probe(){
//    ow_setPin(D0);    //set globally
    _probeIndex = -1; //highly unlikely to have this index... , testing for now.
}

Ohmbrewer::Onewire::Onewire(int probeIndex) : Ohmbrewer::Probe(){
//    ow_setPin(D0);
    _probeIndex = probeIndex;
}

/**
 * The Equipment ID
 * @returns The Sprout ID to use for this piece of Equipment
 */
int Ohmbrewer::Onewire::getID() const {
    return _probeIndex;
}

/**
 * @returns the Celsius reading from the specified connected DS18b20 probe
 *      returns -69 for no value
 */
double Ohmbrewer::Onewire::getReading(){
    //char uid[8];                   //need the unique ID of the probe to search for.
    uint8_t sensors[80];


    uint8_t subzero, cel, celFracBits;        //local vars
//    char msg[100];
    double tempC = -69;
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
    if (_probeIndex == -1) {
        if (DS18X20_read_meas(&sensors[0], &subzero, &cel, &celFracBits) == DS18X20_OK) {
            char sign = (subzero) ? '-' : '+';
            int frac = celFracBits * DS18X20_FRACCONV;
            tempC = (double) cel;
            if (sign == '-') {
                tempC = tempC * -1;
            }
            tempC = tempC + (.0001 * (double) frac);
            //getTemp()->fromC(tempC);
        }
    }else {//_probeIndex is set
        //if _probeIndex < numSensors (good to go) else you stupid.

        //read sensors[_probeIndex * OW_ROMCODE_SIZE]


        if (_probeIndex < numSensors){
            if (sensors[_probeIndex * OW_ROMCODE_SIZE + 0] == 0x10 ||
                sensors[_probeIndex * OW_ROMCODE_SIZE + 0] == 0x28) //0x10=DS18S20, 0x28=DS18B20
            {
        // filters for a desired probe.
//        for (uint8_t i = 0; i < numSensors; i++) {
//            //current probe ID
//            char probeId[8] = {
//                    sensors[(i * OW_ROMCODE_SIZE) + 0],
//                    sensors[(i * OW_ROMCODE_SIZE) + 1],
//                    sensors[(i * OW_ROMCODE_SIZE) + 2],
//                    sensors[(i * OW_ROMCODE_SIZE) + 3],
//                    sensors[(i * OW_ROMCODE_SIZE) + 4],
//                    sensors[(i * OW_ROMCODE_SIZE) + 5],
//                    sensors[(i * OW_ROMCODE_SIZE) + 6],
//                    sensors[(i * OW_ROMCODE_SIZE) + 7]};

//            if (sensors[i * OW_ROMCODE_SIZE + 0] == 0x10 ||
//                sensors[i * OW_ROMCODE_SIZE + 0] == 0x28) //0x10=DS18S20, 0x28=DS18B20
//            {
                // if current probe matches the probe we are looking for

//                if ( _probeId == (unsigned int)probeId ) {
                    if (DS18X20_read_meas(&sensors[ _probeIndex * OW_ROMCODE_SIZE ], &subzero,
                                          &cel, &celFracBits) == DS18X20_OK) {
                        //char sign = (subzero) ? '-' : '+';
                        int frac = celFracBits * DS18X20_FRACCONV;
                        tempC = (double) cel;
                        tempC = tempC + (.0001 * (double) frac);
                        //getTemp()->fromC(tempC);
                    }
//                    else {
//                        if (i == numSensors - 1) {
//                            //sprintf(msg, "Sensor %s not connected", _probeId); //TODO inject probe ID?
//                            //log(msg);
//                            tempC = -69.69;//double error :)
//                        }
//                    }
//                }
            }
        }
    }



    return tempC;


}

/**
 * outputs probe IDs and their current temperatures to the screen
 *
 * Primarily for use as a manual identification tool for probe IDs
 *
 * FIXME probeID output is currently in HEX something or other, *non readable*
 */
void Ohmbrewer::Onewire::displayProbeIds(Ohmbrewer::Screen *screen){
    //header
    screen->displayHeader();

    screen->resetTextSize();
    screen->resetTextColor();
    screen->println("-------------------");
    // Print the section title
    screen->println("PROBE ID# : TEMP");
//    screen->println("");
    screen->printMargin(2);

    char msg[100];
    uint8_t sensors[80];
    uint8_t subzero, cel, celFracBits;        //local vars
    double tempC;
    //Asks all DS18x20 devices to start temperature measurement, takes up to 750ms at max resolution
    DS18X20_start_meas( DS18X20_POWER_PARASITE, NULL );
    //If your code has other tasks, you can store the timestamp instead and return when a second has passed.
    delay(1000);
    uint8_t numSensors = ow_search_sensors(10, sensors);

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

        if (sensors[i*OW_ROMCODE_SIZE+0] == 0x10 || sensors[i*OW_ROMCODE_SIZE+0] == 0x28){ //0x10=DS18S20, 0x28=DS18B20
           //for each probe print probe and temp to screen
            if ( DS18X20_read_meas( &sensors[i*OW_ROMCODE_SIZE], &subzero, &cel, &celFracBits) == DS18X20_OK ) {
                char sign = (subzero) ? '-' : '+';
                int frac = celFracBits*DS18X20_FRACCONV;
                tempC = (double)cel;
                tempC = tempC + (.0001*(double)frac);

                //print the ID and temp

                sprintf(msg, "ID:  %02X%02X%02X%02X%02X%02X%02X%02X   "
                                "Temperature:   %c%d.%04d",
                        sensors[(i*OW_ROMCODE_SIZE)+0],
                        sensors[(i*OW_ROMCODE_SIZE)+1],
                        sensors[(i*OW_ROMCODE_SIZE)+2],
                        sensors[(i*OW_ROMCODE_SIZE)+3],
                        sensors[(i*OW_ROMCODE_SIZE)+4],
                        sensors[(i*OW_ROMCODE_SIZE)+5],
                        sensors[(i*OW_ROMCODE_SIZE)+6],
                        sensors[(i*OW_ROMCODE_SIZE)+7],
                        sign,
                        cel,
                        frac
                );
                screen->println(msg);
//                screen->print((unsigned int)probeId);
//                screen->print((char)(probeId[0]));
//                screen->print((char)probeId[1]);
//                screen->print((char)probeId[2]);
//                screen->print((char)probeId[3]);
//                screen->print((char)probeId[4]);
//                screen->print((char)probeId[5]);
//                screen->print((char)probeId[6]);
//                screen->print((char)probeId[7]);
//                screen->print(" : ");
//                screen->println(tempC);

            }
            else {//not sure this block is actually needed
                if (i == numSensors - 1) {
                    //error
                    tempC = -69;
                    screen->print(probeId);
                    screen->print(" : ");
                    screen->println(tempC);
                }
            }
        }
    }
}

///**
// * sets the probe id for this instance
// * @param probe id  Unique probe ID
// */
//void Ohmbrewer::Onewire::setProbeId(unsigned int probeId){
//    _probeId = probeId;
//}
//
///**
// * @returns _probeID Unique probe ID
// */
//unsigned int Ohmbrewer::Onewire::getProbeId(){
//    return _probeId;
//}

/**
 * sets the probe index in Sensors[] for this instance
 * @param probe id  Unique index ID.
 */
void Ohmbrewer::Onewire::setProbeIndex(int index){
    _probeIndex = index;
}

/**
 * @returns _probeIndex Unique (onewire) probe index ID
 */
int Ohmbrewer::Onewire::getProbeIndex(){
    return _probeIndex;
}

/**
 * reads current probes connected to busPin
 * @param - sensors array to update with ID values of all(max10) the connected ds18b20 probes
 * @returns - number of sensors discovered
 */
int Ohmbrewer::Onewire::findProbeIds(uint8_t (&sensors)[80]){
    return (int)ow_search_sensors(10, sensors);
}

/**
* @returns the Pin in use for this probe
*/
int Ohmbrewer::Onewire::getPin(){
    return 0;       //Onewire global busPin
}

/* OPTIONAL
 * publish to the log
 */
//void Ohmbrewer::Onewire::log(char* msg)
//{
//    Spark.publish("log", msg);
//    delay(500);
//}
