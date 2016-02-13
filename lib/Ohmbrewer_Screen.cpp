#include "Ohmbrewer_Screen.h"
#include "Ohmbrewer_Equipment.h"
#include "Ohmbrewer_Temperature_Sensor.h"
#include "Ohmbrewer_Heating_Element.h"
#include "Ohmbrewer_Pump.h"
#include "Ohmbrewer_Relay.h"
#include "Ohmbrewer_Thermostat.h"
#include "Ohmbrewer_RIMS.h"


/**
 * Constructor
 */
Ohmbrewer::Screen::Screen(uint8_t CS,
                          uint8_t RS,
                          uint8_t RST,
                          std::deque< Ohmbrewer::Equipment* >* sprouts) : Adafruit_ILI9341(CS, RS, RST) {
    _sprouts = sprouts;
    /** 
    *  For better pressure precision, we need to know the resistance
    *  between X+ and X- Use any multimeter to read it
    *  Using value of 285 ohms across the X plate
    */  
    ts = new TouchScreen(XP, YP, XM, YM, 285);
    menu = new Ohmbrewer::Menu_WiFi(this);
    wiFiMenuUp = false;
}

/**
 * Initializes the display screen
 */
void Ohmbrewer::Screen::initScreen() {
    begin();
    reinitScreen();
}

/**
 * Reinitializes the display screen
 */
void Ohmbrewer::Screen::reinitScreen() {
    // Erase everything
    fillScreen(DEFAULT_BG_COLOR);

    // Draw the buttons that are always there
    drawButtons();

    // Reset the cursor
    setCursor(LEFT, TOP);
}

/**
 * Prints the "Ohmbrewer" title onto the touchscreen
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Screen::displayHeader() {
    unsigned long start = micros();

    // Add the title
    setCursor(0, 0);
    setTextColor(ILI9341_WHITE, DEFAULT_BG_COLOR);
    setTextSize(3);
    println("  OHMBREWER");

    return micros() - start;
}

/**
 * Prints a small margin line (size 1).
 * @param current The current text size to reset to.
 */
void Ohmbrewer::Screen::printMargin(const uint8_t current) {
    setTextSize(1);
    println("");
    setTextSize(current);
}

/**
 * Initializes the display screen
 */
void Ohmbrewer::Screen::drawButtons() {

    setTextColor(ILI9341_WHITE, DEFAULT_BG_COLOR);

    // make the buttons
    fillRect(LEFT, BUTTONTOP, BUTTONSIZE, BUTTONSIZE, DEFAULT_BG_COLOR);
    fillRect(BUTTONSIZE, BUTTONTOP, BUTTONSIZE, BUTTONSIZE, DEFAULT_BG_COLOR);
    fillRect(BUTTONSIZE*2, BUTTONTOP, BUTTONSIZE, BUTTONSIZE, DEFAULT_BG_COLOR);
    fillRect(BUTTONSIZE*3, BUTTONTOP, BUTTONSIZE, BUTTONSIZE, DEFAULT_BG_COLOR);
    drawRect(LEFT, BUTTONTOP, BUTTONSIZE, BUTTONSIZE, DEFAULT_TEXT_COLOR);
    drawRect(BUTTONSIZE, BUTTONTOP, BUTTONSIZE, BUTTONSIZE, DEFAULT_TEXT_COLOR);
    drawRect(BUTTONSIZE*2, BUTTONTOP, BUTTONSIZE, BUTTONSIZE, DEFAULT_TEXT_COLOR);
    drawRect(BUTTONSIZE*3, BUTTONTOP, BUTTONSIZE, BUTTONSIZE, DEFAULT_TEXT_COLOR);

    // Add symbols to the buttons
    setTextSize(4);
    setCursor(LEFT + 20, BUTTONTOP + 15);
    print("+");
    setCursor(BUTTONSIZE + 20, BUTTONTOP + 15);
    print("-");

    setTextSize(2);
    setCursor((BUTTONSIZE*2) + 7, BUTTONTOP + 20);
    print("Menu");
    setTextSize(2);
    setCursor((BUTTONSIZE*3) + 14, BUTTONTOP + 20);
    print("Sel");

    resetTextColor();
}

