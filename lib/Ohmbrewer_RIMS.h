/**
 * This library provides the Equipment base class the Rhizome PID/equipment controller.
 * Rhizome is part of the Ohmbrewer project (see http://ohmbrewer.org for details).
 */

#ifndef OHMBREWER_RHIZOME_RIMS_H
#define OHMBREWER_RHIZOME_RIMS_H

#include "Ohmbrewer_Thermostat.h"
#include "Ohmbrewer_Temperature_Sensor.h"
#include "Ohmbrewer_Pump.h"
#include "application.h"

namespace Ohmbrewer {

    class RIMS : public Equipment {
      
        public:
            
            /**
             * The Tube thermostat
             */
            Thermostat* getTube() const;

            /**
             * The temperature sensor located in the tun
             */
            TemperatureSensor* getTunSensor() const;

            /**
             * The recirculation pump between the tun and the tube
             */
            Pump* getRecirculator() const;
            
            
            /**
             * Constructors
             */
            RIMS(int id, int* pins);
            RIMS(int id, int* pins, int stopTime, bool state, char* currentTask);
            RIMS(int id, int* pins, int stopTime, bool state, char* currentTask, const int targetTemp);
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


