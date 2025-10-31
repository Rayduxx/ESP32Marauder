/* FLASH SETTINGS
Board: LOLIN D32
Flash Frequency: 80MHz
Partition Scheme: Minimal SPIFFS
https://www.online-utility.org/image/convert/to/XBM
*/

#include "configs.h"

#include <WiFi.h>
#include <stdio.h>
#include <Arduino.h>
#include <Wire.h>

#include "EvilPortal.h"
#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "Buffer.h"
#include "Assets.h"
#include "settings.h"
#include "CommandLine.h"
#include "lang_var.h"
#include "WiFiScan.h"

#ifndef HAS_SCREEN
#define MenuFunctions_h
#define Display_h
#include "Display.h"
#include "MenuFunctions.h"
#endif

#ifdef HAS_GPS
#include "GpsInterface.h"
#endif

#ifdef HAS_SD
#include "SDInterface.h"
#endif

#ifdef HAS_NEOPIXEL_LED
#include "LedInterface.h"
#endif

#ifdef HAS_BATTERY
#include "BatteryInterface.h"
#endif

WiFiScan wifi_scan_obj;
EvilPortal evil_portal_obj;
Buffer buffer_obj;
Settings settings_obj;
CommandLine cli_obj;
LedInterface led_obj;

#ifdef HAS_GPS
GpsInterface gps_obj;
#endif

#ifdef HAS_BATTERY
BatteryInterface battery_obj;
#endif

#ifdef HAS_SCREEN
Display display_obj;
MenuFunctions menu_function_obj;
#endif

#if defined(HAS_SD) && !defined(HAS_C5_SD)
SDInterface sd_obj;
#endif

const String PROGMEM version_number = MARAUDER_VERSION;

#ifdef HAS_NEOPIXEL_LED
Adafruit_NeoPixel strip = Adafruit_NeoPixel(Pixels, PIN, NEO_GRB + NEO_KHZ800);
#endif

uint32_t currentTime = 0;

void backlightOn()
{
#ifndef MARAUDER_MINI
  digitalWrite(TFT_BL, HIGH);
#endif
}

void backlightOff()
{
#ifndef MARAUDER_MINI
  digitalWrite(TFT_BL, LOW);
#endif
}

void setup()
{
#ifndef HAS_DUAL_BAND
  esp_spiram_init();
#endif

  pinMode(TFT_BL, OUTPUT);
  backlightOff();

#if BATTERY_ANALOG_ON == 1
  pinMode(BATTERY_PIN, OUTPUT);
  pinMode(CHARGING_PIN, INPUT);
#endif

  // Preset SPI CS pins to avoid bus conflicts
  digitalWrite(TFT_CS, HIGH);

#if defined(HAS_SD) && !defined(HAS_C5_SD)
  pinMode(SD_CS, OUTPUT);
  delay(10);
  digitalWrite(SD_CS, HIGH);
  delay(10);
#endif

  Serial.begin(115200);

  while (!Serial)
    delay(10);

  Serial.println("ESP-IDF version is: " + String(esp_get_idf_version()));

  display_obj.RunSetup();
  display_obj.tft.setTextColor(TFT_WHITE, TFT_BLACK);

  backlightOff();

  display_obj.tft.setTextColor(TFT_WHITE, TFT_BLACK);

#ifndef MARAUDER_CARDPUTER
  display_obj.tft.drawCentreString("ESP32 Marauder", TFT_WIDTH / 2, TFT_HEIGHT * 0.33, 1);
  display_obj.tft.drawCentreString("Raydux", TFT_WIDTH / 2, TFT_HEIGHT * 0.5, 1);
  display_obj.tft.drawCentreString(display_obj.version_number, TFT_WIDTH / 2, TFT_HEIGHT * 0.66, 1);

// Check if Sid.png exists on SD card and display it (if not, just show version)
#ifdef HAS_SD
  if (sd_obj.supported)
  {
    File sidFile = sd_obj.getFile("/Sid.png");
    if (sidFile)
    {
      display_obj.tft.drawCentreString("Sid.png found!", TFT_WIDTH / 2, TFT_HEIGHT * 0.82, 1);
      sidFile.close();
    }
  }
#endif
#else
  display_obj.tft.drawCentreString("ESP32 Marauder", TFT_HEIGHT / 2, TFT_WIDTH * 0.33, 1);
  display_obj.tft.drawCentreString("Raydux", TFT_HEIGHT / 2, TFT_WIDTH * 0.5, 1);
  display_obj.tft.drawCentreString(display_obj.version_number, TFT_HEIGHT / 2, TFT_WIDTH * 0.66, 1);

#ifdef HAS_SD
  if (sd_obj.supported)
  {
    File sidFile = sd_obj.getFile("/Sid.png");
    if (sidFile)
    {
      display_obj.tft.drawCentreString("Sid.png found!", TFT_HEIGHT / 2, TFT_WIDTH * 0.82, 1);
      sidFile.close();
    }
  }
#endif
#endif

  backlightOn(); // Need this
  settings_obj.begin();
  buffer_obj = Buffer();
#if defined(HAS_SD)
  // Do some SD stuff
  if (!sd_obj.initSD())
    Serial.println(F("SD Card NOT Supported"));
#endif

  wifi_scan_obj.RunSetup();

  display_obj.tft.setTextColor(TFT_GREEN, TFT_BLACK);
  display_obj.tft.drawCentreString("Initializing...", TFT_WIDTH / 2, TFT_HEIGHT * 0.82, 1);

  evil_portal_obj.setup();

#ifdef HAS_BATTERY
  battery_obj.RunSetup();
  battery_obj.battery_level = battery_obj.getBatteryLevel();
#endif

  led_obj.RunSetup();

#ifdef HAS_GPS
  gps_obj.begin();
#endif

  display_obj.tft.setTextColor(TFT_WHITE, TFT_BLACK);
  menu_function_obj.RunSetup();

  wifi_scan_obj.StartScan(WIFI_SCAN_OFF);
  Serial.println(F("CLI Ready"));
  cli_obj.RunSetup();
}

void loop()
{
  currentTime = millis();
  bool mini = false;

#ifdef SCREEN_BUFFER
#ifndef HAS_ILI9341
  mini = true;
#endif
#endif

  // Update all of our objects
  cli_obj.main(currentTime);
  display_obj.main(wifi_scan_obj.currentScanMode);
  wifi_scan_obj.main(currentTime);

#ifdef HAS_GPS
  gps_obj.main();
#endif

// Detect SD card
#if defined(HAS_SD)
  sd_obj.main();
#endif

  // Save buffer to SD and/or serial
  buffer_obj.save();

#ifdef HAS_BATTERY
  battery_obj.main(currentTime);
#endif

  settings_obj.main(currentTime);
  if (((wifi_scan_obj.currentScanMode != WIFI_PACKET_MONITOR) && (wifi_scan_obj.currentScanMode != WIFI_SCAN_EAPOL)) || (mini))
  {
    menu_function_obj.main(currentTime);
  }

  led_obj.main(currentTime);
  delay(1);
  delay(50);
}
