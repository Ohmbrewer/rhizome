/**
 * This library provides the Equipment base class the Rhizome PID/equipment controller.
 * Rhizome is part of the Ohmbrewer project (see http://ohmbrewer.org for details).
 */

#ifndef OHMBREWER_TEMPERATURE_H
#define OHMBREWER_TEMPERATURE_H

#include "application.h"

namespace Ohmbrewer {

    class Temperature {
      
        public:
            
            /**
             * The temperature in Fahrenheit
             */
            double f() const;
            
            /**
             * The temperature in Celsius
             */
            double c() const;

            /**
             * The temperature in Celsius
             */
            double get() const;

            /**
             * Sets the temperature from a Fahrenheit value
             */
            const bool fromF(const double temp);

            /**
             * Sets the temperature from a Celsius value
             */
            const bool fromC(const double temp);

            /**
             * Sets the temperature. Expects a Celsius value
             */
            const bool set(const double temp);
            
            /**
             * Constructors
             */
            Temperature(const double temp);
            Temperature(const Temperature& copy);
            
            /**
             * Destructor
             */
            virtual ~Temperature();
            
            /**
             * Overloaded << operator.
             */
             // friend std::ostream& operator<<( std::ostream& os, Temperature const& temp);
    
        protected:
            /**
             * The temperature in Celsius
             */
            double _targetTemp;
    };
};

#endif


