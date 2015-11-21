# Firmware for the Rhizome

Anything that should be imported into Particle Build as a "Library" is in the ```lib``` directory. 
Examples of possible .ino file configurations are in the ```firmware``` directory.

### Semi-local build
You can use the Particle CLI tool in conjunction with a local IDE to build the project using the provided CMakeLists.txt
file. Simply add a ```build``` directory to the project directory and navigate to it via the terminal. Then run
```cmake ..``` and the build should get punted off to Particle. If all goes well, you'll get a binary file that you can
then flash to your Core/Photon using ```particle flash DEVICEID firmware.bin```

Note that if you do this, you'll need to have CMake. If you're using CLion, CMake should be bundled within the IDE directory somewhere.

# REST API
The Rhizome supports a number of RESTful operations to support management and operation of Equipment and Equipment Groupings attached to it. These operations are accessible via the [Particle CLI](https://github.com/spark/particle-cli) and curl as specified in the [Particle.function](https://docs.particle.io/reference/firmware/photon/#particle-function-) documentation. They're also provide the interface [Ohmbrewer](https://github.com/Ohmbrewer/ohmbrewer) uses to manage and control the Rhizome.

The available functions and their argument patterns are as follows. Please note that all argument strings must be provided as comma-delimited Strings with no spaces.

* add - *Add Equipment*
  * Format: TYPE,ID,[PINS]
    * TYPE: The Equipment type
      
        | Type               | Type Name |
        |--------------------|-----------|
        | Temperature Sensor | temp      |
        | Relay              | relay     |
        | Pump               | pump      |
        | Heating Element    | heat      |
        | Thermostat         | therm     |
        | RIMS               | rims      |
    * ID: The ID of the desired Equipment
    * PINS: One or more pins that will be logically grouped as representing the Equipment. Order depends on Equipment Type:
      
        | Type               | Pins                                                    |
        |--------------------|---------------------------------------------------------|
        | Temperature Sensor | Bus Pin                                                 |
        | Relay              | Power Pin                                               |
        | Pump               | Power Pin                                               |
        | Heating Element    | Control Pin, Power Pin*                                 |
        | Thermostat         | Sensor Bus Pin, Element Control Pin, Element Power Pin* |
        | RIMS               | Sensor Bus Pin†, Thermostat Control Pin, Thermostat Power Pin*, Recirculation Pump Power Pin |
      * In the event that a Heating Element only uses one pin, please provide -1 for the Power Pin. This applies to any Power Pin marked with a (*) above.
      * Currently, we expect all Temperature Sensors to be OneWire probes. This is why you only specify one Sensor Bus Pin for RIMS (marked with †) - we use the same pin for all Temperature Sensors.
      * Note that currently we do not provide a way to *actually* change the Bus Pin (we always assume it's D0). This may change in the future. Until then, we won't do anything with any of the provided Bus Pins.
      * Also note that currently we do not support adding bare Relays. That may change in future releases, so the expect API is included above.
  * Expected result:
    * Success: Particle.function returns the ID number.
    * Failure: Particle.function returns a negative number indicating the cause of the failure.
* remove - *Remove Equipment*
  * Format: TYPE,ID
    * TYPE: The Equipment type
    * ID: The ID of the desired Equipment
  * Expected result:
    * Success: The specified Equipment instance is removed from the Rhizome's memory and Particle.function returns the ID number.
    * Failure: Particle.function returns a negative number indicating the cause of the failure.
* update - *Update Equipment state*
  * Format: TYPE,ID,CURRENT_TASK,STOP_TIME{,OTHER}
    * TYPE: The Equipment type (see **add** above)
    * ID: The ID of the desired Equipment
    * CURRENT_TASK: The Task ID for the current operation, typically from Ohmbrewer.
    * STOP_TIME: Some time in the future at which to switch the Equipment to the OFF state. This should be provided as an Integer value representing the time in Unix time / Epoch time.
    * OTHER: Zero or more additional arguments. These arguments are generally optional.
      
        | Type       | Additional arguments                                               |
        |------------|--------------------------------------------------------------------|
        | Thermostat | Sensor state, Element state                                        |
        | RIMS       | Safety Sensor state, Pump state{, Thermostat arguments (as above)} |
      * All states should be either ```ON``` or ```OFF```
      * If you wish to skip a given argument but provide a later argument, you must provide ```--```. For example, to set the state of a Thermostat's Element to ON without changing it's Sensor's state, the argument string would be ```therm,1,someuuid,999999,--,ON```
  * Expected result:
    * Success: Particle.function returns the ID number.
    * Failure: Particle.function returns a negative number indicating the cause of the failure.
* index - *Report current Equipment (not yet implemented)*
  * Format: TYPE
    * TYPE: The Equipment type (optional)
  * Expected result:
    * Success: Particle.function returns the number of Equipment instances in the Sprout list and a status report containing information about current pin settings and state for all Equipment is published to the Rhizome's event stream. If TYPE is supplied, only information about that type of Equipment is returned.
    * Failure: Particle.function returns a negative number indicating the cause of the failure.
* info - *Report settings for a particular Equipment instance (not yet implemented)*
  * Format: TYPE,ID
    * TYPE: The Equipment type
    * ID: The ID of the desired Equipment
  * Expected result:
    * Success: Particle.function returns the ID number and a status report containing information about current pin settings and state is published to the Equipment's event stream.
    * Failure: Particle.function returns a negative number indicating the cause of the failure.

As with all things, if this interface isn't working as expected, please consult the code (or other generated documentation) as this README may not be the most up-to-date source of information. (We'll try super hard though. Promise.)
