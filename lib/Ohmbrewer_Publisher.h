/**
 * This library provides the Publisher class the Rhizome PID/equipment controller.
 * Rhizome is part of the Ohmbrewer project (see http://ohmbrewer.org for details).
 */

#ifndef OHMBREWER_PUBLISHER_H
#define OHMBREWER_PUBLISHER_H

// Kludge to allow us to use std::map - for now we have to undefine these macros.
#undef min
#undef max
#undef swap
#include <map>
#include "application.h"

namespace Ohmbrewer {

    class Publisher {

        public:

            typedef std::map<String, String> publish_map_t;

            /**
             * The JSON representation of the provided keys and values.
             * Expects all values to be Strings, so if you need nested objects or arrays
             * you'll have to do the conversion yourself.
             * @returns JSON string of the map provided to the Publisher instance
             */
            String toJSON() const;

            /**
             * Where we actually publish the provided information
             * @returns int Time taken to do the publishing
             */
            int publish() const;

            /**
             * Constructor
             * @param stream The Particle cloud event stream to publish to
             * @param data A map of data to publish as a JSON.
             */
            Publisher(String *stream, publish_map_t *data);
            
            /**
             * Destructor
             */
            virtual ~Publisher();
    
        protected:
            /**
             * The map of the JSON objects to publish, in order of presentation
             */
            publish_map_t* _data;

            /**
             * The stream to publish to
             */
            String* _stream;
    };
};

#endif


