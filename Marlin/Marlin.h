// Tonokip RepRap firmware rewrite based off of Hydra-mmm firmware.
// Licence: GPL

#ifndef MARLIN_H
#define MARLIN_H

#define  FORCE_INLINE __attribute__((always_inline)) inline

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>


#include "fastio.h"
#include "Configuration.h"
#include "pins.h"

#ifndef AT90USB
#define  HardwareSerial_h // trick to disable the standard HWserial
#endif

# include "Arduino.h"


#include "MarlinSerial.h"

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#include "WString.h"

#ifdef AT90USB
  #define MYSERIAL Serial
#else
  #define MYSERIAL MSerial
#endif

#define SERIAL_PROTOCOL(x) MYSERIAL.print(x)
#define SERIAL_PROTOCOL_F(x,y) MYSERIAL.print(x,y)
#define SERIAL_PROTOCOLPGM(x) serialprintPGM(PSTR(x))
#define SERIAL_PROTOCOLLN(x) do {MYSERIAL.print(x);MYSERIAL.write('\n');} while(0)
#define SERIAL_PROTOCOLLNPGM(x) do{serialprintPGM(PSTR(x));MYSERIAL.write('\n');} while(0)

#define SERIAL_BLE_PROTOCOL(x) MYSERIAL.print(x)


#ifdef _DEBUG

#define SERIAL_DEBUG(x) SERIAL_PROTOCOL(x)
#define SERIAL_DEBUGPGM(x) SERIAL_PROTOCOLPGM(x)
#define SERIAL_DEBUGLN(x) SERIAL_PROTOCOLLN(x)
#define SERIAL_DEBUGLNPGM(x) SERIAL_PROTOCOLLNPGM(x)
#define SERIAL_DEBUGPGMPTR(x) serialprintPGM(x)

#else

#define SERIAL_DEBUG(x)
#define SERIAL_DEBUGPGM(x)
#define SERIAL_DEBUGLN(x)
#define SERIAL_DEBUGLNPGM(x)
#define SERIAL_DEBUGPGMPTR(x)

#endif

#ifdef _BED_DEBUG

#define SERIAL_BED_DEBUG(x) SERIAL_PROTOCOL(x)
#define SERIAL_BED_DEBUGPGM(x) SERIAL_PROTOCOLPGM(x)
#define SERIAL_BED_DEBUGLN(x) SERIAL_PROTOCOLLN(x)
#define SERIAL_BED_DEBUGLNPGM(x) SERIAL_PROTOCOLLNPGM(x)
#define SERIAL_BED_DEBUGPGMPTR(x) serialprintPGM(x)

#else

#define SERIAL_BED_DEBUG(x)
#define SERIAL_BED_DEBUGPGM(x)
#define SERIAL_BED_DEBUGLN(x)
#define SERIAL_BED_DEBUGLNPGM(x)
#define SERIAL_BED_DEBUGPGMPTR(x)

#endif


const char errormagic[] PROGMEM ="Error:";
const char echomagic[] PROGMEM ="echo:";
#define SERIAL_ERROR_START serialprintPGM(errormagic)
#define SERIAL_ERROR(x) SERIAL_PROTOCOL(x)
#define SERIAL_ERRORPGM(x) SERIAL_PROTOCOLPGM(x)
#define SERIAL_ERRORLN(x) SERIAL_PROTOCOLLN(x)
#define SERIAL_ERRORLNPGM(x) SERIAL_PROTOCOLLNPGM(x)

#define SERIAL_ECHO_START serialprintPGM(echomagic)
#define SERIAL_ECHO(x) SERIAL_PROTOCOL(x)
#define SERIAL_ECHOPGM(x) SERIAL_PROTOCOLPGM(x)
#define SERIAL_ECHOLN(x) SERIAL_PROTOCOLLN(x)
#define SERIAL_ECHOLNPGM(x) SERIAL_PROTOCOLLNPGM(x)

#define SERIAL_ECHOPAIR(name,value) (serial_echopair_P(PSTR(name),(value)))

void serial_echopair_P(const char *s_P, float v);
void serial_echopair_P(const char *s_P, double v);
void serial_echopair_P(const char *s_P, unsigned long v);


//things to write to serial from Programmemory. saves 400 to 2k of RAM.
FORCE_INLINE void serialprintPGM(const char *str)
{
  char ch=pgm_read_byte(str);
  while(ch)
  {
    MYSERIAL.write(ch);
    ch=pgm_read_byte(++str);
  }
}


void get_command();
void process_commands();

void manage_inactivity();

#if defined(X_ENABLE_PIN) && X_ENABLE_PIN > -1
  #define  enable_x() WRITE(X_ENABLE_PIN, X_ENABLE_ON)
  #define disable_x() WRITE(X_ENABLE_PIN,!X_ENABLE_ON)
