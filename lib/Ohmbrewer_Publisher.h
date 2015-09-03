/**
 * This library provides the Publisher class the Rhizome PID/equipment controller.
 * Rhizome is part of the Ohmbrewer project (see http://ohmbrewer.org for details).
 */

#ifndef OHMBREWER_PUBLISHER_H
#define OHMBREWER_PUBLISHER_H

#include <map>
#include "application.h"

namespace Ohmbrewer {

    class Publisher {

        typedef std::map<String, String> publish_map_t;

        public:

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
            Publisher(const String stream, const publish_map_t &todo);
            
            /**
             * Destructor
             */
            virtual ~Publisher();
    
        protected:
            /**
             * The map of the JSON objects to publish, in order of presentation
             */
            publish_map_t _data;

            /**
             * The stream to publish to
             */
            String _stream;
    };
};

#endif


