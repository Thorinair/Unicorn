#include <Wire.h>
#include <SHT21.h>
#include <TwiFi.h>
#include <VariPass.h>

#include "Configuration.h"
#include "ConfigurationWiFi.h"
#include "ConfigurationVariPass.h"

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

/* Setup Functions */
void setupLED();
void setupSensor();

/* Utility Functions */
void flashStatusLED(int type);

/* Processing Functions */
void processSensors();

void connectAttempt(int idEntry, int attempt);
void connectSuccess(int idEntry);
void connectFail(int idEntry);



/* Variables */
SHT21 SHT21;
bool wifiConnected;



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



/* Utility Functions */
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
    if (wifiConnected) {
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
}



void connectAttempt(int idEntry, int attempt) {

}

void connectSuccess(int idEntry) {

}

void connectFail(int idEntry) {

}

void setup() {
    if (ENABLE_SERIAL)
        Serial.begin(115200);

    setupLED();
    
    flashStatusLED(FLASH_TYPE_WAKE);
    
    setupSensor();

    twifiInit(
        wifis,
        WIFI_COUNT,
        WIFI_HOST,
        WIFI_TIMEOUT,
        &connectAttempt,
        &connectSuccess,
        &connectFail,
        WIFI_DEBUG
        );
    wifiConnected = twifiConnect(false);

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

