#include "Ohmbrewer_Menu.h"
#include "Ohmbrewer_Screen.h"
#include "Ohmbrewer_Runtime_Settings.h"

/**
 * Constructors
 * @param screen A pointer to the Screen object.
 * @param settings A pointer to the RuntimeSettings object.
 */
Ohmbrewer::Menu::Menu(Ohmbrewer::Screen *screen, Ohmbrewer::RuntimeSettings *settings) {
    _screen = screen;
    _settings = settings;
}

/**
 * Destructor
 */
Ohmbrewer::Menu::~Menu() {
    // Do nothing
}

/**
 * Draws the menu to the Rhizome's display.
 */
void Ohmbrewer::Menu::displayMenu() {
    // Disabled by default
}

/**
 * Takes action when the select button is pressed in the Menu Screen
 * The precise action will be determined by the individual menu screens
 */
void Ohmbrewer::Menu::selectPressed() {
    // Disabled by default
}

/**
 * Takes action when the menu button is pressed in the Menu Screen
 * The precise action will be determined by the individual menu screens
 */
void Ohmbrewer::Menu::menuPressed() {
    _screen->setCurrentMenu(_parent);
    _screen->reinitScreen();
}

/**
 * Takes action when the plus button is pressed in the WiFi Menu Screen
 * @returns The time taken to run the method
 */
void Ohmbrewer::Menu::plusPressed(){
    if(_selectedOption < (options.size() - 1)){
        _selectedOption++;
    } else {
        _selectedOption = 0;
    }
}

/**
* Takes action when the minus button is pressed in the WiFi Menu Screen
* @returns The time taken to run the method
*/
void Ohmbrewer::Menu::minusPressed(){
    if(_selectedOption > 0){
        _selectedOption--;
    } else {
        _selectedOption = (options.size() - 1);
    }
}

/**
* Lists the options for the menu with a ">" at the currently selected option
*
*/
void Ohmbrewer::Menu::printOptions(){

    _screen->printMargin(2);

    std::vector<String>::const_iterator i;
    unsigned int j = 0;
    //Print options with ">" at currently selected option
    for(i=options.begin(); i != options.end(); ++i){

        if(_selectedOption == j){
            _screen->print(" > ");
        } else {
            _screen->print("   ");
        }

        _screen->println(*i);
        _screen->printMargin(2);
        j++;
    }

}

/**
 * Transitions to the next level up (parent) in the menu tree
 * @returns The time taken to run the method
 */
void Ohmbrewer::Menu::goUp() {
    _screen->setCurrentMenu(_parent);
}

/**
 * Transitions to the next level down via a specified child in the menu tree
 * @param child The index of the child to transition to within the menu's descendants list
 */
void Ohmbrewer::Menu::goDownTo(const int child) {
    _screen->setCurrentMenu(_children[child]);
}

/**
 * Sets the parent of this menu
 * @param parent The parent to set
 */
void Ohmbrewer::Menu::setParent(Menu* parent) {
    _parent = parent;
}

/**
 * Adds a child menu below this menu
 * @param child The menu to add
 */
void Ohmbrewer::Menu::addChild(Menu* child) {
    child->setParent(this);
    _children.push_back(child);
}

/**
 * True if this is the top-most menu in the tree
 * @returns True if this is the Home menu, False otherwise
 */
bool Ohmbrewer::Menu::isHome() {
    return _parent == nullptr; // Could just return _parent, but let's be explicit
}
