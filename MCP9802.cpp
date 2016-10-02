/*==============================================================================================================*

    @file     MCP9802.cpp
    @author   Nadav Matalon
    @license  GNU General Public License
 
    Driver for MCP9802 (12-BIT I2C TEMPERATURE SENSOR)
 
    Ver. 1.0.0 - First release (23.9.16)
 
 *==============================================================================================================*
    LICENSE
 *==============================================================================================================*
 
    This library is a free software; you can redistribute it and/or modify it under the terms of the
    GNU General Public License as published by the Free Software Foundation; either version 3.0 of the
    License, or any later version. This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
    PURPOSE. See the GNU General Public License for more details.

 *==============================================================================================================*/

#include "MCP9802.h"

/*==============================================================================================================*
    CONSTRUCTOR
 *==============================================================================================================*/

MCP9802::MCP9802(int devAddr) {
    _devAddr        = devAddr;
    _singleConfig   = DEFAULT_CONFIG;
    _tempUnit       = CELSIUS;
}

// add default address for constructor

/*==============================================================================================================*
    DESTRUCTOR
 *==============================================================================================================*/

MCP9802::~MCP9802() {}

/*==============================================================================================================*
    PING
 *==============================================================================================================*/

byte MCP9802::ping() {
    Wire.beginTransmission(_devAddr);
    return Wire.endTransmission();
}

/*==============================================================================================================*
    GET ALERT OUTPUT TYPE (0 = COMPARATOR / 1 = INTERRUPT)
 *==============================================================================================================*/

byte MCP9802::getAlertType() {
    return bitRead(getConfig(), 1);
}

/*==============================================================================================================*
    GET ALERT OUTPUT MODE (0 = ACTIVE-LOW / 1 = ACTIVE-HIGH)
 *==============================================================================================================*/

byte MCP9802::getAlertMode() {
    return bitRead(getConfig(), 2);
}

/*==============================================================================================================*
    GET FAULT-QUEUE SETTINGS (1 / 2 / 4 / 6 FAULT/S)
 *==============================================================================================================*/

byte MCP9802::getFaultQueue() {
    byte fqVal = ((getConfig() & 0x18) >> 2);
    if (!fqVal) fqVal = 1;
    return fqVal;
}

/*==============================================================================================================*
    GET RESOLUTION SETTINGS (9 / 10 / 11 / 12-BIT)
 *==============================================================================================================*/

byte MCP9802::getResolution() {
    return ((getConfig() & 0x60) >> 5) + 9;
}

/*==============================================================================================================*
    GET CONVERSION MODE (0 = CONTINUOUS / 1 = SINGLE-SHOT)
 *==============================================================================================================*/

byte MCP9802::getConMode() {
    return bitRead(getConfig(), 0);
}

/*==============================================================================================================*
    GET TEMPERATURE UNIT (0 = CELSIUS / 1 = FAHRENHEIT)
 *==============================================================================================================*/

byte MCP9802::getTempUnit() {
    return _tempUnit;
}

/*==============================================================================================================*
    GET COMMUNICATION RESULT
 *==============================================================================================================*/

byte MCP9802::getComResult() {
    return _comBuffer;
}

/*==============================================================================================================*
    GET AMBIENT TEMPERATURE
 *==============================================================================================================*/

float MCP9802::getTemp() {
    return (getData16(TEMP) / 16.0);
}

/*==============================================================================================================*
    GET AMBIENT TEMPERATURE x 16
 *==============================================================================================================*/

int MCP9802::getTemp16() {
    return getData16(TEMP);
}

/*==============================================================================================================*
    GET HYSTERESIS
 *==============================================================================================================*/

float MCP9802::getHyst() {
    return (getData16(HYST) / 16.0);
}


/*==============================================================================================================*
    GET LIMIT
 *==============================================================================================================*/

float MCP9802::getLimit() {
    return (getData16(LIMIT) / 16.0);
}

/*==============================================================================================================*
    SINGLE CONVERSION ('SINGLE-SHOT' MODE ONLY)
 *==============================================================================================================*/
//  Conversion Time: 9-BIT = 40ms / 10-BIT = 70ms / 11-BIT = 130ms / 12-BIT = 250ms

float MCP9802::singleCon() {
    if (_singleConfig) {
        unsigned int conTime = MIN_CON_TIME * (1 << ((_singleConfig & 0x60) >> 5)) + 10;
        setConfig(_singleConfig | INIT_SINGLE_SHOT);
        delay(conTime);
        return getTemp();
    }
}

/*==============================================================================================================*
    SET ALERT OUTPUT TYPE (0 = COMPARATOR / 1 = INTERRUPT)
 *==============================================================================================================*/

void MCP9802::setAlertType(alert_type_t alertType) {                    // PARAMS: COMP / INT
    byte newConfig = getConfig();
    alertType ? bitSet(newConfig, 1) : bitClear(newConfig, 1);
    setConfig(newConfig);
}

/*==============================================================================================================*
    SET ALERT OUTPUT MODE (0 = ACTIVE-LOW / 1 = ACTIVE-HIGH)
 *==============================================================================================================*/

void MCP9802::setAlertMode(alert_mode_t alertMode) {                    // PARAMS: ACTIVE-LOW / ACTIVE-HIGH
    byte newConfig = getConfig();
    alertMode ? bitSet(newConfig, 2) : bitClear(newConfig, 2);
    setConfig(newConfig);
}

/*==============================================================================================================*
    SET FAULT-QUEUE SETTINGS (1 / 2 / 4 / 6 FAULT/S)
 *==============================================================================================================*/

