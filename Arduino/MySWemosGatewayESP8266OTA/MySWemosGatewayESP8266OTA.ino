/**
   The MySensors Arduino library handles the wireless radio link and protocol
   between your home built sensors/actuators and HA controller of choice.
   The sensors forms a self healing radio network with optional repeaters. Each
   repeater and gateway builds a routing tables in EEPROM which keeps track of the
   network topology allowing messages to be routed to nodes.

   Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
   Copyright (C) 2013-2015 Sensnology AB
   Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors

   Documentation: http://www.mysensors.org
   Support Forum: http://forum.mysensors.org

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   version 2 as published by the Free Software Foundation.

 *******************************

   Make sure to fill in your ssid and WiFi password below for ssid & pass.
   If you decide to enable ESP8266 OTA update it is recommended to set a password
   Hardware SHA204 signing is probably not supported for ESP8266 gateways but may work if used as a node (untested)
*/

// ** WiFi and network configuration **
#define MY_ESP8266_SSID "MySSID"
#define MY_ESP8266_PASSWORD "MyVerySecretPassword"

// Set the hostname for the WiFi Client. This is the hostname
// it will pass to the DHCP server if not static.
#define MY_ESP8266_HOSTNAME "MySWeMosGWShield"

// Enable UDP communication
//#define MY_USE_UDP

// Enable MY_IP_ADDRESS here if you want a static ip address (no DHCP)
//#define MY_IP_ADDRESS 192,168,178,87

// If using static ip you need to define Gateway and Subnet address as well
#define MY_IP_GATEWAY_ADDRESS 192,168,178,1
#define MY_IP_SUBNET_ADDRESS 255,255,255,0

// The port to keep open on node server mode
#define MY_PORT 5003

// How many clients should be able to connect to this gateway (default 1)
#define MY_GATEWAY_MAX_CLIENTS 2

// Controller ip address. Enables client mode (default is "server" mode).
// Also enable this if MY_USE_UDP is used and you want sensor data sent somewhere.
//#define MY_CONTROLLER_IP_ADDRESS 192, 168, 178, 68


// ** OTA updates configuration **
// Enable OTA updates for ESP8266 based gateway
#define ESP8266_OTA

// Define password for OTA updates (recommended but optional)
//#define ESP8266_OTA_PASSWORD "MySuperSecretOTAPassword"


// ** MySensors Radio configuration **
// Enables and select radio type (if attached)
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

// RF24 settings
// Use custom RF24 channel (default 76)
//#define MY_RF24_CHANNEL 42
// Decrease RF24 power transmission, useful to test in case of Tx problems.
// If your problem is fixed consider adding the 5V to 3.3V voltage regulator
//#define MY_RF24_PA_LEVEL RF24_PA_LOW
// Enables RF24 encryption (all nodes and gateway must have this enabled, and all must be personalized with the same AES key)
//#define MY_RF24_ENABLE_ENCRYPTION

// RF69 settings
// RFM69 Frequency, default = 868MHz
#define MY_RFM69_FREQUENCY RF69_868MHZ // RFM69 frequency to use (RF69_433MHZ for 433MHz, RF69_868MHZ for 868MHz or RF69_915MHZ for 915MHz)
// RFM69 Network ID. Use the same for all nodes that will talk to each other, default = 100
//#define MY_RFM69_NETWORKID     42
// Enables RFM69 encryption (all nodes and gateway must have this enabled, and all must be personalized with the same AES key)
//#define MY_RFM69_ENABLE_ENCRYPTION
// Disable this if you're not running the RFM69HW model (RFM69HW is recommended on the gateway for better coverage)
#define MY_IS_RFM69HW


// ** Mysensors additional functions **
// Enable inclusion mode
#define MY_INCLUSION_MODE_FEATURE

// Enable Inclusion mode button on gateway
#define MY_INCLUSION_BUTTON_FEATURE
// Set inclusion mode duration (in seconds)
#define MY_INCLUSION_MODE_DURATION 60
// Digital pin used for inclusion mode button
#define MY_INCLUSION_MODE_BUTTON_PIN  D3  // GPIO 0

// Software signing settings
//#define MY_SIGNING_SOFT
#define MY_SIGNING_SOFT_RANDOMSEED_PIN A0

