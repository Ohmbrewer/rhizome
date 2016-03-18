cmake_minimum_required(VERSION 3.3)
project(rhizome)
option(DEVICE_TYPE photon)
set(SOURCE_FILES
#firmware files
    ../firmware/rhizome.ino
    #../firmware/rhizome_testing.ino
#Ohmbrewer libraries
    ../lib/Ohmbrewer_Equipment.h
    ../lib/Ohmbrewer_Equipment.cpp
    ../lib/Ohmbrewer_Heating_Element.h
    ../lib/Ohmbrewer_Heating_Element.cpp
    ../lib/Ohmbrewer_Menu.h
    ../lib/Ohmbrewer_Menu.cpp
    ../lib/Ohmbrewer_Menu/Ohmbrewer_Menu_Home.h
    ../lib/Ohmbrewer_Menu/Ohmbrewer_Menu_Home.cpp
    ../lib/Ohmbrewer_Menu/Ohmbrewer_Menu_WiFi.h    
    ../lib/Ohmbrewer_Menu/Ohmbrewer_Menu_WiFi.cpp
    ../lib/Ohmbrewer_Onewire.cpp
    ../lib/Ohmbrewer_Onewire.h
    ../lib/Ohmbrewer_PID_Profile.h
    ../lib/Ohmbrewer_Probe.cpp
    ../lib/Ohmbrewer_Probe.h
    ../lib/Ohmbrewer_Publisher.h
    ../lib/Ohmbrewer_Publisher.cpp
    ../lib/Ohmbrewer_Pump.h
    ../lib/Ohmbrewer_Pump.cpp
    ../lib/Ohmbrewer_Relay.h
    ../lib/Ohmbrewer_Relay.cpp
    ../lib/Ohmbrewer_RIMS.h
    ../lib/Ohmbrewer_RIMS.cpp
    ../lib/Ohmbrewer_Screen.h
    ../lib/Ohmbrewer_Screen.cpp
    ../lib/Ohmbrewer_Runtime_Settings.h
    ../lib/Ohmbrewer_Runtime_Settings.cpp
    ../lib/Ohmbrewer_Sprouts.h
    ../lib/Ohmbrewer_Sprouts.cpp
    ../lib/Ohmbrewer_Temperature.h
    ../lib/Ohmbrewer_Temperature.cpp
    ../lib/Ohmbrewer_Temperature_Sensor.h
    ../lib/Ohmbrewer_Temperature_Sensor.cpp
    ../lib/Ohmbrewer_Thermostat.h
    ../lib/Ohmbrewer_Thermostat.cpp
#external libraries
    ../../Adafruit_ILI9341/firmware/Adafruit_ILI9341.h
    ../../Adafruit_ILI9341/firmware/Adafruit_ILI9341.cpp
    ../../Adafruit_mfGFX_IDE/firmware/Adafruit_mfGFX.h
    ../../Adafruit_mfGFX_IDE/firmware/Adafruit_mfGFX.cpp
    ../../Adafruit_mfGFX_IDE/firmware/fonts.h
    ../../Adafruit_mfGFX_IDE/firmware/fonts.cpp
    ../../particle-ds18x20/firmware/ds18x20.cpp
    ../../particle-ds18x20/firmware/ds18x20.h
    ../../particle-ds18x20/firmware/crc8.cpp
    ../../particle-ds18x20/firmware/crc8.h
    ../../particle-ds18x20/firmware/onewire.cpp
    ../../particle-ds18x20/firmware/onewire.h
    ../../Spark-PID/firmware/pid.cpp
    ../../Spark-PID/firmware/pid.h
    ../../Touch_4Wire/firmware/Touch_4Wire.cpp
    ../../Touch_4Wire/firmware/Touch_4Wire.h
)

if(DEVICE_TYPE)
    # You can specify which device to compile for, if desired...
    if(DEVICE_TYPE STREQUAL "photon")
        execute_process(COMMAND particle compile photon ${SOURCE_FILES})
    elseif(DEVICE_TYPE STREQUAL "core")
        execute_process(COMMAND particle compile core ${SOURCE_FILES})
    endif()
else()
    # Default to Photon
    execute_process(COMMAND particle compile photon ${SOURCE_FILES})
endif()
