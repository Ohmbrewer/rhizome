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
             * Specifies the interface for arguments sent to this Equipment's associated function.
             * Parses the supplied string into an array of strings for setting the Equipment's values.
             * Most likely will be called during update().
             * @param argsStr The arguments supplied as an update to the Rhizome.
             * @param result A map representing the key/value pairs for the update
             */
            static void parseArgs(const String &argsStr, args_map_t &result);

            /**
             * The Equipment Type
             * @returns The Equipment type name
             */
            virtual const char* getType() const { return HeatingElement::TYPE_NAME; };

            /**
             * Constructor
             * @param id The Sprout ID to use for this piece of Equipment
             * @param powerPin - The power pin - on/off line. Digital pin number X.
             * @param controlPin - The Control pin - Data/speed/power level Digital pin number X.
             */
            HeatingElement(int id, int powerPin, int controlPin);

            /**
             * Constructor
             * @param id The Sprout ID to use for this piece of Equipment
             * @param powerPin - The power pin - on/off line. Digital pin number X.
             * @param controlPin - The Control pin - Data/speed/power level Digital pin number X.
             * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
             * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
             * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
             */
            HeatingElement(int id, int powerPin, int controlPin, int stopTime, bool state, String currentTask);

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
             * @param argsMap A map representing the key/value pairs for the update
             * @returns The time taken to run the method
             */
            int doUpdate(String &args, args_map_t &argsMap);

    };
};

#endif


