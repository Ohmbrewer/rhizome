#include "Adafruit_ILI9341.h"
#include "Ohmbrewer_Equipment.h"
#include "Ohmbrewer_Pump.h"
#include "Ohmbrewer_Temperature_Sensor.h"
#include "Ohmbrewer_Heating_Element.h"
#include "Ohmbrewer_Publisher.h"

// Kludge to allow us to use std::map - for now we have to undefine these macros.
#undef min
#undef max
#undef swap
#include <list>

/* ========================================================================= */
/*  Global Vars                                                              */
/* ========================================================================= */

/**
 * Each Sprout is a logical collection of physical pins/relays that are connected
 * to a single piece of Equipment.
 */
std::list< Ohmbrewer::Equipment* > sprouts;

/**
 * This is simply a map of which pins are currently accounted for. We'll want to
 * update it within the Sprout management functions.
 */
int inUse[6] = {0,0,0,0,0,0};

/**
 * The touchscreen object. Handles the display for the Rhizome.
 * TODO: Swap out the default Adafruit_ILI9341 class for the touchscreen with a subclass that includes our display routines.
 */
Adafruit_ILI9341 tft = Adafruit_ILI9341(D6, D7, A6);

/* ========================================================================= */
/*  Button Settings                                                          */
/* ========================================================================= */
#define BUTTONSIZE 60
#define LEFT 0
#define RIGHT 240
#define TOP 0
#define BOTTOM 320
#define BUTTONTOP 260

/* ========================================================================= */
/*  Main Functions                                                           */
/* ========================================================================= */
/**
 * Does any preliminary setup work before the Rhizome starts the operation loop.
 */
void setup() {
    String fakeTask = "fake";

    // Add our initial Equipment. We wouldn't necessarily do this, but it's useful for now.
    // We'll also set some temperatures and relay values explicitly when we probably wouldn't.
    sprouts.push_back(new Ohmbrewer::TemperatureSensor( 1, new std::list<int>(1,0) ));
    ((Ohmbrewer::TemperatureSensor*)sprouts.back())->getTemp()->set(42);

    sprouts.push_back(new Ohmbrewer::TemperatureSensor( 2, new std::list<int>(1,1) ));
    ((Ohmbrewer::TemperatureSensor*)sprouts.back())->getTemp()->set(1337);

    sprouts.push_back(new Ohmbrewer::Pump( 1, new std::list<int>(1,2), 0, true, fakeTask ));
    sprouts.push_back(new Ohmbrewer::Pump( 2, new std::list<int>(1,3) ));
    sprouts.push_back(new Ohmbrewer::HeatingElement( 1, new std::list<int>(1,4) ));
    sprouts.push_back(new Ohmbrewer::HeatingElement( 2, new std::list<int>(1,5), 0, true, fakeTask ));

    initScreen();
}

/**
 * The meat of the program. Runs repeatedly until the Rhizome is powered off.
 */
void loop() {
    refreshDisplay();
}

/* ========================================================================= */
/*  Display Functions                                                        */
/* ========================================================================= */

/**
 * Prints out a status message in the two rows above the buttons.
 * @param char* statusUpdate The status message to display. 40 characters or less.
 * @returns Time it took to run the function
 */
unsigned long displayStatusUpdate(char *statusUpdate) {
    unsigned long start = micros();

    tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
    tft.setCursor(LEFT, BUTTONTOP - 40);
    tft.setTextSize(2);
    tft.println(statusUpdate);

    return micros() - start;
}


/**
 * Initializes the display screen
 */
void initScreen() {

    tft.begin();
    tft.fillScreen(ILI9341_BLACK);
    drawButtons();

    // Reset the cursor
    tft.setCursor(LEFT, TOP);
}

/**
 * Initializes the display screen
 */
