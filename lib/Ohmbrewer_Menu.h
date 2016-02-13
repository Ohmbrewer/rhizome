/**
 * This library provides the Menu base class of the Rhizome PID/equipment controller.
 * Rhizome is part of the Ohmbrewer project (see http://ohmbrewer.org for details).
 */

#ifndef OHMBREWER_MENU_H
#define OHMBREWER_MENU_H

// Kludge to allow us to use std::vector - for now we have to undefine these macros.
#undef swap
#include "application.h"
#include <vector>


namespace Ohmbrewer {

    //forward declaration
    class Screen;
    
    class Menu {
    
        public:
        
        static const int      MENU_WIFI_ADDR = 1;
        
        /**
        * Constructors
        * @param screen A pointer to the Screen object.
        */
        
        //Menu(Ohmbrewer::Screen *screen);
        
        /**
        * Draws the menu to the Rhizome's display.
        * @returns The time taken to run the method
        */      
        
        virtual int displayMenu();
        
        /**
         * Takes action when the plus button is pressed in the Menu Screen
         * The precise action will be determined by the individual menu screens
         * @returns The time taken to run the method
         */
        virtual int plusPressed();
        /**
         * Takes action when the plus button is pressed in the Menu Screen
         * The precise action will be determined by the individual menu screens
         * @returns The time taken to run the method
         */
        virtual int minusPressed();
        
        /**
         * Takes action when the menu button is pressed in the Menu Screen
         * The precise action will be determined by the individual menu screens
         * @returns The time taken to run the method
         */
        virtual int menuPressed();
        
        /**
         * Takes action when the select button is pressed in the Menu Screen
         * The precise action will be determined by the individual menu screens
         * @returns The time taken to run the method
         */     
        virtual int selectPressed();
        
        protected:
        
        
        /**
        * The touchscreen object. Handles the display for the Rhizome.
        */
        Screen * _screen;
        
        /**
        * The list of options for this particular menu screen.
        */    
        std::vector<String> options;
        
        /**
        * The currently selected option.
        */
        int selectedOption;
    
    };


}

#endif
