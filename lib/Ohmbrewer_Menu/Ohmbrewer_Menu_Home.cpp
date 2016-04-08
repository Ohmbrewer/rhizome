#include "Ohmbrewer_Menu_Home.h"
#include "Ohmbrewer_Runtime_Settings.h"
#include "Ohmbrewer_Screen.h"
#include "Ohmbrewer_Menu.h"

/**
 * Constructors
 * @param screen A pointer to the Screen object.
 * @param settings A pointer to the RuntimeSettings object.
 */
Ohmbrewer::MenuHome::MenuHome(Ohmbrewer::Screen *screen, Ohmbrewer::RuntimeSettings *settings) : Ohmbrewer::Menu::Menu(screen, settings) {
    options.push_back("Temp Sensors");
    options.push_back("RIMS");
    options.push_back("Thermostats");
    options.push_back("Pumps");
    _selectedOption = 0;
}

/**
 * Draws the menu to the Rhizome's display.
 * @todo Refactor the display drawing stuff that's currently in Screen into this
 */
void Ohmbrewer::MenuHome::displayMenu() {

    if(_selectedOption == 0) {
        _screen->displayTemps();
    } else if (_selectedOption == 1){
        _screen->displayRIMS();
    } else if (_selectedOption == 2) {
        _screen->displayThermostats();
    } else if (_selectedOption == 3) {
        _screen->displayPumps();
    }




    //    // These are the old, single type displays, for easy debugging.
    //_screen->displayManualRelays();
    //    displayRelays();
    //    displayHeatingElements();
    //    displayPumps();
}

/**
 * Takes action when the menu button is pressed in the Menu Screen
 * The precise action will be determined by the individual menu screens
 */
void Ohmbrewer::MenuHome::menuPressed() {
    _screen->setCurrentMenu(_children[MAIN_MENU]);
    _selectedOption = 0;
    _screen->reinitScreen();
}


