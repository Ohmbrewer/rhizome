#include "Ohmbrewer_Menu_Main.h"
#include "Ohmbrewer_Runtime_Settings.h"
#include "Ohmbrewer_Screen.h"

/**
 * Constructors
 * @param screen A pointer to the Screen object.
 * @param settings A pointer to the RuntimeSettings object.
 */
Ohmbrewer::MenuMain::MenuMain(Ohmbrewer::Screen *screen, Ohmbrewer::RuntimeSettings *settings) : Ohmbrewer::Menu::Menu(screen, settings) {
    options.push_back("WiFi On/Off");
    options.push_back("Temp Units");
    _selectedOption = 0;

}


/**
 * Displays the Main Menu and related options
 */
void Ohmbrewer::MenuMain::displayMenu() {

    _screen->resetTextSize();
    _screen->resetTextColor();

    // Print the section title
    _screen->print("======= MENU =======");
    _screen->printMargin(2);
    _screen->print(" ---- Main Menu --- ");
    _screen->printMargin(2);

    printOptions();

}


/**
 * Takes action when the select button is pressed in the Menu Screen
 * The precise action will be determined by the individual menu screens
 */
void Ohmbrewer::MenuMain::selectPressed(){
    goDownTo(_selectedOption);
    _screen->reinitScreen();
    _selectedOption = 0;
    //_screen->getCurrentMenu()->displayMenu();
}
