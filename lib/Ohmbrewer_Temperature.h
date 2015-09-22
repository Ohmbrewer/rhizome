/**
 * This library provides the Temperature class for the Rhizome PID/equipment controller.
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
             * @returns The temperature in Fahrenheit
             */
            double f() const;

            /**
             * The temperature in Celsius
             * @returns The temperature in Celsius
             */
            double c() const;

            /**
             * The temperature in Celsius
             * @returns The temperature in Celsius
             */
            double get() const;

            /**
             * Fills a provided C-string buffer with the temperature, formatted for display.
             * Note that this expects your buffer to be sufficiently large!
             * @param buffer Buffer to fill with the formatted temperature in Celsius
             * @param width Formatter width, defaults to 2
             * @param precision Formatter precision, defaults to 2
             */
            void toStrC(char* buffer, unsigned int width = 6, unsigned precision = 2) const;

            /**
             * Fills a provided C-string buffer with the temperature, formatted for display.
             * Note that this expects your buffer to be sufficiently large!
             * @param buffer Buffer to fill with the formatted temperature in Celsius
             * @param width Formatter width, defaults to 2
             * @param precision Formatter precision, defaults to 2
             */
            void toStrF(char* buffer, unsigned int width = 2, unsigned precision = 2) const;

            /**
             * Sets the temperature from a Fahrenheit value
             * @param temp The temperature in Fahrenheit
             */
            const bool fromF(const double temp);

            /**
             * Sets the temperature from a Celsius value
             * @param temp The temperature in Celsius
             */
            const bool fromC(const double temp);

            /**
             * Sets the temperature.
             * @param temp The temperature in Celsius
             */
            const bool set(const double temp);

            /**
             * Constructor.
             * @param temp The temperature in Celsius
             */
            Temperature(const double temp);

            /**
             * Copy Constructor.
             * @param copy The Temperature to copy
             */
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


