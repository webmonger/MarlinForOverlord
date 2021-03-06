#include "Marlin.h"
#include "planner.h"
#include "temperature.h"
#include "ultralcd.h"
#include "UltiLCD2.h"
#include "ConfigurationStore.h"
#include "UltiLCD2_menu_print.h"
#include "UltiLCD2_menu_material.h"
#include "UltiLCD2_menu_maintenance.h"

#ifdef SoftwareAutoLevel
#include "fitting_bed.h"
#endif

void _EEPROM_writeData(int &pos, uint8_t* value, uint16_t size)
{
    do
    {
        eeprom_write_byte((unsigned char*)pos, *value);
        pos++;
        value++;
    }while(--size);
}
void _EEPROM_readData(int &pos, uint8_t* value, uint16_t size)
{
    do
    {
        *value = eeprom_read_byte((unsigned char*)pos);
        pos++;
        value++;
    }while(--size);
}
//======================================================================================






// IMPORTANT:  Whenever there are changes made to the variables stored in EEPROM
// in the functions below, also increment the version number. This makes sure that
// the default values are used whenever there is a change to the data, to prevent
// wrong data being written to the variables.
// ALSO:  always make sure the variables in the Store and retrieve sections are in the same order.
#define EEPROM_VERSION "V20"


#ifdef EEPROM_SETTINGS
void Config_StoreSettings()
{
  char ver[4]= "000";
  int i=EEPROM_OFFSET;
  EEPROM_WRITE_VAR(i,ver); // invalidate data first
  EEPROM_WRITE_VAR(i,axis_steps_per_unit);
  EEPROM_WRITE_VAR(i,max_feedrate);
  EEPROM_WRITE_VAR(i,max_acceleration_units_per_sq_second);
  EEPROM_WRITE_VAR(i,acceleration);
  EEPROM_WRITE_VAR(i,retract_acceleration);
  EEPROM_WRITE_VAR(i,minimumfeedrate);
  EEPROM_WRITE_VAR(i,mintravelfeedrate);
  EEPROM_WRITE_VAR(i,minsegmenttime);
  EEPROM_WRITE_VAR(i,max_xy_jerk);
  EEPROM_WRITE_VAR(i,max_z_jerk);
  EEPROM_WRITE_VAR(i,max_e_jerk);
  EEPROM_WRITE_VAR(i,add_homeing);
  #ifndef ULTIPANEL
  int plaPreheatHotendTemp = PLA_PREHEAT_HOTEND_TEMP, plaPreheatHPBTemp = PLA_PREHEAT_HPB_TEMP, plaPreheatFanSpeed = PLA_PREHEAT_FAN_SPEED;
  int absPreheatHotendTemp = ABS_PREHEAT_HOTEND_TEMP, absPreheatHPBTemp = ABS_PREHEAT_HPB_TEMP, absPreheatFanSpeed = ABS_PREHEAT_FAN_SPEED;
  #endif
  EEPROM_WRITE_VAR(i,plaPreheatHotendTemp);
  EEPROM_WRITE_VAR(i,plaPreheatHPBTemp);
  EEPROM_WRITE_VAR(i,plaPreheatFanSpeed);
  EEPROM_WRITE_VAR(i,absPreheatHotendTemp);
  EEPROM_WRITE_VAR(i,absPreheatHPBTemp);
  EEPROM_WRITE_VAR(i,absPreheatFanSpeed);
  #ifdef PIDTEMP
    EEPROM_WRITE_VAR(i,Kp);
    EEPROM_WRITE_VAR(i,Ki);
    EEPROM_WRITE_VAR(i,Kd);
  #else
		float dummy = 3000.0f;
    EEPROM_WRITE_VAR(i,dummy);
		dummy = 0.0f;
    EEPROM_WRITE_VAR(i,dummy);
    EEPROM_WRITE_VAR(i,dummy);
  #endif
  #if MOTOR_CURRENT_PWM_XY_PIN > -1
  EEPROM_WRITE_VAR(i,motor_current_setting);
  #endif
  #ifdef ENABLE_ULTILCD2
  EEPROM_WRITE_VAR(i,led_brightness_level);
  EEPROM_WRITE_VAR(i,led_mode);
  #else
  uint8_t dummyByte=0;
  EEPROM_WRITE_VAR(i,dummyByte);
  EEPROM_WRITE_VAR(i,dummyByte);
  #endif
  
#ifdef SoftwareAutoLevel
  EEPROM_WRITE_VAR(i,plainFactorA);
  EEPROM_WRITE_VAR(i,plainFactorB);
  EEPROM_WRITE_VAR(i,plainFactorC);
  EEPROM_WRITE_VAR(i,touchPlateOffset);
  EEPROM_WRITE_VAR(i,plainFactorABackUp);
  EEPROM_WRITE_VAR(i,plainFactorBBackUp);
  EEPROM_WRITE_VAR(i,plainFactorCBackUp);
  EEPROM_WRITE_VAR(i,fittingBedOffset);
#endif
  
#ifdef FilamentDetection
  EEPROM_WRITE_VAR(i, isFilamentDetectionEnable);
#endif
  
#ifdef AUTOTEMP
  EEPROM_WRITE_VAR(i,autotemp_max);
  EEPROM_WRITE_VAR(i,autotemp_min);
  EEPROM_WRITE_VAR(i,autotemp_factor);
  EEPROM_WRITE_VAR(i,autotemp_enabled);
#endif
  
#ifdef FWRETRACT
  EEPROM_WRITE_VAR(i,retract_length);
  EEPROM_WRITE_VAR(i,retract_feedrate);
  EEPROM_WRITE_VAR(i,retract_recover_feedrate);
#endif

  char ver2[4]=EEPROM_VERSION;
  i=EEPROM_OFFSET;
  EEPROM_WRITE_VAR(i,ver2); // validate data
  SERIAL_ECHO_START;
  SERIAL_ECHOLNPGM("Settings Stored");
}
#endif //EEPROM_SETTINGS


