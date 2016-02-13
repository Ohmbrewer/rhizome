#include "Ohmbrewer_Menu_WiFi.h"
#include "Ohmbrewer_Screen.h"


/**
 * Constructor
 * Initializes the options available for the WiFi menu
 * @param screen Passes the screen object that is used to push touches.
 */
 Ohmbrewer::Menu_WiFi::Menu_WiFi(Ohmbrewer::Screen *screen){
    _screen = screen;
    //Set up list of options in vector
    options.push_back("WiFi On");
    options.push_back("WiFi Off");
    selectedOption = 0;
    
    //WiFi setting is stored at addr 1 
    uint8_t value = EEPROM.read(MENU_WIFI_ADDR); 
    
    if (value == 0x00) {
        wiFiSetting = false;
    } else if (value == 0x01) {
        wiFiSetting = true;
    } else {
        //default to true
        wiFiSetting = true;
        EEPROM.write(MENU_WIFI_ADDR, 0x01);
    }
 }

/**
 * Displays the WiFi Menu and related options
 * @returns The time taken to run the method
 */
int Ohmbrewer::Menu_WiFi::displayMenu() {
    unsigned long start = micros();

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
    if(wiFiSetting){
        _screen->println("ON ");
    } else {
        _screen->println("OFF");
    }
    _screen->printMargin(2);
    
    std::vector<String>::const_iterator i;
    int j = 0;
    //Print options with ">" at currently selected option
    for(i=options.begin(); i!=options.end(); ++i){

        if(selectedOption == j){
            _screen->print(" > ");
        } else {
            _screen->print("   ");
        }

        _screen->println(*i);
        _screen->printMargin(2);
        j++;
    }

    return micros() - start;
}

/**
 * Takes action when the plus button is pressed in the WiFi Menu Screen
 * @returns The time taken to run the method
 */
int Ohmbrewer::Menu_WiFi::plusPressed(){
    unsigned long start = micros();
    
    if(selectedOption < (options.size() - 1)){
        selectedOption++;
    } else {
        selectedOption=0;
    }
    displayMenu();
    return micros() - start;    
 }
 
 /**
 * Takes action when the minus button is pressed in the WiFi Menu Screen
 * @returns The time taken to run the method
 */
int Ohmbrewer::Menu_WiFi::minusPressed(){
    unsigned long start = micros();
    
    if(selectedOption > 0){
        selectedOption--;
    } else {
        selectedOption=(options.size() - 1);
    }
    displayMenu();
    return micros() - start;    
 }
 
/**
 * Takes action when the select button is pressed in the Menu Screen
 * The precise action will be determined by the individual menu screens
 * @returns The time taken to run the method
 */     
int Ohmbrewer::Menu_WiFi::selectPressed(){
    char status [40];
    String wifiConnecting = "Connecting.";
    if(selectedOption == 0 && !wiFiSetting) {  //Turn WiFi on
    
        wiFiSetting = true;
        EEPROM.write(MENU_WIFI_ADDR, 0x01);
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
        
    } else if(selectedOption == 1 && wiFiSetting) {
        //checking for wiFiSetting first prevents unnecessary writes to EEPROM
        wiFiSetting = false;
        EEPROM.write(MENU_WIFI_ADDR, 0x00);
        //disconnect from WiFi
        Particle.disconnect();
        wifiConnecting = "Disconnected.";
        wifiConnecting.toCharArray(status, 40);
        _screen->displayStatusUpdate(status);

    }
    _screen->reinitScreen();
    displayMenu();
}

/**
 * Takes action when the menu button is pressed in the Menu Screen
 * The precise action will be determined by the individual menu screens
 * @returns The time taken to run the method
 */
int Ohmbrewer::Menu_WiFi::menuPressed(){

    //TODO: set the menu setting to parent and displayMenu
    
    return 0;

}