/**
 * Refreshes/redraws the touchscreen display
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Screen::refreshDisplay() {
    unsigned long start = micros();

    displayHeader();

    displayRIMS();
    displayThermostats();

    // We want to show a different border for the Manual Relays section if there were any Thermostats or RIMS
    int thermostats = 0;
    int rims = 0;
    for (std::deque<Ohmbrewer::Equipment*>::iterator itr = _sprouts->begin(); itr != _sprouts->end(); itr++) {
        if (strcmp((*itr)->getType(), Thermostat::TYPE_NAME) == 0) {
            thermostats++;
        }
        if (strcmp((*itr)->getType(), RIMS::TYPE_NAME) == 0) {
            rims++;
        }
    }

    if((thermostats + rims) > 2) {
        // Don't print the header line
    } else if((thermostats + rims) > 0) {
        // We've got 1-2 Thermostats or RIMS, so print a thin line
        print("--------------------");
    } else {
        // There aren't any Thermostats or RIMS, so print a fat line
        print("====================");
    }

    displayManualRelays();
//    // These are the old, single type displays, for easy debugging.
//    displayTemps();
//    displayRelays();
//    displayHeatingElements();
//    displayPumps();

    // 500 seems like a good refresh delay
    delay(500);

    return micros() - start;
}

/**
 * Prints the status information for our current relays onto the touchscreen
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Screen::displayManualRelays() {
    unsigned long start = micros();

    resetTextSizeAndColor();

    for (std::deque<Ohmbrewer::Equipment*>::iterator itr = _sprouts->begin(); itr != _sprouts->end(); itr++) {
        //print out temperature probes
        if (strcmp((*itr)->getType(), TemperatureSensor::TYPE_NAME) == 0) {
            setTextColor(CYAN, DEFAULT_BG_COLOR);
            print("Temp ");
            writeDegree();
            print("C ");
            ((Ohmbrewer::Relay*)(*itr))->display(this);
        }
    }
    for (std::deque<Ohmbrewer::Equipment*>::iterator itr = _sprouts->begin(); itr != _sprouts->end(); itr++) {
        //print out PUMPS
        if (strcmp((*itr)->getType(), Pump::TYPE_NAME) == 0) {
            setTextColor(CYAN, DEFAULT_BG_COLOR);
            print("Pump    ");
            ((Ohmbrewer::Relay*)(*itr))->display(this);
        }
    }
    for (std::deque<Ohmbrewer::Equipment*>::iterator itr = _sprouts->begin(); itr != _sprouts->end(); itr++) {
        //print out relays
        if (strcmp((*itr)->getType(), Relay::TYPE_NAME) == 0) {
            setTextColor(CYAN, DEFAULT_BG_COLOR);
            print("Relay    ");
            ((Ohmbrewer::Relay*)(*itr))->display(this);
        }
    }
    //No Heating elements are supported this way, only manual relays. ... safer

    printMargin(2);

    return micros() - start;
}

/**
 * Prints the status information for our current relays onto the touchscreen
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Screen::displayRelays() {
    unsigned long start = micros();
    bool foundFirst = false;

    resetTextSizeAndColor();

    for (std::deque<Ohmbrewer::Equipment*>::iterator itr = _sprouts->begin(); itr != _sprouts->end(); itr++) {
        if (strcmp((*itr)->getType(), TemperatureSensor::TYPE_NAME) != 0 &&
            strcmp((*itr)->getType(), RIMS::TYPE_NAME) != 0 &&
            strcmp((*itr)->getType(), Thermostat::TYPE_NAME) != 0) {
            if(!foundFirst) {
                // Print the header
                print("====== Relays ======");
                printMargin(2);
                foundFirst = true;
            }
            ((Ohmbrewer::Relay*)(*itr))->display(this);
        }
    }
    if(foundFirst) {
        printMargin(2);
    }

    return micros() - start;
}

/**
 * Prints the status information for our current Heating Elements onto the touchscreen
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Screen::displayHeatingElements() {
    unsigned long start = micros();
    bool foundFirst = false;

    resetTextSizeAndColor();

    for (std::deque<Ohmbrewer::Equipment*>::iterator itr = _sprouts->begin(); itr != _sprouts->end(); itr++) {
        if (strcmp((*itr)->getType(), HeatingElement::TYPE_NAME) == 0) {
            if(!foundFirst) {
                // Print the header
                print("======= Heat =======");
                printMargin(2);
                foundFirst = true;
            }
            ((Ohmbrewer::HeatingElement*)(*itr))->display(this);
        }
    }
    if(foundFirst) {
        printMargin(2);
    }

    return micros() - start;
}

/**
 * Prints the status information for our current Pumps onto the touchscreen
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Screen::displayPumps() {
    unsigned long start = micros();
    bool foundFirst = false;

    resetTextSizeAndColor();

    for (std::deque<Ohmbrewer::Equipment*>::iterator itr = _sprouts->begin(); itr != _sprouts->end(); itr++) {
        if (strcmp((*itr)->getType(), Pump::TYPE_NAME) == 0) {
            if(!foundFirst) {
                // Print the header
                print("======= Pumps ======");
                printMargin(2);
                foundFirst = true;
            }
            ((Ohmbrewer::Pump*)(*itr))->display(this);
        }
    }
    if(foundFirst) {
        printMargin(2);
    }

    return micros() - start;
}

/**
 * Prints the temperature information for our sensors onto the touchscreen.
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Screen::displayTemps() {
    unsigned long start = micros();
    bool foundFirst = false;

    resetTextSizeAndColor();

    for (std::deque<Ohmbrewer::Equipment*>::iterator itr = _sprouts->begin(); itr != _sprouts->end(); itr++) {
        if (strcmp((*itr)->getType(), TemperatureSensor::TYPE_NAME) == 0) {
            if(!foundFirst) {
                // Print the header
                print("= Temperature (");
                writeDegree(); // Degree symbol
                print("C) =");
                printMargin(2);
                foundFirst = true;
            }
            ((Ohmbrewer::TemperatureSensor*)(*itr))->display(this);
        }
    }
    if(foundFirst) {
        printMargin(2);
    }
    return micros() - start;
}

/**
 * Prints the thermostat information onto the touchscreen.
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Screen::displayThermostats() {
    unsigned long start = micros();
    bool foundFirst = false;

    resetTextSizeAndColor();

    for (std::deque<Ohmbrewer::Equipment*>::iterator itr = _sprouts->begin(); itr != _sprouts->end(); itr++) {
        if (strcmp((*itr)->getType(), Thermostat::TYPE_NAME) == 0) {
            ((Ohmbrewer::Thermostat*)(*itr))->display(this);
            printMargin(2);
            printMargin(2);
        }
    }

    return micros() - start;
}

/**
 * Prints the RIMS information onto the touchscreen.
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Screen::displayRIMS() {
    unsigned long start = micros();
    bool foundFirst = false;

    resetTextSizeAndColor();

    for (std::deque<Ohmbrewer::Equipment*>::iterator itr = _sprouts->begin(); itr != _sprouts->end(); itr++) {
        if (strcmp((*itr)->getType(), RIMS::TYPE_NAME) == 0) {
            ((Ohmbrewer::RIMS*)(*itr))->display(this);
            printMargin(2);
            printMargin(2);
        }
    }

    return micros() - start;
}

/**
 * Prints out a status message in the two rows above the buttons.
 * @param char* statusUpdate The status message to display. 40 characters or less.
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Screen::displayStatusUpdate(char *statusUpdate) {
    unsigned long start = micros();

    setTextColor(ILI9341_RED, DEFAULT_BG_COLOR);
    setCursor(LEFT, BUTTONTOP - 40);
    resetTextSize();
    println(statusUpdate);

    return micros() - start;
}

/**
 * Checks for a touch event and triggers actions 
 *  if the the touch was on a screen "button".
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Screen::captureButtonPress() {
    unsigned long start = micros();
    char printx [10];
    char printy [10];
    char status [40];
    
    // a point object holds x y and z coordinates
    TSPoint p = ts->getPoint();
    //According to Particle forums, the read below is necessary to get touch to work on Photon
    ts->readTouchY();

  
    // we have some minimum pressure we consider 'valid'
    // pressure of 0 means no pressing!
    if (p.z < MINPRESSURE || p.z > MAXPRESSURE) {
        displayStatusUpdate("                                        ");
        return micros() - start;
    }
    
    // Get out of here if no one is touching
    if (p.z < 0) {
        displayStatusUpdate("                                        ");
        return micros() - start;
    }
    
    // Scale from ~0->1000 to tft.width using the calibration #'s
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, width()) - 35; // This -35 is a dirty hack. We need to fix the scaling to get this working without it.
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, height());
    
    // Each of these should pad out with spaces on the right
    sprintf(printx, "x is %-5d", p.x);
    sprintf(printy, "y is %-5d", p.y);
    
    String statusUpdate = String(printx);
    statusUpdate.concat(printy);
    
    if (p.y >= BUTTONTOP) {

        if (p.x > 0 && p.x <= BUTTONSIZE) {
            // +                 12345678901234567890
            menu->plusPressed();
            statusUpdate.concat("Pressing +!         ");
        } else if (p.x > BUTTONSIZE && p.x <= BUTTONSIZE*2) {
            // -
            menu->minusPressed();
            statusUpdate.concat("Pressing -!         ");
        } else if (p.x > BUTTONSIZE*2 && p.x <= BUTTONSIZE*3) {
        
            // Menu
            if(!wiFiMenuUp) {
                wiFiMenuUp = true;
                menu->displayMenu();      
            } else {
                wiFiMenuUp = false;
                reinitScreen();
            }
            statusUpdate.concat("Pressing Menu!      ");
        } else if (p.x > BUTTONSIZE*3 && p.x <= BUTTONSIZE*4) {
            // Select
            menu->selectPressed();
            statusUpdate.concat("Pressing Select!    ");
        } else {
            // Nothing. Weird.
            statusUpdate.concat("                    ");
        }
        
        // Barf it onto the display...
        statusUpdate.toCharArray(status, 40);
        displayStatusUpdate(status);
    }
    
    //Serial.print("X = "); Serial.print(p.x);
    //Serial.print("\tY = "); Serial.print(p.y);
    //Serial.print("\tPressure = "); Serial.println(p.z);
    
    return micros() - start;
}

