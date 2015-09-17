#include "Ohmbrewer_Screen.h"
#include "Ohmbrewer_Equipment.h"
#include "Ohmbrewer_Temperature_Sensor.h"
#include "Ohmbrewer_Heating_Element.h"
#include "Ohmbrewer_Pump.h"
#include "Ohmbrewer_Relay.h"

/**
 * Constructor
 */
Ohmbrewer::Screen::Screen(uint8_t CS,
                          uint8_t RS,
                          uint8_t RST,
                          std::list< Ohmbrewer::Equipment* >* sprouts) : Adafruit_ILI9341(CS, RS, RST) {
    _sprouts = sprouts;
}

/**
 * Initializes the display screen
 */
void Ohmbrewer::Screen::initScreen() {

    begin();
    fillScreen(DEFAULT_BG_COLOR);
    drawButtons();

    // Reset the cursor
    setCursor(LEFT, TOP);
}

/**
 * Prints the "Ohmbrewer" title onto the touchscreen
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Screen::displayHeader() {
    unsigned long start = micros();

    // Add the title
    setCursor(0, 0);
    setTextColor(ILI9341_WHITE, DEFAULT_BG_COLOR);
    setTextSize(3);
    println("  OHMBREWER");

    // Add a bottom margin
    printMargin(2);

    return micros() - start;
}

/**
 * Prints a small margin line (size 1).
 * @param current The current text size to reset to.
 */
void Ohmbrewer::Screen::printMargin(const uint8_t current) {
    setTextSize(1);
    println("");
    setTextSize(current);
}

/**
 * Initializes the display screen
 */
void Ohmbrewer::Screen::drawButtons() {

    setTextColor(ILI9341_WHITE, DEFAULT_BG_COLOR);

    // make the buttons
    fillRect(LEFT, BUTTONTOP, BUTTONSIZE, BUTTONSIZE, DEFAULT_BG_COLOR);
    fillRect(BUTTONSIZE, BUTTONTOP, BUTTONSIZE, BUTTONSIZE, DEFAULT_BG_COLOR);
    fillRect(BUTTONSIZE*2, BUTTONTOP, BUTTONSIZE, BUTTONSIZE, DEFAULT_BG_COLOR);
    fillRect(BUTTONSIZE*3, BUTTONTOP, BUTTONSIZE, BUTTONSIZE, DEFAULT_BG_COLOR);
    drawRect(LEFT, BUTTONTOP, BUTTONSIZE, BUTTONSIZE, DEFAULT_TEXT_COLOR);
    drawRect(BUTTONSIZE, BUTTONTOP, BUTTONSIZE, BUTTONSIZE, DEFAULT_TEXT_COLOR);
    drawRect(BUTTONSIZE*2, BUTTONTOP, BUTTONSIZE, BUTTONSIZE, DEFAULT_TEXT_COLOR);
    drawRect(BUTTONSIZE*3, BUTTONTOP, BUTTONSIZE, BUTTONSIZE, DEFAULT_TEXT_COLOR);

    // Add symbols to the buttons
    setTextSize(4);
    setCursor(LEFT + 20, BUTTONTOP + 15);
    print("+");
    setCursor(BUTTONSIZE + 20, BUTTONTOP + 15);
    print("-");

    setTextSize(2);
    setCursor((BUTTONSIZE*2) + 7, BUTTONTOP + 20);
    print("Menu");
    setTextSize(2);
    setCursor((BUTTONSIZE*3) + 14, BUTTONTOP + 20);
    print("Sel");

    resetTextColor();
}