void MCP9802::setFaultQueue(fault_queue_t fqVal) {                      // PARAMS: FQ1 / FQ2 / FQ4 / FQ6
    byte newConfig = getConfig();
    for (byte j=4; j>2; j--) bitClear(newConfig, j);
    newConfig |= (byte)fqVal;
    setConfig(newConfig);
}

/*==============================================================================================================*
    SET RESOLUTION SETTINGS (9 / 10 / 11 / 12-BIT)
 *==============================================================================================================*/

void MCP9802::setResolution(resolution_t resVal) {                 // PARAMS: RES_9 / RES_10 / RES_11 // RES_12
    byte newConfig = getConfig();
    for (byte j=6; j>4; j--) bitClear(newConfig, j);
    newConfig |= (byte)resVal;
    setConfig(newConfig);
}

/*==============================================================================================================*
    SET CONVERSION MODE (CONTINUOUS / SINGLE-SHOT)
 *==============================================================================================================*/

void MCP9802::setConMode(con_mode_t conMode) {                    // PARAMS: CONT / SINGLE
    byte newConfig = getConfig();
    bitClear(newConfig, 7);
    conMode ? bitSet(newConfig, 0) : bitClear(newConfig, 0);
    _singleConfig = conMode ? newConfig : 0;
    setConfig(newConfig);
}

/*==============================================================================================================*
    SET TEMPERATURE UNIT
 *==============================================================================================================*/

void MCP9802::setTempUnit(temp_unit_t newTempUnit) {          // (PARAMS: CELSIUS / FAHRENHEIT)
    _tempUnit = newTempUnit;
}

/*==============================================================================================================*
    SET LIMIT
 *==============================================================================================================*/

void MCP9802::setLimit(float newLimit) {
    setData(LIMIT, newLimit);
}

/*==============================================================================================================*
 SET LIMIT x 16
 *==============================================================================================================*/

void MCP9802::setLimit16(int newLimit16) {
    setData(LIMIT, (newLimit16 / 16));
}

/*==============================================================================================================*
    RESET
 *==============================================================================================================*/

void MCP9802::reset() {
    _singleConfig = 0;
    _tempUnit = CELSIUS;
    setConMode(SINGLE);                 // check if needed
    setConfig(DEFAULT_CONFIG);
    setLimit16(DEFAULT_LIMIT * 16);
    setHyst16(DEFAULT_HYST * 16);
}

/*==============================================================================================================*
    INITIATE I2C COMMUNICATION
 *==============================================================================================================*/

void MCP9802::initCall(byte ptrByte) {
    Wire.beginTransmission(_devAddr);
    Wire.write(ptrByte);
}

/*==============================================================================================================*
    END I2C COMMUNICATION
 *==============================================================================================================*/

void MCP9802::endCall() {
    _comBuffer = Wire.endTransmission();
}

/*==============================================================================================================*
    GET DEVICE CONFIGURATION REGISTER
 *==============================================================================================================*/

byte MCP9802::getConfig() {
    byte config;
    initCall(CONFIG);
    endCall();                              // check if needed
    if (_comBuffer == COM_SUCCESS) {
        Wire.requestFrom(_devAddr, CONFIG_BYTE);
        if (Wire.available() == CONFIG_BYTE) config = Wire.read();
    }
    return config;
}

/*==============================================================================================================*
    GET DATA REGISTERS
 *==============================================================================================================*/

int MCP9802::getData16(reg_ptr_t ptr) {                                    // PARAMS: TEMP / HYST / LIMIT
    int data16;
    initCall(ptr);
    endCall();
    if (_comBuffer == COM_SUCCESS) {
        Wire.requestFrom(_devAddr, DATA_BYTES);
        if (Wire.available() == DATA_BYTES) data16 = (Wire.read() << 8) | (Wire.read());
    }
    return (_tempUnit) ? (DegreeConverter::conC16toF16(data16) / 16) : (data16 / 16);
}

// check replacing last two lines with (Wire.read() << 4 | Wire.read() >> 4)

/*==============================================================================================================*
    SET CONFIGURATION REGISTER
 *==============================================================================================================*/

void MCP9802::setConfig(byte newConfig) {
    initCall(CONFIG_BYTE);
    Wire.write(newConfig);
    endCall();
}

/*==============================================================================================================*
    SET REGISTER DATA
 *==============================================================================================================*/

void MCP9802::setData(reg_ptr_t ptr, int newData) {                                   // PARAMS: HYST / LIMIT
    if ((ptr == HYST || ptr == LIMIT) && (newData >= -55 && newData <= 125)) {
//    if ((ptr == HYST || ptr == LIMIT) && (-55 <= newData && newData <= 125)) {
        union Data_t { int i; byte b[2]; } data;
        if (_tempUnit) newData = (int)DegreeConverter::roundC(DegreeConverter::conFtoC(newData));
        data.i = (newData << 8);
        initCall(ptr);
        for (byte j=2; j>0; j--) Wire.write(data.b[j-1]);
        endCall();
    }
}

/*==============================================================================================================*
    SET HYSTERESIS (I)
 *==============================================================================================================*/

void MCP9802::setHyst(int newHyst) {
    setData(HYST, newHyst);
}

/*==============================================================================================================*
    SET HYSTERESIS (II)
 *==============================================================================================================*/

//void MCP9802::setHyst(float newHyst) {
//    setHyst((int)(DegreeConverter::roundC(newHyst)));
//}

/*==============================================================================================================*
    SET HYSTERESIS (III)
 *==============================================================================================================*/

void MCP9802::setHyst(double newHyst) {
    setHyst((int)(DegreeConverter::roundC(newHyst)));
}

/*==============================================================================================================*
    SET HYSTERESIS x 16
 *==============================================================================================================*/

void MCP9802::setHyst16(int newHyst16) {
    setHyst((float)(newHyst16 / 16.0));
}














