#include "Ohmbrewer_Screen.h"
#include "Ohmbrewer_Equipment.h"
#include "Ohmbrewer_Temperature_Sensor.h"
#include "Ohmbrewer_Heating_Element.h"
#include "Ohmbrewer_Pump.h"
#include "Ohmbrewer_Relay.h"
#include "Ohmbrewer_Thermostat.h"
#include "Ohmbrewer_RIMS.h"

/**
 * Constructor
 */
Ohmbrewer::Screen::Screen(uint8_t CS,
                          uint8_t RS,
                          uint8_t RST,
                          std::deque< Ohmbrewer::Equipment* >* sprouts) : Adafruit_ILI9341(CS, RS, RST) {
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

    // Show the various data readouts
    // FIXME: This ain't quite right... These assume that Thermostats and RIMS would always be the first Sprouts entry.
    if(strcmp(_sprouts->front()->getType(), Thermostat::TYPE_NAME) == 0) {
        displayThermostats();
    } else if(strcmp(_sprouts->front()->getType(), RIMS::TYPE_NAME) == 0) {
        displayRIMS();
    } else {
        displayTemps();
    }
    printMargin(2);
    print("====== Relays ======");
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
    bool foundFirst = false;

    resetTextSizeAndColor();

    for (std::deque<Ohmbrewer::Equipment*>::iterator itr = _sprouts->begin(); itr != _sprouts->end(); itr++) {
        if (strcmp((*itr)->getType(), TemperatureSensor::TYPE_NAME) != 0 &&
            strcmp((*itr)->getType(), RIMS::TYPE_NAME) != 0 &&
            strcmp((*itr)->getType(), Thermostat::TYPE_NAME) != 0) {
            if(!foundFirst) {
                // Print the header
//                print("====== Relays ======");
                printMargin(2);
                foundFirst = true;
            }
            ((Ohmbrewer::Relay*)(*itr))->display(this);
        }
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
    bool foundFirst = false;

    resetTextSizeAndColor();

    for (std::deque<Ohmbrewer::Equipment*>::iterator itr = _sprouts->begin(); itr != _sprouts->end(); itr++) {
        if (strcmp((*itr)->getType(), HeatingElement::TYPE_NAME) == 0) {
            if(!foundFirst) {
                // Print the header
//                print("======= Heat =======");
                printMargin(2);
                foundFirst = true;
            }
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
    bool foundFirst = false;

    resetTextSizeAndColor();

    for (std::deque<Ohmbrewer::Equipment*>::iterator itr = _sprouts->begin(); itr != _sprouts->end(); itr++) {
        if (strcmp((*itr)->getType(), Pump::TYPE_NAME) == 0) {
            if(!foundFirst) {
                // Print the header
//                print("======= Pumps ======");
                printMargin(2);
                foundFirst = true;
            }
            ((Ohmbrewer::Pump*)(*itr))->display(this);
        }
    }
    printMargin(2);

    return micros() - start;
}

/**
 * Prints the temperature information for our sensors onto the touchscreen.
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Screen::displayTemps() {
    unsigned long start = micros();
    bool foundFirst = false;

    resetTextSizeAndColor();

    for (std::deque<Ohmbrewer::Equipment*>::iterator itr = _sprouts->begin(); itr != _sprouts->end(); itr++) {
        if (strcmp((*itr)->getType(), TemperatureSensor::TYPE_NAME) == 0) {
            if(!foundFirst) {
                // Print the header
                print("= Temperature (");
                writeDegree(); // Degree symbol
                print("C) =");
                printMargin(2);
                foundFirst = true;
            }
            ((Ohmbrewer::TemperatureSensor*)(*itr))->display(this);
        }
    }
    printMargin(2);

    return micros() - start;
}

/**
 * Prints the thermostat information onto the touchscreen.
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Screen::displayThermostats() {
    unsigned long start = micros();

    resetTextSizeAndColor();

    printMargin(2);
    for (std::deque<Ohmbrewer::Equipment*>::iterator itr = _sprouts->begin(); itr != _sprouts->end(); itr++) {
        if (strcmp((*itr)->getType(), Thermostat::TYPE_NAME) == 0) {
            ((Ohmbrewer::Thermostat*)(*itr))->display(this);
        }
    }
    printMargin(2);

    return micros() - start;
}

/**
 * Prints the RIMS information onto the touchscreen.
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Screen::displayRIMS() {
    unsigned long start = micros();

    resetTextSizeAndColor();

    printMargin(2);
    for (std::deque<Ohmbrewer::Equipment*>::iterator itr = _sprouts->begin(); itr != _sprouts->end(); itr++) {
        if (strcmp((*itr)->getType(), RIMS::TYPE_NAME) == 0) {
            ((Ohmbrewer::RIMS*)(*itr))->display(this);
        }
    }
    printMargin(2);

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