void drawButtons() {

    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);

    // make the buttons
    tft.fillRect(LEFT, BUTTONTOP, BUTTONSIZE, BUTTONSIZE, ILI9341_BLACK);
    tft.fillRect(BUTTONSIZE, BUTTONTOP, BUTTONSIZE, BUTTONSIZE, ILI9341_BLACK);
    tft.fillRect(BUTTONSIZE*2, BUTTONTOP, BUTTONSIZE, BUTTONSIZE, ILI9341_BLACK);
    tft.fillRect(BUTTONSIZE*3, BUTTONTOP, BUTTONSIZE, BUTTONSIZE, ILI9341_BLACK);
    tft.drawRect(LEFT, BUTTONTOP, BUTTONSIZE, BUTTONSIZE, ILI9341_GREEN);
    tft.drawRect(BUTTONSIZE, BUTTONTOP, BUTTONSIZE, BUTTONSIZE, ILI9341_GREEN);
    tft.drawRect(BUTTONSIZE*2, BUTTONTOP, BUTTONSIZE, BUTTONSIZE, ILI9341_GREEN);
    tft.drawRect(BUTTONSIZE*3, BUTTONTOP, BUTTONSIZE, BUTTONSIZE, ILI9341_GREEN);

    // Add symbols to the buttons
    tft.setTextSize(4);
    tft.setCursor(LEFT + 20, BUTTONTOP + 15);
    tft.print("+");
    tft.setCursor(BUTTONSIZE + 20, BUTTONTOP + 15);
    tft.print("-");

    tft.setTextSize(2);
    tft.setCursor((BUTTONSIZE*2) + 7, BUTTONTOP + 20);
    tft.print("Menu");
    tft.setTextSize(2);
    tft.setCursor((BUTTONSIZE*3) + 14, BUTTONTOP + 20);
    tft.print("Sel");

    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
}

/**
 * Refreshes/redraws the touchscreen display
 * @returns Time it took to run the function
 */
unsigned long refreshDisplay() {
    unsigned long start = micros();

    displayHeader();

    // FIXME: This sort of call will be moved into TemperatureSensor.doDisplay(). We won't be calling it like this explicitly.
    displayTemps(((Ohmbrewer::TemperatureSensor*)sprouts.front())->getTemp()->c(), 100.00);

    displayRelays();

    // 500 seems like a good refresh delay
    delay(500);

    return micros() - start;
}

/**
 * Prints the status information for our current relays onto the touchscreen
 * @returns Time it took to run the function
 */
unsigned long displayRelays() {
    unsigned long start = micros();
    unsigned int count = 0;

    tft.println("====== Relays ======");
    for (std::list<Ohmbrewer::Equipment*>::iterator itr = sprouts.begin(); itr != sprouts.end(); itr++) {
        if (count > 1) {
            // We'll ignore the TemperatureSensors here.
            displayRelay(count, (*itr)->getState());
        }
        count++;
    }

    return micros() - start;
}

/**
 * Prints the status information for a given relay onto the touchscreen
 * @param x The relay to display, 0-based
 * @param state The state of the relay
 * @returns Time it took to run the function
 */
unsigned long displayRelay(int x, bool state) {
    unsigned long start = micros();
    char relay_id[2];

    // Print a fancy identifier
    tft.print(" [");
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);

    sprintf(relay_id,"%d", x-1);
    tft.print(relay_id);

    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
    tft.print("]:");

    if (state){
        tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
        tft.println(" ON ");

    } else {
        tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
        tft.println(" OFF");
    }

    // Always reset to green
    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);

    return micros() - start;
}

/**
 * Prints the temperature information for our sensors onto the touchscreen
 * @returns Time it took to run the function
 */
unsigned long displayTemps(double current, double target) {
    unsigned long start = micros();
    char probeStr [24];
    char targetStr [24];

    sprintf(probeStr, "%2.2f", current);
    sprintf(targetStr, "%2.2f", target);

    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
    tft.setTextSize(2);
    tft.println("");
    tft.print("= Temperature (");
    tft.write(247); // Degree symbol
    tft.println("C) =");

    // Print out the current temp
    tft.print(" Current: ");
    if(current > target) {
        tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
    } else if(current < target) {
        tft.setTextColor(ILI9341_BLUE, ILI9341_BLACK);
    } else {
        tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
    }
    tft.println(probeStr);

    // Print out the target temp
    tft.setTextColor(ILI9341_GREEN);
    tft.print(" Target:  ");
    tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
    tft.println(targetStr);

    // Reset to green
    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);

    // Add a wee space
    tft.println("");

    return micros() - start;
}

/**
 * Prints the "Ohmbrewer" title onto the touchscreen
 * @returns Time it took to run the function
 */
unsigned long displayHeader() {
    unsigned long start = micros();
    tft.setCursor(0, 0);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft.setTextSize(3);
    tft.println("  OHMBREWER");
    return micros() - start;
}