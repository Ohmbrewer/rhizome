/**
 * This library provides the Equipment base class the Rhizome PID/equipment controller.
 * Rhizome is part of the Ohmbrewer project (see http://ohmbrewer.org for details).
 */

#ifndef OHMBREWER_RHIZOME_RIMS_H
#define OHMBREWER_RHIZOME_RIMS_H

#include "Ohmbrewer_Thermostat.h"
#include "Ohmbrewer_Temperature_Sensor.h"
#include "Ohmbrewer_Pump.h"
#include "Ohmbrewer_Temperature.h"
#include "application.h"

namespace Ohmbrewer {

    class RIMS : public Equipment {
      
        public:

            /**
             * The Tube thermostat
             * @returns The Thermostat object representing the RIMS tube elements
             */
            Thermostat* getTube() const;

            /**
             * The temperature sensor located in the tun
             * @returns The Temperature Sensor object representing the sensor located in the mash tun
             */
            TemperatureSensor* getTunSensor() const;

            /**
             * The recirculation pump between the tun and the tube
             * @returns The Pump object representing the recirculation pump
             */
            Pump* getRecirculator() const;

            /**
             * Constructor
             * @param id The Sprout ID to use for this piece of Equipment
             * @param pins The list of physical pins this Equipment is attached to
             */
            RIMS(int id, int* pins);

            /**
             * Constructor
             * @param id The Sprout ID to use for this piece of Equipment
             * @param pins The list of physical pins this Equipment is attached to
             * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
             * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
             * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
             */
            RIMS(int id, int* pins, int stopTime, bool state, char* currentTask);

            /**
             * Constructor
             * @param id The Sprout ID to use for this piece of Equipment
             * @param pins The list of physical pins this Equipment is attached to
             * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
             * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
             * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
             * @param targetTemp The new target temperature in Celsius
             */
            RIMS(int id, int* pins, int stopTime, bool state, char* currentTask, const double targetTemp);

            /**
             * Copy constructor
             * @param clonee The Equipment object to copy
             */
            RIMS(const RIMS& clonee);
            
            /**
             * Destructor
             */
            virtual ~RIMS();
            
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
            static args_map_t parseArgs(const String argsStr);

            /**
             * Sets the Equipment state. True => On, False => Off
             * This turns *EVERYTHING* on, so watch out. You may want to turn the components on individually instead.
             * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
             * @returns The time taken to run the method
             */
            const int setState(const bool);

            /**
             * The Equipment state. True => On, False => Off
             * @returns True => On, False => Off
             */
            bool getState() const;

            /**
             * True if the Equipment state is On.
             * @returns Whether the Equipment is turned ON
             */
            bool isOn() const;

            /**
             * True if the Equipment state is Off.
             * @returns Whether the Equipment is turned OFF
             */
            bool isOff() const;


        protected:
            /**
             * The tube thermostat
             */
            Thermostat* _tube;
            /**
             * The temperature sensor in the tun
             */
            TemperatureSensor* _tunSensor;
            /**
             * The recirculation pump between the tun and the tube
             */
            Pump* _recirc;

        private:
            /**
             * Performs the Equipment's current task. Expect to use this during loop().
             * This function is called by work().
             * @returns The time taken to run the method
             */
            int doWork();

            /**
             * Draws information to the Rhizome's display.
             * This function is called by display().
             * @returns The time taken to run the method
             */
            int doDisplay();

            /**
             * Publishes updates to Ohmbrewer, etc.
             * This function is called by update().
             * @returns The time taken to run the method
             */
            int doUpdate();

            /**
             * Reports which of the Rhizome's pins are occupied by the
             * Equipment, forming a logical Sprout.
             * @returns The list of physical pins that the Equipment is connected to.
             */
            int* whichPins() const;
    };
};

#endif


