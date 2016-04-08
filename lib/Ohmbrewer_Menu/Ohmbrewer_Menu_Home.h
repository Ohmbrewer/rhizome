/**
 * This library provides the Home Screen class of the Rhizome PID/equipment controller.
 * Rhizome is part of the Ohmbrewer project (see http://ohmbrewer.org for details).
 */

#ifndef OHMBREWER_MENU_HOME_H
#define OHMBREWER_MENU_HOME_H

// Kludge to allow us to use std::vector - for now we have to undefine these macros.
#undef swap
#include "application.h"
#include "Ohmbrewer_Menu.h"
#include "Ohmbrewer_Screen.h"
#include "Ohmbrewer_Runtime_Settings.h"
#include <vector>


namespace Ohmbrewer {

    class MenuHome : public Menu {
    
        public:

        /* Constants */
        static const int MAIN_MENU = 0;

        /* Methods */

        /**
         * Constructors
         * @param screen A pointer to the Screen object.
         * @param settings A pointer to the RuntimeSettings object.
         */
        MenuHome(Screen *screen, RuntimeSettings *settings);

        /**
         * Destructor
         */
        virtual ~MenuHome() {};
        
        /**
         * Draws the menu to the Rhizome's display.
         * @todo Refactor the display drawing stuff that's currently in Screen into this
         */
        void displayMenu();
        
        /**
         * Takes action when the menu button is pressed in the Menu Screen
         * The precise action will be determined by the individual menu screens
         */
        void menuPressed();
    };


}

#endif
