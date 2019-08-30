/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#ifndef TARGET_STM32F1
  #error "Oops! Select an STM32F1 board in 'Tools > Board.'"
#endif

#define BOARD_INFO_NAME "BIGTREE SKR Mini E3"

// Release PB3/PB4 (E0 STP/DIR) from JTAG pins
#define DISABLE_JTAG

// Ignore temp readings during development.
//#define BOGUS_TEMPERATURE_GRACE_PERIOD 2000

//
// Servos
//
#define SERVO0_PIN         PA1

//
// Limit Switches
//
#define X_STOP_PIN         PC0
#define Y_STOP_PIN         PC1
#define Z_STOP_PIN         PC14 // PC2

//
// Z Probe must be this pins
//
#define Z_MIN_PROBE_PIN    PC14

//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN   PC15
#endif

//
// Steppers
//
#define X_ENABLE_PIN       PB14
#define X_STEP_PIN         PB13
#define X_DIR_PIN          PB12

#define Y_ENABLE_PIN       PB11
#define Y_STEP_PIN         PB10
#define Y_DIR_PIN          PB2

#define Z_ENABLE_PIN       PB1
#define Z_STEP_PIN         PB0
#define Z_DIR_PIN          PC5

#define E0_ENABLE_PIN      PD2
#define E0_STEP_PIN        PB3
#define E0_DIR_PIN         PB4

#if HAS_DRIVER(TMC2209)
  /**
   * TMC2209 stepper drivers
   * Hardware serial communication ports.
   */
  #define X_HARDWARE_SERIAL  Serial4
  #define Y_HARDWARE_SERIAL  Serial4
  #define Z_HARDWARE_SERIAL  Serial4
  #define E0_HARDWARE_SERIAL Serial4
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN         PA0   // Analog Input
#define TEMP_BED_PIN       PC3   // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN       PC8   // EXTRUDER
#define HEATER_BED_PIN     PC9   // BED
#define FAN_PIN            PA8

//
// USB connect control
//
#define USB_CONNECT_PIN    PC13
#define USB_CONNECT_INVERTING false

#define SD_DETECT_PIN      PC4

#undef LCD_SDSS
#undef LED_PIN
#undef LCD_PINS_RS
#undef LCD_PINS_ENABLE
#undef LCD_PINS_D4
#undef LCD_PINS_D5
#undef LCD_PINS_D6
#undef LCD_PINS_D7
// #undef FIL_RUNOUT_PIN  

/**
 *                 _____
 *             5V | · · | GND
 *  (LCD_EN) PB7  | · · | PB8  (LCD_RS)
 *  (LCD_D4) PB9  | · ·[  PA10 (BTN_EN2)
 *          RESET | · · | PA9  (BTN_EN1)
 * (BTN_ENC) PB6  | · · | PB5  (BEEPER)
 *                 -----
 *                 EXP1
 */


#if HAS_SPI_LCD
  #define BEEPER_PIN      PB5

  // The encoder is validated to work with the Ender2 LCD and the Ender3 LCD
  #define BTN_ENC         PB6
  #define BTN_EN1         PA9
  #define BTN_EN2         PA10

  #if ENABLED(CR10_STOCKDISPLAY)
    #define LCD_PINS_RS    PB8

    #define LCD_PINS_ENABLE PB7
    #define LCD_PINS_D4     PB9
  #elif ENABLED(MINIPANEL)
    // #define DOGLCD_CS       PB8
    // #define DOGLCD_A0       PB9  
    
    #define DOGLCD_CS       PB8
    #define DOGLCD_MOSI     PB7
    #define DOGLCD_SCK      PB5
    #define DOGLCD_A0       PB9
    
    #define LCD_SCREEN_ROT_0

  #else
    #error "Only CR10_STOCKDISPLAY and MINIPANEL are currently supported on the BIGTREE_SKR_MINI_E3."
  #endif

#endif // HAS_SPI_LCD

// Alter timing for graphical display
#ifndef ST7920_DELAY_1
  #define ST7920_DELAY_1 DELAY_NS(125)
#endif
#ifndef ST7920_DELAY_2
  #define ST7920_DELAY_2 DELAY_NS(125)
#endif
#ifndef ST7920_DELAY_3
  #define ST7920_DELAY_3 DELAY_NS(125)
#endif


/* SBW: 27/8/19:  THIS CONFIGURATION WAS WORKING FOR THE ENCODER, BUT NOT THE LCD. 
                  Below is the copied code from the various pins_ headers, with the correct pin definitions where able
#if HAS_SPI_LCD
  #define BEEPER_PIN      PB5

  // The encoder is validated to work with the Ender2 LCD and the Ender3 LCD
  #define BTN_ENC         PB6
  #define BTN_EN1         PA9
  #define BTN_EN2         PA10

  #if ENABLED(CR10_STOCKDISPLAY)
    #define LCD_PINS_RS    PB8

    #define LCD_PINS_ENABLE PB7
    #define LCD_PINS_D4     PB9
  #elif ENABLED(MINIPANEL)
    #define DOGLCD_CS       PB8
    #define DOGLCD_A0       PB9  
    
    #define LCD_PINS_ENABLE PB7 
    #define LCD_PINS_RS     DOGLCD_CS 
    #define LCD_PINS_D4     DOGLCD_A0
    #define LCD_SDSS        SD_DETECT_PIN
    
    #define LCD_SCREEN_ROT_0

  #else
    #error "Only CR10_STOCKDISPLAY and MINIPANEL are currently supported on the BIGTREE_SKR_MINI_E3."
  #endif

#endif // HAS_SPI_LCD


// Alter timing for graphical display
#ifndef ST7920_DELAY_1
  #define ST7920_DELAY_1 DELAY_NS(125)
#endif
#ifndef ST7920_DELAY_2
  #define ST7920_DELAY_2 DELAY_NS(125)
#endif
#ifndef ST7920_DELAY_3
  #define ST7920_DELAY_3 DELAY_NS(125)
#endif
*/

