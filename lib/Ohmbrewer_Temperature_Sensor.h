/**
 * This library provides the Temperature Sensor class the Rhizome PID/equipment controller.
 * Rhizome is part of the Ohmbrewer project (see http://ohmbrewer.org for details).
 */

#ifndef OHMBREWER_RHIZOME_TEMPERATURE_SENSOR_H
#define OHMBREWER_RHIZOME_TEMPERATURE_SENSOR_H

// Kludge to allow us to use std::list - for now we have to undefine these macros.
#undef min
#undef max
#undef swap
#include <list>
#include "Ohmbrewer_Equipment.h"
#include "Ohmbrewer_Temperature.h"
#include "application.h"
#include "Ohmbrewer_Probe.h"


namespace Ohmbrewer {

    class TemperatureSensor : public Equipment {
      
        public:

            /**
             * The short-hand type name. Used for communicating with Ohmbrewer and disambiguating Equipment* pointers.
             */
            const static constexpr char* TYPE_NAME = "temp";

            /**
             * The Equipment Type
             * @returns The Equipment type name
             */
            virtual const char* getType() const { return TemperatureSensor::TYPE_NAME; };

            /**
             * Constructor
             * @param id The Sprout ID to use for this TemperatureSensor
             * @param pins The list of physical pins this TemperatureSensor is attached to
             */
            TemperatureSensor(int id, int busPin);

            /**
             * Constructor
             * @param id The Sprout ID to use for this TemperatureSensor
             * @param pins The list of physical pins this TemperatureSensor is attached to
             * @param stopTime The time at which the TemperatureSensor should shut off, assuming it isn't otherwise interrupted
             * @param state Whether the TemperatureSensor is ON (or OFF). True => ON, False => OFF
             * @param currentTask The unique identifier of the task that the TemperatureSensor believes it should be processing
             */
            TemperatureSensor(int id, int busPin, int stopTime, bool state, String currentTask);

            /**
             * Copy Constructor
             * @param clonee The TemperatureSensor object to copy
             */
            TemperatureSensor(const TemperatureSensor& clonee);
            
            /**
             * Destructor
             */
            virtual ~TemperatureSensor();

            /**
             * The Bus pin - Data input line
             * onewire protocol input location for DS18b20
             * @returns The pin number in use for this piece of Equipment
             */
            int getBusPin() const;

            /**
             * Overloaded << operator.
             */
            // friend std::ostream& operator<<( std::ostream& os, Pump const& pump);

            /**
             * The last temperature read by the sensor. Currently returns in Celsius.
             * @returns A pointer to the Temperature object representing the last temperature reading
             */
            Temperature* getTemp() const;

            /**
             * The last time the temperature was read by the sensor
             * @returns The last temperature reading time
             */
            int getLastReadTime() const;

            /**
             * Sets the last time the temperature was read by the sensor
             * @param lastReadTime The last temperature reading time
             */
            const int setLastReadTime(const int lastReadTime);

            /**
             * @returns the probe for this sensor
             */
            Probe* getProbe() const;

            /**
             * Specifies the interface for arguments sent to this TemperatureSensor's associated function.
             * Parses the supplied string into an array of strings for setting the TemperatureSensor's values.
             * Most likely will be called during update().
             * @param argsStr The arguments supplied as an update to the Rhizome.
             * @param result A map representing the key/value pairs for the update
             */
            static void parseArgs(const String &argsStr, args_map_t &result);

            /**
             * Sets the TemperatureSensor state. True => On, False => Off
             * @param state Whether the TemperatureSensor is ON (or OFF). True => ON, False => OFF
             * @returns The time taken to run the method
             */
            const int setState(const bool);

            /**
             * The TemperatureSensor state. True => On, False => Off
             * @returns True => On, False => Off
             */
            bool getState() const;

            /**
             * True if the TemperatureSensor state is On.
             * @returns Whether the Equipment is turned ON
             */
            bool isOn() const;

            /**
             * True if the TemperatureSensor state is Off.
             * @returns Whether the Equipment is turned OFF
             */
            bool isOff() const;

            /**
             * Performs the TemperatureSensor's current task. Expect to use this during loop().
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
             * Publishes updates to Ohmbrewer, etc.
             * This function is called by update().
             * @param args The argument string passed into the Particle Cloud
             * @param argsMap A map representing the key/value pairs for the update
             * @returns The time taken to run the method
             */
            int doUpdate(String &args, args_map_t &argsMap);

            /**
             * Reports which of the Rhizome's pins are occupied by the
             * Equipment, forming a logical Sprout.
             * @param pins The list of physical pins that the TemperatureSensor is connected to.
             */
            void whichPins(std::list<int>* pins);

        protected:
            /**
             * Last temperature read by the sensor
             */
            Temperature* _lastReading;

            /**
             * Last time the temperature was read by the sensor
             */
            int _lastReadTime;

            /**
             * temperature Probe
             */
            Probe* _probe;

    };
};

#endif


