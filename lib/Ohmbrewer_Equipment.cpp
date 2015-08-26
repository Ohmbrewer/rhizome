#include "Ohmbrewer_Equipment.h"

/**
 * Constructor
 */
Ohmbrewer::Equipment::Equipment(int id, int* pins) {
    this->_id = id;
    this->_pins = pins;
    this->_stopTime = 0;
    this->_state = false;
    this->_currentTask = "";
}

/**
 * Constructor
 */
Ohmbrewer::Equipment::Equipment(int id, int* pins, int stopTime, bool state, char* currentTask) {
    this->_id = id;
    this->_pins = pins;
    this->_stopTime = stopTime;
    this->_state = state;
    this->_currentTask = currentTask;
}

/**
 * Copy Constructor
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
 */
int Ohmbrewer::Equipment::getID() const {
    return this->_id;
}

/**
 * The Equipment Type
 */
const char* Ohmbrewer::Equipment::getType() const {
    return this->_type;
}

/**
 * The time at which the Equipment will stop operating.
 */
int Ohmbrewer::Equipment::getStopTime() const {
    return this->_stopTime;
}

/**
 * Sets the time at which the Equipment will stop operating.
 */
const int Ohmbrewer::Equipment::setStopTime(const int stopTime) {
    unsigned long start = millis();
    
    this->_stopTime = stopTime;
    
    return start - millis();
}

/**
 * The Task the Equipment is currently working on.
 */
char* Ohmbrewer::Equipment::getCurrentTask() const {
    return this->_currentTask;
}

/**
 * Sets the Task the Equipment is currently working on.
 */
const int Ohmbrewer::Equipment::setCurrentTask(char* currentTask) {
    unsigned long start = millis();
    
    this->_currentTask = currentTask;
    
    return start - millis();
}

/**
 * The Particle event stream to publish Equipment status updates to.
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
 */
const int Ohmbrewer::Equipment::work() {
    return this->doWork();
}

/**
 * Draws information to the Rhizome's display.
 */
const int Ohmbrewer::Equipment::display() {
    return this->doDisplay();
}

/**
 * Publishes updates to Ohmbrewer, etc.
 */
const int Ohmbrewer::Equipment::update() {
    return this->doUpdate();
}

/**
 * Destructor
 */
Ohmbrewer::Equipment::~Equipment() {
    // Nothing to do here.
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
 */
char** Ohmbrewer::Equipment::parseArgs(const char* argsStr) {
    char* placeholder[] = {"fixme"};
    return placeholder;
}

