#include "Ohmbrewer_Rhizome.h"
#include "Ohmbrewer_Equipment.h"
#include "Ohmbrewer_Pump.h"
#include "Ohmbrewer_Temperature_Sensor.h"
#include "Ohmbrewer_Thermostat.h"
#include "Ohmbrewer_Heating_Element.h"
#include "Ohmbrewer_Publisher.h"
#include "Ohmbrewer_Thermostat.h"
#include "Ohmbrewer_RIMS.h"
#include "Ohmbrewer_Onewire.h"
#include "Ohmbrewer_Screen.h"


/**
 * Constructor
 */
Ohmbrewer::Rhizome::Rhizome(Timer *put) {
    _sprouts = new std::deque< Equipment* >;
    _settings = new RuntimeSettings();
    _screen = new Screen(D6, D7, A6, _sprouts, _settings);

    _periodicUpdateTimer = put;

    Particle.function("add", &Rhizome::addSprout, this);
    Particle.function("update", &Rhizome::updateSprout, this);
    Particle.function("remove", &Rhizome::removeSprout, this);
    Particle.variable("index",_index);

}

/**
 * Destructor. Kills the internal deque.
 */
Ohmbrewer::Rhizome::~Rhizome() {
    delete _sprouts;
    delete _screen;
    delete _settings;
    delete _periodicUpdateTimer;
}

/**
 * Dynamically adds Equipment to the Rhizome.
 * The Equipment will be added in the OFF state, with no current task.
 * Hence, it is probably necessary to send an update immediately after a successful addition.
 *
 * Additionally, the argument string for this function must match the following format:
 * TYPE,ID,{PINS}
 * where
 * TYPE matches the result of TYPE_NAME for the desired equipment class
 * ID isn't already in use
 * PINS is the appropriate length for the equipment type AND
 *      is in the expected order for the equipment constructor that will be called (as described above)
 *
 * @param argsStr The argument string passed via the Particle Cloud.
 * @returns Equipment ID if successful,
 *          (negative) error codes if unsuccessful:
 *          -1 : Invalid ID
 *          -2 : ID already in use for given Equipment Type
 *          -3 : One or more of the provided pins is already in use
 *          -4 : Incorrect number of pins provided for given Equipment Type
 *          -5 : Not Implemented
 */
int Ohmbrewer::Rhizome::addSprout(String argsStr) {
    int errorCode = 0;
    char* params = new char[argsStr.length() + 1];
    strcpy(params, argsStr.c_str());

    // Parse the parameters
    String type = String(strtok(params, ","));

    // Now, depending on the type we need to parse differently. Then we add the Equipment.
    if(type.equalsIgnoreCase(Ohmbrewer::TemperatureSensor::TYPE_NAME)) {
        errorCode = addTemperatureSensor(params);
    } else if(type.equalsIgnoreCase(Ohmbrewer::Pump::TYPE_NAME)) {
        errorCode = addPump(params);
    } else if(type.equalsIgnoreCase(Ohmbrewer::HeatingElement::TYPE_NAME)) {
        errorCode = addHeatingElement(params);
    } else if(type.equalsIgnoreCase(Ohmbrewer::Thermostat::TYPE_NAME)) {
        errorCode = addThermostat(params);
    } else if(type.equalsIgnoreCase(Ohmbrewer::RIMS::TYPE_NAME)) {
        errorCode = addRIMS(params);
    } else {
        // Trying to add an unrecognized Equipment Type.
        errorCode = -5;
    }

    // Clear out that dynamically allocated buffer
    delete params;

    // Simply return an error code if it failed
    if(errorCode != 0) {
        return errorCode;
    }

    if(_sprouts->size() == 1) {
        // We should only have to start the update timer if this is the first Sprout
        _periodicUpdateTimer->start();
    }

    // Otherwise, refresh the screen and return success.
    _screen->initScreen();
    return _sprouts->back()->getID(); // Success!
}

