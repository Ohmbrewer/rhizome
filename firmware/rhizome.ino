#include "Ohmbrewer_Screen.h"
#include "Ohmbrewer_Equipment.h"
#include "Ohmbrewer_Pump.h"
#include "Ohmbrewer_Temperature_Sensor.h"
#include "Ohmbrewer_Thermostat.h"
#include "Ohmbrewer_Heating_Element.h"
#include "Ohmbrewer_Publisher.h"
#include "Ohmbrewer_Thermostat.h"
#include "Ohmbrewer_RIMS.h"

// Kludge to allow us to use std::list - for now we have to undefine these macros.
#undef min
#undef max
#undef swap
#include <deque>

/* ========================================================================= */
/*  Global Vars                                                              */
/* ========================================================================= */

/**
 * Each Sprout is a logical collection of physical pins/relays that are connected
 * to a single piece of Equipment.
 */
std::deque< Ohmbrewer::Equipment* > sprouts;

/**
 * This is simply a map of which pins are currently accounted for. We'll want to
 * update it within the Sprout management functions.
 */
int inUse[6] = {0,0,0,0,0,0};

/**
 * The touchscreen object. Handles the display for the Rhizome.
 */
Ohmbrewer::Screen screen = Ohmbrewer::Screen(D6, D7, A6, &sprouts);

unsigned long lastUpdate = millis();

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
// EX 1: Temperature Sensors
//    sprouts.push_back(new Ohmbrewer::TemperatureSensor( 1, new std::deque<int>(1,0) ));
//    ((Ohmbrewer::TemperatureSensor*)sprouts.back())->getTemp()->set(42);
//
//    sprouts.push_back(new Ohmbrewer::TemperatureSensor( 2, new std::list<int>(1,1) ));
//    ((Ohmbrewer::TemperatureSensor*)sprouts.back())->getTemp()->set(1337);

// EX 2: A Thermostat
//    sprouts.push_back(new Ohmbrewer::Thermostat( 1, new std::list<int>(1,0), 100 ));
//    ((Ohmbrewer::Thermostat*)sprouts.front())->getElement()->setState(true);

// EX 3: Pumps & Heating Elements
//    sprouts.push_back(new Ohmbrewer::Pump( 1, new std::list<int>(1,2), 0, true, fakeTask ));
//    sprouts.push_back(new Ohmbrewer::Pump( 2, new std::list<int>(1,3) ));
//    sprouts.push_back(new Ohmbrewer::HeatingElement( 1, new std::list<int>(1,4) ));
//    sprouts.push_back(new Ohmbrewer::HeatingElement( 2, new std::list<int>(1,5), 0, true, fakeTask ));

// EX 4: A RIMS w/ a Pump to the Kettle
    std::list<int>* rimsPins = new std::list<int>();
    for(int i = 1; i < 5; i++) {rimsPins->push_back(i);}

    sprouts.push_back(new Ohmbrewer::RIMS( 1, rimsPins ));
    ((Ohmbrewer::RIMS*)sprouts.front())->setState(true); // Turn everything on.
    ((Ohmbrewer::RIMS*)sprouts.front())->getTube()->setTargetTemp(559);

    sprouts.push_back(new Ohmbrewer::Pump( 1, new std::list<int>(1,5) ));

    screen.initScreen();
}

/**
 * The meat of the program. Runs repeatedly until the Rhizome is powered off.
 */
void loop() {
    if((millis() - lastUpdate) > 10000) {
        // Toggle the last relay every 10 seconds, for illustration.
//        sprouts.back()->setState(sprouts.back()->isOff()); // The last HeatingElement
//        ((Ohmbrewer::Thermostat*)sprouts.at(0))->getElement()
//                                               ->setState(!((Ohmbrewer::Thermostat*)sprouts.at(0))->getElement()
//                                                                                                  ->getState()); // The Thermostat's HeatingElement
        ((Ohmbrewer::RIMS*)sprouts.front())->getTube()
                                           ->getElement()
                                           ->toggleState();
        lastUpdate = millis();
    }
    screen.refreshDisplay();
}


///**
// * Prints the status information for our current relays onto the touchscreen
// * @returns Time it took to run the function
// */
//Ohmbrewer::Equipment* getSprout(const char* typeName, const int id) {
//    std::deque<Ohmbrewer::Equipment*>::iterator itr = sprouts.begin();
//
//    for (itr; itr != sprouts.end(); itr++) {
//        if (((*itr)->getID() == id) && (strcmp((*itr)->getType(), typeName) == 0)) {
//            return *itr;
//        }
//    }
//
//    return *itr;
//}