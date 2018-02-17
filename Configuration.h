/*
 * The main configuration file.
*/

/*******
 * WiFi Configuration
 *******/

/* Name of the Unicorn device, as visible on the WiFi network. */
#define WIFI_HOST "Unicorn_Example"

/* WiFi network definitions. Unicorn will attempt to connect to them in the order as defined here.
 * The first one that works will become the default one once successfully connected.
 * Up to 8 WiFi networks can be defined. Unused ones should be commented out. 
 * Second WiFi here is an example of a static IP. Use the | symbol to separate values:
 *   ip|gateway|subnet|dns1|dns2
 */
#define WIFI_0_SSID "ExampleWiFiName"
#define WIFI_0_PASS "password"
#define WIFI_0_CONF "DHCP"

#define WIFI_1_SSID "AnotherWiFi"
#define WIFI_1_PASS "password"
#define WIFI_1_CONF "192.168.0.201|192.168.0.1|255.255.255.0|8.8.8.8|8.8.4.4"

//#define WIFI_2_SSID ""
//#define WIFI_2_PASS ""
//#define WIFI_2_CONF ""

//#define WIFI_3_SSID ""
//#define WIFI_3_PASS ""
//#define WIFI_3_CONF ""

//#define WIFI_4_SSID ""
//#define WIFI_4_PASS ""
//#define WIFI_4_CONF ""

//#define WIFI_5_SSID ""
//#define WIFI_5_PASS ""
//#define WIFI_5_CONF ""

//#define WIFI_6_SSID ""
//#define WIFI_6_PASS ""
//#define WIFI_6_CONF ""

//#define WIFI_7_SSID ""
//#define WIFI_7_PASS ""
//#define WIFI_7_CONF ""

/* WiFi Timeout, time in seconds until WiFi connection is cancelled if unavailable. */
#define WIFI_TIMEOUT 10



/********
 * VariPass Variables
 *******/

/* VariPass account key. You can obtain this on the varipass.org website. */
#define VARIPASS_KEY "apikey"

/* VariPass variable IDs. To disable sending of a specific variable, comment it out. */
#define VARIPASS_ID_TEMPERATURE "id1"
#define VARIPASS_ID_HUMIDITY    "id2"
#define VARIPASS_ID_BATTERY     "id3"



/********
 * Intervals and optimization
 *******/

/* Enables deep sleep. Unicorn will go in a deep sleep mode between measurements, massively prolonging battery duration. 
 * NOTE: If enabled, the reset button needs to be pressed on the board during re-flashing.*/
#define DEEP_SLEEP true

/* Measuring interval, also acts as deep sleep duration if enabled. Defined in seconds. */
#define MEASURE_INTERVAL 60



/********
 * Status LED Configuration
 *******/

/* Status LED. This LED flashes to indicate the status of the device. Single short flash indicates that the device has woken up. 
 * Afterwards, three short flashes indicate successful upload of data, while one long flash indicates failure. 
 * Additionally, the LED mey glow passively to indicate that the device is on. */
#define LED_STATUS_ENABLE true

/* Brightness of the status LED while in idle. Can range from 0 to 1024, but setting it to 0 is suggested.
 * NOTE: If deep sleep is enabled, the LED will not glow during the sleeping phase. */
#define LED_STATUS_IDLE_BRIGHTNESS  2

/* Brightness of the status LED while during a flash. Can range from 0 to 1024. */
#define LED_STATUS_FLASH_BRIGHTNESS 250