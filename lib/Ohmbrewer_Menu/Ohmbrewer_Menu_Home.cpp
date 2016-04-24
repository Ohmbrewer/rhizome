#include "Ohmbrewer_Menu_Home.h"
#include "Ohmbrewer_Runtime_Settings.h"
#include "Ohmbrewer_Screen.h"
#include "Ohmbrewer_Menu.h"
#include "Ohmbrewer_Thermostat.h"
#include "Ohmbrewer_RIMS.h"

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
    options.push_back("Everything");
    _selectedOption = 4;
}

/**
 * Draws the menu to the Rhizome's display.
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
    } else {
        _screen->displayRIMS();
        _screen->displayThermostats();

        // We want to show a different border for the Manual Relays section if there were any Thermostats or RIMS
        int thermostats = 0;
        int rims = 0;
        for (std::deque<Ohmbrewer::Equipment *>::iterator itr = _screen->getSprouts()->begin();
             itr != _screen->getSprouts()->end(); itr++) {
            if (strcmp((*itr)->getType(), Thermostat::TYPE_NAME) == 0) {
                thermostats++;
            }
            if (strcmp((*itr)->getType(), RIMS::TYPE_NAME) == 0) {
                rims++;
            }
        }

        if ((thermostats + rims) > 2) {
            // Don't print the header line
        } else if ((thermostats + rims) > 0) {
            // We've got 1-2 Thermostats or RIMS, so print a thin line
            _screen->print("--------------------");
        }
        _screen->displayManualRelays();
    }
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


