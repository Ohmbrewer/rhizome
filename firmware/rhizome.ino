/**
 * This is the master firmware file for the Ohmbrewer Rhizome V1_0_0
 *
 * Equipment is dynamically added and removed via Sprouts class methods.
 * See README.md file for details.
 *
 * TODO insert licence file ?
 */

#include "Ohmbrewer_Screen.h"
#include "Ohmbrewer_Equipment.h"
#include "Ohmbrewer_Pump.h"
#include "Ohmbrewer_Temperature_Sensor.h"
#include "Ohmbrewer_Thermostat.h"
#include "Ohmbrewer_Heating_Element.h"
#include "Ohmbrewer_Publisher.h"
#include "Ohmbrewer_Thermostat.h"
#include "Ohmbrewer_RIMS.h"
#include "Ohmbrewer_Onewire.h"
#include "Ohmbrewer_Sprouts.h"
//external libraries
#include "onewire.h"

// Kludge to allow us to use std::list - for now we have to undefine these macros.
#undef min
#undef max
#undef swap
#include <deque>
#include <list>

/* ========================================================================= */
/*  Global Vars                                                              */
/* ========================================================================= */

// Each Sprout is a logical collection of physical pins/relays that are connected
// to a single piece of Equipment.
std::deque< Ohmbrewer::Equipment* > sprouts;

// The touchscreen object. Handles the display for the Rhizome.
Ohmbrewer::Screen screen = Ohmbrewer::Screen(D6, D7, A6, &sprouts);

// A timer for doing things every 15 seconds. Used by the sproutList below.
Timer periodicUpdateTimer = Timer(15000, doPeriodicUpdates);

// The managed list of sprouts
Ohmbrewer::Sprouts sproutList = Ohmbrewer::Sprouts(&sprouts, &screen, &periodicUpdateTimer);

unsigned long lastUpdate = millis();

/* ========================================================================= */
/*  Main Functions   (setup, loop)                                           */
/* ========================================================================= */
/**
 * Does any preliminary setup and initializations before the Rhizome starts the operation loop.
 */
void setup() {
    //initialize the Dallas Onewire bus pin - Digital 0
    ow_setPin(D0); //This should later be accomplished by equipment setup OR constructor

    // Turn on the display
    screen.initScreen();
}

/**
 * The bulk of the program. Runs repeatedly until the Rhizome is powered off.
 */
void loop() {
    //call work on all installed equipment in the sprouts list
    sproutList.work();

    //refresh the display
    screen.refreshDisplay();
}

/* ========================================================================= */
/*  Other Global Functions                                                   */
/* ========================================================================= */

/**
 * Delegation function that allows us to call the managed method for publishing periodic updates.
 */
void doPeriodicUpdates() {
    sproutList.publishPeriodicUpdates();
}