#ifdef EEPROM_CHITCHAT
void Config_PrintSettings()
{  // Always have this function, even with EEPROM_SETTINGS disabled, the current values will be shown
    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("Steps per unit:");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("  M92 X",axis_steps_per_unit[0]);
    SERIAL_ECHOPAIR(" Y",axis_steps_per_unit[1]);
    SERIAL_ECHOPAIR(" Z",axis_steps_per_unit[2]);
    SERIAL_ECHOPAIR(" E",axis_steps_per_unit[3]);
    SERIAL_ECHOLN("");
      
    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("Maximum feedrates (mm/s):");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("  M203 X",max_feedrate[0]);
    SERIAL_ECHOPAIR(" Y",max_feedrate[1] ); 
    SERIAL_ECHOPAIR(" Z", max_feedrate[2] ); 
    SERIAL_ECHOPAIR(" E", max_feedrate[3]);
    SERIAL_ECHOLN("");

    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("Maximum Acceleration (mm/s2):");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("  M201 X" ,max_acceleration_units_per_sq_second[0] ); 
    SERIAL_ECHOPAIR(" Y" , max_acceleration_units_per_sq_second[1] ); 
    SERIAL_ECHOPAIR(" Z" ,max_acceleration_units_per_sq_second[2] );
    SERIAL_ECHOPAIR(" E" ,max_acceleration_units_per_sq_second[3]);
    SERIAL_ECHOLN("");
    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("Acceleration: S=acceleration, T=retract acceleration");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("  M204 S",acceleration ); 
    SERIAL_ECHOPAIR(" T" ,retract_acceleration);
    SERIAL_ECHOLN("");

    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("Advanced variables: S=Min feedrate (mm/s), T=Min travel feedrate (mm/s), B=minimum segment time (ms), X=maximum XY jerk (mm/s),  Z=maximum Z jerk (mm/s),  E=maximum E jerk (mm/s)");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("  M205 S",minimumfeedrate ); 
    SERIAL_ECHOPAIR(" T" ,mintravelfeedrate ); 
    SERIAL_ECHOPAIR(" B" ,minsegmenttime ); 
    SERIAL_ECHOPAIR(" X" ,max_xy_jerk ); 
    SERIAL_ECHOPAIR(" Z" ,max_z_jerk);
    SERIAL_ECHOPAIR(" E" ,max_e_jerk);
    SERIAL_ECHOLN(""); 

    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("Home offset (mm):");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("  M206 X",add_homeing[0] );
    SERIAL_ECHOPAIR(" Y" ,add_homeing[1] );
    SERIAL_ECHOPAIR(" Z" ,add_homeing[2] );
    SERIAL_ECHOLN("");
#ifdef PIDTEMP
    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("PID settings:");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("   M301 P",Kp); 
    SERIAL_ECHOPAIR(" I" ,unscalePID_i(Ki)); 
    SERIAL_ECHOPAIR(" D" ,unscalePID_d(Kd));
    SERIAL_ECHOLN(""); 
#endif
} 
#endif


