#include "Adafruit_ILI9341.h"
#include "Ohmbrewer_Pump.h"
#include "Ohmbrewer_Publisher.h"

/* ========================================================================= */
/*  Global Vars                                                              */
/* ========================================================================= */

// index is related to Digital pin number, 0 to 0, 1 to 1...etc. 0 and 1 are the temp pins.
int relays[6] = {0,0,0,0,0,0};

//temp variable definitions
int sensor_dig = D1;
int sensor_analog = D0;
char temperatureInfo[64];
float fahrenheit = 156;
float celsius = 68.8889;
double f;
double target_temp_1 = 100; //0;
double target_temp_2 = 0;

// Set the screen object
Adafruit_ILI9341 tft = Adafruit_ILI9341(D6, D7, A6);

/* ========================================================================= */
/*  Button Settings                                                          */
/* ========================================================================= */
#define BUTTONSIZE 60
#define LEFT 0
#define RIGHT 240
#define TOP 0
#define BOTTOM 320
#define BUTTONTOP 260

/* ========================================================================= */
/*  Main Functions                                                           */
/* ========================================================================= */
/**
 * Does any preliminary setup work before the Rhizome starts the operation loop.
 */
void setup() {
    initScreen();
    Spark.variable("temp", temperatureInfo, STRING);
    Spark.variable("f", &f, DOUBLE);
}

/**
 * The meat of the program. Runs repeatedly until the Rhizome is powered off.
 */
void loop() {
    refreshDisplay();
}

/* ========================================================================= */
/*  Display Functions                                                        */
/* ========================================================================= */

/**
 * Prints out a status message in the two rows above the buttons.
 * @param char* statusUpdate The status message to display. 40 characters or less.
 * @returns Time it took to run the function
 */
unsigned long displayStatusUpdate(char *statusUpdate) {
    unsigned long start = micros();

    tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
    tft.setCursor(LEFT, BUTTONTOP - 40);
    tft.setTextSize(2);
    tft.println(statusUpdate);

    return micros() - start;
}


/**
 * Initializes the display screen
 */
void initScreen() {

    tft.begin();
    tft.fillScreen(ILI9341_BLACK);
    drawButtons();

    // Reset the cursor
    tft.setCursor(LEFT, TOP);
}

/**
 * Initializes the display screen
 */
void drawButtons() {

    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);

    // make the buttons
    tft.fillRect(LEFT, BUTTONTOP, BUTTONSIZE, BUTTONSIZE, ILI9341_BLACK);
    tft.fillRect(BUTTONSIZE, BUTTONTOP, BUTTONSIZE, BUTTONSIZE, ILI9341_BLACK);
    tft.fillRect(BUTTONSIZE*2, BUTTONTOP, BUTTONSIZE, BUTTONSIZE, ILI9341_BLACK);
    tft.fillRect(BUTTONSIZE*3, BUTTONTOP, BUTTONSIZE, BUTTONSIZE, ILI9341_BLACK);
    tft.drawRect(LEFT, BUTTONTOP, BUTTONSIZE, BUTTONSIZE, ILI9341_GREEN);
    tft.drawRect(BUTTONSIZE, BUTTONTOP, BUTTONSIZE, BUTTONSIZE, ILI9341_GREEN);
    tft.drawRect(BUTTONSIZE*2, BUTTONTOP, BUTTONSIZE, BUTTONSIZE, ILI9341_GREEN);
    tft.drawRect(BUTTONSIZE*3, BUTTONTOP, BUTTONSIZE, BUTTONSIZE, ILI9341_GREEN);

    // Add symbols to the buttons
    tft.setTextSize(4);
    tft.setCursor(LEFT + 20, BUTTONTOP + 15);
    tft.print("+");
    tft.setCursor(BUTTONSIZE + 20, BUTTONTOP + 15);
    tft.print("-");

    tft.setTextSize(2);
    tft.setCursor((BUTTONSIZE*2) + 7, BUTTONTOP + 20);
    tft.print("Menu");
    tft.setTextSize(2);
    tft.setCursor((BUTTONSIZE*3) + 14, BUTTONTOP + 20);
    tft.print("Sel");

    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
}

/**
 * Refreshes/redraws the touchscreen display
 * @returns Time it took to run the function
 */
unsigned long refreshDisplay() {
    unsigned long start = micros();

    displayHeader();
    displayTemps();
    displayRelays();

    // 500 seems like a good refresh delay
    delay(500);

    return micros() - start;
}

/**
 * Prints the status information for our current relays onto the touchscreen
 * @returns Time it took to run the function
 */
unsigned long displayRelays() {
    unsigned long start = micros();
    char relay_id[2];

    tft.println("====== Relays ======");
    for(int x=2; x<6; x++){

        // Print a fancy identifier
        tft.print(" [");
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);

        sprintf(relay_id,"%d", x-1);
        tft.print(relay_id);

        tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
        tft.print("]:");

        // Print the conditional status
        if (relays[x]){
            tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
            tft.println(" ON ");

        } else {
            tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
            tft.println(" OFF");
        }

        // Always reset to green
        tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
    }

    return micros() - start;
}

/**
 * Prints the temperature information for our sensors onto the touchscreen
 * @returns Time it took to run the function
 */
unsigned long displayTemps() {
    unsigned long start = micros();
    char probe_1 [24];
    char target_1 [24];

    sprintf(probe_1,"%2.2f",celsius);
    sprintf(target_1,"%2.2f",target_temp_1);

    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
    tft.setTextSize(2);
    tft.println("");
    tft.print("= Temperature (");
    tft.write(247); // Degree symbol
    tft.println("C) =");

    // Print out the current temp
    tft.print(" Current: ");
    if(celsius > target_temp_1) {
        tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
    } else if(celsius < target_temp_1) {
        tft.setTextColor(ILI9341_BLUE, ILI9341_BLACK);
    } else {
        tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
    }
    tft.println(probe_1);

    // Print out the target temp
    tft.setTextColor(ILI9341_GREEN);
    tft.print(" Target:  ");
    tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
    tft.println(target_1);

    // Reset to green
    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);

    // Add a wee space
    tft.println("");

    return micros() - start;
}

/**
 * Prints the "Ohmbrewer" title onto the touchscreen
 * @returns Time it took to run the function
 */
unsigned long displayHeader() {
    unsigned long start = micros();
    tft.setCursor(0, 0);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft.setTextSize(3);
    tft.println("  OHMBREWER");
    return micros() - start;
}