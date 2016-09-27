/* 
  MCP9802 LIBRARY - PRINTABLE DEVICE INFORMATION STRING EXAMPLE
  -------------------------------------------------------------

  INTRODUCTION
  ------------
  This sketch presents a minimal example of extending the MCP9802 Library to include an additional function for generating a printable device 
  information string which may be useful, for example, during debugging sessions.

  As can be seen in the sketch below, implementation of this extended functionality only requires adding a single 'include' to the code, namely: 
  to that of the relevant *.h file (MCP9802InfoStr.h).
  
  Note that this functional extension does come at the cost of an increased memory usage, and therefore it seemed preferable to maintain it 
  as an optional add-on rather than include it in the core MCP9802 Library itself.
  
  WIRING DIAGRAM
  --------------
                                       MCP9802
                                       -------
                                VCC --| •     |-- SDA
                                      |       |
                                GND --|       |
                                      |       |
                              ALERT --|       |-- SCL
                                       -------

   PIN 1 -    Connect VCC to the Arduino's 5V output
   PIN 2 -    Connect GND to the Arduino's GND
   PIN 3 -    This pin can be left unconnected for the purpose of running this sketch.
   PIN 4 -    Conntect SCL to the Arduino's PIN A5 with a 2K2 pull-up resistor
   PIN 5 -    Conntect SDA to the Arduino's PIN A4 with a 2K2 pull-up resistor
   DECOUPING: Connect a 0.1uF Ceramic Capacitor between the MCP9802's VCC & GND PINS

  I2C ADDRESSES
  -------------
  Each MCP9802 has 1 of 8 possible I2C addresses (factory hardwired & recognized by its specific part number & top marking 
  on the package itself):
 
    PART               DEVICE I2C ADDRESS          PART
   NUMBER          (BIN)      (HEX)     (DEC)     MARKING
  MCP9802A0       1001000      0x48       72       JKNN
  MCP9802A1       1001001      0x49       73       JLNN
  MCP9802A2       1001010      0x4A       74       JMNN
  MCP9802A3       1001011      0x4B       75       JPNN
  MCP9802A4       1001100      0x4C       76       JQNN
  MCP9802A5       1001101      0x4D       77       JRNN
  MCP9802A6       1001110      0x4E       78       JSNN
  MCP9802A7       1001111      0x4F       79       JTNN

  BUG REPORTS
  -----------
  Please report any bugs/issues/suggestions at the GITHUB Repository of this library at: https://github.com/nadavmatalon/MCP9802

  LICENSE
  -------
  This library is a free software; you can redistribute it and/or modify it under the terms of the 
  GNU General Public License as published by the Free Software Foundation; either version 3.0 of 
  the License, or any later version.This program is distributed in the hope that it will be useful, 
  but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
  PARTICULAR PURPOSE. See the GNU General Public License for more details.
*/
#include "MCP9802.h"
#include "MCP9802InfoStr.h"

const int MCP9802_ADDR = 0x48;                   // I2C address of the MCP9802 (Change as needed)

MCP9802 MCP9802(MCP9802_ADDR);

void setup() {
    Serial.begin(9600);
    Wire.begin();
    while(!Serial);
    Serial.print(MCP9802InfoStr(MCP9802));
}

void loop() {}

