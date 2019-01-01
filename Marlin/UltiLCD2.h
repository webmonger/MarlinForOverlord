#ifndef ULTI_LCD2_H
#define ULTI_LCD2_H

#include "Configuration.h"

#ifdef ENABLE_ULTILCD2
#include "UltiLCD2_low_lib.h"
#include "stepper.h"

void lcd_init();
void lcd_update();
FORCE_INLINE void lcd_setstatus(const char* message) {}
void lcd_buttons_update();
FORCE_INLINE void lcd_reset_alert_level() {}
FORCE_INLINE void lcd_buzz(long duration,uint16_t freq) {}

#define LCD_MESSAGEPGM(x)
#define LCD_ALERTMESSAGEPGM(x)

extern unsigned long lastSerialCommandTime;
extern uint8_t led_brightness_level;
extern uint8_t led_mode;
#define LED_MODE_ALWAYS_ON      0
#define LED_MODE_ALWAYS_OFF     1
#define LED_MODE_WHILE_PRINTING 2
#define LED_MODE_BLINK_ON_DONE  3

void lcd_menu_main();
void lcd_menu_power_check();
void lcd_menu_bluetooth();

void doCooldown();//TODO


#endif

#endif//ULTI_LCD2_H
