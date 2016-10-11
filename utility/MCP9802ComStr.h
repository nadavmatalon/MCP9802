/*==============================================================================================================*

    @file     MCP9802ComStr.h
    @author   Nadav Matalon
    @license  MIT (c) 2016 Nadav Matalon
 
    A complemetary I2C Communications Result String Generator for the MCP9802 Library
 
    Ver. 1.0.0 - First release (26.9.16)
    Ver. 1.1.0 - Small change in functionality: attempting to set hysteresis or limit beyond the legitimate range
                 (-55°C - 125°C / -67°F - 257°F) now sets the register to the maximum/minumum allowable value
                 rather than do nothing (4.10.16)
    Ver. 1.2.0 - Changed license to MIT (5.10.16)
    Ver. 1.3.0 - Changed auxilliary functions: MCP9802InfoStr() and MCP9802ComStr() to work with the PString class
                 instead of the String class to further reduce memory footprint. For this purpose, added PString.h
                 PString.cpp files to /utility folder. In addition added "I2C STATUS" (CONNECTED / NOT CONNECTED)
                 field to device information string (9.10.16)

*===============================================================================================================*
    LICENSE
*===============================================================================================================*
 
    The MIT License (MIT)
    Copyright (c) 2016 Nadav Matalon

    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
    documentation files (the "Software"), to deal in the Software without restriction, including without
    limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
    the Software, and to permit persons to whom the Software is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice shall be included in all copies or substantial
    portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
    LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
*==============================================================================================================*/

#ifndef MCP9802ComStr_h
#define MCP9802ComStr_h

#include <avr/pgmspace.h>

const byte COM_BUFFER_SIZE  = 60;
const int  NUM_OF_COM_CODES =  8;

const char comMsg0[] PROGMEM = "Success";
const char comMsg1[] PROGMEM = "Error Code #1: I2C Buffer overflow";
const char comMsg2[] PROGMEM = "Error Code #2: Address sent, NACK received";
const char comMsg3[] PROGMEM = "Error Code #3: Data send, NACK received";
const char comMsg4[] PROGMEM = "Error Code #4: Other error (bus error, etc.)";
const char comMsg5[] PROGMEM = "Error Code #5: Timed-out while trying to become Bus Master";
const char comMsg6[] PROGMEM = "Error Code #6: Timed-out while waiting for data to be sent";
const char comMsgDefault[] PROGMEM = "Error Code #%d: Unlisted error";

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

PString MCP9802ComStr(const MCP9802& devParams) {
    char devComBuffer[COM_BUFFER_SIZE];
    PString comStr(devComBuffer, COM_BUFFER_SIZE);
    char comCodeResult = devParams._comBuffer;
    char * ptr = (char *) pgm_read_word(&comCodes[comCodeResult]);
    snprintf_P(devComBuffer, COM_BUFFER_SIZE, ptr, comCodeResult);
    return comStr;
}

#endif
