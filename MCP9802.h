/*==============================================================================================================*

    @file     MCP9802.h
    @author   Nadav Matalon
    @license  GNU General Public License
 
    Driver for MCP9802 (12-BIT I2C TEMPERATURE SENSOR)
 
    Ver. 1.0.0 - First release (23.9.16)

*===============================================================================================================*
    I2C ADDRESSES
*===============================================================================================================*

    Each MCP9802 has 1 of 8 possible I2C addresses (factory hardwired & recognized by its specific
    part number & top marking on the package itself):
 
        PART               DEVICE I2C ADDRESS          PART
       NUMBER          (BIN)      (HEX)     (DEC)     MARKING
    MCP9802A0T-M/OT   01001000    0x48       72        JKNN
    MCP9802A1T-M/OT   01001001    0x49       73        JLNN
    MCP9802A2T-M/OT   01001010    0x4A       74        JMNN
    MCP9802A3T-M/OT   01001000    0x4B       75        JPNN
    MCP9802A4T-M/OT   01001100    0x4C       76        JQNN
    MCP9802A5T-M/OT   01001101    0x4D       77        JRNN
    MCP9802A6T-M/OT   01001110    0x4E       78        JSNN
    MCP9802A7T-M/OT   01001111    0x4F       79        JTNN

*===============================================================================================================*
    POWER-UP DEFAULTS
*===============================================================================================================*

    CONFIG BYTE:        B00000000
    ALERT OUTPUT TYPE:  COMPARATOR
    ALERT OUTPUT MODE:  ACTIVE-LOW
    FAULT-QUEUE:        1 FAULT
    RESOLUTION:         9-BIT
    CONVERSION MODE:    CONTINUOUS
    TEMP  (DATA):       0x0000 (0°C)
    LIMIT (DATA):       0xA000 (80°C)
    HYST  (DATA):       0x9600 (75°C)

*===============================================================================================================*
    REGISTER POINTERS
*===============================================================================================================*

    TEMP                0x00        // Ambient Temperature Register  (R)    B00000000 (Default)
    CONFIG              0x01        // Configuration Register        (R/W)  B00000001
    HYST                0x02        // Hysteresis Register           (R/W)  B00000010
    LIMIT               0x03        // Temperature Limit Register    (R/W)  B00000011

*===============================================================================================================*
    CONFIGURATION REGIASTER (BIT 0): DEVICE MODE (0 = ON / 1 = OFF)
*===============================================================================================================*

    ON                  0x00        // Shutdown - DISABLED (0)  B00000000 (0) (Default)
    OFF                 0x01        // Shutdown - ENABLED  (1)  B00000001 (1)

*===============================================================================================================*
    CONFIGURATION REGIASTER (BIT 1): ALERT OUTPUT TYPE (0 = COMPARATOR / 1 = INTERUPT)
*===============================================================================================================*

    COMP                0x00        // Alert Output Type - COMPARATOR (0)  B00000000 (0x00) (Default)
    INT                 0x02        // Alert Output Type - INTERUPT   (2)  B00000010 (0x02)

*===============================================================================================================*
    CONFIGURATION REGIASTER (BIT 2): ALERT OUTPUT MODE (0 = ACTIVE-LOW / 1 = ACTIVE-HIGH)
*===============================================================================================================*

    ACTIVE-LOW          0x00        // Alert Polarity Output Mode: ACTIVE-LOW  (0)  B00000000 (0x00) (Default)
    ACTIVE-HIGH         0x04        // Alert Polarity Output Mode: ACTIVE-HIGH (4)  B00000100 (0x04)

*===============================================================================================================*
    CONFIGURATION REGIASTER (BITS 3 & 4): FAULT QUEUE (1 / 2 / 4 / 6 FAULTS)
*===============================================================================================================*

    FQ1                 0x00        // Fault Queue - 1 Fault    B00000000 (0x00) (Default)
    FQ2                 0x08        // Fault Queue - 2 Faults   B00001000 (0x08)
    FQ4                 0x10        // Fault Queue - 4 Faults   B00010000 (0x10)
    FQ6                 0x18        // Fault Queue - 6 Faults   B00011000 (0x18)

*===============================================================================================================*
    CONFIGURATION REGIASTER (BITS 5 & 6): RESOLUTION (9 / 10 / 11 / 12-BIT)
*===============================================================================================================*

     9-BIT              0x00        //  9-Bit Resolution (0.5°C    / 30ms)   B00000000 (0x00) (Default)
    10-BIT              0x20        // 10-Bit Resolution (0.25°C   / 60ms)   B00100000 (0x20)
    11-BIT              0x40        // 11-Bit Resolution (0.125°C  / 120ms)  B01000000 (0x40)
    12-BIT              0x60        // 12-Bit Resolution (0.0625°C / 240ms)  B01100000 (0x60)
 
*===============================================================================================================*
    CONFIGURATION REGIASTER (BIT 7): CONVERSION MODE (0 = CONTINUOUS / 1 = SINGLE-SHOT)
*===============================================================================================================*

    CONT                0x00        // Conversion Mode: CONTINUOUS  (0)  B00000000 (0x00) (Default)
    SINGLE              0x80        // Conversion Mode: SINGLE-SHOT (1)  B10000000 (0x80)

*===============================================================================================================*
    LICENSE
*===============================================================================================================*
 
    This library is a free software; you can redistribute it and/or modify it under the terms of the
    GNU General Public License as published by the Free Software Foundation; either version 3.0 of the
    License, or any later version. This program is distributed in the hope that it will be useful, but 
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
    PURPOSE. See the GNU General Public License for more details.

*==============================================================================================================*/

