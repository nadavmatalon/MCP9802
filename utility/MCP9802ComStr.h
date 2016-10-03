/*==============================================================================================================*

    @file     MCP9802ComStr.h
    @author   Nadav Matalon
    @license  GNU General Public License
 
    A complemetary I2C Communications Result String Generator for the MCP9802
 
    Ver. 1.0.0 - First release (26.9.16)

*===============================================================================================================*
    LICENSE
*===============================================================================================================*
 
    This library is a free software; you can redistribute it and/or modify it under the terms of the
    GNU General Public License as published by the Free Software Foundation; either version 3.0 of the
    License, or any later version. This program is distributed in the hope that it will be useful, but 
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
    PURPOSE. See the GNU General Public License for more details.

*==============================================================================================================*/

#ifndef MCP9802ComStr_h
#define MCP9802ComStr_h

#include <avr/pgmspace.h>

const byte COM_BUFFER_SIZE  = 60;
const int  NUM_OF_COM_CODES =  8;

const char comMsg0[] PROGMEM = "Success\0";
const char comMsg1[] PROGMEM = "Error Code #1: I2C Buffer overflow\0";
const char comMsg2[] PROGMEM = "Error Code #2: Address sent, NACK received\0";
const char comMsg3[] PROGMEM = "Error Code #3: Data send, NACK received\0";
const char comMsg4[] PROGMEM = "Error Code #4: Other error (bus error, etc.)\0";
const char comMsg5[] PROGMEM = "Error Code #5: Timed-out while trying to become Bus Master\0";
const char comMsg6[] PROGMEM = "Error Code #6: Timed-out while waiting for data to be sent\0";
const char comMsgDefault[] PROGMEM = "Error Code #%d: Unlisted error\0";

const char * const comCodes[NUM_OF_COM_CODES] PROGMEM = {
        comMsg0,
        comMsg1,
        comMsg2,
        comMsg3,
        comMsg4,
        comMsg5,
        comMsg6,
        comMsgDefault
};

/*==============================================================================================================*
    GET I2C COMMUNICATIONS RESULT MESSAGE (PRINTABLE FORMAT)
 *==============================================================================================================*/

String MCP9802ComStr(const MCP9802& devParams) {
    char devComBuffer[COM_BUFFER_SIZE];
    char comCodeResult = devParams._comBuffer;
    char * ptr = (char *) pgm_read_word(&comCodes[comCodeResult]);
    snprintf_P(devComBuffer, COM_BUFFER_SIZE, ptr, comCodeResult);
    return String(devComBuffer);
}

#endif
