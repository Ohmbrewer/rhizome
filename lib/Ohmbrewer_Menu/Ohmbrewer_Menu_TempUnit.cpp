#include "Ohmbrewer_Menu_TempUnit.h"
#include "Ohmbrewer_Runtime_Settings.h"
#include "Ohmbrewer_Screen.h"

/**
 * Constructors
 * @param screen A pointer to the Screen object.
 * @param settings A pointer to the RuntimeSettings object.
 */
Ohmbrewer::MenuTempUnit::MenuTempUnit(Ohmbrewer::Screen *screen, Ohmbrewer::RuntimeSettings *settings) : Ohmbrewer::Menu::Menu(screen, settings) {
    options.push_back("Celsius");
    options.push_back("Fahrenheit");
    _selectedOption = 0;

}

/**
 * Displays the Temp Unit Menu and options
 */
void Ohmbrewer::MenuTempUnit::displayMenu() {

    _screen->resetTextSize();
    _screen->resetTextColor();

    // Print the section title
    _screen->print("======= MENU =======");
    _screen->printMargin(2);
    _screen->print(" ---- Temp Unit --- ");
    _screen->printMargin(2);
    //Print current status
    _screen->print("Selected Unit: ");
    _screen->writeDegree();
    //_screen->printMargin(2);
    if(_settings->isTempUnitCelsius()){
        _screen->println("C");
    } else {
        _screen->println("F");
    }

    printOptions();

}

/**
 * Takes action when the select button is pressed in the Menu Screen
 * The precise action will be determined by the individual menu screens
 * @returns The time taken to run the method
 */
void Ohmbrewer::MenuTempUnit::selectPressed() {

    if(_selectedOption == 0) {
        //Degrees Centigrade!
        _settings->setTempUnitAndSave(UNIT_CELSIUS);


    } else if(_selectedOption == 1) {
        // Degrees Fahrenheit
        _settings->setTempUnitAndSave(UNIT_FAHRENHEIT);
    }
}


