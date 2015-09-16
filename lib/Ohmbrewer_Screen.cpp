#include "Ohmbrewer_Screen.h"
#include "Ohmbrewer_Equipment.h"
#include "Ohmbrewer_Temperature_Sensor.h"
#include "Ohmbrewer_Relay.h"

/**
 * Constructor
 */
Ohmbrewer::Screen::Screen(uint8_t CS,
                          uint8_t RS,
                          uint8_t RST,
                          std::list< Ohmbrewer::Equipment* >* sprouts) : Adafruit_ILI9341(CS, RS, RST) {
    _sprouts = sprouts;
}

/**
 * Initializes the display screen
 */
void Ohmbrewer::Screen::initScreen() {

    begin();
    fillScreen(DEFAULT_BG_COLOR);
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
    setCursor(0, 0);
    setTextColor(ILI9341_WHITE, DEFAULT_BG_COLOR);
    setTextSize(3);
    println("  OHMBREWER");
    return micros() - start;
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

    // FIXME: This sort of call will be moved into TemperatureSensor.doDisplay(Ohmbrewer::Screen *screen). We won't be calling it like this explicitly.
    displayTemps(((Ohmbrewer::TemperatureSensor*)_sprouts->front())->getTemp()->c(), 100.00);

    displayRelays();

    // 500 seems like a good refresh delay
    delay(500);

    return micros() - start;
}

/**
 * Prints the status information for our current relays onto the touchscreen
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Screen::displayRelays() {
    unsigned long start = micros();
    unsigned int count = 0;

    println("====== Relays ======");
    for (std::list<Ohmbrewer::Equipment*>::iterator itr = _sprouts->begin(); itr != _sprouts->end(); itr++) {
        if (count > 1) {
            // We'll ignore the TemperatureSensors here.
            //displayRelay(count, (*itr)->getState());
            ((Ohmbrewer::Relay*)(*itr))->display(this);
        }
        count++;
    }

    return micros() - start;
}

///**
// * Prints the status information for a given relay onto the touchscreen
// * @param x The relay to display, 0-based
// * @param state The state of the relay
// * @returns Time it took to run the function
// */
//unsigned long Ohmbrewer::Screen::displayRelay(int x, bool state) {
//    unsigned long start = micros();
//    char relay_id[2];
//
//    // Print a fancy identifier
//    print(" [");
//    setTextColor(ILI9341_WHITE, DEFAULT_BG_COLOR);
//
//    sprintf(relay_id,"%d", x-1);
//    print(relay_id);
//
//    resetTextColor();
//    print("]:");
//
//    // Print the state
//    if (state){
//        setTextColor(ILI9341_YELLOW, DEFAULT_BG_COLOR);
//        println(" ON ");
//
//    } else {
//        setTextColor(ILI9341_RED, DEFAULT_BG_COLOR);
//        println(" OFF");
//    }
//
//    resetTextColor();
//
//    return micros() - start;
//}

/**
 * Prints the temperature information for our sensors onto the touchscreen.
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Screen::displayTemps(double current, double target) {
    unsigned long start = micros();

    resetTextColor();

    // Add a top margin
    setTextSize(2);
    println("");

    // Print the section title
    print("= Temperature (");
    write(247); // Degree symbol
    println("C) =");

    // Print out the current temp
    displayCurrentTemp(current,target);

    // Print out the target temp
    displayTargetTemp(target);

    // Add a wee space
    println("");

    return micros() - start;
}

/**
 * Prints the temperature information for our sensors onto the touchscreen.
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Screen::displayCurrentTemp(double current, double target) {
    unsigned long start = micros();

    // If current == target, we'll default to yellow, 'cause we're golden...
    uint16_t color = ILI9341_YELLOW;

    if(current > target) {
        // Too hot
        color = ILI9341_RED;
    } else if(current < target) {
        // Too cold
        color = ILI9341_BLUE;
    }

    displayTemp(current, "Current: ", color);
    
    return micros() - start;
}

/**
 * Prints the temperature information for our sensors onto the touchscreen.
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Screen::displayTargetTemp(double target) {
    unsigned long start = micros();
    displayTemp(target, "Target:  ", ILI9341_YELLOW);
    return micros() - start;
}

/**
 * Prints the temperature information for our sensors onto the touchscreen.
 * @param temp The temperature to display
 * @param label The text label to print to the left of the temperature
 * @param color The color of the temperature text
 * @returns Time it took to run the function
 */
unsigned long Ohmbrewer::Screen::displayTemp(double temp, char* label, uint16_t color) {
    unsigned long start = micros();
    char tempStr [24];

    sprintf(tempStr, "%2.2f", temp);

    // Print the label
    resetTextColor();
    print(" "); // We want a little margin
    print(label);

    // Print out the target temp
    setTextColor(color, DEFAULT_BG_COLOR);
    println(tempStr);

    resetTextColor();

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
    setTextSize(2);
    println(statusUpdate);

    return micros() - start;
}