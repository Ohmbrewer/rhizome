/**
 * This library provides the Heating Element class for the Rhizome PID/equipment controller.
 * Rhizome is part of the Ohmbrewer project (see http://ohmbrewer.org for details).
 */

#ifndef OHMBREWER_RHIZOME_HEATING_ELM_H
#define OHMBREWER_RHIZOME_HEATING_ELM_H

// Kludge to allow us to use std::list - for now we have to undefine these macros.
#undef min
#undef max
#undef swap
#include <list>
#include "Ohmbrewer_Relay.h"
#include "application.h"

namespace Ohmbrewer {

    class HeatingElement : public Relay {
      
        public:

            /**
             * The short-hand type name. Used for communicating with Ohmbrewer and disambiguating Equipment* pointers.
             */
            const static constexpr char* TYPE_NAME = "heat";

            /**
             * The Equipment Type
             * @returns The Equipment type name
             */
            virtual const char* getType() const { return HeatingElement::TYPE_NAME; };

            /**
             * Constructor
             * @param id The Sprout ID to use for this piece of Equipment
             * @param pins The list of physical pins this Equipment is attached to
             */
            HeatingElement(int id, std::list<int>* pins);

            /**
             * Constructor
             * @param id The Sprout ID to use for this piece of Equipment
             * @param pins The list of physical pins this Equipment is attached to
             * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
             * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
             * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
             */
            HeatingElement(int id, std::list<int>* pins, int stopTime, bool state, String currentTask);

            /**
             * Copy Constructor
             * @param clonee The Equipment object to copy
             */
            HeatingElement(const HeatingElement& clonee);
            
            /**
             * Destructor
             */
            virtual ~HeatingElement();
            
            /**
             * Overloaded << operator.
             */
            // friend std::ostream& operator<<( std::ostream& os, Pump const& pump);

            /**
             * Performs the Equipment's current task. Expect to use this during loop().
             * This function is called by work().
             * @returns The time taken to run the method
             */
            int doWork();

            /**
             * Draws information to the Rhizome's display.
             * This function is called by display().
             * @param screen The Rhizome's touchscreen
             * @returns The time taken to run the method
             */
            int doDisplay(Screen *screen);

            /**
             * Publishes updates to Ohmbrewer, etc.
             * This function is called by update().
             * @param args The argument string passed into the Particle Cloud
             * @returns The time taken to run the method
             */
            int doUpdate(String* args);

    };
};

#endif


