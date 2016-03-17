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