// Hardware signing settings (currently unsupported?)
//#define MY_SIGNING_ATSHA204
#define MY_SIGNING_ATSHA204_PIN D0  // GPIO 16

// General signing settings
// Enable this if you want destination node to sign all messages sent to this gateway.
//#define MY_SIGNING_REQUEST_SIGNATURES
// Enable node whitelisting
//#define MY_SIGNING_NODE_WHITELISTING {{.nodeId = GATEWAY_ADDRESS,.serial = {0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01}}}

// Flash leds on rx/tx/err
// Set blinking period
#define MY_DEFAULT_LED_BLINK_PERIOD 300

// Led pins used if blinking feature is enabled above
// LED_BUILTIN, the on board LED is used (D4, GPIO 2)
#define MY_DEFAULT_ERR_LED_PIN LED_BUILTIN // Error led pin
#define MY_DEFAULT_RX_LED_PIN  LED_BUILTIN // Receive led pin
#define MY_DEFAULT_TX_LED_PIN  LED_BUILTIN // Transmit led pin

// Enable debug prints to serial monitor
//#define MY_DEBUG


// **************************************//
// You probably don't need to edit below //
// **************************************//

#include <EEPROM.h>
#include <SPI.h>
#if defined ESP8266_OTA
#include <ArduinoOTA.h>
#endif
#if defined(MY_USE_UDP)
#include <WiFiUDP.h>
#else
#include <ESP8266WiFi.h>
#endif

// Set the gateway type to ESP8266
#define MY_GATEWAY_ESP8266

// Disable serial and allow TX and RX to be used as GPIO when debug is disabled
#ifndef MY_DEBUG
#define MY_DISABLED_SERIAL
#endif

// Enable signing related debug prints to serial monitor if debug and signing are both enabled
#if ((defined MY_DEBUG) && ( defined MY_SIGNING_SOFT || defined MY_SIGNING_ATSHA204))
#define MY_DEBUG_VERBOSE_SIGNING
#endif

// Define RFM69 pins to match PCB traces
#if defined MY_RADIO_RFM69
#define MY_RF69_IRQ_PIN D2  // GPIO 4
#define MY_RF69_IRQ_NUM D2  // GPIO 4
#define MY_RF69_SPI_CS  D8  // GPIO 15
#endif

#define ARDUINO_ARCH_ESP8266

#include <MySensors.h>


void setup() {
  // Executed once at boot
#if defined ESP8266_OTA
#if defined MY_DEBUG
  Serial.println("");
  Serial.println("ESP8266 OTA enabled");
#endif

#if defined ESP8266_OTA_PASSWORD
#if defined MY_DEBUG
  Serial.println("OTA Password set");
#endif
  ArduinoOTA.setPassword(ESP8266_OTA_PASSWORD);
#endif

  ArduinoOTA.setHostname(MY_ESP8266_HOSTNAME);
  ArduinoOTA.onStart([]() {
    Serial.println("ArduinoOTA start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nArduinoOTA end");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("OTA Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
#endif

#if defined MY_DEBUG
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Not connected to WiFi");
  } else {
    Serial.println("");
    Serial.println("-- STA (client) informations --");
    Serial.print("Hostname: ");
    Serial.println(WiFi.hostname());
    Serial.print("Connected to: ");
    Serial.print(WiFi.SSID());
    Serial.print(" (");
    Serial.print(WiFi.BSSIDstr());
    Serial.print("), ");
    Serial.print(WiFi.RSSI());
    Serial.println("dBm");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Gateway address: ");
    Serial.println(WiFi.gatewayIP());
    Serial.print("Subnet mask: ");
    Serial.println(WiFi.subnetMask());
    Serial.print("MAC STA address: ");
    Serial.println(WiFi.macAddress());
    Serial.println("-- AP (server) informations --");
    Serial.print("Soft IP address: ");
    Serial.println(WiFi.softAPIP());
    Serial.print("MAC AP address: ");
    Serial.println(WiFi.softAPmacAddress());
    Serial.println("----");
    Serial.println("");
  }
#endif
}


void presentation() {
  // Present locally attached sensors here
}


void loop() {
  // Send locally attech sensors data here
#if defined ESP8266_OTA
  ArduinoOTA.handle();
#endif
}
