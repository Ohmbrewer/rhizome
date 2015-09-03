#include "Ohmbrewer_Publisher.h"

/**
 * The JSON representation of the provided keys and values
 * @returns String JSON string of the map provided to the Publisher instance
 */
String Ohmbrewer::Publisher::toJSON() const {
    String returnVal = String("{");

    for(publish_map_t::const_iterator itr = _todo.begin(); itr != _todo.end(); ++itr) {
        returnVal.concat(" \"");
        returnVal.concat(itr->first);
        returnVal.concat("\": \"");
        returnVal.concat(itr->second);
        returnVal.concat("\"");
        if(itr->first != _todo.rbegin()->first) {
            returnVal.concat(",");
        }
    }
    returnVal.concat(" }");

    return returnVal;
}

/**
 * Where we actually publish the provided information
 * @returns int Time taken to do the publishing
 */
int Ohmbrewer::Publisher::publish() const {
    unsigned long start = millis();

    Particle.publish(_stream, this->toJSON(), 30, PRIVATE);

    return start - millis();
}

/**
 * Constructor
 */
Ohmbrewer::Publisher::Publisher(const String stream, const publish_map_t &todo) {
    _stream = stream;
    _todo = todo;
}

/**
 * Destructor
 */
Ohmbrewer::Publisher::~Publisher() {
    // Nothing to do here...
}
