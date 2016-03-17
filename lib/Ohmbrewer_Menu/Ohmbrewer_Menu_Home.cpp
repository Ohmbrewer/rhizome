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
    // Nothing new to do
}

/**
 * Draws the menu to the Rhizome's display.
 * @todo Refactor the display drawing stuff that's currently in Screen into this
 */
void Ohmbrewer::MenuHome::displayMenu() {
    // Do nothing for now...
}

/**
 * Takes action when the menu button is pressed in the Menu Screen
 * The precise action will be determined by the individual menu screens
 */
void Ohmbrewer::MenuHome::menuPressed() {
    _screen->setCurrentMenu(_children[WIFI_MENU]);
}
