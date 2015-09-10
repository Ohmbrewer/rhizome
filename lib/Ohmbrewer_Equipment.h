/**
 * This library provides the Equipment base class the Rhizome PID/equipment controller.
 * Rhizome is part of the Ohmbrewer project (see http://ohmbrewer.org for details).
 */

#ifndef OHMBREWER_RHIZOME_EQUIPMENT_H
#define OHMBREWER_RHIZOME_EQUIPMENT_H

#undef min
#undef max
#undef swap
#include <map>
#include "application.h"

namespace Ohmbrewer {

    class Equipment {
      
        public:

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
            const char* getType() const;

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
            char* getCurrentTask() const;

            /**
             * Sets the Task the Equipment is currently working on.
             * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
             * @returns The time taken to run the method
             */
            const int setCurrentTask(char* currentTask);

            /**
             * The Particle event stream to publish Equipment status updates to.
             * @returns The Particle event stream the Equipment expects to publish to.
             */
            String getStream();

            /**
             * Performs the Equipment's current task. Expect to use this during loop().
             * @returns The time taken to run the method
             */
            const int work();

            /**
             * Draws information to the Rhizome's display.
             * @returns The time taken to run the method
             */
            const int display();

            /**
             * Publishes updates to Ohmbrewer, etc.
             * @returns The time taken to run the method
             */
            const int update();
            
            /**
             * Constructors
             */
            Equipment();

            /**
             * Constructor
             * @param id The Sprout ID to use for this piece of Equipment
             * @param pins The list of physical pins this Equipment is attached to
             */
            Equipment(int id, int* pins);

            /**
             * Constructor
             * @param id The Sprout ID to use for this piece of Equipment
             * @param pins The list of physical pins this Equipment is attached to
             * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
             * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
             * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
             */
            Equipment(int id, int* pins, int stopTime, bool state, char* currentTask);

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
            virtual const int setState(const bool);
        
            /**
             * The Equipment state. True => On, False => Off
             */
            virtual bool getState() const;
            
            /**
             * True if the Equipment state is On.
             */
            virtual bool isOn() const;
            
            /**
             * True if the Equipment state is Off.
             */
            virtual bool isOff() const;

        protected:
            /**
             * Equipment ID
             */
            int         _id;

            /**
             * Equipment Type
             */
            const char* _type;

            /**
             * Designated Stop Time
             */
            int         _stopTime;

            /**
             * State (True => On, False => Off)
             */
            bool        _state;

            /**
             * Which of the Rhizome's pins are occupied by the
             * Equipment, forming a logical Sprout.
             */
            int*        _pins;

            /**
             * The Task the Equipment is currently processing.
             */
            char*       _currentTask;

        private:
            /**
             * Performs the Equipment's current task. Expect to use this during loop().
             * This function is called by work().
             */
            virtual int doWork();
            
            /**
             * Draws information to the Rhizome's display.
             * This function is called by display().
             */
            virtual int doDisplay();
            
            /**
             * Publishes updates to Ohmbrewer, etc.
             * This function is called by update().
             */
            virtual int doUpdate();
            
            /**
             * Reports which of the Rhizome's pins are occupied by the
             * Equipment, forming a logical Sprout.
             */
            virtual int* whichPins() const;
    };
};

#endif


