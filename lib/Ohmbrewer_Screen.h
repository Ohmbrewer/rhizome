#ifndef RHIZOME_OHMBREWER_SCREEN_H
#define RHIZOME_OHMBREWER_SCREEN_H


#undef min
#undef max
#undef swap
#include <deque>
#include "Adafruit_ILI9341.h"
#include "application.h"

namespace Ohmbrewer {

    class Equipment;

    // TODO: Add a member object to Ohmbrewer::Screen that represents the capacitive touch capabilities (e.g. an instance of Adafruit Touch 4Wire TouchScreen)
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
             * Prints out a status message in the two rows above the buttons.
             * @param char* statusUpdate The status message to display. 40 characters or less.
             * @returns Time it took to run the function
             */
            unsigned long displayStatusUpdate(char *statusUpdate);

            Screen(uint8_t CS, uint8_t RS, uint8_t RST, std::deque< Ohmbrewer::Equipment* >* sprouts);

        private:
            /**
             * Pointer to the global Sprouts list
             */
            std::deque< Ohmbrewer::Equipment* >* _sprouts;
    };

}


#endif //RHIZOME_OHMBREWER_SCREEN_H
