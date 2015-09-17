#include "Ohmbrewer_Heating_Element.h"

/**
 * Constructor
 * @param id The Sprout ID to use for this piece of Equipment
 * @param pins The list of physical pins this Equipment is attached to
 */
Ohmbrewer::HeatingElement::HeatingElement(int id, std::list<int>* pins) : Ohmbrewer::Relay(id, pins) {
    _type = "heat";
}

/**
 * Constructor
 * @param id The Sprout ID to use for this piece of Equipment
 * @param pins The list of physical pins this Equipment is attached to
 * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
 * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
 * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
 */
Ohmbrewer::HeatingElement::HeatingElement(int id, std::list<int>* pins, int stopTime,
                                          bool state, String currentTask) : Ohmbrewer::Relay(id, pins, stopTime, state, currentTask) {
    _type = "heat";
}

/**
 * Copy Constructor
 * @param clonee The Equipment object to copy
 */
Ohmbrewer::HeatingElement::HeatingElement(const HeatingElement& clonee) : Ohmbrewer::Relay(clonee) {
    // This has probably already been set, but maybe clonee is a more complicated child class...
    _type = "heat";
}

/**
 * Destructor
 */
Ohmbrewer::HeatingElement::~HeatingElement() {
    // Nothing to do here...
}

/**
 * Overloaded << operator.
 */
// friend std::ostream& operator<<( std::ostream& os, HeatingElement const& HeatingElement);

/**
 * Performs the Equipment's current task. Expect to use this during loop().
 * This function is called by work().
 * @returns The time taken to run the method
 */
int Ohmbrewer::HeatingElement::doWork() {
    // TODO: Implement HeatingElement::doWork
    return -1;
}

/**
 * Draws information to the Rhizome's display.
 * This function is called by display().
 * @returns The time taken to run the method
 */
int Ohmbrewer::HeatingElement::doDisplay(Ohmbrewer::Screen *screen) {
    return Relay::doDisplay(screen);
}

/**
 * Publishes updates to Ohmbrewer, etc.
 * This function is called by update().
 * @returns The time taken to run the method
 */
int Ohmbrewer::HeatingElement::doUpdate() {
    // TODO: Implement HeatingElement::doUpdate
    return -1;
}

