/*
 * Unicorn
 *   Author:      Thorinair
 *   Version:     v1.1.0
 *   Description: A high precision wireless thermometer and humidity meter.
 *   
 *   This is the main source code file. All configuration is to be done inside the Configuration.h file.
*/

#include "ESP8266WiFi.h"
#include <EEPROM.h>
#include <Wire.h>
#include <SHT21.h>
#include <VariPass.h>

#include "Configuration.h"



/* PIN Definitions */
#define PIN_LED       13
#define PIN_BATTERY   A0

/* Flash Types */
#define FLASH_TYPE_WAKE 0
#define FLASH_TYPE_DONE 1
#define FLASH_TYPE_FAIL 2

/* Flash Timings */
#define FLASH_TIME_SHORT 50
#define FLASH_TIME_PAUSE 100
#define FLASH_TIME_LONG  500

/* EEPROM Addresses */
#define EEPROM_SAVED 0
#define EEPROM_WIFI  1

/* Default Settings */
#define DEFAULT_WIFI 0

/* WiFI Connection Results */
#define WIFI_RESULT_FAIL 0
#define WIFI_RESULT_DONE 1

/* WiFi Attempt Duration */
#define WIFI_ATTEMPT_TIME 500



/* Formatting Functions */
String splitData(String data, char separator, int index);
IPAddress getIPFromString(char* addressString, int id);

/* Setup Functions */
void setupLED();
void setupSensor();
void setupSettings();
void setupWiFi();

/* Utility Functions */
int connectWiFi(char* ssid, char* pass, char* conf);
void flashStatusLED(int type);

/* Processing Functions */
void processSensors();



/* Variables */
SHT21 SHT21;
int prevWiFi;
int currWiFi;



/* Formatting Functions */
String splitData(String data, char separator, int index) {
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = data.length() - 1;

    for(int i = 0; i <= maxIndex && found <= index; i++) {
        if(data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
    }

    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
} 

IPAddress getIPFromString(char* addressString, int id) {
    String address = splitData(String(addressString), '|', id);
    return IPAddress(splitData(address, '.', 0).toInt(), splitData(address, '.', 1).toInt(), splitData(address, '.', 2).toInt(), splitData(address, '.', 3).toInt());
}



/* Setup Functions */
void setupLED() {
    if (LED_STATUS_ENABLE) {
        pinMode(PIN_LED, OUTPUT);
        analogWrite(PIN_LED, PWMRANGE - LED_STATUS_IDLE_BRIGHTNESS); 
    }
}

void setupSensor() {
    SHT21.begin();
}

void setupSettings() {    
    EEPROM.begin(512);
    int saved = EEPROM.read(EEPROM_SAVED);
    EEPROM.end();

    if (saved == 1) {
        //Serial.println("\nSettings already exist. Loading...");
        EEPROM.begin(512);
        prevWiFi = currWiFi = (int) EEPROM.read(EEPROM_WIFI);
        EEPROM.end();
        //Serial.println("Loaded settings from EEPROM.");
    }
    else {   
        prevWiFi = currWiFi = DEFAULT_WIFI;
        //Serial.println("\nCreated new settings. Saving...");
        EEPROM.begin(512);
        EEPROM.write(EEPROM_SAVED, 1);
        EEPROM.write(EEPROM_WIFI,  prevWiFi);  
        EEPROM.end();
        //Serial.println("Saved settings to EEPROM.");       
    }
}

void setupWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.hostname(WIFI_HOST);

    int result;

    do {
        result = 0;
        switch (currWiFi) {
            case 0:  
                #ifdef WIFI_0_SSID         
                    result = connectWiFi(WIFI_0_SSID, WIFI_0_PASS, WIFI_0_CONF);
                #endif  
                break;  
            case 1:  
                #ifdef WIFI_1_SSID         
                    result = connectWiFi(WIFI_1_SSID, WIFI_1_PASS, WIFI_1_CONF);
                #endif  
                break;   
            case 2:  
                #ifdef WIFI_2_SSID         
                    result = connectWiFi(WIFI_2_SSID, WIFI_2_PASS, WIFI_2_CONF);
                #endif   
                break;
            case 3:  
                #ifdef WIFI_3_SSID         
                    result = connectWiFi(WIFI_3_SSID, WIFI_3_PASS, WIFI_3_CONF);
                #endif   
                break;
            case 4:  
                #ifdef WIFI_4_SSID         
                    result = connectWiFi(WIFI_4_SSID, WIFI_4_PASS, WIFI_4_CONF);
                #endif   
                break;
            case 5:  
                #ifdef WIFI_5_SSID         
                    result = connectWiFi(WIFI_5_SSID, WIFI_5_PASS, WIFI_5_CONF);
                #endif   
                break;
            case 6:  
                #ifdef WIFI_6_SSID         
                    result = connectWiFi(WIFI_6_SSID, WIFI_6_PASS, WIFI_6_CONF);
                #endif   
                break;
            case 7:  
                #ifdef WIFI_7_SSID         
                    result = connectWiFi(WIFI_7_SSID, WIFI_7_PASS, WIFI_7_CONF);
                #endif   
                break;
        }
        if (result == WIFI_RESULT_FAIL) {
            //Serial.println("\nWiFi " + String(currWiFi) + " failed..." );
            currWiFi++;
            if (currWiFi >= 8)
                currWiFi = 0;
        }
    } while (result == WIFI_RESULT_FAIL);        
    //Serial.println("\nWiFi " + String(currWiFi) + " connected!" );
    
    if (currWiFi != prevWiFi) {
        //Serial.println("Saved settings to EEPROM.");     
        prevWiFi = currWiFi;
        EEPROM.begin(512);
        EEPROM.write(EEPROM_SAVED, 1);
        EEPROM.write(EEPROM_WIFI,  prevWiFi);  
        EEPROM.end();
    }
}



