/**
 * This library provides the Equipment base class the Rhizome PID/equipment controller.
 * Rhizome is part of the Ohmbrewer project (see http://ohmbrewer.org for details).
 */

#ifndef OHMBREWER_RHIZOME_HEATING_ELM_H
#define OHMBREWER_RHIZOME_HEATING_ELM_H
    
#include "Ohmbrewer_Equipment.h"
#include "application.h"

namespace Ohmbrewer {

    class HeatingElement : public Equipment {
      
        public:

            /**
             * The voltage at which the Heating Element runs.
             * @returns The current voltage setting
             */
            int getVoltage() const;

            /**
             * Sets the voltage at which the Heating Element runs.
             * @returns The time taken to run the method
             */
            const int setVoltage(const int speed);

            /**
             * Constructor
             * @param id The Sprout ID to use for this piece of Equipment
             * @param pins The list of physical pins this Equipment is attached to
             */
            HeatingElement(int id, int* pins);

            /**
             * Constructor
             * @param id The Sprout ID to use for this piece of Equipment
             * @param pins The list of physical pins this Equipment is attached to
             * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
             * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
             * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
             */
            HeatingElement(int id, int* pins, int stopTime, bool state, char* currentTask);

            /**
             * Constructor
             * @param id The Sprout ID to use for this piece of Equipment
             * @param pins The list of physical pins this Equipment is attached to
             * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
             * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
             * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
             * @param voltage The current voltage setting
             */
            HeatingElement(int id, int* pins, int stopTime, bool state, char* currentTask, int voltage);

            /**
             * Copy Constructor
             * @param clonee The Equipment object to copy
             */
            HeatingElement(const HeatingElement& clonee);
            
            /**
             * Destructor
             */
            virtual ~HeatingElement();
            
            /**
             * Overloaded << operator.
             */
            // friend std::ostream& operator<<( std::ostream& os, Pump const& pump);

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
             * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
             * @returns The time taken to run the method
             */
            const int setState(const bool);

            /**
             * The Equipment state.
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
             * Voltage to provide the heating element
             */
            int  _voltage;

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