/**
 * Updates Equipment by routing/delegating to the Equipment subclass's update() method.
 *
 * The argument string for this function must match the following format:
 * TYPE,ID,UPDATE_STRING
 * where
 * TYPE matches the TYPE_NAME for the desired Equipment
 * ID matches the ID of the desired Equipment
 * UPDATE_STRING is a comma-delimited string that matches the format expected by the desired Equipment's class
 *
 * Both the ID and the UPDATE_STRING will be passed onto the update() method, but ID is validated within
 * this method before update() is called as one more check against Hard Faults.
 *
 * @param argsStr The argument string passed via the Particle Cloud.
 * @returns Equipment ID if successful,
 *          (negative) error codes if unsuccessful:
 *          -1 : Invalid Equipment Type
 *          -2 : Invalid ID
 *          -3 : Specified Equipment was not found in the list
 *          -4 : Update failed
 */
int Ohmbrewer::Rhizome::updateSprout(String argsStr) {
    char* params = new char[argsStr.length() + 1];
    strcpy(params, argsStr.c_str());

    // Parse the parameters
    String type    = String(strtok(params, ","));
    String idStr   = String(strtok(NULL, ","));
    int id = idStr.toInt();

    // Type should be valid
    if(!type.equalsIgnoreCase(Ohmbrewer::TemperatureSensor::TYPE_NAME) &&
       !type.equalsIgnoreCase(Ohmbrewer::Pump::TYPE_NAME) &&
       !type.equalsIgnoreCase(Ohmbrewer::HeatingElement::TYPE_NAME) &&
       !type.equalsIgnoreCase(Ohmbrewer::Thermostat::TYPE_NAME) &&
       !type.equalsIgnoreCase(Ohmbrewer::RIMS::TYPE_NAME)) {
        delete params;
        return -1; // Fail! Bad Type.
    } else {
        // Ok, we like this Type. Let's remove it (and the comma) from the args string
        // so it doesn't get in the way of further processing.
        argsStr.remove(0, type.length() + 1);
    }

    // If toInt() fails due to a bad parse, it gives 0.
    if(isFakeZero(idStr)) {
        delete params;
        return -2; // Fail! Bad ID.
    }

    std::deque<Ohmbrewer::Equipment*>::iterator itr = _sprouts->begin();
    for (itr; itr != _sprouts->end(); itr++) {
        if (((*itr)->getID() == id) && (type.equalsIgnoreCase((*itr)->getType()))) {
            // Update the equipment, removing the Type Name from the string that's passed in
            (*itr)->update(argsStr);

            delete params; // Clean up
            return id; // Success!
        }
    }

    // Clear out that dynamically allocated buffer
    delete params;
    return -3; // Fail! Not Found!
}

/**
 * Dynamically removes Equipment to the Rhizome.
 *
 * The argument string for this function must match the following format:
 * TYPE,ID
 *
 * @param argsStr The argument string passed via the Particle Cloud.
 * @returns Equipment ID if successful,
 *          (negative) error codes if unsuccessful:
 *          -1 : Invalid ID
 *          -2 : No match found
 */
int Ohmbrewer::Rhizome::removeSprout(String argsStr) {
    char* params = new char[argsStr.length() + 1];
    strcpy(params, argsStr.c_str());

    // Parse the parameters
    String type = String(strtok(params, ","));
    String idStr   = String(strtok(NULL, ","));
    int id = idStr.toInt();

    // If toInt() fails due to a bad parse, it gives 0.
    if(isFakeZero(idStr)) {
        delete params;
        return -1; // Fail!
    }

    std::deque<Ohmbrewer::Equipment*>::iterator itr = _sprouts->begin();
    for (itr; itr != _sprouts->end(); itr++) {
        if (((*itr)->getID() == id) && (type.equalsIgnoreCase((*itr)->getType()))) {
            _sprouts->erase(itr);
            delete params;
            rebuildIndex();
            _screen->initScreen(); // Gotta do this to clear artifacts from the screen
            return id; // Success!
        }
    }

    // Clear out that dynamically allocated buffer
    delete params;
    return -2; // Fail!
}

