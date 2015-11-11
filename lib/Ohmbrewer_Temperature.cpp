#include "Ohmbrewer_Temperature.h"
#include "Ohmbrewer_Screen.h"


/**
 * Constructor.
 * @param temp The temperature in Celsius
 */
Ohmbrewer::Temperature::Temperature(const double temp) {
    _targetTemp = temp;
}

/**
 * Copy Constructor.
 * @param copy The Temperature to copy
 */
Ohmbrewer::Temperature::Temperature(const Temperature& copy) {
    _targetTemp = copy.c();
}

/**
 * Destructor
 */
Ohmbrewer::Temperature::~Temperature() {
    // Nothing to do here...
}

/**
 * The temperature in Fahrenheit
 * @returns The temperature in Fahrenheit
 */
double Ohmbrewer::Temperature::f() const {
    return (c() * 1.8) + 32;
}

/**
 * The temperature in Celsius
 * @returns The temperature in Celsius
 */
double Ohmbrewer::Temperature::c() const {
    return get();
}

/**
 * The temperature in Celsius
 * @returns The temperature in Celsius
 */
double Ohmbrewer::Temperature::get() const {
    return _targetTemp;
}

/**
 * Fills a provided C-string buffer with the temperature, formatted for display.
 * Note that this expects your buffer to be sufficiently large!
 * @param buffer Buffer to fill with the formatted temperature in Celsius
 */
void Ohmbrewer::Temperature::toStrC(char* buffer, unsigned int width, unsigned precision) const {
    sprintf(buffer, "%*.*f", width, precision, c());
    //sprintf(buffer, "%*.f", width, precision, c());
    //TODO reformat for printing to only print 1 decimal.
}

/**
 * Fills a provided C-string buffer with the temperature, formatted for display.
 * Note that this expects your buffer to be sufficiently large!
 * @param buffer Buffer to fill with the formatted temperature in Fahrenheit
 */
void Ohmbrewer::Temperature::toStrF(char* buffer, unsigned int width, unsigned precision) const {
    sprintf(buffer, "%*.*f", width, precision, f());
}

/**
 * Sets the temperature from a Fahrenheit value
 * @param temp The temperature in Fahrenheit
 */
const bool Ohmbrewer::Temperature::fromF(const double temp) {
    _targetTemp = (temp - 32)/1.8;
    return true;
}

/**
 * Sets the temperature from a Celsius value
 * @param temp The temperature in Celsius
 */
const bool Ohmbrewer::Temperature::fromC(const double temp) {
    _targetTemp = temp;
    return true;
}

/**
 * Sets the temperature.
 * @param temp The temperature in Celsius
 */
const bool Ohmbrewer::Temperature::set(const double temp) {
    _targetTemp = temp;
    return true;
}

/**
 * Prints the temperature information for temp in yellow onto the touchscreen.
 * @param screen The Rhizome's touchscreen
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Temperature::displayTargetTempC(Ohmbrewer::Screen *screen) {
    unsigned long start = micros();
    char tempStr [10];
    //print target temp in Yellow
    screen->setTextColor(screen->YELLOW, screen->DEFAULT_BG_COLOR);

    toStrC(tempStr);

    screen->print(tempStr);
    screen->resetTextColor();

    return micros() - start;
}

