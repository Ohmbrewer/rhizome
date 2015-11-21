
#ifndef RHIZOME_OHMBREWER_ONEWIRE_H
#define RHIZOME_OHMBREWER_ONEWIRE_H

#include "Ohmbrewer_Probe.h"
#include "application.h"
#include "Ohmbrewer_Screen.h"


namespace Ohmbrewer {

    class Onewire : public Probe {

    public:
        /**
         * Constructors
         * @param probeId Unique ID for the temperature probe [8] char array ID code
         */
        Onewire();

        Onewire(int probeIndex);

        /**
         * @returns the Celsius reading from the specified connected DS18b20 probe
         *      returns -69 for no value
         */
        double getReading();

        /**
         * outputs probe IDs and their current temperatures to the screen
         *
         * Primarily for use as a manual identification tool for probe IDs
         */
        void displayProbeIds(Screen *screen);

//        /**
//         * sets the probe id for this instance
//         * @param probe id  Unique probe ID
//         */
//        void setProbeId(unsigned int probeId);
//
//        /**
//         * @returns _probeID Unique probe ID
//         */
//        unsigned int getProbeId();

        /**
         * sets the probe index in Sensors[] for this instance
         * @param probe id  Unique index ID.
         */
        void setProbeIndex(int index);

        /**
         * @returns _probeIndex Unique (onewire) probe index ID
         */
        int getProbeIndex();

        /**
         * reads current probes connected to busPin
         * @param - sensors array to update with ID values of all(max10) the connected ds18b20 probes
         * @returns - number of sensors discovered
         */
        int findProbeIds(uint8_t (&sensors)[80]);

        /**
         * @returns the Pin in use for this probe
         */
        int getPin();

    protected:

//        /**TODO may still want this in the future.
//         * Unique ID for the temperature probe [8] char array ID code
//         */
//        unsigned int _probeId;

        /**
         * Unique index for the temperature probe in teh sensor array.
         * must be less than numSensors attached
         */
        int _probeIndex;

        /**
         * Digital pin that the data stream is on
         */
        int _dataPin;

    };
};

#endif

