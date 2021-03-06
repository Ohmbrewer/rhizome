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
 * Constructor that initializes the data map internally.
 * @param stream The Particle cloud event stream to publish to
 */
Ohmbrewer::Publisher::Publisher(String *stream) {
    _stream = stream;
    _data = new publish_map_t;
}

/**
 * Constructor that adds the provided key/value pair to the Publisher's stream map.
 * @param key A single key for data to publish as a JSON.
 * @param value A single value for data to publish as a JSON.
 */
Ohmbrewer::Publisher::Publisher(String *stream, String key, String value) {
    _stream = stream;
    _data = new publish_map_t;
    (*_data)[key] = value;
}

/**
 * Constructor
 * @param stream The Particle cloud event stream to publish to
 * @param data A map of data to publish as a JSON.
 * @return Whether the key/value pair was added successfully.
 */
bool Ohmbrewer::Publisher::add(String key, String value) {
    (*_data)[key] = value;
    return true;
}

/**
 * Clears the internal data map
 */
void Ohmbrewer::Publisher::clear() {
    (*_data).clear();
}

/**
 * Destructor
 */
Ohmbrewer::Publisher::~Publisher() {
    delete _stream;
}