#else
  #define enable_x() ;
  #define disable_x() ;
#endif

#if defined(Y_ENABLE_PIN) && Y_ENABLE_PIN > -1
  #define  enable_y() WRITE(Y_ENABLE_PIN, Y_ENABLE_ON)
  #define disable_y() WRITE(Y_ENABLE_PIN,!Y_ENABLE_ON)
#else
  #define enable_y() ;
  #define disable_y() ;
#endif

#if defined(Z_ENABLE_PIN) && Z_ENABLE_PIN > -1
  #ifdef Z_DUAL_STEPPER_DRIVERS
    #define  enable_z() { WRITE(Z_ENABLE_PIN, Z_ENABLE_ON); WRITE(Z2_ENABLE_PIN, Z_ENABLE_ON); }
    #define disable_z() { WRITE(Z_ENABLE_PIN,!Z_ENABLE_ON); WRITE(Z2_ENABLE_PIN,!Z_ENABLE_ON); }
  #else
    #define  enable_z() WRITE(Z_ENABLE_PIN, Z_ENABLE_ON)
    #define disable_z() WRITE(Z_ENABLE_PIN,!Z_ENABLE_ON)
  #endif
#else
  #define enable_z() ;
  #define disable_z() ;
#endif

#if defined(E0_ENABLE_PIN) && (E0_ENABLE_PIN > -1)
  #define enable_e0() WRITE(E0_ENABLE_PIN, E_ENABLE_ON)
  #define disable_e0() WRITE(E0_ENABLE_PIN,!E_ENABLE_ON)
#else
  #define enable_e0()  /* nothing */
  #define disable_e0() /* nothing */
#endif

#if (EXTRUDERS > 1) && defined(E1_ENABLE_PIN) && (E1_ENABLE_PIN > -1)
  #define enable_e1() WRITE(E1_ENABLE_PIN, E_ENABLE_ON)
  #define disable_e1() WRITE(E1_ENABLE_PIN,!E_ENABLE_ON)
#else
  #define enable_e1()  /* nothing */
  #define disable_e1() /* nothing */
#endif

#if (EXTRUDERS > 2) && defined(E2_ENABLE_PIN) && (E2_ENABLE_PIN > -1)
  #define enable_e2() WRITE(E2_ENABLE_PIN, E_ENABLE_ON)
  #define disable_e2() WRITE(E2_ENABLE_PIN,!E_ENABLE_ON)
#else
  #define enable_e2()  /* nothing */
  #define disable_e2() /* nothing */
#endif


enum AxisEnum {X_AXIS=0, Y_AXIS=1, Z_AXIS=2, E_AXIS=3};


void FlushSerialRequestResend();
void ClearToSend();

void get_coordinates();
void prepare_move();
void kill();
#define STOP_REASON_MAXTEMP              1
#define STOP_REASON_MINTEMP              2
#define STOP_REASON_MAXTEMP_BED          3
#define STOP_REASON_HEATER_ERROR         4
#define STOP_REASON_X_ENDSTOP_BROKEN_ERROR 30
#define STOP_REASON_X_ENDSTOP_STUCK_ERROR  31
#define STOP_REASON_Y_ENDSTOP_BROKEN_ERROR 32
#define STOP_REASON_Y_ENDSTOP_STUCK_ERROR  33
#define STOP_REASON_Z_ENDSTOP_BROKEN_ERROR 34
#define STOP_REASON_Z_ENDSTOP_STUCK_ERROR  35

#define STOP_REASON_SAFETY_TRIGGER       10
#define STOP_REASON_REDUNDANT_TEMP 20

#define STOP_REASON_OUT_OF_MEMORY 40

void Stop(uint8_t reasonNr);

bool IsStopped();
uint8_t StoppedReason();

void clear_command_queue();
void enquecommand(const char *cmd); //put an ascii command at the end of the current buffer.
void enquecommand_P(const char *cmd); //put an ascii command at the end of the current buffer, read from flash
bool is_command_queued();
uint8_t commands_queued();
void prepare_arc_move(char isclockwise);
void clamp_to_software_endstops(float target[3]);

#ifdef FAST_PWM_FAN
void setPwmFrequency(uint8_t pin, int val);
#endif

#ifndef CRITICAL_SECTION_START
  #define CRITICAL_SECTION_START  unsigned char _sreg = SREG; cli();
  #define CRITICAL_SECTION_END    SREG = _sreg;
#endif //CRITICAL_SECTION_START


extern unsigned long previous_millis_cmd;
extern float homing_feedrate[];
extern bool axis_relative_modes[];
extern int feedmultiply;
extern int extrudemultiply[EXTRUDERS]; // Sets extrude multiply factor (in percent)
extern float current_position[NUM_AXIS] ;
extern float add_homeing[3];
extern float min_pos[3];
extern float max_pos[3];
extern uint8_t fanSpeed;
extern uint8_t fanSpeedPercent;
#ifdef BARICUDA
extern int ValvePressure;
extern int EtoPPressure;
#endif