#ifdef EEPROM_SETTINGS
void Config_RetrieveSettings()
{
    int i=EEPROM_OFFSET;
    char stored_ver[4];
    char ver[4]=EEPROM_VERSION;
    EEPROM_READ_VAR(i,stored_ver); //read stored version
    //  SERIAL_ECHOLN("Version: [" << ver << "] Stored version: [" << stored_ver << "]");
    if (strncmp(ver,stored_ver,3) == 0)
    {
        // version number match
        EEPROM_READ_VAR(i,axis_steps_per_unit);  
        EEPROM_READ_VAR(i,max_feedrate);  
        EEPROM_READ_VAR(i,max_acceleration_units_per_sq_second);
        
        // steps per sq second need to be updated to agree with the units per sq second (as they are what is used in the planner)
		reset_acceleration_rates();
        
        EEPROM_READ_VAR(i,acceleration);
        EEPROM_READ_VAR(i,retract_acceleration);
        EEPROM_READ_VAR(i,minimumfeedrate);
        EEPROM_READ_VAR(i,mintravelfeedrate);
        EEPROM_READ_VAR(i,minsegmenttime);
        EEPROM_READ_VAR(i,max_xy_jerk);
        EEPROM_READ_VAR(i,max_z_jerk);
        EEPROM_READ_VAR(i,max_e_jerk);
        EEPROM_READ_VAR(i,add_homeing);
        #ifndef ULTIPANEL
        int plaPreheatHotendTemp, plaPreheatHPBTemp, plaPreheatFanSpeed;
        int absPreheatHotendTemp, absPreheatHPBTemp, absPreheatFanSpeed;
        #endif
        EEPROM_READ_VAR(i,plaPreheatHotendTemp);
        EEPROM_READ_VAR(i,plaPreheatHPBTemp);
        EEPROM_READ_VAR(i,plaPreheatFanSpeed);
        EEPROM_READ_VAR(i,absPreheatHotendTemp);
        EEPROM_READ_VAR(i,absPreheatHPBTemp);
        EEPROM_READ_VAR(i,absPreheatFanSpeed);
        #ifndef PIDTEMP
        float Kp,Ki,Kd;
        #endif
        // do not need to scale PID values as the values in EEPROM are already scaled		
        EEPROM_READ_VAR(i,Kp);
        EEPROM_READ_VAR(i,Ki);
        EEPROM_READ_VAR(i,Kd);
    #if MOTOR_CURRENT_PWM_XY_PIN > -1
        EEPROM_READ_VAR(i,motor_current_setting);
    #endif
        #ifdef ENABLE_ULTILCD2
        EEPROM_READ_VAR(i,led_brightness_level);
        EEPROM_READ_VAR(i,led_mode);
        #else
        uint8_t dummyByte;
        EEPROM_READ_VAR(i,dummyByte);
        EEPROM_READ_VAR(i,dummyByte);
        #endif
    
#ifdef SoftwareAutoLevel
      EEPROM_READ_VAR(i,plainFactorA);
      EEPROM_READ_VAR(i,plainFactorB);
      EEPROM_READ_VAR(i,plainFactorC);
      EEPROM_READ_VAR(i,touchPlateOffset);
      EEPROM_READ_VAR(i,plainFactorABackUp);
      EEPROM_READ_VAR(i,plainFactorBBackUp);
      EEPROM_READ_VAR(i,plainFactorCBackUp);
      EEPROM_READ_VAR(i,fittingBedOffset);
#endif
      
#ifdef FilamentDetection
    EEPROM_READ_VAR(i, isFilamentDetectionEnable);
#endif
    
#ifdef AUTOTEMP
    EEPROM_READ_VAR(i,autotemp_max);
    EEPROM_READ_VAR(i,autotemp_min);
    EEPROM_READ_VAR(i,autotemp_factor);
    EEPROM_READ_VAR(i,autotemp_enabled);
#endif

    
#ifdef FWRETRACT
    EEPROM_READ_VAR(i,retract_length);
    EEPROM_READ_VAR(i,retract_feedrate);
    EEPROM_READ_VAR(i,retract_recover_feedrate);

#endif

		// Call updatePID (similar to when we have processed M301)
        updatePID();
        fittingBedUpdateK();
        SERIAL_ECHO_START;
        SERIAL_ECHOLNPGM("Stored settings retrieved");
//        SERIAL_DEBUGPGM("Size of the setting in eeprom:");
//    SERIAL_DEBUGLN(i);
    
    if (Device_isLevelSensor) {
      readSensorOffset();
    }
    }
    else
    {
    eeprom_write_byte((uint8_t*)EEPROM_FIRST_RUN_DONE_OFFSET, 0);
        Config_ResetDefault();
        Config_StoreSettings();
        SERIAL_ECHO_START;
        SERIAL_ECHOLNPGM("Hardcoded Default Settings stored");
    }
    Config_PrintSettings();
}
#endif

