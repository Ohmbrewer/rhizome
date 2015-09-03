/**
 * This library provides the Temperature Sensor class the Rhizome PID/equipment controller.
 * Rhizome is part of the Ohmbrewer project (see http://ohmbrewer.org for details).
 */

#ifndef OHMBREWER_RHIZOME_TEMPERATURE_SENSOR_H
#define OHMBREWER_RHIZOME_TEMPERATURE_SENSOR_H
    
#include "Ohmbrewer_Equipment.h"
#include "Ohmbrewer_Temperature.h"
#include "application.h"

namespace Ohmbrewer {

    class TemperatureSensor : public Equipment {
      
        public:
            
            /**
             * The last temperature read by the sensor. Currently returns in Celsius.
             */
            Temperature* getTemp() const;
            
            /**
             * The last time the temperature was read by the sensor
             */
            int getLastReadTime() const;
            
            /**
             * Sets the last time the temperature was read by the sensor
             */
            const int setLastReadTime(const int lastReadTime);
            
            
            /**
             * Constructors
             */
            TemperatureSensor(int id, int* pins);
            TemperatureSensor(int id, int* pins, int stopTime, bool state, char* currentTask);
            TemperatureSensor(const TemperatureSensor& clonee);
            
            /**
             * Destructor
             */
            virtual ~TemperatureSensor();
            
            /**
             * Overloaded << operator.
             */
            // friend std::ostream& operator<<( std::ostream& os, Pump const& pump);
            
            /**
             * Specifies the interface for arguments sent to this Equipment's associated function. 
             * Parses the supplied string into an array of strings for setting the Equipment's values.
             * Most likely will be called during update().
             */
            static args_map_t parseArgs(const char* argsStr);

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
             * Last temperature read by the sensor
             */
            Temperature* _lastReading;
            /**
             * Last time the temperature was read by the sensor
             */
            int _lastReadTime;

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


