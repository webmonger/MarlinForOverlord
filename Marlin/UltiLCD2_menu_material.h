#ifndef ULTI_LCD2_MENU_MATERIAL_H
#define ULTI_LCD2_MENU_MATERIAL_H

#include "ConfigurationStore.h"

struct materialSettings
{
    int16_t temperature;
    int16_t bed_temperature;
    uint8_t fan_speed; //0-100% of requested speed by GCode
    int16_t flow;      //Flow modification in %
    float diameter; //Filament diameter in mm
};

extern struct materialSettings material[EXTRUDERS];
extern bool isReload;
void lcd_material_clean_nozzle_option();

#define FILAMENT_REVERSAL_SPEED       50
#define FILAMENT_LONG_MOVE_ACCELERATION 30

#define FILAMENT_INSERT_SPEED         20     //Initial insert speed to grab the filament.
#define FILAMENT_INSERT_FAST_SPEED    50   //Speed during the forward length
#define FILAMENT_INSERT_EXTRUDE_SPEED 4     //Final speed when extruding

#define FILAMENT_INSERT_TIME 90
#define FILAMENT_HEATING_WAIT_TIME 30

#define EEPROM_MATERIAL_SETTINGS_MAX_COUNT 4
#define EEPROM_MATERIAL_SETTINGS_SIZE   (8 + 16)
#define EEPROM_MATERIAL_COUNT_OFFSET()            ((uint8_t*)(EEPROM_MATERIAL_SETTINGS_OFFSET + 0))
#define EEPROM_MATERIAL_NAME_OFFSET(n)            ((uint8_t*)(EEPROM_MATERIAL_SETTINGS_OFFSET + 1 + EEPROM_MATERIAL_SETTINGS_SIZE * uint16_t(n)))
#define EEPROM_MATERIAL_TEMPERATURE_OFFSET(n)     ((uint16_t*)(EEPROM_MATERIAL_SETTINGS_OFFSET + 1 + EEPROM_MATERIAL_SETTINGS_SIZE * uint16_t(n) + 8))
#define EEPROM_MATERIAL_BED_TEMPERATURE_OFFSET(n) ((uint16_t*)(EEPROM_MATERIAL_SETTINGS_OFFSET + 1 + EEPROM_MATERIAL_SETTINGS_SIZE * uint16_t(n) + 10))
#define EEPROM_MATERIAL_FAN_SPEED_OFFSET(n)       ((uint8_t*)(EEPROM_MATERIAL_SETTINGS_OFFSET + 1 + EEPROM_MATERIAL_SETTINGS_SIZE * uint16_t(n) + 12))
#define EEPROM_MATERIAL_FLOW_OFFSET(n)            ((uint16_t*)(EEPROM_MATERIAL_SETTINGS_OFFSET + 1 + EEPROM_MATERIAL_SETTINGS_SIZE * uint16_t(n) + 13))
#define EEPROM_MATERIAL_DIAMETER_OFFSET(n)        ((float*)(EEPROM_MATERIAL_SETTINGS_OFFSET + 1 + EEPROM_MATERIAL_SETTINGS_SIZE * uint16_t(n) + 15))

void lcd_menu_material();
void lcd_menu_insert_material();
bool lcd_material_verify_material_settings();
void lcd_material_reset_defaults();
void lcd_material_set_material(uint8_t nr, uint8_t e);
void lcd_material_store_material(uint8_t nr);
void lcd_material_read_current_material();
void lcd_material_store_current_material();
void lcd_menu_change_material_preheat();
void lcd_menu_material_select();



#endif//ULTI_LCD2_MENU_MATERIAL_H
