/**
 * This library provides the Thermostat class the Rhizome PID/equipment controller.
 * Rhizome is part of the Ohmbrewer project (see http://ohmbrewer.org for details).
 */

#ifndef OHMBREWER_RHIZOME_THERMOSTAT_H
#define OHMBREWER_RHIZOME_THERMOSTAT_H

// Kludge to allow us to use std::list - for now we have to undefine these macros.
#undef min
#undef max
#undef swap
#include <list>
#include "Ohmbrewer_Equipment.h"
#include "Ohmbrewer_Heating_Element.h"
#include "Ohmbrewer_Temperature_Sensor.h"
#include "Ohmbrewer_Temperature.h"
#include "application.h"

namespace Ohmbrewer {

    class Thermostat : public Equipment {
      
        public:

            /**
             * The desired target temperature. Defaults to Celsius
             * @returns The target temperature in Celsius
             */
            Temperature* getTargetTemp() const;

            /**
             * Sets the target temperature
             * @param targetTemp The new target temperature in Celsius
             * @returns The time taken to run the method
             */
            const int setTargetTemp(const double targetTemp);

            /**
             * The Thermostat's heating element
             * @returns The heating element
             */
            HeatingElement* getElement() const;

            /**
             * The Thermostat's temperature sensor
             * @returns The temperature sensor
             */
            TemperatureSensor* getSensor() const;


            /**
             * Constructor
             * @param id The Sprout ID to use for this piece of Equipment
             * @param pins The list of physical pins this Equipment is attached to
             */
            Thermostat(int id, std::list<int>* pins);

            /**
             * Constructor
             * @param id The Sprout ID to use for this piece of Equipment
             * @param pins The list of physical pins this Equipment is attached to
             * @param targetTemp The new target temperature in Celsius
             */
            Thermostat(int id, std::list<int>* pins, const double targetTemp);

            /**
             * Constructor
             * @param id The Sprout ID to use for this piece of Equipment
             * @param pins The list of physical pins this Equipment is attached to
             * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
             * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
             * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
             */
            Thermostat(int id, std::list<int>* pins, int stopTime, bool state, char* currentTask);

            /**
             * Constructor
             * @param id The Sprout ID to use for this piece of Equipment
             * @param pins The list of physical pins this Equipment is attached to
             * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
             * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
             * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
             * @param targetTemp The new target temperature in Celsius
             */
            Thermostat(int id, std::list<int>* pins, int stopTime, bool state, char* currentTask, const double targetTemp);

            /**
             * Copy Constructor
             * @param clonee The Equipment object to copy
             */
            Thermostat(const Thermostat& clonee);
            
            /**
             * Destructor
             */
            virtual ~Thermostat();
            
            /**
             * Overloaded << operator.
             */
            // friend std::ostream& operator<<( std::ostream& os, Thermostat const& thermostat);


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
             * The thermostat's heating element
             */
            HeatingElement* _heatingElm;
            /**
             * The thermostat's temperature sensor
             */
            TemperatureSensor* _tempSensor;
            /**
             * The desired operating temperature
             */
            Temperature* _targetTemp;

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
            std::list<int>* whichPins() const;
    };
};

#endif


