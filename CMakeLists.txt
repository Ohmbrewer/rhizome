cmake_minimum_required(VERSION 3.3)
project(rhizome)
option(DEVICE_TYPE photon)
set(SOURCE_FILES
    ../firmware/rhizome.ino
    ../lib/Ohmbrewer_Equipment.h
    ../lib/Ohmbrewer_Equipment.cpp
    ../lib/Ohmbrewer_Heating_Element.h
    ../lib/Ohmbrewer_Heating_Element.cpp
    ../lib/Ohmbrewer_Pump.h
    ../lib/Ohmbrewer_Pump.cpp
    ../lib/Ohmbrewer_Temperature_Sensor.h
    ../lib/Ohmbrewer_Temperature_Sensor.cpp
    ../lib/Ohmbrewer_Thermostat.h
    ../lib/Ohmbrewer_Thermostat.cpp
    ../lib/Ohmbrewer_Temperature.h
    ../lib/Ohmbrewer_Temperature.cpp)

if(DEVICE_TYPE)
    if(DEVICE_TYPE STREQUAL "photon")
        execute_process(COMMAND particle compile photon ${SOURCE_FILES})
    elseif(DEVICE_TYPE STREQUAL "core")
        execute_process(COMMAND particle compile core ${SOURCE_FILES})
    endif()
else()
    # Default to Photon
    execute_process(COMMAND particle compile photon ${SOURCE_FILES})
endif()
