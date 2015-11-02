/**
 * This library provides the Equipment base class the Rhizome PID/equipment controller.
 * Rhizome is part of the Ohmbrewer project (see http://ohmbrewer.org for details).
 */

#ifndef OHMBREWER_RHIZOME_RELAY_H
#define OHMBREWER_RHIZOME_RELAY_H

// Kludge to allow us to use std::list - for now we have to undefine these macros.
#undef min
#undef max
#undef swap
#include <list>
#include "Ohmbrewer_Equipment.h"
#include "application.h"

namespace Ohmbrewer {

    // Forward declaration
    class Screen;

    class Relay : public Equipment {

        public:

            /**
             * The short-hand type name. Used for communicating with Ohmbrewer and disambiguating Equipment* pointers.
             */
            const static constexpr char* TYPE_NAME = "relay";

            /**
             * The Equipment Type
             * @returns The Equipment type name
             */
            virtual const char* getType() const { return Relay::TYPE_NAME; };

            /**
             * Constructor - used by Pump to manually instantiate Relay
             * @param id The Sprout ID to use for this piece of Equipment
             * @param pumpPin - Single speed pump will only have PowerPin

             */
            Relay(int id, int powerPin);

            /**
             * Constructor
             * @param id The Sprout ID to use for this piece of Equipment
             * @param pumpPin - Single speed pump will only have PowerPin
             * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
             * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
             * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
             */
            Relay(int id, int powerPin, int stopTime, bool state, String currentTask);

            /**
             * Constructor
             * @param id The Sprout ID to use for this piece of Equipment
             * @param relayPins - controlPin always first in <list>
             *  controlPin - The Control pin - Data/speed/power level Digital pin number X.
             *  powerPin - The power pin - on/off line. Digital pin number X.
             *
             */
            Relay(int id, std::list<int>* relayPins);

            /**
             * Constructor
             * @param id The Sprout ID to use for this piece of Equipment
             * @param relayPins - controlPin always first in <list>
             *  controlPin - The Control pin - Data/speed/power level Digital pin number X.
             *  powerPin - The power pin - on/off line. Digital pin number X.
             * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
             * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
             * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
             */
            Relay(int id, std::list<int>* relayPins, int stopTime, bool state, String currentTask);

            /**
             * Copy Constructor
             * @param clonee The Equipment object to copy
             */
            Relay(const Relay& clonee);
            
            /**
             * Destructor
             */
            virtual ~Relay();

            /**
             * The power pin - on/off line
             * hardware destinations (hardware switches - DPDT, main power)
             * @returns The pin number in use for this piece of Equipment
             */
            int getPowerPin() const;

            /**
             * Sets the Digital pin for the power pin.
             * @param pinNum Dx
             * @returns The time taken to run the method
             */
            const int setPowerPin(const int pinNum);

            /**
             * The Control pin - Data/speed/power level
             * hardware destinations (SSR, databus(temp sensor), pump speed control)
             * @returns The pin number in use for this piece of Equipment
             */
            int getControlPin() const;

            /**
             * Sets the Digital pin for the control pin.
             * @param pinNum Dx
             * @returns The time taken to run the method
             */
            const int setControlPin(const int pinNum);

            /**
             * Specifies the interface for arguments sent to this Equipment's associated function.
             * Parses the supplied string into an array of strings for setting the Equipment's values.
             * Most likely will be called during update().
             * @param argsStr The arguments supplied as an update to the Rhizome.
             * @param result A map representing the key/value pairs for the update
             */
            static void parseArgs(const String &argsStr, args_map_t &result);

            /**
             * Sets the Equipment state. True => On, False => Off
             * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
             * @returns The time taken to run the method
             */
            const int setState(const bool);

            /**
             * The Equipment state. True => On, False => Off
             * @returns True => On, False => Off
             */
            bool getState() const;

            /**
             * True if the Equipment state is On.
             * @returns Whether the Equipment is turned ON
             */
            bool isOn() const;

            /**
             * True if the Equipment state is Off.
             * @returns Whether the Equipment is turned OFF
             */
            bool isOff() const;

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

            /**
             * Reports which of the Rhizome's pins are occupied by the
             * Equipment, forming a logical Sprout.
             * @param pins The list of physical pins that the Equipment is connected to.
             */
            void whichPins(std::list<int>* pins);

        protected:
            /**
             * Digital Pin for on/off relay power
             */
            int            _powerPin;

            /**
             * Digital pin for relay control
             */
            int            _controlPin;
    };
};

#endif


