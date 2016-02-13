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
#include <vector>


namespace Ohmbrewer {

    //forward declaration
    class Screen;
    
    class Menu_WiFi : public Menu {
    
        public:
        
        /**
         * Constructors
         * @param screen A pointer to the Screen object.
         */
        
        Menu_WiFi(Ohmbrewer::Screen *screen);
        
        /**
        * Draws the menu to the Rhizome's display.
        * @returns The time taken to run the method
        */      
        int displayMenu();
        
        /**
         * Takes action when the plus button is pressed in the Menu Screen
         * The precise action will be determined by the individual menu screens
         * @returns The time taken to run the method
         */
        int plusPressed();
        /**
         * Takes action when the plus button is pressed in the Menu Screen
         * The precise action will be determined by the individual menu screens
         * @returns The time taken to run the method
         */
        int minusPressed();
        
        /**
         * Takes action when the menu button is pressed in the Menu Screen
         * The precise action will be determined by the individual menu screens
         * @returns The time taken to run the method
         */
        int menuPressed();
        
        /**
         * Takes action when the select button is pressed in the Menu Screen
         * The precise action will be determined by the individual menu screens
         * @returns The time taken to run the method
         */     
        int selectPressed();
        
        /**
         * Boolean to keep track of whether WiFi should be connected. 
         */         
        bool wiFiSetting;
            
    
    };


}

#endif