#ifndef MCP9802_h
#define MCP9802_h

#if defined(ARDUINO_ARCH_AVR)
    #include <Arduino.h>
    #include <WSWire.h>
    #include "utility/DegreeConverter.h"
#else
    #error “The MCP9802 library only supports AVR processors.”
#endif

const byte DEFAULT_CONFIG   =   0;      // Configuration Register Default Settings for 'Continuous' conversion mode (0x00)
const int  DEFAULT_HYST     =  75;      // Hysteresis Register Default Settings in degrees Celsius (0x9600)
const int  DEFAULT_LIMIT    =  80;      // Limit Register Default Settings in degrees Celsius (0xA000)
const int  CONFIG_BYTE      =   1;      // Number of Configuration Register Bytes (CONFIG)
const int  DATA_BYTES       =   2;      // Number of Data Register Bytes (TEMP, HYST, LIMIT)
const byte INIT_SINGLE_SHOT = 129;      // Initiates a single conversion in 'Single-Shot' mode (0x81)
const byte MIN_CON_TIME     =  30;      // 30ms - Minimal Conversion Time (for 9-BIT Resolution)
const byte COM_SUCCESS      =   0;      // I2C Communication Success (No Error)

typedef enum:byte {
    TEMP   = 0,
    CONFIG = 1,
    HYST   = 2,
    LIMIT  = 3
} reg_ptr_t;

typedef enum:byte {
    COMP = 0,
    INT  = 2
} alert_type_t;

typedef enum:byte {
    ACTIVE_LOW  = 0,
    ACTIVE_HIGH = 4
} alert_mode_t;

typedef enum:byte {
    FQ1 = 0,
    FQ2 = 8,
    FQ4 = 16,
    FQ6 = 24
} fault_queue_t;

typedef enum:byte {
    RES_9  = 0,
    RES_10 = 32,
    RES_11 = 64,
    RES_12 = 96
} resolution_t;

typedef enum:byte {
    CONT   = 0,
    SINGLE = 128
} con_mode_t;

typedef enum:byte {
    CELSIUS    = 0,
    FAHRENHEIT = 1
} temp_unit_t;

class MCP9802 {
     public:
        MCP9802(int devAddr);
        ~MCP9802();
        byte   ping();
        byte   getAlertType();
        byte   getAlertMode();
        byte   getFaultQueue();
        byte   getResolution();
        byte   getConMode();
        byte   getTempUnit();
        void   setAlertType(alert_type_t alertType);
        void   setAlertMode(alert_mode_t alertMode);
        void   setFaultQueue(fault_queue_t fqVal);
        void   setResolution(resolution_t resVal);
        void   setConMode(con_mode_t conMode);
        void   setTempUnit(temp_unit_t newTempUnit);
        void   reset();
        byte   getComResult();
        float  getTemp();
        int    getTemp16();
        float  getHyst();
        int    getHyst16();
        float  getLimit();
        int    getLimit16();
//      void   setHyst(int newHyst);
        void   setHyst(float newHyst);
        void   setHyst16(int newHyst16);
//      void   setLimit(int newLimit);
        void   setLimit(float newLimit);
        void   setLimit16(int newLimit16);
        float  singleConC();
        int    singleCon16();
    private:
        int    _devAddr;
        byte   _tempUnit;
        byte   _comBuffer;
        byte   _singleConfig;
        void   initCall(byte ptrByte);
        void   endCall();
        byte   getConfig();
        int    getData16(reg_ptr_t ptr);
        void   setConfig(byte newConfig);
//      void   setData(reg_ptr_t ptr, int newData);
        void   setData(reg_ptr_t ptr, float newData);
        void   setData16(reg_ptr_t ptr, int newData16);
        friend String MCP9802ComStr(const MCP9802&);
        friend String MCP9802InfoStr(const MCP9802&);
};

#endif