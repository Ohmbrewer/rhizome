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
#include "Ohmbrewer_Sprouts.h"
// #include "Ohmbrewer_Publisher.h" // (*)

// Kludge to allow us to use std::list - for now we have to undefine these macros.
#undef min
#undef max
#undef swap
#include <deque>
#include <list>

/* ========================================================================= */
/*  Global Vars                                                              */
/* ========================================================================= */

/**
 * Each Sprout is a logical collection of physical pins/relays that are connected
 * to a single piece of Equipment.
 */
std::deque< Ohmbrewer::Equipment* > sprouts;

//initializer Pin list
std::list<int> thermPins (1, 0);
std::list<int> thermPins2 (1, 0);

/**
 * The touchscreen object. Handles the display for the Rhizome.
 */
Ohmbrewer::Screen screen = Ohmbrewer::Screen(D6, D7, A6, &sprouts);

/**
 * A timer for doing things every 15 seconds. Used by the sproutList below.
 */
Timer periodicUpdateTimer = Timer(15000, doPeriodicUpdates);

/**
 * The managed list of sprouts
 * @todo Refactor how we handle Sprouts/SproutList so that the deque is encapsulated and Screen takes the list object
 */
Ohmbrewer::Sprouts sproutList = Ohmbrewer::Sprouts(&sprouts, &screen, &periodicUpdateTimer);

unsigned long lastUpdate = millis();

// EX: Using a Publisher object. Other parts are marked with a (*). You'll probably want to use a different update
//     rate (more like 30s instead of 10s) below.
//Ohmbrewer::Publisher::publish_map_t pMap;
//Ohmbrewer::Publisher* navi = new Ohmbrewer::Publisher(new String("fairies"), &pMap);

/* ========================================================================= */
/*  Main Functions                                                           */
/* ========================================================================= */
/**
 * Does any preliminary setup work before the Rhizome starts the operation loop.
 */
void setup() {
//    Serial.begin(9600); // Enable serial for debugging messages
    String fakeTask = "fake";
    thermPins.push_back(2); //control pin (relay pin)
    thermPins.push_back(3); //power pin (switch pin)
    thermPins2.push_back(4); //control pin (relay pin)
    thermPins2.push_back(5); //power pin (switch pin)

    ow_setPin(D0); //This should later be accomplished by equipment setup OR constructor

    // Add our initial Equipment. We wouldn't necessarily do this, but it's useful for now.
    // We'll also set some temperatures and relay values explicitly when we probably wouldn't.
// EX 1: Temperature Sensors
//    sprouts.push_back(new Ohmbrewer::TemperatureSensor( 8, new Ohmbrewer::Onewire() ));
//    ((Ohmbrewer::TemperatureSensor*)sprouts.back())->getTemp()->set(42);
//
//    sprouts.push_back(new Ohmbrewer::TemperatureSensor( 9, new Ohmbrewer::Onewire() ));
//    ((Ohmbrewer::TemperatureSensor*)sprouts.back())->getTemp()->set(-20);

// EX 2: A Thermostat
    sprouts.push_front(new Ohmbrewer::Thermostat( 1, &thermPins, 25 ));
    sprouts.push_front(new Ohmbrewer::Thermostat( 4, &thermPins2, 45 ));
    //((Ohmbrewer::Thermostat*)sprouts.front())->getElement()->setState(true);

// EX 3: Pumps & Heating Elements
//    sprouts.push_back(new Ohmbrewer::Pump( 1, 1, 0, true, fakeTask ));
//    sprouts.push_back(new Ohmbrewer::Pump( 7, 1 ));
//    sprouts.push_back(new Ohmbrewer::HeatingElement( 3, &thermPins ));
//    sprouts.push_back(new Ohmbrewer::HeatingElement( 4, new std::list<int>(1,5), 0, true, fakeTask ));

// EX 4: A RIMS w/ a Pump to the Kettle
//
//    int pumpPin = 4;
//    int pumpPin2 = 5;
//    sprouts.push_back(new Ohmbrewer::RIMS( 1, &thermPins, pumpPin ));
//    sprouts.push_back(new Ohmbrewer::Pump( 8 , pumpPin2 )); //transfer pump
//    ((Ohmbrewer::RIMS*)sprouts.front())->setState(true); // Turn everything on.
//    ((Ohmbrewer::Thermostat*)sprouts.front())->setState(true); // Turn everything on.
//    ((Ohmbrewer::RIMS*)sprouts.front())->getTube()->setTargetTemp(62.7);

    // Turn on screen
    screen.initScreen();
//    pMap[String("hey")] = String("listen!"); // (*)
}

/**
 * The meat of the program. Runs repeatedly until the Rhizome is powered off.
 */
void loop() {
//    //probe ID finder command (switch this for the other below commands to enable)
//    ((Ohmbrewer::Onewire*)(((Ohmbrewer::RIMS*)sprouts.front())->getTube()->getSensor()->getProbe()))
//            ->displayProbeIds(&screen);

//Thermostat
    //((Ohmbrewer::Thermostat*)sprouts.front())->getSensor()->work();
    ((Ohmbrewer::Thermostat*)sprouts.front())->setState(true);
    ((Ohmbrewer::Thermostat*)sprouts.front())->work();
    ((Ohmbrewer::Thermostat*)sprouts.back())->setState(true);
    ((Ohmbrewer::Thermostat*)sprouts.back())->work();

//    if((millis() - lastUpdate) > 10000) {
//        // Toggle the last relay every 10 seconds, for illustration.
//        ((Ohmbrewer::RIMS*)sprouts.front())->getTube()
//                                           ->getElement()
//                                           ->toggleState();
//
//        navi->publish(); // (*)
//        lastUpdate = millis();
//    }
//    ((Ohmbrewer::RIMS*)sprouts.front())->getTube()->getSensor()->work(); //Temp patch to make sensor read in Tun temp

    //refresh the display
    screen.refreshDisplay();
}

/* ========================================================================= */
/*  Other Global Functions                                                   */
/* ========================================================================= */

/**
 * Delegator function that allows us to call the managed method for publishing periodic updates.
 */
void doPeriodicUpdates() {
    sproutList.publishPeriodicUpdates();
}
