/**
 * This library provides the WiFi Menu class of the Rhizome PID/equipment controller.
 * Rhizome is part of the Ohmbrewer project (see http://ohmbrewer.org for details).
 */

#ifndef OHMBREWER_MENU_WIFI_H
#define OHMBREWER_MENU_WIFI_H

// Kludge to allow us to use std::vector - for now we have to undefine these macros.
#undef swap
#include "application.h"
#include "Ohmbrewer_Menu.h"
#include "Ohmbrewer_Screen.h"
#include "Ohmbrewer_Runtime_Settings.h"
#include <vector>


namespace Ohmbrewer {

    class MenuWiFi : public Menu {
    
        public:

        /**
         * Constructors
         * @param screen A pointer to the Screen object.
         */
        MenuWiFi(Screen *screen, RuntimeSettings *settings);

        /**
         * Destructor
         */
        virtual ~MenuWiFi() {};
        
        /**
        * Draws the menu to the Rhizome's display.
        */      
        void displayMenu();
        
        /**
         * Takes action when the menu button is pressed in the Menu Screen
         * The precise action will be determined by the individual menu screens
         */
        void menuPressed();
        
        /**
         * Takes action when the select button is pressed in the Menu Screen
         * The precise action will be determined by the individual menu screens
         */
        void selectPressed();
            
    
    };


}

#endif