/**
 * Publishes any periodic updates that need to be published.
 * @see _periodicUpdateTimer
 */
void Ohmbrewer::Rhizome::publishPeriodicUpdates() {

    // Publish all the Temperature Sensor updates
    for (std::deque<Ohmbrewer::Equipment*>::iterator itr = _sprouts->begin(); itr != _sprouts->end(); itr++) {
        if (strcmp((*itr)->getType(), TemperatureSensor::TYPE_NAME) == 0) {
            ((TemperatureSensor*)(*itr))->publishSensorReading();
            continue;
        }
        if (strcmp((*itr)->getType(), Thermostat::TYPE_NAME) == 0) {
            ((Thermostat*)(*itr))->getSensor()->publishSensorReading();
            continue;
        }
        if (strcmp((*itr)->getType(), RIMS::TYPE_NAME) == 0) {
            ((RIMS*)(*itr))->getTunSensor()->publishSensorReading();
            ((RIMS*)(*itr))->getSafetySensor()->publishSensorReading();
        }
    }

    _periodicUpdateTimer->reset();
    return;
}

/**
 * Gets the deque of Sprouts
 * @returns The settings
 */
std::deque< Ohmbrewer::Equipment* >* Ohmbrewer::Rhizome::getSprouts() {
    return _sprouts;
}

/**
 * Gets the current runtime settings
 * @returns The settings
 */
Ohmbrewer::RuntimeSettings* Ohmbrewer::Rhizome::getRuntimeSettings() {
    return _settings;
}

/**
 * Gets the screen
 * @returns The screen
 */
Ohmbrewer::Screen* Ohmbrewer::Rhizome::getScreen() {
    return _screen;
}

/**
 * Called in loop, iterates through the the spouts equipment list
 * and calls work() on each equipment stored in the sprouts list
 */
void Ohmbrewer::Rhizome::work() {
    for (std::deque<Ohmbrewer::Equipment*>::iterator itr = _sprouts->begin(); itr != _sprouts->end(); itr++) {
        (*itr)->work();
    }
}

/**
 * Determines if the supplied string failed Particle's toInt() conversion.
 * @param raw String to examine
 * @returns Whether the string failed the conversion
 */
bool Ohmbrewer::Rhizome::isFakeZero(String raw) {
    return raw == NULL || raw.length() == 0 || (raw.toInt() == 0 && isNotActualZero(raw));
}

/**
 * Determines if the supplied string's first character is the number 0.
 * @param raw String to examine
 * @returns Whether the string starts with 0
 */
bool Ohmbrewer::Rhizome::isNotActualZero(String raw) {
    return ((raw.charAt(0) - '0') != 0);
}

/**
 * Determines if any of the pins in the supplied list are already assigned to a Sprout.
 * @param newPins The pins to check for
 * @returns Whether any of the supplied pins are already in use
 */
bool Ohmbrewer::Rhizome::arePinsInUse(std::list<int>* newPins) {
    // Make sure ID isn't in use
    std::deque<Ohmbrewer::Equipment*>::iterator itr = _sprouts->begin();

    // Iterate through all Sprouts
    for (itr; itr != _sprouts->end(); itr++) {
            std::list<int> pinsInUse;
            std::list<int>::iterator inusePinItr;
            std::list<int>::iterator newPinItr;

        // Iterate through all the pins for this Equipment...
        (*itr)->whichPins(&pinsInUse);
        for(inusePinItr = pinsInUse.begin(); inusePinItr != pinsInUse.end(); inusePinItr++) {
            // Compare against all the pins for the new Equipment...
            for(newPinItr = newPins->begin(); newPinItr != newPins->end(); newPinItr++) {
                if((*newPinItr) == (*inusePinItr)) {
                    return true;
                }
            }
        }
    }

    return false;
}