#if 0
// pins_SANGUINOLOLU_11.h
//
// LCD / Controller
//
#if HAS_SPI_LCD

  #if HAS_GRAPHICAL_LCD

    #if ENABLED(U8GLIB_ST7920) // SPI GLCD 12864 ST7920 ( like [www.digole.com] ) For Melzi V2.0

      #if ENABLED(IS_MELZI)
        #define LCD_PINS_RS     PB9   // CS chip select /SS chip slave select
        #define LCD_PINS_ENABLE ??29   // SID (MOSI)
        #define LCD_PINS_D4     PB7   // SCK (CLK) clock
        // Pin 27 is taken by LED_PIN, but Melzi LED does nothing with
        // Marlin so this can be used for BEEPER_PIN. You can use this pin
        // with M42 instead of BEEPER_PIN.
        #define BEEPER_PIN      PB5
      #else        // Sanguinololu >=1.3
        #define LCD_PINS_RS      4
        #define LCD_PINS_ENABLE 17
        #define LCD_PINS_D4     30
        #define LCD_PINS_D5     29
        #define LCD_PINS_D6     28
        #define LCD_PINS_D7     27
      #endif

    #else // DOGM SPI LCD Support

      #define DOGLCD_A0         PB9

      #if ENABLED(MAKRPANEL)

        #define BEEPER_PIN      29
        #define DOGLCD_CS       17
        #define LCD_BACKLIGHT_PIN 28   // PA3

      #elif ENABLED(IS_MELZI)

        #define BEEPER_PIN      27
        #define DOGLCD_CS       28

      #else // !MAKRPANEL

        #define DOGLCD_CS       PB8

      #endif

    #endif

    // Uncomment screen orientation
    #define LCD_SCREEN_ROT_0
    //#define LCD_SCREEN_ROT_90
    //#define LCD_SCREEN_ROT_180
    //#define LCD_SCREEN_ROT_270

  #else // !HAS_GRAPHICAL_LCD

    #define LCD_PINS_RS          4
    #define LCD_PINS_ENABLE     17
    #define LCD_PINS_D4         30
    #define LCD_PINS_D5         29
    #define LCD_PINS_D6         28
    #define LCD_PINS_D7         27

  #endif // !HAS_GRAPHICAL_LCD

  #if ENABLED(LCD_I2C_PANELOLU2)

    #if ENABLED(IS_MELZI)
      #define BTN_ENC           29
      #define LCD_SDSS          30   // Panelolu2 SD card reader rather than the Melzi
    #else
      #define BTN_ENC           30
    #endif

  #elif ENABLED(LCD_FOR_MELZI)

    #define LCD_PINS_RS         17
    #define LCD_PINS_ENABLE     16
    #define LCD_PINS_D4         11
    #define BTN_ENC             28
    #define BTN_EN1             29
    #define BTN_EN2             30

    #ifndef ST7920_DELAY_1
      #define ST7920_DELAY_1 DELAY_NS(0)
    #endif
    #ifndef ST7920_DELAY_2
      #define ST7920_DELAY_2 DELAY_NS(188)
    #endif
    #ifndef ST7920_DELAY_3
      #define ST7920_DELAY_3 DELAY_NS(0)
    #endif

  #elif ENABLED(ZONESTAR_LCD) // For the Tronxy Melzi boards

    #define LCD_PINS_RS         28
    #define LCD_PINS_ENABLE     29
    #define LCD_PINS_D4         10
    #define LCD_PINS_D5         11
    #define LCD_PINS_D6         16
    #define LCD_PINS_D7         17
    #define ADC_KEYPAD_PIN       1

    #define BTN_EN1             -1
    #define BTN_EN2             -1

  #else  // !LCD_I2C_PANELOLU2 && !LCD_FOR_MELZI && !ZONESTAR_LCD

    #define BTN_ENC             PB6
    #define LCD_SDSS            PB8   // Smart Controller SD card reader rather than the Melzi

  #endif

  #if ENABLED(NEWPANEL) && !defined(BTN_EN1)
    #define BTN_EN1             PA9
    #define BTN_EN2             PA10
  #endif

  // #define SD_DETECT_PIN         -1

#endif // HAS_SPI_LCD

// pins_MELZI_CREALITY

// #define LCD_SDSS           SD_DETECT_PIN   // Smart Controller SD card reader (rather than the Melzi)
#define LCD_PINS_RS        PB8   // ST9720 CS
#define LCD_PINS_ENABLE    PB7   // ST9720 DAT
#define LCD_PINS_D4        PB9   // ST9720 CLK

// #if DISABLED(SPEAKER) && ENABLED(BLTOUCH)
//   #define SERVO0_PIN 29
// #endif

// Alter timing for graphical display
#ifndef ST7920_DELAY_1
  #define ST7920_DELAY_1 DELAY_NS(125)
#endif
#ifndef ST7920_DELAY_2
  #define ST7920_DELAY_2 DELAY_NS(125)
#endif
#ifndef ST7920_DELAY_3
  #define ST7920_DELAY_3 DELAY_NS(125)
#endif

#if ENABLED(MINIPANEL)
  #undef DOGLCD_CS
  #define DOGLCD_CS        LCD_PINS_RS
#endif
#endif // 0