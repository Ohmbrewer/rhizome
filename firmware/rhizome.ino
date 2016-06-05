/**
 * This is the master firmware file for the Ohmbrewer Rhizome V1_0_0
 *
 * Equipment is dynamically added and removed via Rhizome class methods.
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
#include "Ohmbrewer_Rhizome.h"
#include "Ohmbrewer_Runtime_Settings.h"
//external libraries
#include "onewire.h"

// Kludge to allow us to use std::list - for now we have to undefine these macros.
#undef min
#undef max
#undef swap
#include <deque>
#include <list>

/* ========================================================================= */
/* Global Vars                                                               */
/* ========================================================================= */

/**
 * The object representing all of the Rhizome's functionality.
 */
Ohmbrewer::Rhizome rhizome = Ohmbrewer::Rhizome();

unsigned long lastUpdate = millis();

// Setting the photon to semi automatic mode, which means it
// does not connect to WiFi until Particle.connect() is called.
SYSTEM_MODE(SEMI_AUTOMATIC);

/* ========================================================================= */
/* Main Functions (setup, loop)                                              */
/* ========================================================================= */

/**
 * Does any preliminary setup and initializations before the Rhizome starts the operation loop.
 */
void setup() {
	//initialize the Dallas Onewire bus pin - Digital 0
	ow_setPin(D0); //This should later be accomplished by equipment setup OR constructor

    if(!Particle.connected()){
        if(EEPROM.read(Ohmbrewer::RuntimeSettings::WIFI_STATUS_ADDR) == Ohmbrewer::RuntimeSettings::EEPROM_WIFI_STATUS_ON){
            //WiFi is not connected and should be - attempt to connect
            Particle.connect();
        }
    }
	// Turn on the display
	rhizome.getScreen()->initScreen();
	
	//Turn on for debugging
	Serial.begin(9600);
}

/**
 * The bulk of the program. Runs repeatedly until the Rhizome is powered off.
 */
void loop() {
    // Call work on all installed equipment in the sprouts list
    rhizome.work();

	// Check for button press and refresh the screen
    rhizome.getScreen()->captureButtonPress();
    rhizome.getScreen()->refreshDisplay();
}