/* Utility Functions */
int connectWiFi(char* ssid, char* pass, char* conf) {
    if (conf != "DHCP") {
        String address;
        
        IPAddress ipLocal =   getIPFromString(conf, 0);
        IPAddress ipGateway = getIPFromString(conf, 1);
        IPAddress ipSubnet =  getIPFromString(conf, 2);
        IPAddress ipDNS1 =    getIPFromString(conf, 3);
        IPAddress ipDNS2 =    getIPFromString(conf, 4);

        WiFi.config(ipLocal, ipGateway, ipSubnet, ipDNS1, ipDNS2);
    }
    WiFi.begin(ssid, pass);

    int attempt = 0;
    while (WiFi.status() != WL_CONNECTED && attempt < (WIFI_TIMEOUT * 1000) / WIFI_ATTEMPT_TIME) {        
        delay(WIFI_ATTEMPT_TIME);
        //Serial.print(".");
        attempt++;
    }
    if (WiFi.status() == WL_CONNECTED) {
        //Serial.println("\nConnected to WiFi: '" + (String) ssid + "' With IP: " + WiFi.localIP()); 
        return WIFI_RESULT_DONE;
    }
    else {
        //Serial.println("\nWiFi connection timed out."); 
        return WIFI_RESULT_FAIL;
    }
}

void flashStatusLED(int type) {
    if (LED_STATUS_ENABLE) {
        if (type == FLASH_TYPE_WAKE) {
            analogWrite(PIN_LED, PWMRANGE - LED_STATUS_FLASH_BRIGHTNESS);   
            delay(FLASH_TIME_SHORT);              
            analogWrite(PIN_LED, PWMRANGE - LED_STATUS_IDLE_BRIGHTNESS);  
        }
        else if (type == FLASH_TYPE_DONE) {
            analogWrite(PIN_LED, PWMRANGE - LED_STATUS_FLASH_BRIGHTNESS);   
            delay(FLASH_TIME_SHORT);              
            analogWrite(PIN_LED, PWMRANGE - LED_STATUS_IDLE_BRIGHTNESS);  
            delay(FLASH_TIME_SHORT);       
        }
        else if (type == FLASH_TYPE_FAIL) {
            analogWrite(PIN_LED, PWMRANGE - LED_STATUS_FLASH_BRIGHTNESS);   
            delay(FLASH_TIME_LONG);              
            analogWrite(PIN_LED, PWMRANGE - LED_STATUS_IDLE_BRIGHTNESS);  
            delay(FLASH_TIME_SHORT);       
        }
    }
}



/* Processing Functions */
void processSensors() {
    int result;

    #ifdef VARIPASS_ID_TEMPERATURE    
        float temp = SHT21.getTemperature();
        
        varipassWriteFloat(VARIPASS_KEY, VARIPASS_ID_TEMPERATURE, temp, &result);
        
        if (result == VARIPASS_RESULT_SUCCESS)
            flashStatusLED(FLASH_TYPE_DONE);
        else
            flashStatusLED(FLASH_TYPE_FAIL);            
    #endif
    
    #ifdef VARIPASS_ID_HUMIDITY
        float humi = SHT21.getHumidity();
    
        varipassWriteFloat(VARIPASS_KEY, VARIPASS_ID_HUMIDITY,    humi, &result);
        
        if (result == VARIPASS_RESULT_SUCCESS)
            flashStatusLED(FLASH_TYPE_DONE);
        else
            flashStatusLED(FLASH_TYPE_FAIL);
    #endif
    
    #ifdef VARIPASS_ID_BATTERY
        float batt;
        if (BATTERY_VOLTAGE)
            batt = ((float)analogRead(PIN_BATTERY) / 1024) * BATTERY_MULTIPLIER; 
        else
            batt = ((((float)analogRead(PIN_BATTERY) / 1024) * BATTERY_MULTIPLIER - BATTERY_V_MIN) / (BATTERY_V_MAX - BATTERY_V_MIN)) * 100; 
        
        if (batt < 0)
            batt = 0;
        else if (batt > 100)
            batt = 100;
            
        varipassWriteFloat(VARIPASS_KEY, VARIPASS_ID_BATTERY,     batt, &result);
        
        if (result == VARIPASS_RESULT_SUCCESS)
            flashStatusLED(FLASH_TYPE_DONE);
        else
            flashStatusLED(FLASH_TYPE_FAIL);
    #endif
    
    //Serial.println(String(batt) + " V Battery, " + String(temp) + "°C, " + String(humi) + "% Humidity");
}



/* Standard Functions */
void setup() {
    setupLED();
    
    flashStatusLED(FLASH_TYPE_WAKE);
    
    Serial.begin(9600);
    setupSensor();
    setupSettings();
    setupWiFi();

    processSensors();

    if (DEEP_SLEEP) {
        ESP.deepSleep(MEASURE_INTERVAL * 1000000);
    }
}

void loop() {    
    if (!DEEP_SLEEP) {
        delay(MEASURE_INTERVAL * 1000);
          
        flashStatusLED(FLASH_TYPE_WAKE);
    
        processSensors();  
    }
}

