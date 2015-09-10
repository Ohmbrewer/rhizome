#include "Ohmbrewer_Equipment.h"

/**
 * Constructor
 */
Ohmbrewer::Equipment::Equipment() {
    _id = 0;
    _stopTime = 0;
    _state = false;
    _currentTask = "";
    _pins = new std::list<int>;
}

/**
 * Constructor
 * @param id The Sprout ID to use for this piece of Equipment
 * @param pins The list of physical pins this Equipment is attached to
 */
Ohmbrewer::Equipment::Equipment(int id, std::list<int>* pins) {
    _id = id;
    _pins = pins;
    _stopTime = 0;
    _state = false;
    _currentTask = "";
}

/**
 * Constructor
 * @param id The Sprout ID to use for this piece of Equipment
 * @param pins The list of physical pins this Equipment is attached to
 * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
 * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
 * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
 */
Ohmbrewer::Equipment::Equipment(int id, std::list<int>* pins, int stopTime, bool state, char* currentTask) {
    _id = id;
    _pins = pins;
    _stopTime = stopTime;
    _state = state;
    _currentTask = currentTask;
}

/**
 * Copy Constructor
 * @param clonee The Equipment object to copy
 */
Ohmbrewer::Equipment::Equipment(const Equipment& clonee) {
    _id = clonee.getID();
    _pins = clonee.whichPins();
    _stopTime = clonee.getStopTime();
    _state = clonee.getState();
    _currentTask = clonee.getCurrentTask();
}

/**
 * The Equipment ID
 * @returns The Sprout ID to use for this piece of Equipment
 */
int Ohmbrewer::Equipment::getID() const {
    return _id;
}

/**
 * The Equipment Type
 * @returns The Equipment type name
 */
const char* Ohmbrewer::Equipment::getType() const {
    return _type;
}

/**
 * The time at which the Equipment will stop operating.
 * @returns The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
 */
int Ohmbrewer::Equipment::getStopTime() const {
    return _stopTime;
}

/**
 * Sets the time at which the Equipment will stop operating.
 * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
 * @returns The time taken to run the method
 */
const int Ohmbrewer::Equipment::setStopTime(const int stopTime) {
    unsigned long start = millis();
    
    _stopTime = stopTime;
    
    return start - millis();
}

/**
 * The Task the Equipment is currently working on.
 * @returns The unique identifier of the task that the Equipment believes it should be processing
 */
char* Ohmbrewer::Equipment::getCurrentTask() const {
    return _currentTask;
}

/**
 * Sets the Task the Equipment is currently working on.
 * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
 * @returns The time taken to run the method
 */
const int Ohmbrewer::Equipment::setCurrentTask(char* currentTask) {
    unsigned long start = millis();
    
    _currentTask = currentTask;
    
    return start - millis();
}

/**
 * The Particle event stream to publish Equipment status updates to.
 * @returns The Particle event stream the Equipment expects to publish to.
 */
String Ohmbrewer::Equipment::getStream() {
    String stream = String("/");
    stream.concat(this->getType());
    stream.concat("/");
    stream.concat(this->getID());
    return stream;
}

/**
 * Performs the Equipment's current task. Expect to use this during loop().
 * @returns The time taken to run the method
 */
const int Ohmbrewer::Equipment::work() {
    return this->doWork();
}

/**
 * Draws information to the Rhizome's display.
 * @returns The time taken to run the method
 */
const int Ohmbrewer::Equipment::display() {
    return this->doDisplay();
}

/**
 * Publishes updates to Ohmbrewer, etc.
 * @returns The time taken to run the method
 */
const int Ohmbrewer::Equipment::update() {
    return this->doUpdate();
}

/**
 * Destructor
 */
Ohmbrewer::Equipment::~Equipment() {
    delete _pins;
}

/**
 * Overloaded << operator. Returns information about the Equipment in such a way that adding
 * curly-braces on either side should provide a JSON-formatted string.
 */
// std::ostream& operator<<(std::ostream& os, Ohmbrewer::Equipment const& equipment) {
//     os << " \"type\": \"" << equipment.getType() << "\"," << 
//           " \"sprout_id\": \"" << equipment.getID() << "\"," << 
//           " \"pins\": \"[";
//     for (i=0; i < 6 && equipment.whichPins()[i] != NULL; i++) {
//         if(i > 0) {
//             os << ", ";
//         }
//         os << equipment.whichPins()[i]; 
//     }
//     os << "]\"," <<
          
//     os << " \"state\": \"" << (equipment.isOn() ? "ON" : "OFF") << "\"," << 
//           " \"stop_time\": \"" << equipment.getStopTime() << "\"," << 
//           " \"current_task\": \"" << equipment.getCurrentTask() << "\",";
//     return os;
// }

/**
 * Specifies the interface for arguments sent to this Equipment's associated function. 
 * Parses the supplied string into an array of strings for setting the Equipment's values.
 * Most likely will be called during update().
 * @param argsStr The arguments supplied as an update to the Rhizome.
 * @returns A map representing the key/value pairs for the update
 */
Ohmbrewer::Equipment::args_map_t Ohmbrewer::Equipment::parseArgs(const String argsStr) {
    args_map_t result;
    char* params = new char[argsStr.length() + 1];
    strcpy(params, argsStr.c_str());

    // Parse the parameters
    String id          = String(strtok(params, ","));
    String currentTask = String(strtok(params, ","));
    String state       = String(strtok(NULL, ","));
    String stopTime    = String(strtok(NULL, ","));

    // Save them to the map
    result[String("id")] = id;
    result[String("current_task")] = currentTask;
    result[String("state")] = state;
    result[String("stopTime")] = stopTime;

    // Clear out that dynamically allocated buffer
    delete params;

    return result;
}

