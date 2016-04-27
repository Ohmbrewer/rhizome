/**
 * This is a testing file that you can enable by swapping out the comment
 * in the makelist.  This is for creating and adding equipment manually.
 * The regular .ino file is reserved for dynamic equipment modification.
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
#include "onewire.h"
#include "Ohmbrewer_Rhizome.h"

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
 * A timer for doing things every 15 seconds. Used by the rhizome below.
 */
Timer periodicUpdateTimer = Timer(15000, doPeriodicUpdates);

/**
 * The object representing all of the Rhizome's functionality.
 */
Ohmbrewer::Rhizome rhizome = Ohmbrewer::Rhizome(&periodicUpdateTimer);

unsigned long lastUpdate = millis();

// EX: Using a Publisher object. Other parts are marked with a (*). You'll probably want to use a different update
//     rate (more like 30s instead of 10s) below.
//Ohmbrewer::Publisher::publish_map_t pMap;
//Ohmbrewer::Publisher* navi = new Ohmbrewer::Publisher(new String("fairies"), &pMap);

// Setting the photon to semi automatic mode, which means it
// does not connect to WiFi until Particle.connect() is called.
SYSTEM_MODE(SEMI_AUTOMATIC);

/* ========================================================================= */
/* Main Functions (setup, loop)                                              */
/* ========================================================================= */
/**
 * Does any preliminary setup work before the Rhizome starts the operation loop.
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

    String fakeTask = "fake";
//    thermPins.push_back(0); //onewire index
//    thermPins.push_back(2); //control pin (relay pin)
//    thermPins.push_back(3); //power pin (switch pin)
//    thermPins2.push_back(4); //control pin (relay pin)
//    thermPins2.push_back(5); //power pin (switch pin)

    // Add our initial Equipment. We wouldn't necessarily do this, but it's useful for now.
    // We'll also set some temperatures and relay values explicitly when we probably wouldn't.
// EX 1: Temperature Sensors
//    rhizome.getSprouts()->push_back(new Ohmbrewer::TemperatureSensor( new Ohmbrewer::Onewire() ));
//    ((Ohmbrewer::TemperatureSensor*)rhizome.getSprouts()->back())->getTemp()->set(42);
//
//    rhizome.getSprouts()->push_back(new Ohmbrewer::TemperatureSensor( new Ohmbrewer::Onewire() ));
//    ((Ohmbrewer::TemperatureSensor*)rhizome.getSprouts()->back())->getTemp()->set(-20);

// EX 2: A Thermostat
//    rhizome.getSprouts()->push_front(new Ohmbrewer::Thermostat( &thermPins, 25 ));
//    rhizome.getSprouts()->push_front(new Ohmbrewer::Thermostat( &thermPins2, 45 ));
    //((Ohmbrewer::Thermostat*)rhizome.getSprouts()->front())->getElement()->setState(true);

// EX 3: Pumps & Heating Elements
//    rhizome.getSprouts()->push_back(new Ohmbrewer::Pump( 1, 0, true, fakeTask ));
//    rhizome.getSprouts()->push_back(new Ohmbrewer::Pump( 1 ));
//    rhizome.getSprouts()->push_back(new Ohmbrewer::HeatingElement( &thermPins ));
//    rhizome.getSprouts()->push_back(new Ohmbrewer::HeatingElement( new std::list<int>(1,5), 0, true, fakeTask ));

// EX 4: A RIMS w/ a Pump to the Kettle
//
//    int pumpPin = 4;
//    int pumpPin2 = 5;
//    rhizome.getSprouts()->push_back(new Ohmbrewer::RIMS( &thermPins, pumpPin ));
//    rhizome.getSprouts()->push_back(new Ohmbrewer::Pump( pumpPin2 )); //transfer pump
//    ((Ohmbrewer::RIMS*)rhizome.getSprouts()->front())->setState(true); // Turn everything on.
//    ((Ohmbrewer::Thermostat*)rhizome.getSprouts()->front())->setState(true); // Turn everything on.
//    ((Ohmbrewer::RIMS*)rhizome.getSprouts()->front())->getTube()->setTargetTemp(62.7);

    // Turn on screen
    rhizome.getScreen()->initScreen();
//    pMap[String("hey")] = String("listen!"); // (*)

    //Turn on for debugging
    Serial.begin(9600);
}

/**
 * The meat of the program. Runs repeatedly until the Rhizome is powered off.
 */
void loop() {

    rhizome.work();
//    //probe ID finder command (switch this for the other below commands to enable)
//    ((Ohmbrewer::Onewire*)(((Ohmbrewer::RIMS*)rhizome.getSprouts()->front())->getTube()->getSensor()->getProbe()))
//            ->displayProbeIds(&screen);

//Thermostat
    //((Ohmbrewer::Thermostat*)rhizome.getSprouts()->front())->getSensor()->work();
//    ((Ohmbrewer::Thermostat*)rhizome.getSprouts()->front())->setState(true);
//    ((Ohmbrewer::Thermostat*)rhizome.getSprouts()->front())->work();
//    ((Ohmbrewer::Thermostat*)rhizome.getSprouts()->back())->setState(true);
//    ((Ohmbrewer::Thermostat*)rhizome.getSprouts()->back())->work();

//    if((millis() - lastUpdate) > 10000) {
//        // Toggle the last relay every 10 seconds, for illustration.
//        ((Ohmbrewer::RIMS*)rhizome.getSprouts()->front())->getTube()
//                                           ->getElement()
//                                           ->toggleState();
//
//        navi->publish(); // (*)
//        lastUpdate = millis();
//    }
//    ((Ohmbrewer::RIMS*)rhizome.getSprouts()->front())->getTube()->getSensor()->work(); //Temp patch to make sensor read in Tun temp

    // Check for button press and refresh the screen
    rhizome.getScreen()->captureButtonPress();
    rhizome.getScreen()->refreshDisplay();
}

/* ========================================================================= */
/* Other Global Functions                                                    */
/* ========================================================================= */

/**
 * Delegation function that allows us to call the managed method for publishing periodic updates.
 */
void doPeriodicUpdates() {
    // Do not attempt to publish updates if disconnected from the cloud
    if(!Particle.connected()){
        if(EEPROM.read(Ohmbrewer::RuntimeSettings::WIFI_STATUS_ADDR) == Ohmbrewer::RuntimeSettings::EEPROM_WIFI_STATUS_ON){
            // WiFi is not connected and should be - attempt to connect
            Particle.connect();
        }
    } else {
        rhizome.publishPeriodicUpdates();
    }
}
