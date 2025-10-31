#pragma once

#ifndef configs_h

  #define configs_h
  #define POLISH_POTATO
  #define MARAUDER_V4                 // Board Target Selection
  #define MARAUDER_VERSION "v1.8.8"   // Software Version
  #define GRAPH_REFRESH 100           // Graph Refresh Rate in ms
  #define DUAL_BAND_CHANNELS 51       // Number of channels for dual band devices
  #define HARDWARE_NAME "Marauder v4" // Default Hardware Name
  #define MEM_LOWER_LIM 10000         // Default memory lower limit (values are in bytes)
  #define MAX_HTML_SIZE 11400         // Maximum size of HTML page for Evil Portal
  #define MARAUDER_TITLE_BYTES 13578  // Size of Marauder Title Image in bytes

  #ifdef HAS_NEOPIXEL_LED
    #define PIN 25                      // Default Neopixel Pin
  #endif

  /* Board Features */
  #define HAS_BT
  #define HAS_TOUCH
  #define HAS_FULL_SCREEN
  #define HAS_SCREEN
  #define HAS_SD
  #define USE_SD
  #define HAS_TEMP_SENSOR
  //#define HAS_NEOPIXEL_LED
  // #define HAS_GPS
  // #define HAS_BATTERY
  // #define HAS_PWR_MGMT
  // #define FLIPPER_ZERO_HAT

  /* Display Definitions */
  #define SCREEN_CHAR_WIDTH 40
  #define HAS_ILI9341
  #define BANNER_TEXT_SIZE 2
  //#define TFT_WIDTH 320
  //#define TFT_HEIGHT 480
  #define TFT_SHIELD
  #define GRAPH_VERT_LIM TFT_HEIGHT / 2
  #define EXT_BUTTON_WIDTH 20
  #define SCREEN_ORIENTATION 0
  #define CHAR_WIDTH 12
  #define SCREEN_WIDTH TFT_WIDTH
  #define SCREEN_HEIGHT TFT_HEIGHT
  #define HEIGHT_1 TFT_WIDTH
  #define WIDTH_1 TFT_HEIGHT
  #define STANDARD_FONT_CHAR_LIMIT (TFT_WIDTH / 6) // number of characters on a single line with normal font
  #define TEXT_HEIGHT 16                           // Height of text to be printed and scrolled
  #define BOT_FIXED_AREA 0                         // Number of lines in bottom fixed area (lines counted from bottom of screen)
  #define TOP_FIXED_AREA 48                        // Number of lines in top fixed area (lines counted from top of screen)
  #define YMAX 480                                 // Bottom of screen area
  #define minimum(a, b) (((a) < (b)) ? (a) : (b))
  // #define MENU_FONT NULL
  #define MENU_FONT &FreeMono9pt7b // Winner
  // #define MENU_FONT &FreeMonoBold9pt7b
  // #define MENU_FONT &FreeSans9pt7b
  // #define MENU_FONT &FreeSansBold9pt7b
  #define BUTTON_SCREEN_LIMIT 20
  #define BUTTON_ARRAY_LEN BUTTON_SCREEN_LIMIT
  #define STATUS_BAR_WIDTH 16
  #define LVGL_TICK_PERIOD 6

  #define FRAME_X 100
  #define FRAME_Y 64
  #define FRAME_W 120
  #define FRAME_H 50

  #define REDBUTTON_X FRAME_X                              // Red zone size
  #define REDBUTTON_Y FRAME_Y
  #define REDBUTTON_W (FRAME_W / 2)
  #define REDBUTTON_H FRAME_H

  #define GREENBUTTON_X (REDBUTTON_X + REDBUTTON_W)       // Green zone size
  #define GREENBUTTON_Y FRAME_Y
  #define GREENBUTTON_W (FRAME_W / 2)
  #define GREENBUTTON_H FRAME_H

  #define STATUSBAR_COLOR 0x4A49
  #define KIT_LED_BUILTIN 13

  /* Menu Definitions */
  #define BANNER_TIME 100
  #define COMMAND_PREFIX "!"
  #define KEY_X 120 // Centre of key
  #define KEY_Y 50
  #define KEY_W 240 // Width and height
  #define KEY_H 22
  #define KEY_SPACING_X 0 // X and Y gap
  #define KEY_SPACING_Y 1
  #define KEY_TEXTSIZE 1 // Font size multiplier
  #define ICON_W 22
  #define ICON_H 22
  #define BUTTON_PADDING 22
  //#define BUTTON_ARRAY_LEN 5

  /* SD Definitions */
  #if defined(USE_SD)
    #define SD_CS 5
  #endif

  /* Power Management */
  #ifdef HAS_PWR_MGMT
    #if defined(MARAUDER_M5STICKC) || defined(MARAUDER_M5STICKCP2)
      #include "AXP192.h"
    #endif
    #ifdef MARAUDER_M5STICKCP2
      #define POWER_HOLD_PIN 4 // Prevent StickCP2 from turning off when disconnect USB cable
    #endif
  #endif

  /* Space Saving Colors */
  #define TFTWHITE 1
  #define TFTCYAN 2
  #define TFTBLUE 3
  #define TFTRED 4
  #define TFTGREEN 5
  #define TFTGREY 6
  #define TFTGRAY 7
  #define TFTMAGENTA 8
  #define TFTVIOLET 9
  #define TFTORANGE 10
  #define TFTYELLOW 11
  #define TFTLIGHTGREY 12
  #define TFTPURPLE 13
  #define TFTNAVY 14
  #define TFTSILVER 15
  #define TFTDARKGREY 16
  #define TFTSKYBLUE 17
  #define TFTLIME 18
  #define TFT_FARTGRAY 0x528a

  /* GPS Things */
  #ifdef HAS_GPS
    #define mac_history_len 100
    #define GPS_SERIAL_INDEX 2
    #define GPS_TX 4
    #define GPS_RX 13
  #else
    #define mac_history_len 100
  #endif

  /* Battery Stuffs */
  #ifdef HAS_BATTERY
    #define I2C_SDA 33
    #define I2C_SCL 22
  #endif

  /* PCAP Buffer Stuffs */
  #define BUF_SIZE 3 * 1024 // Had to reduce buffer size to save RAM. GG @spacehuhn
  #define SNAP_LEN 2324     // max len of each recieved packet

  /* Wifi Stuffs */
  #ifndef HAS_DUAL_BAND
    #define HOP_DELAY 1000
  #else
    #define HOP_DELAY 250
  #endif

#endif
