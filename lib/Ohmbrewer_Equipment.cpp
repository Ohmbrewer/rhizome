#include "Ohmbrewer_Equipment.h"
#include "Ohmbrewer_Screen.h"

/**
 * Constructor
 */
Ohmbrewer::Equipment::Equipment() {
    _id = 0;
    _stopTime = 0;
    _state = false;
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
}

/**
 * Constructor
 * @param id The Sprout ID to use for this piece of Equipment
 * @param pins The list of physical pins this Equipment is attached to
 * @param stopTime The time at which the Equipment should shut off, assuming it isn't otherwise interrupted
 * @param state Whether the Equipment is ON (or OFF). True => ON, False => OFF
 * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
 */
Ohmbrewer::Equipment::Equipment(int id, std::list<int>* pins, int stopTime, bool state, String currentTask) {
    _id = id;
    _pins = pins;
    _stopTime = stopTime;
    _state = state;
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
String Ohmbrewer::Equipment::getCurrentTask() const {
    return _currentTask;
}

/**
 * Sets the Task the Equipment is currently working on.
 * @param currentTask The unique identifier of the task that the Equipment believes it should be processing
 * @returns The time taken to run the method
 */
const int Ohmbrewer::Equipment::setCurrentTask(String currentTask) {
    unsigned long start = millis();
    
    _currentTask = currentTask;
    
    return start - millis();
}

/**
 * The Particle event stream to publish Equipment status updates to.
 * @returns The Particle event stream the Equipment expects to publish to.
 */
String Ohmbrewer::Equipment::getStream() const {
    String stream = String("/");
    stream.concat(this->getType());
    stream.concat("/");
    stream.concat(this->getID());
    return stream;
}

/**
 * The name of the Spark.function for updating this Sprout.
 * Currently, looks like "type_#" where # is the Sprout's ID number.
 * Assumes that the buffer is empty!
 * @param buffer Buffer to fill with the name
 * @returns The name of the Spark.function for updating this Sprout
 */
void Ohmbrewer::Equipment::getUpdateFunctionName(String* buffer) const {
    buffer->concat(getType());
    buffer->concat("_");
    buffer->concat(getID());
}

/**
 * Registers the given function name for use with the Particle Cloud as the way to run update().
 * @param name The name that should appear on the Particle Cloud that will be used to kick off update()
 * @returns Any error code thrown during registration
 */
int Ohmbrewer::Equipment::registerUpdateFunction() {
    String updateFunction;
    getUpdateFunctionName(&updateFunction);
    return Spark.function(updateFunction.c_str(), [this](const String& args) -> int { return this->update(args); } );
}

/**
 * Performs the Equipment's current task. Expect to use this during loop().
 * @returns The time taken to run the method
 */
const int Ohmbrewer::Equipment::work() {
    return doWork();
}

/**
 * Draws information to the Rhizome's display.
 * @param screen The Rhizome's touchscreen
 * @returns The time taken to run the method
 */
const int Ohmbrewer::Equipment::display(Ohmbrewer::Screen *screen) {
    return doDisplay(screen);
}

/**
 * Publishes updates to Ohmbrewer, etc.
 * @param args The argument string passed into the Particle Cloud
 * @returns The time taken to run the method
 */
const int Ohmbrewer::Equipment::update(const String &args) {
    unsigned long start = millis();
    unsigned long duration;
    args_map_t argsMap;

    Serial.println(args);

    // Parse the supplied arguments into a map to figure out where to put them.
    parseArgs(args, argsMap);

    // Trim the argument string for further processing.
    String remainingArgs = cleanArgsString(args, argsMap);

    // Now assign the appropriate variables.
    assignArgs(argsMap);

    Serial.print("Still have this in the args buffer:");
    Serial.println(remainingArgs);
    duration = millis() - start;
    return doUpdate(remainingArgs, argsMap) + duration;
}

/**
 * Publishes updates to Ohmbrewer, etc.
 * @param args The argument string passed into the Particle Cloud
 * @param argsMap A map representing the key/value pairs for the update
 * @returns The time taken to run the method
 */
String Ohmbrewer::Equipment::cleanArgsString(const String &args, args_map_t &argsMap) {
    unsigned int firstArgsLen = argsMap[String("id")].length() + argsMap[String("current_task")].length() +
                                argsMap[String("state")].length() + argsMap[String("stop_time")].length() + 4;

    firstArgsLen = (args.length() <= firstArgsLen ? args.length() : firstArgsLen); // If there's only 4 args, we've miscounted the commas.

    return args.substring(firstArgsLen);
}

/**
 * Publishes updates to Ohmbrewer, etc.
 * @param argsMap A map representing the key/value pairs for the update
 * @returns Any error codes raised during assignment
 */
int Ohmbrewer::Equipment::assignArgs(args_map_t &argsMap) {

    setCurrentTask(argsMap[String("current_task")]);

    if(argsMap[String("state")].equalsIgnoreCase("ON")) {
        setState(true);
    } else if(argsMap[String("state")].equalsIgnoreCase("OFF")) {
        setState(false);
    } else if(argsMap[String("state")].equalsIgnoreCase("--")) {
        // Do nothing. Intentional.
    } else {
        // Do nothing. TODO: Should probably raise an error code...
    }

    setStopTime(argsMap[String("stop_time")].toInt());

    return 0;
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
 * @param result A map representing the key/value pairs for the update
 */
void Ohmbrewer::Equipment::parseArgs(const String &argsStr, Ohmbrewer::Equipment::args_map_t &result) {
    char* params = new char[argsStr.length() + 1];
    strcpy(params, argsStr.c_str());

    // Parse the parameters
    String id          = String(strtok(params, ","));
    String currentTask = String(strtok(NULL, ","));
    String state       = String(strtok(NULL, ","));
    String stopTime    = String(strtok(NULL, ","));

    // Save them to the map
    result[String("id")] = id;
    result[String("current_task")] = currentTask;
    result[String("state")] = state;
    result[String("stop_time")] = stopTime;

    Serial.println("Got these results: ");
    Serial.println(id);
    Serial.println(currentTask);
    Serial.println(state);
    Serial.println(stopTime);

    // Clear out that dynamically allocated buffer
    delete params;
}

