
#ifndef RHIZOME_OHMBREWER_ONEWIRE_H
#define RHIZOME_OHMBREWER_ONEWIRE_H

#include "Ohmbrewer_Probe.h"
#include "ds18x20.h"
#include "onewire.h"

namespace Ohmbrewer {

    class Onewire : public Probe {

    public:
        /**
         * Constructors
         * @param busPin Digital pin that serves as the bus for all onewire
         * @param probeId Unique ID for the temperature probe [8] char array ID code
         */
        Onewire();

        Onewire(int busPin);

        Onewire(int busPin, char* probeId);

        /**
         * @returns the
         */
        double getTempReading();

        /**
         * sets the class variable busPin
         */
        void setBusPin(int busPin);

        /**
         * @returns _busPin
         */
        double getBusPin();

        /**
         * sets the probe id for this instance
         * @param probe id  Unique probe ID
         */
        void setProbeId(char* probeId);

        /**
         * @returns _probeID
         */
        char* getProbeId();

        /**
         * reads current probes connected to busPin
         * @param - sensors array to update with ID values of all(max10) the connected ds18b20 probes
         * @returns - number of sensors discovered
         */
        int findProbeIds(uint8_t sensors[80]);

        /**
         * @returns the Pin in use for this probe
         */
        int getPin();

    protected:
        /**
         * Digital Pin for the temperature bus one wire protocol
         */
        static int _busPin; //TODO class variable??

        /**
         * Unique ID for the temperature probe [8] char array ID code
         */
        char* _probeId;
    };
};

#endif

