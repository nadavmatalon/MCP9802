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

const int INFO_BUFFER_SIZE  = 264;
const byte NUM_OF_INFO_CODES =  1;

const char devInfoStrBase[] PROGMEM = "\nMCP9802 DEVICE INFORMATION\n--------------------------\nI2C ADDRESS:\t %d (%#X)\nCONFIG BYTE:\t B%d%d%d%d%d%d%d%d \nALERT TYPE:\t %s\nALERT MODE:\t ACTIVE-%s\nFAULT-QUEUE:\t %d FAULT%s\nRESOLUTION:\t %d-BIT\nCONVERSION MODE: %s\nHYSTERESIS:\t %d.%dC\nLIMIT:\t\t %d.%dC\n\0";

const char * const infoCodes[NUM_OF_INFO_CODES] PROGMEM = { devInfoStrBase };

/*==============================================================================================================*
    GET DEVICE INFORMATION (PRINTABLE FORMAT)
 *==============================================================================================================*/

String MCP9802InfoStr(const MCP9802& devParams) {
    char devInfoBuffer[INFO_BUFFER_SIZE];
    int devAddr  = devParams._devAddr;
    char * ptr = (char *) pgm_read_word(&infoCodes[0]);
    MCP9802 mcp9802(devAddr);
    byte config  = devParams._singleConfig ? devParams._singleConfig : mcp9802.getConfig();
    byte fqVal   = (config & 0x18) >> 2;
    float hystC  = mcp9802.getHystC();
    float limitC = mcp9802.getLimitC();
    snprintf_P(devInfoBuffer, INFO_BUFFER_SIZE, ptr, devAddr, devAddr, (config >> 7)&1,
       (config >> 6)&1, (config >> 5)&1, (config >> 4)&1, (config >> 3)&1, (config >> 2)&1, (config >> 1)&1,
       config&1, ((config >> 1)&1 ? "INTERRUPT": "COMPARATOR"), ((config >> 2)&1 ? "HIGH": "LOW"),
       (fqVal ? fqVal : 1), (fqVal ? "S" : ""), (((config & 0x60) >> 5) + 9),
       (config ? "SINGLE-SHOT" : "CONTINUOUS"), (int)hystC, ((int)(hystC * 10) % 10 ? 5 : 0),
       (int)limitC, ((int)(limitC * 10) % 10 ? 5 : 0));
    return String(devInfoBuffer);
}

#endif
