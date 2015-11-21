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
#include "pid.h"
#include "Ohmbrewer_PID_Profile.h"

namespace Ohmbrewer {

    class Thermostat : public Equipment {
      
        public:

            /**
             * The short-hand type name. Used for communicating with Ohmbrewer and disambiguating Equipment* pointers.
             */
            const static constexpr char* TYPE_NAME = "therm";

            /**
             * The Equipment Type
             * @returns The Equipment type name
             */
            virtual const char* getType() const { return Thermostat::TYPE_NAME; };

            /**
             * Constructor
             * @param id The Sprout ID to use for this piece of Thermostat
             * @param thermPins list with formatting of: [ temp busPin ; onewire index ; heating controlPin ; heating powerPin ]
             * NOTE: if a Pin value is not enabled pass -1 as the value for that pin.
             */
            Thermostat(int id, std::list<int>* thermPins);

            /**
             * Constructor
             * @param id The Sprout ID to use for this piece of Thermostat
             * @param thermPins list with formatting of: [ temp busPin ; onewire index ; heating controlPin ; heating powerPin ]
             * NOTE: if a Pin value is not enabled pass -1 as the value for that pin.
             * @param targetTemp The new target temperature in Celsius
             */
            Thermostat(int id, std::list<int>* thermPins, const double targetTemp);

            /**
             * Constructor
             * @param id The Sprout ID to use for this piece of Thermostat
             * @param thermPins list with formatting of: [ temp busPin ; onewire index ; heating controlPin ; heating powerPin ]
             * NOTE: if a Pin value is not enabled pass -1 as the value for that pin.
             * @param stopTime The time at which the Thermostat should shut off, assuming it isn't otherwise interrupted
             * @param state Whether the Thermostat is ON (or OFF). True => ON, False => OFF
             * @param currentTask The unique identifier of the task that the Thermostat believes it should be processing
             */
            Thermostat(int id, std::list<int>* thermPins, int stopTime, bool state, String currentTask);

            /**
             * Constructor
             * @param id The Sprout ID to use for this piece of Thermostat
             * @param thermPins list with formatting of: [ temp busPin ; onewire index ; heating controlPin ; heating powerPin ]
             * NOTE: if a Pin value is not enabled pass -1 as the value for that pin.
             * @param stopTime The time at which the Thermostat should shut off, assuming it isn't otherwise interrupted
             * @param state Whether the Thermostat is ON (or OFF). True => ON, False => OFF
             * @param currentTask The unique identifier of the task that the Thermostat believes it should be processing
             * @param targetTemp The new target temperature in Celsius
             */
            Thermostat(int id, std::list<int>* thermPins, int stopTime, bool state, String currentTask, const double targetTemp);

            /**
             * Copy Constructor
             * @param clonee The Thermostat object to copy
             */
            Thermostat(const Thermostat& clonee);
            
            /**
             * Destructor
             */
            virtual ~Thermostat();

            /**
             * logic for initializing the constructors
             * @param id The Sprout ID to use for this piece of Thermostat
             * @param thermPins list with formatting of: [ temp busPin ; onewire index ; heating controlPin ; heating powerPin ]
             * NOTE: if a Pin value is not enabled pass -1 as the value for that pin.
             */
            void initThermostat(int id, std::list<int>* thermPins);

            /**
             * The Thermostat's temperature sensor
             * @returns The temperature sensor
             */
            TemperatureSensor* getSensor() const;

            /**
             * Sets the Thermostat's temperature sensor
             * @param sensor -  The temperature sensor
             * @returns The time taken to run the method
             */
            const int setSensor(TemperatureSensor* sensor);

            /**
             * The desired target temperature. Defaults to Celsius
             * @returns The target temperature in Celsius, as a Temperature object pointer
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
             * Specifies the interface for arguments sent to this Thermostat's associated function.
             * Parses the supplied string into an array of strings for setting the Thermostat's values.
             * Most likely will be called during update().
             * @param argsStr The arguments supplied as an update to the Rhizome.
             * @param result A map representing the key/value pairs for the update
             */
            static void parseArgs(const String &argsStr, args_map_t &result);

            /**
             * Sets the Thermostat state. True => On, False => Off
             * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
             * @returns The time taken to run the method
             */
            const int setState(const bool);

            /**
             * The Thermostat state. True => On, False => Off
             * @returns True => On, False => Off
             */
            bool getState() const;

            /**
             * True if the Thermostat state is On.
             * @returns Whether the Equipment is turned ON
             */
            bool isOn() const;

            /**
             * True if the Thermostat state is Off.
             * @returns Whether the Equipment is turned OFF
             */
            bool isOff() const;

            /**
             * Performs the Thermostat's current task. Expect to use this during loop().
             * This function is called by work().
             * @returns The time taken to run the method
             */
            int doWork();

            /**
            * Controls all the inner workings of the PID functionality
            * Should be called by _timer
            *
            * Controls the heating element Relays manually, overriding the standard relay
            * functionality
            *
            * The pid is designed to Output an analog value, but the relay can only be On/Off.
            *
            * "time proportioning control"  it's essentially a really slow version of PWM.
            * first we decide on a window size. Then set the pid to adjust its output between 0 and that window size.
            * lastly, we add some logic that translates the PID output into "Relay On Time" with the remainder of the
            * window being "Relay Off Time"
            *
            * PID Adaptive Tuning
            * You can change the tuning parameters.  this can be
            * helpful if we want the controller to be agressive at some
            * times, and conservative at others.
            *
            */
            void doPID();

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
            unsigned long displayThermTemp(Screen *screen);

            /**
             * Draws information to the Rhizome's display.
             * @param screen The Rhizome's touchscreen
             * @returns The time taken to run the method
             */
            int displayRelay(Screen *screen);

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
             * @param pins The list of physical pins that the Thermostat is connected to.
             */
            void whichPins(std::list<int>* pins);

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

            /**
             * PID instance for the thermostat
             */
            PID* _thermPID;

            /**
             * Aggressive Tuning Parameters profile for the PID
             */
            PIDProfile agg  = PIDProfile(4, 0.2, 1);

            /**
             * Timer to control PID functionality
             */
            //Timer* _timer;

            /**
             * Conservative Tuning Parameters profile for the PID
             */
            PIDProfile cons = PIDProfile(1, 0.05, 0.25);


            // Define Variables we'll be connecting to with PID
            double setPoint;
            double input;
            double output;

            // PID windowing variables
            int windowSize = 5000;
            unsigned long windowStartTime;

    };
};

#endif