void Config_ResetDefault()
{
  eeprom_write_byte((uint8_t*)EEPROM_FIRST_RUN_DONE_OFFSET, 0);

    float tmp1[]=DEFAULT_AXIS_STEPS_PER_UNIT;
    float tmp2[]=DEFAULT_MAX_FEEDRATE;
    long tmp3[]=DEFAULT_MAX_ACCELERATION;
    for (short i=0;i<4;i++) 
    {
        axis_steps_per_unit[i]=tmp1[i];  
        max_feedrate[i]=tmp2[i];  
        max_acceleration_units_per_sq_second[i]=tmp3[i];
    }
    
    // steps per sq second need to be updated to agree with the units per sq second
    reset_acceleration_rates();
    
    acceleration=DEFAULT_ACCELERATION;
    retract_acceleration=DEFAULT_RETRACT_ACCELERATION;
    minimumfeedrate=DEFAULT_MINIMUMFEEDRATE;
    minsegmenttime=DEFAULT_MINSEGMENTTIME;       
    mintravelfeedrate=DEFAULT_MINTRAVELFEEDRATE;
    max_xy_jerk=DEFAULT_XYJERK;
    max_z_jerk=DEFAULT_ZJERK;
    max_e_jerk=DEFAULT_EJERK;
    add_homeing[0] = add_homeing[1] = add_homeing[2] = 0.0;
#ifdef ULTIPANEL
    plaPreheatHotendTemp = PLA_PREHEAT_HOTEND_TEMP;
    plaPreheatHPBTemp = PLA_PREHEAT_HPB_TEMP;
    plaPreheatFanSpeed = PLA_PREHEAT_FAN_SPEED;
    absPreheatHotendTemp = ABS_PREHEAT_HOTEND_TEMP;
    absPreheatHPBTemp = ABS_PREHEAT_HPB_TEMP;
    absPreheatFanSpeed = ABS_PREHEAT_FAN_SPEED;
#endif
#ifdef PIDTEMP

  if (Device_isNewHeater) {
    Kp = DEFAULT_NEW_HEATER_Kp;
    Ki = scalePID_i(DEFAULT_NEW_HEATER_Ki);
    Kd = scalePID_d(DEFAULT_NEW_HEATER_Kd);
  }
  else{
    Kp = DEFAULT_Kp;
    Ki = scalePID_i(DEFAULT_Ki);
    Kd = scalePID_d(DEFAULT_Kd);
  }
    
    // call updatePID (similar to when we have processed M301)
    updatePID();
    
#ifdef PID_ADD_EXTRUSION_RATE
    Kc = DEFAULT_Kc;
#endif//PID_ADD_EXTRUSION_RATE
#endif//PIDTEMP
  
#if MOTOR_CURRENT_PWM_XY_PIN > -1
    float tmp_motor_current_setting[]=DEFAULT_PWM_MOTOR_CURRENT;
    motor_current_setting[0] = tmp_motor_current_setting[0];
    motor_current_setting[1] = tmp_motor_current_setting[1];
    motor_current_setting[2] = tmp_motor_current_setting[2];
#endif

    #ifdef ENABLE_ULTILCD2
    led_brightness_level = 100;
    led_mode = LED_MODE_ALWAYS_ON;
    #endif
  
  
#ifdef SoftwareAutoLevel

    fittingBedReset();
    fittingBedResetBackUp();
    fittingBedUpdateK();
    fittingBedOffsetInit();
  restoreFactorySensorOffset();
  if (Device_isLevelSensor) {
    touchPlateOffset=TouchPlateOffsetSensor;
  }
  else{
    touchPlateOffset=TouchPlateOffset;
  }
#endif
  
#ifdef FilamentDetection
  isFilamentDetectionEnable=false;
#endif
  
#ifdef AUTOTEMP
  autotemp_max=240;
  autotemp_min=220;
  autotemp_factor=0.8;
  autotemp_enabled=true;
#endif
  
#ifdef FWRETRACT
  retract_length=6;
  retract_feedrate=80*60;
  retract_recover_feedrate=80*60;
#endif
  
  lcd_material_reset_defaults();
  for(uint8_t e=0; e<EXTRUDERS; e++)
    lcd_material_set_material(0, e);
  
  SERIAL_ECHO_START;
  SERIAL_ECHOLNPGM("Hardcoded Default Settings Loaded");
}




