#include "Ohmbrewer_Publisher.h"

/**
 * The JSON representation of the provided keys and values.
 * Expects all values to be Strings, so if you need nested objects or arrays
 * you'll have to do the conversion yourself.
 * @returns JSON string of the map provided to the Publisher instance
 */
String Ohmbrewer::Publisher::toJSON() const {
    String returnVal = String("{");

    for(publish_map_t::const_iterator itr = _data->begin(); itr != _data->end(); ++itr) {
        returnVal.concat(" \"");
        returnVal.concat(itr->first);
        returnVal.concat("\": \"");
        returnVal.concat(itr->second);
        returnVal.concat("\"");
        if(itr->first != _data->rbegin()->first) {
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

    Spark.publish(*_stream, toJSON(), 30, PRIVATE);

    return start - millis();
}

/**
 * Constructor
 * @param stream The Particle cloud event stream to publish to
 * @param data A map of data to publish as a JSON.
 */
Ohmbrewer::Publisher::Publisher(String *stream, publish_map_t *data) {
    _stream = stream;
    _data = data;
}

/**
 * Destructor
 */
Ohmbrewer::Publisher::~Publisher() {
    delete _stream;
}
