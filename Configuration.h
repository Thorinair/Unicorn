/*
 * The main configuration file.
*/

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
#define MEASURE_INTERVAL 120



/********
 * Status LED Configuration
 *******/

/* Status LED. This LED flashes to indicate the status of the device. Single short flash indicates that the device has woken up. 
 * Afterwards, flashes indicate the result of uploading the data. A short flash indicates a success, while a long flash indicates failure. 
 * Additionally, the LED mey glow passively to indicate that the device is on. */
#define LED_STATUS_ENABLE true

/* Brightness of the status LED while in idle. Can range from 0 to 1024, but setting it to 0 is suggested.
 * NOTE: If deep sleep is enabled, the LED will not glow during the sleeping phase. */
#define LED_STATUS_IDLE_BRIGHTNESS  2

/* Brightness of the status LED while during a flash. Can range from 0 to 1024. */
#define LED_STATUS_FLASH_BRIGHTNESS 250



/********
 * Battery Status
 *******/

/* Use voltage instead of percentage. */
#define BATTERY_VOLTAGE false

/* Voltage conversion multiplier. Should be adjusted so that the reported voltage is same as measured on battery. 
 * You will need to enable battery voltage above to tune this.
 * Use the following formula:  NewMultiplier = OldMultiplier * (MeasuredVoltage / ReportedVoltage) */
#define BATTERY_MULTIPLIER 5.622
 
/* Battery voltage when empty. */
#define BATTERY_V_MIN 2.56

/* Battery voltage when full. */
#define BATTERY_V_MAX 4.13

