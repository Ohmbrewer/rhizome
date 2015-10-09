
#include "Ohmbrewer_Onewire.h"
#include "ds18x20.h"
#include "onewire.h"

/**
 * Constructors
 * @param busPin Digital pin that serves as the bus for all onewire
 * @param probeId Unique ID for the temperature probe [8] char array ID code
 */
Ohmbrewer::Onewire::Onewire() : Ohmbrewer::Probe(){

}

Ohmbrewer::Onewire::Onewire(int busPin) : Ohmbrewer::Probe(){

}

Ohmbrewer::Onewire::Onewire(int busPin, char* probeId) : Ohmbrewer::Probe(){
    _probeId = probeId;
}

/**
 * The short-hand type name. Used for communicating with Ohmbrewer and disambiguating Equipment* pointers.
 */
double Ohmbrewer::Onewire::getTempReading(){
    //char uid[8];                   //need the unique ID of the probe to search for.
    uint8_t sensors[80];


    uint8_t subzero, cel, celFracBits;        //local vars
//    char msg[100];
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
        //getTemp()->fromC(tempC);
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
            if ( strcmp(_probeId, probeId) ){
                if ( DS18X20_read_meas( &sensors[0], &subzero, &cel, &celFracBits) == DS18X20_OK ) {
                    char sign = (subzero) ? '-' : '+';
                    int frac = celFracBits*DS18X20_FRACCONV;
                    tempC = (double)cel;
                    tempC = tempC + (.0001*(double)frac);
                    //getTemp()->fromC(tempC);
                }
                else {
                    if (i == numSensors - 1) {
                        sprintf(msg, "Sensor %s not connected", _probeId); //TODO inject probe ID
                        //log(msg);
                        //TempC = -69;
                    }
                }
            }
        }
    }
*/


    return tempC;


}

/**
 * sets the class variable busPin
 */
void Ohmbrewer::Onewire::setBusPin(int busPin){
    _busPin = busPin;
}

/**
 * @returns _busPin
 */
double Ohmbrewer::Onewire::getBusPin(){
    return _busPin;
}

/**
 * sets the probe id for this instance
 * @param probe id  Unique probe ID
 */
void Ohmbrewer::Onewire::setProbeId(char* probeId){
    _probeId = probeId;
}

/**
 * @returns _probeID
 */
char* Ohmbrewer::Onewire::getProbeId(){
    return _probeId;
}

/**
 * reads current probes connected to busPin
 * @param - sensors array to update with ID values of all(max10) the connected ds18b20 probes
 * @returns - number of sensors discovered
 */
int Ohmbrewer::Onewire::findProbeIds(uint8_t sensors[80]){
    return (int)ow_search_sensors(10, sensors);
}

/**
* @returns the Pin in use for this probe
*/
int Ohmbrewer::Onewire::getPin(){
    return _dataPin;
}

/* OPTIONAL
 * publish to the log
 */
//void Ohmbrewer::Onewire::log(char* msg)
//{
//    Spark.publish("log", msg);
//    delay(500);
//}
