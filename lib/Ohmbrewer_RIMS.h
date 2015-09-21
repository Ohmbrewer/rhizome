/**
 * This library provides the Equipment base class the Rhizome PID/equipment controller.
 * Rhizome is part of the Ohmbrewer project (see http://ohmbrewer.org for details).
 */

#ifndef OHMBREWER_RHIZOME_RIMS_H
#define OHMBREWER_RHIZOME_RIMS_H

// Kludge to allow us to use std::list - for now we have to undefine these macros.
#undef min
#undef max
#undef swap
#include <list>
#include "Ohmbrewer_Thermostat.h"
#include "Ohmbrewer_Temperature_Sensor.h"
#include "Ohmbrewer_Pump.h"
#include "Ohmbrewer_Temperature.h"
#include "application.h"

namespace Ohmbrewer {

    class RIMS : public Equipment {
      
        public:

            /**
             * The short-hand type name. Used for communicating with Ohmbrewer and disambiguating Equipment* pointers.
             */
            const static constexpr char* TYPE_NAME = "rims";

            /**
             * The Equipment Type
             * @returns The Equipment type name
             */
            virtual const char* getType() const { return RIMS::TYPE_NAME; };

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
            RIMS(int id, std::list<int>* pins);

            /**
             * Constructor
             * @param id The Sprout ID to use for this piece of Equipment
             * @param pins The list of physical pins this Equipment is attached to
             * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
             * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
             * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
             */
            RIMS(int id, std::list<int>* pins, int stopTime, bool state, String currentTask);

            /**
             * Constructor
             * @param id The Sprout ID to use for this piece of Equipment
             * @param pins The list of physical pins this Equipment is attached to
             * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
             * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
             * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
             * @param targetTemp The new target temperature in Celsius
             */
            RIMS(int id, std::list<int>* pins, int stopTime, bool state, String currentTask, const double targetTemp);

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

            /**
             * Performs the Equipment's current task. Expect to use this during loop().
             * This function is called by work().
             * @returns The time taken to run the method
             */
            int doWork();

            /**
             * Draws information to the Rhizome's display.
             * This function is called by display().
             * @param screen The Rhizome's touchscreen
             * @returns The time taken to run the method
             */
            int doDisplay(Screen *screen);

            /**
             * Prints the temperature information for our sensors onto the touchscreen.
             * @param screen The Rhizome's touchscreen
             * @returns Time it took to run the function
             */
            unsigned long displayTunTemp(Screen *screen);

            /**
             * rints the recirculation pump status onto the touchscreen.
             * @param screen The Rhizome's touchscreen
             * @returns Time it took to run the function
             */
            unsigned long displayRecircStatus(Screen *screen);

            /**
             * Publishes updates to Ohmbrewer, etc.
             * This function is called by update().
             * @param args The argument string passed into the Particle Cloud
             * @returns The time taken to run the method
             */
            int doUpdate(String args);

            /**
             * Reports which of the Rhizome's pins are occupied by the
             * Equipment, forming a logical Sprout.
             * @returns The list of physical pins that the Equipment is connected to.
             */
            std::list<int>* whichPins() const;

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
             * Adds the update function for the instance.
             */
            void addUpdateFunction();
    };
};

#endif


