# Firmware for the Rhizome

Anything that should be imported into Particle Build as a "Library" is in the ```lib``` directory. 
Examples of possible .ino file configurations are in the ```firmware``` directory.

### Semi-local build
You can use the Particle CLI tool in conjunction with a local IDE to build the project using the provided CMakeLists.txt
file. Simply add a ```build``` directory to the project directory and navigate to it via the terminal. Then run
```cmake ..``` and the build should get punted off to Particle. If all goes well, you'll get a binary file that you can
then flash to your Core/Photon using ```particle flash DEVICEID firmware.bin```

Note that if you do this, you'll need to have CMake. If you're using CLion, CMake should be bundled within the IDE directory somewhere.
