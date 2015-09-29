/**
 * This library provides the Equipment base class the Rhizome PID/equipment controller.
 * Rhizome is part of the Ohmbrewer project (see http://ohmbrewer.org for details).
 */

#ifndef OHMBREWER_RHIZOME_EQUIPMENT_H
#define OHMBREWER_RHIZOME_EQUIPMENT_H

// Kludge to allow us to use std::map and std::list - for now we have to undefine these macros.
#undef min
#undef max
#undef swap
#include <map>
#include <list>
#include "application.h"

namespace Ohmbrewer {
    // Forward declaration
    class Screen;

    class Equipment {
      
        public:

            /**
             * The short-hand type name. Used for communicating with Ohmbrewer and disambiguating Equipment* pointers.
             */
            const static constexpr char* TYPE_NAME = "equipment";

            /**
             * A map of arguments provided via a Particle cloud function call,
             * mapped to the appropriate Equipment members.
             */
            typedef std::map <String, String> args_map_t;


            /**
             * The Equipment ID
             * @returns The Sprout ID to use for this piece of Equipment
             */
            int getID() const;
            
            /**
             * The Equipment Type
             * @returns The Equipment type name
             */
            virtual const char* getType() const { return Equipment::TYPE_NAME; };

            /**
             * The time at which the Equipment will stop operating.
             * @returns The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
             */
            int getStopTime() const;

            /**
             * Sets the time at which the Equipment will stop operating.
             * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
             * @returns The time taken to run the method
             */
            const int setStopTime(const int stopTime);

            /**
             * The Task the Equipment is currently working on.
             * @returns The unique identifier of the task that the Equipment believes it should be processing
             */
            String getCurrentTask() const;

            /**
             * Sets the Task the Equipment is currently working on.
             * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
             * @returns The time taken to run the method
             */
            const int setCurrentTask(String currentTask);

            /**
             * The Particle event stream to publish Equipment status updates to.
             * @returns The Particle event stream the Equipment expects to publish to.
             */
            String getStream() const;

            /**
             * The name of the Spark.function for updating this Sprout.
             * Currently, looks like "type_#" where # is the Sprout's ID number.
             * @param buffer Buffer to fill with the name
             * @returns The name of the Spark.function for updating this Sprout
             */
            virtual void getUpdateFunctionName(String* buffer) const;

            /**
             * Registers the function name for use with the Particle Cloud as the way to run update().
             * @returns Any error code thrown during registration
             */
            int registerUpdateFunction();

            /**
             * Performs the Equipment's current task. Expect to use this during loop().
             * @returns The time taken to run the method
             */
            const int work();

            /**
             * Draws information to the Rhizome's display.
             * @param screen The Rhizome's touchscreen
             * @returns The time taken to run the method
             */
            const int display(Screen *screen);

            /**
             * Publishes updates to Ohmbrewer, etc.
             * @param args The argument string passed into the Particle Cloud
             * @returns The time taken to run the method
             */
            int update(String args);
            
            /**
             * Constructors
             */
            Equipment();

            /**
             * Constructor
             * @param id The Sprout ID to use for this piece of Equipment
             */
            Equipment(int id,  int powerPin, int controlPin);

            /**
             * Constructor
             * @param id The Sprout ID to use for this piece of Equipment
             * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
             * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
             * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
             */
            Equipment(int id, int stopTime, bool state, String currentTask);

            /**
             * Copy Constructor
             * @param clonee The Equipment object to copy
             */
            Equipment(const Equipment& clonee);
            
            /**
             * Destructor
             */
            virtual ~Equipment();
            
            /**
             * Overloaded << operator.
             */
            // friend std::ostream& operator<<( std::ostream& os, Equipment const& equipment);

            /**
             * Specifies the interface for arguments sent to this Equipment's associated function.
             * Parses the supplied string into an array of strings for setting the Equipment's values.
             * Most likely will be called during update().
             * @param argsStr The arguments supplied as an update to the Rhizome.
             * @returns A map representing the key/value pairs for the update
             */
            static args_map_t parseArgs(const String argsStr);

            /*
             * Virtual Functions! All of these need to be defined in child classes! 
             */
        
            /**
             * Sets the Equipment state. True => On, False => Off
             */
            virtual const int setState(const bool) = 0;
        
            /**
             * The Equipment state. True => On, False => Off
             */
            virtual bool getState() const = 0;

            /**
             * Toggles the Equipment state.
             */
            virtual void toggleState() { _state = !_state; };
            
            /**
             * True if the Equipment state is On.
             */
            virtual bool isOn() const = 0;
            
            /**
             * True if the Equipment state is Off.
             */
            virtual bool isOff() const = 0;

            /**
             * Performs the Equipment's current task. Expect to use this during loop().
             * This function is called by work().
             * @returns The time taken to run the method
             */
            virtual int doWork() = 0;

            /**
             * Draws information to the Rhizome's display.
             * This function is called by display().
             * @param screen The Rhizome's touchscreen
             * @returns The time taken to run the method
             */
            virtual int doDisplay(Screen *screen) = 0;

            /**
             * Publishes updates to Ohmbrewer, etc.
             * This function is called by update().
             * @param args The argument string passed into the Particle Cloud
             * @returns The time taken to run the method
             */
            virtual int doUpdate(String* args) = 0;

            /**
             * Reports which of the Rhizome's pins are occupied by the
             * Equipment, forming a logical Sprout.
             */
            virtual std::list<int>* whichPins() const = 0;

        protected:
            /**
             * Equipment ID
             */
            int            _id;

            /**
             * Designated Stop Time
             */
            int            _stopTime;

            /**
             * State (True => On, False => Off)
             */
            bool           _state;

            /**
             * Which of the Rhizome's pins are occupied by the
             * Equipment, forming a logical Sprout.
             */
            std::list<int>* _pins;

            /**
             * The Task the Equipment is currently processing.
             */
            String          _currentTask;
    };
};

#endif