#ifdef FAN_SOFT_PWM
extern unsigned char fanSpeedSoftPwm;
#endif

#ifdef FWRETRACT
extern bool autoretract_enabled;
extern bool retracted;
extern float retract_length, retract_feedrate, retract_zlift;
#if EXTRUDERS > 1
extern float extruder_swap_retract_length;
#endif
extern float retract_recover_length, retract_recover_feedrate;
#endif

extern unsigned long starttime;
extern unsigned long stoptime;
extern unsigned long pausetime;


extern uint8_t targetFanSpeed;
extern int targetFeedmultiply;

extern float feedrate;
extern float next_feedrate;


#ifdef PowerOnDemand

#define PowerOnDemandSleeping 0
#define PowerOnDemandStarting 1
#define PowerOnDemandWorking  2
#define PowerOnDemandStopping 3
#define PowerOnDemandOutOfPower 10
#define PowerOnDemandOutOfPowerStopping 11
#define PowerOnDemandOutOfPowerSleeping 12



extern uint8_t powerOnDemandState;
extern unsigned long powerOnDemandTimer;
extern unsigned long powerOnDemandEnergyTimer;

#endif

//The printing state from the main command processor. Is not zero when the command processor is in a loop waiting for a result.
extern uint8_t printing_state;
#define PRINT_STATE_NORMAL      0
#define PRINT_STATE_DWELL       1
#define PRINT_STATE_WAIT_USER   2
#define PRINT_STATE_HEATING     3
#define PRINT_STATE_HEATING_BED 4
#define PRINT_STATE_HOMING      5
#define PRINT_STATE_WAIT_PLANNER   20
#define PRINT_STATE_HOMING_ABORT   50


// Handling multiple extruders pins
extern uint8_t active_extruder;

#if EXTRUDERS > 3
  # error Unsupported number of extruders
#elif EXTRUDERS > 2
  # define ARRAY_BY_EXTRUDERS(v1, v2, v3) { v1, v2, v3 }
#elif EXTRUDERS > 1
  # define ARRAY_BY_EXTRUDERS(v1, v2, v3) { v1, v2 }
#else
  # define ARRAY_BY_EXTRUDERS(v1, v2, v3) { v1 }
#endif


#define EnqueueingCommandBufSize  16
void discardEnqueueingCommand();
void discardCommandInBuffer();
bool isCommandInBuffer();

void newPowerSleep();
void newPowerWakeUp();

#ifdef SoftwareAutoLevel
extern float touchPlateOffset;
#endif

#ifdef FilamentDetection
#define FilamentAvailable() (!READ(FilamentDetectionPin))
#endif

extern bool isWindowsServerStarted;
extern bool isWindowsPrinting;
extern bool isWindowsSD;

extern bool isBedPreheat;

void sleepAll();

bool gateOpened();

extern "C"{
int freeMemory();
}

#define LANGUAGE_ENGLISH 0
#define LANGUAGE_CHINESE 1
#define LANGUAGE_KOREAN 2
extern uint8_t languageType;

void storeLanguage(uint8_t language);

void retriveLanguage(uint8_t language);

#define LS(a,b,c) (languageType==LANGUAGE_ENGLISH?a:(languageType==LANGUAGE_CHINESE?b:c))

void printFreeMemory();


#define OVERLORD_TYPE_P     1
#define OVERLORD_TYPE_M     2
#define OVERLORD_TYPE_MB    3
#define OVERLORD_TYPE_PNH   4
#define OVERLORD_TYPE_MNH   5
#define OVERLORD_TYPE_MBNH  6
#define OVERLORD_TYPE_PNHW  7
#define OVERLORD_TYPE_PNHL  8
#define OVERLORD_TYPE_PS    9
#define OVERLORD_TYPE_MS    10
#define OVERLORD_TYPE_PSD   11


#define OVERLORD_TYPE_MIN    OVERLORD_TYPE_P
#define OVERLORD_TYPE_MAX    OVERLORD_TYPE_PSD


extern bool Device_isGate;
extern bool Device_isNewHeater;
extern bool Device_isPro;
extern bool Device_isWifi;
extern bool Device_isBedHeat;
extern bool Device_isLevelSensor;
extern bool Device_isNewPCB;
extern bool Device_isABS;
extern bool Device_isPowerSaving;
extern bool Device_isBattery;

extern uint8_t Device_type;


extern int FILAMENT_FORWARD_LENGTH;
extern int FILAMENT_REVERSAL_LENGTH;
extern int PID_MAX;

void retrieveDevice();
bool storeDevice(uint8_t type);



#endif
