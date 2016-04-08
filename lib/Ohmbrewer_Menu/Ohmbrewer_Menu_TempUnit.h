/**
 * This library provides the Temperature Unit Menu class of the Rhizome PID/equipment controller.
 * Rhizome is part of the Ohmbrewer project (see http://ohmbrewer.org for details).
 */

#ifndef OHMBREWER_MENU_TEMPUNIT_H
#define OHMBREWER_MENU_TEMPUNIT_H

// Kludge to allow us to use std::vector - for now we have to undefine these macros.
#undef swap
#include "application.h"
#include "Ohmbrewer_Menu.h"
#include "Ohmbrewer_Screen.h"
#include "Ohmbrewer_Runtime_Settings.h"
#include <vector>


namespace Ohmbrewer {

    class MenuTempUnit : public Menu {

    public:

        static const bool UNIT_CELSIUS = true;
        static const bool UNIT_FAHRENHEIT = false;

        /**
         * Constructors
         * @param screen A pointer to the Screen object.
         */
        MenuTempUnit(Screen *screen, RuntimeSettings *settings);

        /**
         * Destructor
         */
        virtual ~MenuTempUnit() {};

        /**
        * Draws the menu to the Rhizome's display.
        */
        void displayMenu();


        /**
         * Takes action when the select button is pressed in the Menu Screen
         * The precise action will be determined by the individual menu screens
         */
        void selectPressed();


    };


}

#endif //OHMBREWER_MENU_TEMPUNIT_H
