/**
 * This library provides the Temperature class for the Rhizome PID/equipment controller.
 * Rhizome is part of the Ohmbrewer project (see http://ohmbrewer.org for details).
 */

#ifndef OHMBREWER_TEMPERATURE_H
#define OHMBREWER_TEMPERATURE_H

#include "application.h"
#include "Ohmbrewer_Screen.h"


namespace Ohmbrewer {

    class Temperature {
      
        public:

            /**
             * Represents a temperature that doesn't make sense. Used for bad readings.
             */
            const static constexpr double INVALID_TEMPERATURE = -69.0;

            /**
             * Constructor.
             */
            Temperature();

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
            void toStrC(char* buffer, unsigned int width = 5, unsigned precision = 1) const;

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

//            /**
//             * Prints the temperature information for temp in yellow onto the touchscreen.
//             * @param screen The Rhizome's touchscreen
//             * @returns Time it took to run the function
//             */
//            unsigned long displayTargetTempC(Temperature* temp, Screen *screen);

            /**
             * Prints the temperature information for our sensors onto the touchscreen.
             * @param color The color of the temperature text
             * @param screen The Rhizome's touchscreen
             * @returns Time it took to run the function
             */
            unsigned long displayTempC(uint16_t color, Screen *screen);

    protected:
            /**
             * The temperature in Celsius
             */
            double _temp;
    };
};

#endif