/**
 * Refreshes/redraws the touchscreen display
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Screen::refreshDisplay() {
    unsigned long start = micros();

    displayHeader();

    // FIXME: This sort of call will be moved into TemperatureSensor.doDisplay(Ohmbrewer::Screen *screen). We won't be calling it like this explicitly.
    displayTemps(((Ohmbrewer::TemperatureSensor*)_sprouts->front())->getTemp()->c(), 100.00);

//    displayRelays();
    displayHeatingElements();
    displayPumps();


    // 500 seems like a good refresh delay
    delay(500);

    return micros() - start;
}

/**
 * Prints the status information for our current relays onto the touchscreen
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Screen::displayRelays() {
    unsigned long start = micros();
    unsigned int count = 0;

    resetTextSize();
    print("====== Relays ======");
    printMargin(2);
    for (std::list<Ohmbrewer::Equipment*>::iterator itr = _sprouts->begin(); itr != _sprouts->end(); itr++) {
        // TODO: Fix this when we start using Thermostats and RIMS - needs to be more sophisticated
        if (strcmp((*itr)->getType(), "temp") != 0) {
            // We'll ignore the TemperatureSensors here.
            ((Ohmbrewer::Relay*)(*itr))->display(this);
        }
        count++;
    }
    printMargin(2);

    return micros() - start;
}

/**
 * Prints the status information for our current Heating Elements onto the touchscreen
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Screen::displayHeatingElements() {
    unsigned long start = micros();

    resetTextSize();
    print("======= Heat =======");
    printMargin(2);
    for (std::list<Ohmbrewer::Equipment*>::iterator itr = _sprouts->begin(); itr != _sprouts->end(); itr++) {
        if (strcmp((*itr)->getType(), "heat") == 0) {
            ((Ohmbrewer::HeatingElement*)(*itr))->display(this);
        }
    }
    printMargin(2);

    return micros() - start;
}

/**
 * Prints the status information for our current Pumps onto the touchscreen
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Screen::displayPumps() {
    unsigned long start = micros();

    resetTextSize();
    print("======= Pumps ======");
    printMargin(2);
    for (std::list<Ohmbrewer::Equipment*>::iterator itr = _sprouts->begin(); itr != _sprouts->end(); itr++) {
        if (strcmp((*itr)->getType(), "pump") == 0) {
            ((Ohmbrewer::HeatingElement*)(*itr))->display(this);
        }
    }
    printMargin(2);

    return micros() - start;
}

/**
 * Prints the temperature information for our sensors onto the touchscreen.
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Screen::displayTemps(double current, double target) {
    unsigned long start = micros();

    resetTextSize();
    resetTextColor();

    // Print the section title
    print("= Temperature (");
    write(247); // Degree symbol
    print("C) =");

    // Add a wee margin
    printMargin(2);

    // Print out the current temp
    displayCurrentTemp(current,target);

    // Print out the target temp
    displayTargetTemp(target);

    // Add another wee margin
    printMargin(2);

    return micros() - start;
}

/**
 * Prints the temperature information for our sensors onto the touchscreen.
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Screen::displayCurrentTemp(double current, double target) {
    unsigned long start = micros();

    // If current == target, we'll default to yellow, 'cause we're golden...
    uint16_t color = ILI9341_YELLOW;

    if(current > target) {
        // Too hot
        color = ILI9341_RED;
    } else if(current < target) {
        // Too cold
        color = ILI9341_CYAN;
    }

    displayTemp(current, "Current: ", color);
    
    return micros() - start;
}

/**
 * Prints the temperature information for our sensors onto the touchscreen.
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Screen::displayTargetTemp(double target) {
    unsigned long start = micros();
    displayTemp(target, "Target:  ", ILI9341_YELLOW);
    return micros() - start;
}

/**
 * Prints the temperature information for our sensors onto the touchscreen.
 * @param temp The temperature to display
 * @param label The text label to print to the left of the temperature
 * @param color The color of the temperature text
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Screen::displayTemp(double temp, char* label, uint16_t color) {
    unsigned long start = micros();
    char tempStr [24];

    sprintf(tempStr, "%2.2f", temp);

    // Print the label
    resetTextColor();
    print(" "); // We want a little margin
    print(label);

    // Print out the target temp
    setTextColor(color, DEFAULT_BG_COLOR);
    println(tempStr);

    resetTextColor();

    return micros() - start;
}

/**
 * Prints out a status message in the two rows above the buttons.
 * @param char* statusUpdate The status message to display. 40 characters or less.
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Screen::displayStatusUpdate(char *statusUpdate) {
    unsigned long start = micros();

    setTextColor(ILI9341_RED, DEFAULT_BG_COLOR);
    setCursor(LEFT, BUTTONTOP - 40);
    resetTextSize();
    println(statusUpdate);

    return micros() - start;
}