/**
 * This library provides the Temperature class for the Rhizome PID/equipment controller.
 * Rhizome is part of the Ohmbrewer project (see http://ohmbrewer.org for details).
 */

#ifndef OHMBREWER_SPROUTS_H
#define OHMBREWER_SPROUTS_H

#undef min
#undef max
#undef swap
#include <deque>
#include "Ohmbrewer_Equipment.h"
#include "Ohmbrewer_Screen.h"
#include "application.h"


namespace Ohmbrewer {

    class Equipment;

    class Sprouts {
      
        public:

        /**
         * Constructor
         * @param sprouts A pointer to a pre-constructed deque object. Sprouts takes over responsibility of memory management for it.
         * @param screen A pointer to the Screen object. Sprouts does not handle its memory management.
         */
        Sprouts(std::deque< Ohmbrewer::Equipment* > *sprouts, Ohmbrewer::Screen *screen);

        /**
         * Destructor. Kills the internal deque.
         */
        ~Sprouts();

        /**
         * Dynamically adds Equipment to the Rhizome.
         * The Equipment will be added in the OFF state, with no current task.
         * Hence, it is probably necessary to send an update immediately after a successful addition.
         *
         * Additionally, the argument string for this function must match the following format:
         * TYPE,ID,{PINS}
         * where
         * TYPE matches the result of TYPE_NAME for the desired equipment class
         * ID isn't already in use
         * PINS is the appropriate length for the equipment type AND
         *      is in the expected order for the equipment constructor that will be called (as described above)
         *
         * @param argsStr The argument string passed via the Particle Cloud.
         * @returns Equipment ID if successful,
         *          (negative) error codes if unsuccessful:
         *          -1 : Invalid ID
         *          -2 : ID already in use for given Equipment Type
         *          -3 : One or more of the provided pins is already in use
         *          -4 : Incorrect number of pins provided for given Equipment Type
         *          -5 : Not Implemented
         */
        int addSprout(String argsStr);

        /**
         * Updates Equipment by routing/delegating to the Equipment subclass's update() method.
         *
         * The argument string for this function must match the following format:
         * TYPE,ID,UPDATE_STRING
         * where
         * TYPE matches the TYPE_NAME for the desired Equipment
         * ID matches the ID of the desired Equipment
         * UPDATE_STRING is a comma-delimited string that matches the format expected by the desired Equipment's class
         *
         * Both the ID and the UPDATE_STRING will be passed onto the update() method, but ID is validated within
         * this method before update() is called as one more check against Hard Faults.
         *
         * @param argsStr The argument string passed via the Particle Cloud.
         * @returns Equipment ID if successful,
         *          (negative) error codes if unsuccessful:
         *          -1 : Invalid Equipment Type
         *          -2 : Invalid ID
         *          -3 : Specified Equipment was not found in the list
         *          -4 : Update failed
         */
        int updateSprout(String argsStr);

        /**
         * Dynamically removes Equipment to the Rhizome.
         *
         * The argument string for this function must match the following format:
         * TYPE,ID
         *
         * @param argsStr The argument string passed via the Particle Cloud.
         * @returns Equipment ID if successful,
         *          (negative) error codes if unsuccessful:
         *          -1 : Invalid ID
         *          -2 : No match found
         */
        int removeSprout(String argsStr);

    protected:

            /**
             * Each Sprout is a logical collection of physical pins/relays that are connected
             * to a single piece of Equipment.
             */
            std::deque< Equipment* >* _sprouts;

            /**
             * The touchscreen object. Handles the display for the Rhizome.
             */
            Screen *_screen;

    private:

        /**
         * Determines if the supplied string failed Particle's toInt() conversion.
         * @param raw String to examine
         * @returns Whether the string failed the conversion
         */
        bool isFakeZero(String raw);

        /**
         * Determines if the supplied string's first character is the number 0.
         * @param raw String to examine
         * @returns Whether the string starts with 0
         */
        bool isNotActualZero(String raw);

        /**
         * Determines if any of the pins in the supplied list are already assigned to a Sprout.
         * @param newPins The pins to check for
         * @returns Whether any of the supplied pins are already in use
         */
        bool arePinsInUse(std::list<int>* newPins);

        /**
         * Parses a given string of characters into the pins for a Temperature Sensor
         * @param params The buffer to use for strtok'ing. This method will not delete the buffer!
         * @param busPin The bus pin
         * @return Error or success code, according to the requirements specified by addSprout
         */
        int parseTemperatureSensorPins(char* params, int &busPin);

        /**
         * Adds a Temperature Sensor based on the next chunk of parsed data.
         * @param id The ID for the equipment
         * @param params The buffer to use for strtok'ing. This method will not delete the buffer!
         * @return Error or success code, according to the requirements specified by addSprout
         */
        int addTemperatureSensor(int id, char* params);

        /**
         * Parses a given string of characters into the pins for a Pump
         * @param params The buffer to use for strtok'ing. This method will not delete the buffer!
         * @param pin The power pin
         * @return Error or success code, according to the requirements specified by addSprout
         */
        int parsePumpPins(char* params, int &pin);

        /**
         * Adds a Pump based on the next chunk of parsed data.
         * @param id The ID for the equipment
         * @param params The buffer to use for strtok'ing. This method will not delete the buffer!
         * @return Error or success code, according to the requirements specified by addSprout
         */
        int addPump(int id, char* params);

        /**
         * Parses a given string of characters into the pins for a Heating Element
         * @param params The buffer to use for strtok'ing. This method will not delete the buffer!
         * @param elementPins The heating element pins
         * @return Error or success code, according to the requirements specified by addSprout
         */
        int parseHeatingElementPins(char* params, std::list<int> &elementPins);

        /**
         * Adds a Heating Element based on the next chunk of parsed data.
         * @param id The ID for the equipment
         * @param params The buffer to use for strtok'ing. This method will not delete the buffer!
         * @return Error or success code, according to the requirements specified by addSprout
         */
        int addHeatingElement(int id, char* params);

        /**
         * Parses a given string of characters into the pins for a Thermostat
         * @param params The buffer to use for strtok'ing. This method will not delete the buffer!
         * @param thermPins The thermostat pins
         * @return Error or success code, according to the requirements specified by addSprout
         */
        int parseThermostatPins(char* params, std::list<int> &thermPins);

        /**
         * Adds a Thermostat based on the next chunk of parsed data.
         * @param id The ID for the equipment
         * @param params The buffer to use for strtok'ing. This method will not delete the buffer!
         * @return Error or success code, according to the requirements specified by addSprout
         */
        int addThermostat(int id, char* params);

        /**
         * Parses a given string of characters into the pins for a RIMS
         * @param params The buffer to use for strtok'ing. This method will not delete the buffer!
         * @param thermPins The thermostat pins
         * @param pumpPin The pump pin
         * @return Error or success code, according to the requirements specified by addSprout
         */
        int parseRIMSPins(char* params, std::list<int> &thmPins, int &pumpPin);

        /**
         * Adds a RIMS based on the next chunk of parsed data.
         * @param id The ID for the equipment
         * @param params The buffer to use for strtok'ing. This method will not delete the buffer!
         * @return Error or success code, according to the requirements specified by addSprout
         */
        int addRIMS(int id, char* params);
    };
};

#endif

