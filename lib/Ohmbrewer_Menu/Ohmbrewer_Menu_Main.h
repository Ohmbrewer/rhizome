/**
 * This library provides the Main menu class of the Rhizome PID/equipment controller.
 * Rhizome is part of the Ohmbrewer project (see http://ohmbrewer.org for details).
 */
#ifndef OHMBREWER_MENU_MAIN_H
#define OHMBREWER_MENU_MAIN_H


// Kludge to allow us to use std::vector - for now we have to undefine these macros.
#undef swap
#include "application.h"
#include "Ohmbrewer_Menu.h"
#include "Ohmbrewer_Screen.h"
#include "Ohmbrewer_Runtime_Settings.h"
#include <vector>


namespace Ohmbrewer {

    class MenuMain : public Menu {

    public:

        /* Constants */
        static const int WIFI_MENU = 0;
        static const int TEMPUNIT_MENU = 1;

        /* Methods */

        /**
         * Constructors
         * @param screen A pointer to the Screen object.
         * @param settings A pointer to the RuntimeSettings object.
         */
        MenuMain(Screen *screen, RuntimeSettings *settings);

        /**
         * Destructor
         */
        virtual ~MenuMain() {};

        /**
         * Draws the menu to the Rhizome's display.
         *
         */
        void displayMenu();


        /**
         * Takes action when the select button is pressed in the Menu Screen
         * The precise action will be determined by the individual menu screens
         */
        void selectPressed();
    };


}

#endif //OHMBREWER_MENU_MAIN_H
