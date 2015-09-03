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
             * The JSON representation of the provided keys and values
             */
            String toJSON() const;
            
            /**
             * Where we actually publish the provided information
             * @returns Time taken to do the publishing
             */
            int publish() const;
            
            /**
             * Constructors
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
            publish_map_t _todo;

            /**
             * The stream to publish to
             */
            String _stream;
    };
};

#endif


