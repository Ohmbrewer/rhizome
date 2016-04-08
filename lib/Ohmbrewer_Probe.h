
#ifndef RHIZOME_OHMBREWER_PROBE_H
#define RHIZOME_OHMBREWER_PROBE_H

//#include "Ohmbrewer_Temperature_Sensor.h"


namespace Ohmbrewer {

    class Probe {

    public:

        /**
         * @returns the Celsius temperature reading from the connected probe
         */
        virtual double getReading() = 0;

        /**
         * Constructor
         */
        Probe();

        /**
        * Destructor
        */
        virtual ~Probe();

        /**
         * The Equipment ID
         * @returns The Sprout ID to use for this piece of Equipment
         */
        virtual int getID() const;

        /**
         * @returns the Pin in use for this probe
         */
        virtual int getPin() = 0;

    protected:
        /**
         * Digital pin that the data stream is on
         */
        int _dataPin;
    };
};
#endif
