/**
 * This library provides the Equipment base class the Rhizome PID/equipment controller.
 * Rhizome is part of the Ohmbrewer project (see http://ohmbrewer.org for details).
 */

#ifndef OHMBREWER_RHIZOME_EQUIPMENT_H
#define OHMBREWER_RHIZOME_EQUIPMENT_H
    
#include "application.h"

namespace Ohmbrewer {

    class Equipment {
      
        public:
            /**
             * The Equipment ID
             */
            int getID() const;
            
            /**
             * The Equipment Type
             */
            const char* getType() const;
            
            /**
             * The time at which the Equipment will stop operating.
             */
            int getStopTime() const;
            
            /**
             * Sets the time at which the Equipment will stop operating.
             */
            const int setStopTime(const int stopTime);
            
            /**
             * The Task the Equipment is currently working on.
             */
            char* getCurrentTask() const;
            
            /**
             * Sets the Task the Equipment is currently working on.
             */
            const int setCurrentTask(char* currentTask);
            
            /**
             * The Particle event stream to publish Equipment status updates to.
             */
            String getStream();
            
            /**
             * Performs the Equipment's current task. Expect to use this during loop().
             */
            const int work();
            
            /**
             * Draws information to the Rhizome's display.
             */
            const int display();
            
            /**
             * Publishes updates to Ohmbrewer, etc.
             */
            const int update();
            
            /**
             * Constructors
             */
            Equipment(int id, int* pins);
            Equipment(int id, int* pins, int stopTime, bool state, char* currentTask);
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
             */
            static char** parseArgs(const char* argsStr);

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


