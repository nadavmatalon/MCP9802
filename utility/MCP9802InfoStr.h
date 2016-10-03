/*==============================================================================================================*

    @file     MCP9802InfoStr.h
    @author   Nadav Matalon
    @license  GNU General Public License
 
    A complemetary Device Information String Generator Debugging Function for the MCP9802
 
    Rev 1.0 - First release (26.9.16)

*===============================================================================================================*
    LICENSE
*===============================================================================================================*
 
    This library is a free software; you can redistribute it and/or modify it under the terms of the
    GNU General Public License as published by the Free Software Foundation; either version 3.0 of the
    License, or any later version. This program is distributed in the hope that it will be useful, but 
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
    PURPOSE. See the GNU General Public License for more details.

*==============================================================================================================*/

#ifndef MCP9802InfoStr_h
#define MCP9802InfoStr_h

#include <avr/pgmspace.h>

const int  INFO_BUFFER_SIZE = 40;
const byte NUM_OF_INFO_STR  = 13;

const char infoStr0[]  PROGMEM = "\nMCP9802 DEVICE INFORMATION";
const char infoStr1[]  PROGMEM = "\n--------------------------";
const char infoStr2[]  PROGMEM = "\nI2C ADDRESS:\t %d (%#X)";
const char infoStr3[]  PROGMEM = "\nCONFIG BYTE:\t B%d%d%d%d%d%d%d%d";
const char infoStr4[]  PROGMEM = "\nDEVICE MODE:\t %s";
const char infoStr5[]  PROGMEM = "\nALERT TYPE:\t %s";
const char infoStr6[]  PROGMEM = "\nALERT MODE:\t ACTIVE-%s";
const char infoStr7[]  PROGMEM = "\nFAULT-QUEUE:\t %d FAULT%s";
const char infoStr8[]  PROGMEM = "\nRESOLUTION:\t %d-BIT";
const char infoStr9[]  PROGMEM = "\nCONVERSION MODE: %s";
const char infoStr10[] PROGMEM = "\nDEGREES UNIT:\t %s";
const char infoStr11[] PROGMEM = "\nHYSTERESIS:\t %d.%d%s";
const char infoStr12[] PROGMEM = "\nLIMIT:\t\t %d.%d%s\n\0";

const char * const infoCodes[NUM_OF_INFO_STR] PROGMEM = {
    infoStr0,
    infoStr1,
    infoStr2,
    infoStr3,
    infoStr4,
    infoStr5,
    infoStr6,
    infoStr7,
    infoStr8,
    infoStr9,
    infoStr10,
    infoStr11,
    infoStr12
};

/*==============================================================================================================*
    GET DEVICE INFORMATION (PRINTABLE FORMAT)
 *==============================================================================================================*/

String MCP9802InfoStr(const MCP9802& devParams) {
    String resultStr = "";
    char devInfoBuffer[INFO_BUFFER_SIZE];
    int  devAddr   = devParams._devAddr;
    byte tempUnit  = devParams._tempUnit;
    MCP9802 mcp9802(devAddr);
    mcp9802.setTempUnit(tempUnit ? FAHRENHEIT : CELSIUS);
    byte  config   = devParams._singleConfig ? devParams._singleConfig : mcp9802.getConfig();
    byte  fqVal    = (config & 0x18) >> 2;
    float hyst     = mcp9802.getHyst();
    float limit    = mcp9802.getLimit();
    for (byte i=0; i<NUM_OF_INFO_STR; i++) {
        char * ptr = (char *) pgm_read_word(&infoCodes[i]);
        if (i < 2)   snprintf_P(devInfoBuffer, INFO_BUFFER_SIZE, ptr);
        if (i == 2)  snprintf_P(devInfoBuffer, INFO_BUFFER_SIZE, ptr, devAddr, devAddr);
        if (i == 3)  snprintf_P(devInfoBuffer, INFO_BUFFER_SIZE, ptr, (config >> 7)&1, (config >> 6)&1, (config >> 5)&1,
                         (config >> 4)&1, (config >> 3)&1, (config >> 2)&1, (config >> 1)&1, config&1);
        if (i == 4)  snprintf_P(devInfoBuffer, INFO_BUFFER_SIZE, ptr, (config ? "STANDBY" : "ON"));
        if (i == 5)  snprintf_P(devInfoBuffer, INFO_BUFFER_SIZE, ptr, ((config >> 1)&1 ? "INTERRUPT": "COMPARATOR"));
        if (i == 6)  snprintf_P(devInfoBuffer, INFO_BUFFER_SIZE, ptr, ((config >> 2)&1 ? "HIGH": "LOW"));
        if (i == 7)  snprintf_P(devInfoBuffer, INFO_BUFFER_SIZE, ptr, (fqVal ? fqVal : 1), (fqVal ? "S" : ""));
        if (i == 8)  snprintf_P(devInfoBuffer, INFO_BUFFER_SIZE, ptr, (((config & 0x60) >> 5) + 9));
        if (i == 9)  snprintf_P(devInfoBuffer, INFO_BUFFER_SIZE, ptr, (config ? "SINGLE-SHOT" : "CONTINUOUS"));
        if (i == 10) snprintf_P(devInfoBuffer, INFO_BUFFER_SIZE, ptr, (tempUnit ? "FAHRENHEIT" : "CELSIUS"));
        if (i == 11) snprintf_P(devInfoBuffer, INFO_BUFFER_SIZE, ptr, (int)hyst, abs((int)(hyst * 10) % 10), (tempUnit ? "F" : "C"));
        if (i == 12) snprintf_P(devInfoBuffer, INFO_BUFFER_SIZE, ptr, (int)limit, abs((int)(limit * 10) % 10), (tempUnit ? "F" : "C"));
        resultStr += String(devInfoBuffer);
    }
    return resultStr;
}
// change buffer to minimal size
// remove Strings (replace with string)?

#endif