/**
 * Parses a given string of characters into the pins for a Temperature Sensor
 * @param params The buffer to use for strtok'ing. This method will not delete the buffer!
 * @param index The onewire sensor index (-1 if unused / non onewire)
 * @return Error or success code, according to the requirements specified by addSprout
 */
int Ohmbrewer::Rhizome::parseOnewireSensorPins(char *params, int &index) {
    String ind = String(strtok(NULL, ","));

    if(ind == NULL) {
        return -4;
    }
    index = ind.toInt();

    return 0;
}

/**
  * Adds a Temperature Sensor
  * @param params The buffer to use for strtok'ing. This method will not delete the buffer!
  * @return Error or success code, according to the requirements specified by addSprout
  */
int Ohmbrewer::Rhizome::addTemperatureSensor(char* params) {
    int index;
    int errorCode = parseOnewireSensorPins(params, index);

    if(errorCode == 0) {
        saveNewSprout(new Ohmbrewer::TemperatureSensor( new Ohmbrewer::Onewire(index) ));
    }

    return errorCode;
}

/**
 * Parses a given string of characters into the pins for a Pump
 * @param params The buffer to use for strtok'ing. This method will not delete the buffer!
 * @param pin The power pin
 * @return Error or success code, according to the requirements specified by addSprout
 */
int Ohmbrewer::Rhizome::parsePumpPins(char* params, int &pin) {
    String powerPin = String(strtok(NULL, ","));

    // Verify that D0 values are intentional
    if(isFakeZero(powerPin)) {
        return -1;
    }

    // Verify that the pins are not in use
    std::list<int> pinTest;
    pinTest.push_back(powerPin.toInt());
    if(arePinsInUse(&pinTest)) {
        return -3;
    }

    pin = powerPin.toInt();
    return 0;
}

/**
 * Adds a Pump
 * @param params The buffer to use for strtok'ing. This method will not delete the buffer!
 * @return Error or success code, according to the requirements specified by addSprout
 */
int Ohmbrewer::Rhizome::addPump(char* params) {
    int pumpPin;
    int errorCode = parsePumpPins(params, pumpPin);

    if(errorCode == 0) {
        saveNewSprout(new Ohmbrewer::Pump(pumpPin ));
    }

    return errorCode;
}

/**
 * Parses a given string of characters into the pins for a Heating Element
 * @param params The buffer to use for strtok'ing. This method will not delete the buffer!
 * @param elementPins The heating element pins
 * @return Error or success code, according to the requirements specified by addSprout
 */
int Ohmbrewer::Rhizome::parseHeatingElementPins(char* params, std::list<int> &elementPins) {
    String controlPin = String(strtok(NULL, ","));
    String powerPin   = String(strtok(NULL, ","));

    // Verify that D0 values are intentional
    if(isFakeZero(controlPin)) {
        return -1;
    }
    if(powerPin != NULL && isFakeZero(powerPin)) {
        return -1;
    }

    elementPins.push_back(controlPin.toInt());
    if(powerPin != NULL && powerPin.toInt() > -1) {
        elementPins.push_back(powerPin.toInt());
    }
    // Verify that the pins are not in use
    if(arePinsInUse(&elementPins)) {
        return -3;
    }

    return 0;
}

/**
 * Adds a Heating Element
 * @param params The buffer to use for strtok'ing. This method will not delete the buffer!
 * @return Error or success code, according to the requirements specified by addSprout
 */
int Ohmbrewer::Rhizome::addHeatingElement(char* params) {
    std::list<int> elementPins;
    int errorCode = parseHeatingElementPins(params, elementPins);

    if(errorCode == 0) {
        saveNewSprout(new Ohmbrewer::HeatingElement(&elementPins ));
    }

    return errorCode;
}

/**
 * Parses a given string of characters into the pins for a Thermostat
 * @param params The buffer to use for strtok'ing. This method will not delete the buffer!
 * @param thermPins The thermostat pins [tempbus, OW probe index, controlpin, powerpin]
 * @return Error or success code, according to the requirements specified by addSprout
 */
