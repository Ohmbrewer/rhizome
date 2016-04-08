/**
 * This library provides the Runtime Settings base class of the Rhizome PID/equipment controller.
 * Rhizome is part of the Ohmbrewer project (see http://ohmbrewer.org for details).
 */

#ifndef OHMBREWER_RUNTIME_SETTINGS_H
#define OHMBREWER_RUNTIME_SETTINGS_H

#include "application.h"


namespace Ohmbrewer {

    class RuntimeSettings {
    
        public:

        /* Constants */

        /**
         * Where the WiFi setting is stored
         */
        static const int WIFI_STATUS_ADDR = 1;
        static const int EEPROM_WIFI_STATUS_OFF = 0x01;
        static const int EEPROM_WIFI_STATUS_ON = 0x00;

        static const int TEMP_UNIT_ADDR = 2;
        static const int EEPROM_TEMP_UNIT_F = 0x01;
        static const int EEPROM_TEMP_UNIT_C = 0x00;


        /* Methods */

        /**
         * Constructor.
         * All settings will be pulled from EEPROM, if supported
         */
        RuntimeSettings();

        /**
         * Constructor
         * Settings that are not provided will be pulled from EEPROM, if supported
         * @param wifiStatus Whether the Wifi is ON (or OFF). True => ON, False => OFF.
         */
        RuntimeSettings(bool wifiStatus);

        /**
         * The Wifi status. True => On, False => Off
         * @returns True => On, False => Off
         */
        bool getWifiStatus() const { return _wifiStatus; };

        /**
         * Sets the Wifi status. True => On, False => Off
         * @param status Whether the Wifi is ON (or OFF). True => ON, False => OFF
         */
        const void setWifiStatus(const bool status) { _wifiStatus = status; };

        /**
         * Sets the Wifi status and immediately saves to EEPROM. True => On, False => Off
         * @param status Whether the Wifi is ON (or OFF). True => ON, False => OFF
         */
        const void setWifiStatusAndSave(const bool status);

        /**
         * The Wifi status. True => On, False => Off
         * @returns True => On, False => Off
         */
        bool isWifiOn() const {return getWifiStatus();};

        /**
         * The Wifi status. True => On, False => Off
         * @returns True => On, False => Off
         */
        bool isWifiOff() const {return !getWifiStatus();};

        /**
         * The current unit. True => C, False => F
         * @returns True => C, False => F
         */
        bool isTempUnitCelsius() const {return _celsius;};


        /**
         * Sets the Temp Unit and immediately saves to EEPROM. True => C, False => F
         * @param status Whether the Celsius or not. True => C, False => F
         */
        const void setTempUnitAndSave(const bool celsius);

    protected:

        /**
         * Whether the WiFi should be on (true) or off (false)
         */
        bool _wifiStatus;

        /**
         * Whether the Temp Unit should be Celsius (true) or Fahrenheit (false)
         */
        bool _celsius;


    private:

        /**
         * Writes the current Wifi Status out to EEPROM
         * @param status Whether the Wifi is ON (or OFF). True => ON, False => OFF
         */
        const void writeEEPROMWifiStatus();

        /**
         * Reads the current Wifi Status from EEPROM.
         * Because the value could potentially have never been set, this is more complex than a simple bool.
         * @returns The saved Wifi Status. True => 1, False => 0, Unset/Other => -1
         */
        const int readEEPROMWifiStatus();

        /**
         * Writes the current Temp Unit to EEPROM
         * @param status Whether the Temp Unit is Fahrenheit or Celsius. True => Fahrenheit, False => Celsius
         */
        const void writeEEPROMTempUnit();

        /**
         * Reads the current Temp Unit from EEPROM.
         * Because the value could potentially have never been set, this is more complex than a simple bool.
         * @returns The saved Temp Unit. Fahrenheit => 1, Celsius => 0, Unset/Other => -1
         */
        const int readEEPROMTempUnit();

    };


}

#endif
