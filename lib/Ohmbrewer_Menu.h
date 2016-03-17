/**
 * This library provides the Menu base class of the Rhizome PID/equipment controller.
 * Rhizome is part of the Ohmbrewer project (see http://ohmbrewer.org for details).
 */

#ifndef OHMBREWER_MENU_H
#define OHMBREWER_MENU_H

// Kludge to allow us to use std::vector - for now we have to undefine these macros.
#undef min
#undef max
#undef swap

#include <vector>
#include "application.h"
#include "Ohmbrewer_Runtime_Settings.h"
#include "Ohmbrewer_Screen.h"


namespace Ohmbrewer {

    //forward declaration
    class Screen;

    class Menu {
    
        public:

        /**
         * Constructors
         * @param screen A pointer to the Screen object.
         * @param settings A pointer to the RuntimeSettings object.
         */
        Menu(Screen *screen, RuntimeSettings *settings);

        /**
         * Destructor
         */
        virtual ~Menu();
        
        /**
         * Draws the menu to the Rhizome's display.
         */
        virtual void displayMenu();
        
        /**
         * Takes action when the plus button is pressed in the Menu Screen
         * The precise action will be determined by the individual menu screens
         */
        virtual void plusPressed();

        /**
         * Takes action when the plus button is pressed in the Menu Screen
         * The precise action will be determined by the individual menu screens
         */
        virtual void minusPressed();
        
        /**
         * Takes action when the menu button is pressed in the Menu Screen
         * The precise action will be determined by the individual menu screens
         */
        virtual void menuPressed();
        
        /**
         * Takes action when the select button is pressed in the Menu Screen
         * The precise action will be determined by the individual menu screens
         */     
        virtual void selectPressed();

        /**
         * Transitions to the next level up (parent) in the menu tree
         */
        void goUp();

        /**
         * Transitions to the next level down via a specified child in the menu tree
         * @param child The index of the child to transition to within the menu's descendants list
         */
        void goDownTo(const int child);

        /**
         * Sets the parent of this menu
         * @param parent The parent to set
         */
        void setParent(Menu* parent);

        /**
         * Adds a child menu below this menu
         * @param child The menu to add
         */
        void addChild(Ohmbrewer::Menu* child);

        /**
         * True if this is the top-most menu in the tree
         * @returns True if this is the Home menu, False otherwise
         */
        bool isHome();
        
        protected:
        
        /**
         * The touchscreen object. Handles the display for the Rhizome.
         */
        Screen* _screen;

        /**
         * The touchscreen object. Handles the display for the Rhizome.
         */
        RuntimeSettings* _settings;
        
        /**
        * The list of options for this particular menu screen.
        */    
        std::vector<String> options;
        
        /**
        * The currently selected option.
        */
        unsigned int _selectedOption;

        /**
         * The menu above this menu in the menu tree
         */
        Menu* _parent;

        /**
         * Zero or more menus reachable from this menu below this menu on the menu tree
         */
        std::vector<Menu*> _children;
    };


}

#endif
