# Unicorn
Unicorn is an ESP8266 based high precision wireless thermometer and humidity meter. To achieve high accuracy, the sensor used is the SHT21. Once measured, the device uploads the data to variables hosted on [VariPass](https://varipass.org/). The whole device is designed to run off a single 18650 Li-Ion battery, and can potentially keep running for a month thanks to ESP's deep sleep functionality (Not yet tested however!). Please check the `Configuration.h` file for details on configuration and battery calibration.

## Features
* Measures temperature and humidity using an SHT21 sensor.
* Measures battery status as percentage or voltage.
* Can connect to up to 8 different WiFi networks and switch between them.
* Uploads data to [VariPass](https://varipass.org/).
* A status LED indicates the success of the upload process when enabled.
* Runs off a single 18650 Li-Ion battery.
* Supports ESP's deep sleep feature for prolonged battery life.
* Lots of configuration available in the `Configuration.h` file.

## Libraries Used
* [SHT21](https://github.com/markbeee/SHT21) by markbeee
* [VariPass-for-ESP8266](https://github.com/Thorinair/VariPass-for-ESP8266) by Thorinair
* [TwiFi](https://github.com/Thorinair/TwiFi) by Thorinair

## Electronics Used
* [Croduino Nova](https://github.com/e-radionicacom/Croduino-NOVA-Eagle-Files) - An ESP8266 board designed and manufactured by [e-radionica.com](https://e-radionica.com/en/)
* [SHT21 Breakout](https://github.com/e-radionicacom/SHT21-breakout-board-Eagle-files) - An SHT21 breakout designed and manufactured by [e-radionica.com](https://e-radionica.com/en/)