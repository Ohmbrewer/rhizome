#include "Ohmbrewer_Menu_WiFi.h"
#include "Ohmbrewer_Runtime_Settings.h"
#include "Ohmbrewer_Screen.h"


/**
 * Constructor
 * Initializes the options available for the WiFi menu
 * @param screen Passes the screen object that is used to push touches.
 */
 Ohmbrewer::MenuWiFi::MenuWiFi(Ohmbrewer::Screen *screen, Ohmbrewer::RuntimeSettings *settings) : Menu::Menu(screen, settings) {
    //Set up list of options in vector
    options.push_back("WiFi On");
    options.push_back("WiFi Off");
    _selectedOption = _settings->isWifiOn();
 }

/**
 * Displays the WiFi Menu and related options
 */
void Ohmbrewer::MenuWiFi::displayMenu() {

    _screen->resetTextSize();
    _screen->resetTextColor();

    // Print the section title
    _screen->print("======= MENU =======");
    _screen->printMargin(2);
    _screen->print("   ---- WiFi ----   ");
    _screen->printMargin(2);
    //Print current status
    _screen->print("WiFi is now ");
    //_screen->printMargin(2);
    if(_settings->isWifiOn()){
        _screen->println("ON ");
    } else {
        _screen->println("OFF");
    }
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
 * Takes action when the select button is pressed in the Menu Screen
 * The precise action will be determined by the individual menu screens
 * @returns The time taken to run the method
 */     
void Ohmbrewer::MenuWiFi::selectPressed() {
    char status [40];
    String wifiConnecting = "Connecting.";
    if(_selectedOption == 0 && _settings->isWifiOff()) {
        // Turn WiFi on
        _settings->setWifiStatusAndSave(true);

        if(!Particle.connected()){
        
            Particle.connect();
            while(!WiFi.ready()) {
            
                wifiConnecting.concat(".");
                if(wifiConnecting.length() > 39){
                
                    //This has gone on long enough, break
                    break;
                }
                wifiConnecting.toCharArray(status, 40);
                _screen->displayStatusUpdate(status);
                //Serial.println(wifiConnecting);
                delay(100);
            }
            wifiConnecting = "Connected";
            wifiConnecting.toCharArray(status, 40);
            _screen->displayStatusUpdate(status);
        } 
        
    } else if(_selectedOption == 1 && _settings->isWifiOn()) {
        // Turn WiFi off
        _settings->setWifiStatusAndSave(false);

        // Disconnect from WiFi
        Particle.disconnect();
        wifiConnecting = "Disconnected.";
        wifiConnecting.toCharArray(status, 40);
        _screen->displayStatusUpdate(status);
    }
}

/**
 * Takes action when the menu button is pressed in the Menu Screen
 * The precise action will be determined by the individual menu screens
 */
void Ohmbrewer::MenuWiFi::menuPressed() {
    _screen->setCurrentMenu(_parent);
    _screen->reinitScreen();
}
