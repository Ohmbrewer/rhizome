/**
 * This library provides the Thermostat class the Rhizome PID/equipment controller.
 * Rhizome is part of the Ohmbrewer project (see http://ohmbrewer.org for details).
 */

#ifndef OHMBREWER_RHIZOME_THERMOSTAT_H
#define OHMBREWER_RHIZOME_THERMOSTAT_H
    
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
             */
            Temperature* getTargetTemp() const;

            /**
             * Sets the target temperature
             */
            const int setTargetTemp(const double targetTemp);
            
            /**
             * The heating element
             */
            HeatingElement* getElement() const;

            /**
             * The temperature sensor
             */
            TemperatureSensor* getSensor() const;
            
            
            /**
             * Constructors
             */
            Thermostat(int id, int* pins);
            Thermostat(int id, int* pins, const double targetTemp);
            Thermostat(int id, int* pins, int stopTime, bool state, char* currentTask);
            Thermostat(int id, int* pins, int stopTime, bool state, char* currentTask, const double targetTemp);
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


