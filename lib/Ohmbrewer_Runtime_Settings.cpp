#include "Ohmbrewer_Runtime_Settings.h"

/**
 * Constructor.
 * All settings will be pulled from EEPROM, if supported
 */
Ohmbrewer::RuntimeSettings::RuntimeSettings() {

    // Figure out what was in EEPROM and assign it to the appropriate variables
    int eepromWifiStatus = readEEPROMWifiStatus();

    // If the WiFi Status has never been written to EEPROM, we'll want to do so now.
    // Otherwise, let's save a write for later.
    if(eepromWifiStatus == -1) {
        _wifiStatus = true;
        writeEEPROMWifiStatus();
    } else {
        _wifiStatus = eepromWifiStatus;
    }

    // Figure out what was in EEPROM and assign it to the appropriate variables
    int eepromTempUnit = readEEPROMTempUnit();

    // If the WiFi Status has never been written to EEPROM, we'll want to do so now.
    // Otherwise, let's save a write for later.
    if(eepromTempUnit == -1) {
        _celsius = true;
        writeEEPROMTempUnit();
    } else {
        _celsius = eepromWifiStatus;
    }
}

/**
 * Constructor
 * Settings that are not provided will be pulled from EEPROM, if supported
 * @param wifiStatus Whether the Wifi is ON (or OFF). True => ON, False => OFF.
 */
Ohmbrewer::RuntimeSettings::RuntimeSettings(bool wifiStatus) {
    _wifiStatus = wifiStatus;
    writeEEPROMWifiStatus();
}

/**
 * Writes the current Wifi Status out to EEPROM
 * @param status Whether the Wifi is ON (or OFF). True => ON, False => OFF
 */
const void Ohmbrewer::RuntimeSettings::writeEEPROMWifiStatus() {
    EEPROM.write(WIFI_STATUS_ADDR, (_wifiStatus ? EEPROM_WIFI_STATUS_ON : EEPROM_WIFI_STATUS_OFF));
}

/**
 * Writes the current Temp Unit to EEPROM
 * @param status Whether the Temp Unit is Fahrenheit or Celsius. True => Fahrenheit, False => Celsius
 */
const void Ohmbrewer::RuntimeSettings::writeEEPROMTempUnit() {
    EEPROM.write(TEMP_UNIT_ADDR, (_celsius ? EEPROM_TEMP_UNIT_C : EEPROM_TEMP_UNIT_F));
}

/**
 * Sets the Wifi status and immediately saves to EEPROM. True => On, False => Off
 * @param status Whether the Wifi is ON (or OFF). True => ON, False => OFF
 */
const void Ohmbrewer::RuntimeSettings::setWifiStatusAndSave(const bool status) {
    setWifiStatus(status);

    // Only write when actually necessary
    if(readEEPROMWifiStatus() != _wifiStatus) {
        writeEEPROMWifiStatus();
    }
}

const void Ohmbrewer::RuntimeSettings::setTempUnitAndSave(const bool celsius) {
    _celsius = celsius;

    // Only write when actually necessary
    if(readEEPROMTempUnit() != _celsius) {
        writeEEPROMTempUnit();
    }
}

/**
 * Reads the current Wifi Status from EEPROM.
 * Because the value could potentially have never been set, this is more complex than a simple bool.
 * @returns The saved Wifi Status. True => 1, False => 0, Unset/Other => -1
 */
const int Ohmbrewer::RuntimeSettings::readEEPROMWifiStatus() {
    // WiFi setting is stored at addr 1
    uint8_t value = EEPROM.read(WIFI_STATUS_ADDR);

    // Compare against possible results
    if (value == EEPROM_WIFI_STATUS_OFF) {
        return 0;
    } else if (value == EEPROM_WIFI_STATUS_ON) {
        return 1;
    }

    // Don't know what to make of the result
    return -1;
}

/**
 * Reads the current Temp Unit from EEPROM.
 * Because the value could potentially have never been set, this is more complex than a simple bool.
 * @returns The saved Temp Unit. Fahrenheit => 1, Celsius => 0, Unset/Other => -1
 */
const int Ohmbrewer::RuntimeSettings::readEEPROMTempUnit() {
    // WiFi setting is stored at addr 1
    uint8_t value = EEPROM.read(TEMP_UNIT_ADDR);

    // Compare against possible results
    if (value == EEPROM_TEMP_UNIT_F) {
        return 0;
    } else if (value == EEPROM_TEMP_UNIT_C) {
        return 1;
    }

    // Don't know what to make of the result
    return -1;
}
