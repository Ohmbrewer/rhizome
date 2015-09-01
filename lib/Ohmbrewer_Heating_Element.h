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
             * The speed at which the Pump runs..
             */
            int getVoltage() const;
            
            /**
             * Sets the speed at which the Pump runs.
             */
            const int setVoltage(const int speed);
            
            
            /**
             * Constructors
             */
            HeatingElement(int id, int* pins);
            HeatingElement(int id, int* pins, int stopTime, bool state, char* currentTask);
            HeatingElement(int id, int* pins, int stopTime, bool state, char* currentTask, int voltage);
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
             */
            static char** parseArgs(const char* argsStr);

            /**
             * Sets the Equipment state. True => On, False => Off
             */
            const int setState(const bool);
        
            /**
             * The Equipment state. True => On, False => Off
             */
            bool getState() const;
            
            /**
             * True if the Equipment state is On.
             */
            bool isOn() const;
            
            /**
             * True if the Equipment state is Off.
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
             */
            int doWork();
            
            /**
             * Draws information to the Rhizome's display.
             * This function is called by display().
             */
            int doDisplay();
            
            /**
             * Publishes updates to Ohmbrewer, etc.
             * This function is called by update().
             */
            int doUpdate();
            
            /**
             * Reports which of the Rhizome's pins are occupied by the
             * Equipment, forming a logical Sprout.
             */
            int* whichPins() const;
    };
};

#endif


