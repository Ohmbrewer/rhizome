#ifndef RHIZOME_OHMBREWER_SCREEN_H
#define RHIZOME_OHMBREWER_SCREEN_H


#undef min
#undef max
#undef swap

//Below definitions are for the touch screen
#define XP A1
#define YP A0
#define XM A7
#define YM A2

#include <deque>
#include "Adafruit_ILI9341.h"
#include "application.h"
#include "Touch_4Wire.h"
#include "Ohmbrewer_Runtime_Settings.h"
#include "Ohmbrewer_Menu.h"

namespace Ohmbrewer {

    class Equipment;
    class Menu;

    // TODO: Add a member object to Ohmbrewer::Screen that represents the capacitive touch capabilities
    // (e.g. an instance of Adafruit Touch 4Wire TouchScreen)
    class Screen : public Adafruit_ILI9341 {

        public:

            static const int      BUTTONSIZE = 60;
            static const int      LEFT = 0;
            static const int      RIGHT = 240;
            static const int      TOP = 0;
            static const int      BOTTOM = 320;
            static const int      BUTTONTOP = 260;
            static const uint8_t  DEFAULT_TEXT_SIZE = 2;
            static const uint16_t DEFAULT_TEXT_COLOR = ILI9341_GREEN;
            static const uint16_t DEFAULT_BG_COLOR = ILI9341_BLACK;
            static const uint16_t RED = ILI9341_RED;
            static const uint16_t BLACK = ILI9341_BLACK;
            static const uint16_t BLUE = ILI9341_BLUE;
            static const uint16_t GREEN = ILI9341_GREEN;
            static const uint16_t WHITE = ILI9341_WHITE;
            static const uint16_t YELLOW = ILI9341_YELLOW;
            static const uint16_t CYAN = ILI9341_CYAN;


            // This is calibration data for the raw touch data to the screen coordinates
            static const int      TS_MINX = 500;
            static const int      TS_MINY = 300;
            static const int      TS_MAXX = 3650;
            static const int      TS_MAXY = 3650;
            static const int      MAXPRESSURE = 4000;
            static const int      MINPRESSURE = 50;

            /**
             * CONSTRUCTOR
             */
            Screen(uint8_t CS, uint8_t RS, uint8_t RST, std::deque< Ohmbrewer::Equipment* >* sprouts, Ohmbrewer::RuntimeSettings *settings);

            /**
             * Resets the foreground and background text colors to the defaults above.
             */
            void resetTextColor() { setTextColor(DEFAULT_TEXT_COLOR, DEFAULT_BG_COLOR); }

            /**
             * Resets the foreground and background text colors to the defaults above.
             */
            void resetTextSize() { setTextSize(DEFAULT_TEXT_SIZE); }

            /**
             * Resets the foreground and background text colors to the defaults above.
             */
            void resetTextSizeAndColor() { resetTextSize(); resetTextColor(); }

            /**
             * Writes a degree symbol to the screen.
             */
            void writeDegree() { write(247); }

            /**
             * Writes a solid block symbol to the screen. Useful for erasing characters.
             */
            void writeBlock() { write(218); }

            /**
             * Prints a small margin line (size 1).
             * @param current The current text size to reset to.
             */
            void printMargin(const uint8_t current);

            /**
             * Initializes the display screen
             */
            void initScreen();

            /**
             * Reinitializes the display screen
             */
            void reinitScreen();

            /**
             * Prints the "Ohmbrewer" title onto the touchscreen
             * @returns Time it took to run the function
             */
            unsigned long displayHeader();

            /**
             * Initializes the display screen
             */
            void drawButtons();

            /**
             * Refreshes/redraws the touchscreen display
             * @returns Time it took to run the function
             */
            unsigned long refreshDisplay();

            /**
             * Prints the status information for our current relays onto the touchscreen
             * @returns Time it took to run the function
             */
            unsigned long displayManualRelays();

            /**
             * Prints the status information for our current relays onto the touchscreen
             * @returns Time it took to run the function
             */
            unsigned long displayRelays();

            /**
             * Prints the status information for our current Heating Elements onto the touchscreen
             * @returns Time it took to run the function
             */
            unsigned long displayHeatingElements();

            /**
             * Prints the status information for our current Pumps onto the touchscreen
             * @returns Time it took to run the function
             */
            unsigned long displayPumps();

            /**
             * Prints the temperature information for our sensors onto the touchscreen
             * @returns Time it took to run the function
             */
            unsigned long displayTemps();

            /**
             * Prints the thermostat information onto the touchscreen.
             * @returns Time it took to run the function
             */
            unsigned long displayThermostats();

            /**
             * Prints the RIMS information onto the touchscreen.
             * @returns Time it took to run the function
             */
            unsigned long displayRIMS();

            /**
             * Prints out a status message in the two rows above the buttons.
             * @param char* statusUpdate The status message to display. 40 characters or less.
             * @returns Time it took to run the function
             */
            unsigned long displayStatusUpdate(char *statusUpdate);
            
            /**
             * Checks for a touch event and triggers actions 
             *  if the the touch was on a screen "button".
             * @returns Time it took to run the function
             */
            unsigned long captureButtonPress();

            /**
             * Moves the Current Menu pointer
             * @param nextMenu The menu to move to
             */
            void setCurrentMenu(Menu* nextMenu);

            /**
             * Moves the Current Menu pointer
             * @param nextMenu The menu to move to
             */
            Menu* getCurrentMenu() const;

            /**
             * Moves the Current settings pointer
             * @returns RuntimeSettings object
             */
            RuntimeSettings* getSettings() const;

        private:
            /**
             * Pointer to the global Sprouts list
             */
            std::deque< Equipment* >* _sprouts;

            /**
             * Pointer to the global Sprouts list
             */
            RuntimeSettings* _settings;
            
            /** 
             *  The touchscreen object used to check for taps on the screen
             */
            TouchScreen* _ts;
            
            /**
             * Pointer to the current menu
             */            
            Menu* _currentMenu;

            /**
             * Pointer to the home menu, for quickly jumping to home
             */
            Menu* _homeMenu;

    };

}


#endif //RHIZOME_OHMBREWER_SCREEN_H