int Ohmbrewer::Rhizome::parseThermostatPins(char* params, std::list<int> &thermPins) {
    int index = -1;
    int errorCode = 0;

    errorCode = parseOnewireSensorPins(params, index);
    if(errorCode != 0) {
        return errorCode;
    }

    errorCode = parseHeatingElementPins(params, thermPins);
    if(errorCode != 0) {
        return errorCode;
    }

    thermPins.push_front(index);
    thermPins.push_front(0);//Bus Pin
    return 0;
}

/**
 * Adds a Thermostat
 * @param params The buffer to use for strtok'ing. This method will not delete the buffer!
 * @return Error or success code, according to the requirements specified by addSprout
 */
int Ohmbrewer::Rhizome::addThermostat(char* params) {
    std::list<int> thermPins;
    int errorCode = parseThermostatPins(params, thermPins);

    if(errorCode == 0) {
        saveNewSprout(new Ohmbrewer::Thermostat(&thermPins));
    }

    return errorCode;
}

/**
 * Parses a given string of characters into the pins for a RIMS
 * @param params The buffer to use for strtok'ing. This method will not delete the buffer!
 * @param thermPins The thermostat pins
 * @param pumpPin The pump pin
 * @param safetyIndex The onewire index for the safety sensor
 * @return Error or success code, according to the requirements specified by addSprout
 */
int Ohmbrewer::Rhizome::parseRIMSPins(char* params, std::list<int> &thermPins, int &pumpPin, int &safetyIndex) {
    int errorCode = 0;

    errorCode = parseThermostatPins(params, thermPins);
    if(errorCode != 0) {
        return errorCode;
    }

    errorCode = parsePumpPins(params, pumpPin);
    if(errorCode != 0) {
        return errorCode;
    }

    errorCode = parseOnewireSensorPins(params, safetyIndex);
    if(errorCode != 0) {
        return errorCode;
    }

    return errorCode;
}

/**
 * Adds a RIMS
 * @param params The buffer to use for strtok'ing. This method will not delete the buffer!
 * @return Error or success code, according to the requirements specified by addSprout
 */
int Ohmbrewer::Rhizome::addRIMS(char* params) {
    int pumpPin;
    int safetyIndex;
    std::list<int> thermPins;
    int errorCode = parseRIMSPins(params, thermPins, pumpPin, safetyIndex);

    if(errorCode == 0) {
        saveNewSprout(new Ohmbrewer::RIMS(&thermPins, pumpPin, safetyIndex ));
    }

    return errorCode;
}

/**
 * Adds a sprout to the list and rebuids the index
 * @param equipment The sprout being added
 */
void Ohmbrewer::Rhizome::saveNewSprout(Equipment* sprout) {
    _sprouts->push_back(sprout);
    rebuildIndex();
}


/**
 * Rebuilds index based on current list of equipment
 */
void Ohmbrewer::Rhizome::rebuildIndex() {

    String tempIndex = String("{ ");

    for (std::deque<Ohmbrewer::Equipment*>::iterator itr = _sprouts->begin(); itr != _sprouts->end(); itr++) {
        tempIndex.concat("{ ");
        tempIndex.concat(" \"id\": \"");
        tempIndex.concat((*itr)->getID());
        tempIndex.concat("\", ");
        tempIndex.concat("\"type\": \"");
        tempIndex.concat((*itr)->getType());
        tempIndex.concat("\", ");
        tempIndex.concat("\"state\": \"");
        tempIndex.concat((*itr)->getState());
        tempIndex.concat("\"");
        tempIndex.concat("\", ");
        tempIndex.concat("\"current task\": \"");
        tempIndex.concat((*itr)->getCurrentTask());
        tempIndex.concat("\"");
        tempIndex.concat(" }");
        if((*itr) != _sprouts->back()) {
            tempIndex.concat(",");
        }
    }
    tempIndex.concat(" }");

    _index = tempIndex;
    Serial.println(_index);
}



