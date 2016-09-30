/* 
  MCP9802 LIBRARY - COMPLETE USAGE EXAMPLE
  ----------------------------------------

  INTRODUCTION
  ------------ 

  This sketch offers a complete usage illustration, as well as a rubust testing mechanism, for the MCP9802.

  The sketch exposes all available configuration settings, data reading, data setting, and alarm functionality encapsulated 
  within the MCP9802.


  GENERAL NOTES
  ------------- 

  1) I2C COMMUNICATION LIBRARY
  
  This library uses the 'WSWire' library (https://github.com/steamfire/WSWireLib/tree/master/Library/WSWire) for I2C communication with 
  the ADS1110, so it is NECESSARY to have it installed prior to using the current libraty. Alternatively, if you wish to use the 'Wire' library 
  (https://github.com/arduino/Arduino/tree/master/hardware/arduino/avr/libraries/Wire) - or any other I2C library for that matter - simply change 
  the following line the the 'MCP9802.h' file:

      #include <WSWire.h>
 
  to this:

      #include <Wire.h>  // or to whatever I2C library name you are using.

  As noted above, whichever library you intend to use for this purpose must be alredy installed for the ADS1110 library to work.

  2) SHUTDOWN & CONVERSION MODE
  
  The first bit of the configuration byte controls the device mode of operation, namely: ON, in which the device operates in 'CONTINUOUS' mode,
  or OFF - or more precisely HYBERNATE (as I2C communication remains active), in which the device operates in 'SINGLE-SHOT' nmode. As such, setting 
  the 'CONVERSION MODE' of the device to 'CONTINUOUS' will effectively ensure that it is 'ON', while setting it to 'SINGLE-SHOT' mode will turn it OFF 
  (or more accurately, put it in hybernate mode).

  3) HYSTERESIS & LIMIT REGISTERS RESOLUTION
  
  The Temperature register has a setteble range of 9 to 12-BIT (0.5 to 0.0625 degrees Celsius respectively). However, both the LIMIT and HYSTERESIS 
  registers only have a 9-BIT fixed resolution. This means these registers can only be set with a maximum resolution of 0.5 degrees Celsius. 
  Hence, while the relevant functions (e.g. setTempC(); ) will happily accept any float value within the premmitted parameter range (-55C to 125C) 
  for either of these two registers, this float value will be automatically rounded to the nearest 0.5C.

  4) DEGREES CELSIUS & FAHRENHEIT
  
  The libraty offers the option of getting/setting all termperature values (Abmient [read-only], Limit [read-write] and/or Hysteresis [read-write]) 
  in either degrees Celsuis or Fahrenheit. These can be obtained in a floating point format or, if prefered, in a x16 integer format to speed-up 
  conversion calculations and save memory space. 

  5) DEGREES FAHRENHEIT ACCURACY LIMITATIONS
  
  As the MCP9802 was designed primerily to work in a degree Celsuis scheme, all Fahrenheit values obtained (or custom set by the user) 
  can only represent approximations of the precise Celsius values generated or stored by the device. This limitation is particualarly 
  noticable when setting the LIMIT or HYSTERESIS registers to custom Fahrenheit values, as a double operation takes place, namely: 
  rounding the given value to the nearest 0.5 degree Celisus and the subsequent conversion of this figure to the equivalent Fahrenheit value.

  6) ALERT FUNCTIONALITY
  
  The MCP9802's Alert functionality is based on an 'open collector' architecture which means it requires a pull-up resistor in order to work
  (this is true for both Alert Types, i.e. 'ACTIVE-LOW' and 'ACTIVE-HIGH). For the purposes of this testing sketch, the Atmega's (weak) internal
  pull-up resistor is used and so the only connection needed in this context is between the MCP9802's ALERT pin and the Arduino's Digital Pin D2. 
  However, for any real-life use of the device, it is highly recommended to implement a suitable external pull-up resistor (typically 10K) hooked-up 
  betweem the ALERT pin and VCC.


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
   PIN 3 -    Connect ALERT to the Arduino's Digital Pin 2
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

#include <MCP9802.h>

const byte PIN_D2 = 2;                                                    // Arduino PIN D2 (PIN 2) connected to the MCP9802's ALERT Pin
const int MCP9802_ADDR = 0x48;                                            // DEC: 72 - I2C address of the MCP9802 (Change as needed)

byte    config;                                                           // Variables to hold configuration register data
int     tempC16, tempF16, hystC16, hystF16, limitC16, limitF16;           // Variables to hold conversion data
float   tempC, tempF, hystC, hystF, limitC, limitF;                       // Variables to hold conversion data
String  deviceMode, alertType, alertMode, conMode;                        // Variables to hold configuration data
alert_mode_t alertModeParams[2] = { ACTIVE_LOW, ACTIVE_HIGH };            // Array for Alert testing

typedef enum:byte {
    OFF = 0,
    ON  = 1
} alert_state_t;

MCP9802 MCP9802(MCP9802_ADDR);                                            // Constructs a new MCP9802 object with the relevant I2C address

void setup() {
    pinMode(PIN_D2, INPUT_PULLUP);                                        // Setting Arduino Digital Pin 2 to INPUT with pull-up resistors (for Alert testing)
    Serial.begin(9600);                                                   // Initiallizes the Serial Communications Port (at 9600bd)
    Wire.begin();                                                         // Initiallizes the I2C Communications bus
    while(!Serial);                                                       // Waits for Serial Port to initialize
    Serial.print(F("\nMCP9802 LIBRARY TESTING\n"));
    printDivider();
    Serial.print(F("\nOPENNING SERIAL PORT\n"));
    Serial.print(F("\nSerial Port is Open\n"));
    printDivider();
    runTests();
}

void loop() {}

void runTests() {
    Serial.print(F("\nINITIALIZING TESTS\n"));
    testPingDevice();
    printDivider();
    Serial.print(F("\nGETTING CONFIGURATION\n"));
    testGetConfigData();
    printDivider();
    Serial.print(F("\nGETTING REGISTERS DATA (TEMPERATURE / HYSTERESIS / LIMIT)\n"));
    testGetRegData();
    printDivider();
    Serial.print(F("\nSETTING CONFIGURATION\n"));
    testSetConfigData();
    printDivider();
    Serial.print(F("\nSETTING REGISTERS (HYSTERESIS / LIMIT)\n"));
    testSetRegData();
    printDivider();
    Serial.print(F("\nGETTING SINGLE-SHOT CONVERSION\n"));
    testSingleConversion();
    printDivider();
    Serial.print(F("\nTESTING ALERT FUNCTIONALITY\n"));
    testAlertFunctionality();
    printDivider();
    Serial.print(F("\nTESTING DEVICE RESET\n"));
    testReset();
    printDivider();
}

void testPingDevice() {
    Serial.print(F("\nSearching for device...Device "));
    Serial.print(MCP9802.ping() ? "Not Found\n" : "Found!\n");
    quickDelay();
}

void testGetConfigData() {
    testGetAlertType();
    testGetAlertMode();
    testGetFaultQueue();
    testGetResolution();
    testGetConMode();
    testGetTempUnit();
}

void testGetAlertType() {
    Serial.print(F("\nALERT TYPE:\t\t"));
    Serial.print(MCP9802.getAlertType() ? "INTERRUPT\n" : "COMPARATOR\n");
    quickDelay();
}

void testGetAlertMode() {
    Serial.print(F("\nALERT MODE:\t\t"));
    Serial.print(MCP9802.getAlertMode() ? "ACTIVE-HIGH\n" : "ACTIVE-LOW\n");
    quickDelay();
}

void testGetFaultQueue() {
    byte fqVal = MCP9802.getFaultQueue();
    Serial.print("\nFAULT QUEUE:\t\t");
    Serial.print(fqVal);
    Serial.print(fqVal == 1 ? " FAULT\n" : " FAULTS\n");
    quickDelay();
}

void testGetResolution() {
    Serial.print(F("\nRESOLUTION:\t\t"));
    Serial.print(MCP9802.getResolution());
    Serial.print(F("-BIT\n"));
    quickDelay();
}

void testGetConMode() {
    Serial.print(F("\nCONVERSION MODE:\t"));
    Serial.print(MCP9802.getConMode() ? "SINGLE-SHOT\n" : "CONTINUOUS\n");
    quickDelay();
}

void testGetTempUnit() {
    Serial.print(F("\nTEMPERATURE UNIT:\t"));
    Serial.print(MCP9802.getTempUnit() ? "FAHRENHEIT\n" : "CELSIUS\n");
}

void testGetRegData() {
    Serial.print(F("\nGETTING TEMPERATURE READINGS\n"));
    testGetTempReadings();
    printDivider();
    Serial.print(F("\nGETTING HYSTERESIS\n"));
    testGetHystSetting();
    printDivider();
    Serial.print(F("\nGETTING LIMIT\n"));
    testGetLimitSetting();
}

void testGetTempReadings() {
    testGetTempC16();
    testGetTempC();
    testSwitchTempUnit();
    testGetTempF16();
    testGetTempF();
    testSwitchTempUnit();
}

void testGetTempC16() {
    tempC16 = MCP9802.getTemp16();
    Serial.print(F("\nTemp (Cx16): \t"));
    Serial.println(tempC16);
    quickDelay();
}

void testGetTempC() {
    tempC = MCP9802.getTemp();
    Serial.print(F("\nTemp (C): \t"));
    Serial.println(tempC, 4);
    quickDelay();
}

void testGetTempF16() {
    tempF16 = MCP9802.getTemp16();
    Serial.print(F("\nTemp (Fx16): \t"));
    Serial.println(tempF16);
    quickDelay();
}

void testGetTempF() {
    tempF = MCP9802.getTemp();
    Serial.print(F("\nTemp (F): \t"));
    Serial.println(tempF, 4);
    quickDelay();
}

void testGetHystSetting() {
    testGetHystC16();
    testGetHystC();
    testSwitchTempUnit();
    testGetHystF16();
    testGetHystF();
    testSwitchTempUnit();
}

void testGetHystC16() {
    hystC16 = MCP9802.getHyst16();
    Serial.print(F("\nHyst (Cx16): \t"));
    Serial.println(hystC16);
    quickDelay();
}

void testGetHystC() {
    hystC = MCP9802.getHyst();
    Serial.print(F("\nHyst (C): \t"));
    Serial.println(hystC, 1);
    quickDelay();
}

void testGetHystF16() {
    hystF16 = MCP9802.getHyst16();
    Serial.print(F("\nHyst (Fx16): \t"));
    Serial.println(hystF16);
    quickDelay();
}

void testGetHystF() {
    hystF = MCP9802.getHyst();
    Serial.print(F("\nHyst (F): \t"));
    Serial.println(hystF, 1);
    quickDelay();
}

void testGetLimitSetting() {
    testGetLimitC16();
    testGetLimitC();
    testSwitchTempUnit();    
    testGetLimitF16();
    testGetLimitF();
    testSwitchTempUnit();
}

void testGetLimitC16() {
    limitC16 = MCP9802.getLimit16();
    Serial.print(F("\nLimit (Cx16): \t"));
    Serial.println(limitC16);
    quickDelay();
}

void testGetLimitC() {
    limitC = MCP9802.getLimit();
    Serial.print(F("\nLimit (C): \t"));
    Serial.println(limitC, 1);
    quickDelay();
}

void testGetLimitF16() {
    limitF16 = MCP9802.getLimit16();
    Serial.print(F("\nLimit (Fx16): \t"));
    Serial.println(limitF16);
    quickDelay();
}

void testGetLimitF() {
    limitF = MCP9802.getLimit();
    Serial.print(F("\nLimit (F): \t"));
    Serial.println(limitF, 1);
    quickDelay();
}

void testSetConfigData() {
    testSetAlertType();
    testSetAlertMode();
    testSetFaultQueue();
    testSetResolution();
    testSetConMode();
    testSetTempUnit();
}

void testSetAlertType() {
    alert_type_t alertTypeParams[2] = { INT, COMP };
    for (byte i=0; i<2; i++) {
        Serial.print(F("\nSetting Alert Type to:\t"));
        i ? Serial.print("COMPARATOR") : Serial.print("INTERRUPT");
        MCP9802.setAlertType(alertTypeParams[i]);
        Serial.print(F("...DONE\n"));
        testGetAlertType();
        quickDelay();
    }
}

void testSetAlertMode() {
    alert_mode_t alertModeParams[2] = { ACTIVE_HIGH, ACTIVE_LOW };
    for (byte i=0; i<2; i++) {
        Serial.print(F("\nSetting Alert Mode to:\t"));
        i ? Serial.print("ACTIVE-LOW") : Serial.print("ACTIVE-HIGH");
        MCP9802.setAlertMode(alertModeParams[i]);
        Serial.print(F("...DONE\n"));
        testGetAlertMode();
        quickDelay();
    }
}

void testSetFaultQueue() {
    fault_queue_t fqParams[4] = { FQ2, FQ4, FQ6, FQ1 };
    for (byte i=0; i<4 ; i++) {
        Serial.print(F("\nSetting Fault Queue to:\t"));     
        fqParams[i] == 0 ? Serial.print(F("1")) : Serial.print(fqParams[i] >> 2);
        fqParams[i] == 0 ? Serial.print(F(" FAULT")) : Serial.print(F(" FAULTS"));
        MCP9802.setFaultQueue(fqParams[i]);
        Serial.print(F("...DONE\n"));
        testGetFaultQueue();
        quickDelay();     
    }
}

void testSetResolution() {
    resolution_t resParams[4] = { RES_10, RES_11, RES_12, RES_9 };
    for (byte i=0; i<4 ; i++) {
        Serial.print(F("\nSetting Resolution to:\t"));     
        resParams[i] == 0 ? Serial.print(F("9")) : Serial.print((resParams[i] >> 5) + 9);
        Serial.print(F("-BIT"));
        MCP9802.setResolution(resParams[i]);
        Serial.print(F("...DONE\n"));
        testGetResolution();
        quickDelay();
    }
}

void testSetConMode() {
    con_mode_t conModeParams[2] = { SINGLE, CONT };
    for (byte i=0; i<2; i++) {
        Serial.print(F("\nSetting Conversion Mode to: "));
        i ? Serial.print("CONTINUOUS") : Serial.print("SINGLE-SHOT");
        MCP9802.setConMode(conModeParams[i]);
        Serial.print(F("...DONE\n"));
        testGetConMode();
        quickDelay();
    }
}

void testSetTempUnit() {
    temp_unit_t tempUnitParams[2] = { FAHRENHEIT, CELSIUS };
    for (byte i=0; i<2; i++) {
        Serial.print(F("\nSetting Temperature Unit to: "));
        i ? Serial.print("CELSIUS") : Serial.print("FAHRENHEIT");
        MCP9802.setTempUnit(tempUnitParams[i]);  
        Serial.print(F("...DONE\n"));
        testGetTempUnit();
        quickDelay();
    }
}

void testSetRegData() {
   testSettingHyst();
   printDivider();
   testSettingLimit();
}

void testSettingHyst() {
   Serial.print(F("\nSETTING HYSTERESIS (CELSIUS)\n"));
   testSetHystC();
   testSwitchTempUnit();
   Serial.print(F("\n\nSETTING HYSTERESIS (FAHRENHEIT)\n"));
   testSetHystF();
   testSwitchTempUnit();
}

void testSetHystC() { 
    float hystVal[8] = { 57, -32, 43.5, 21.1, -11.6, 1640, -782, DEFAULT_HYST };
    String valStr = "\nCurrent Hysteresis:\t";
    valStr += String(MCP9802.getHyst(), 0);
    Serial.print(valStr + "C\n");
    for (byte i=0; i<8; i++) {
        valStr = "\nSetting Hysteresis to:\t";
        if (i < 2) {
            valStr += String((int)hystVal[i]);
            MCP9802.setHyst(hystVal[i]);
            valStr += "C...DONE\n\nNew Hysteresis:\t\t";
            valStr += String((int)MCP9802.getHyst());
        } else if (i == 5 || i == 6) {
            valStr += String((hystVal[i] / 16.0), 1);
            MCP9802.setHyst16(hystVal[i]);
            valStr += "C...DONE\n\nNew Hysteresis:\t\t";
            valStr += String(MCP9802.getHyst(), 1);
        } else if (i == 7) {
            valStr += String((int)hystVal[i]);
            MCP9802.setHyst((int)hystVal[i]);
            valStr += "C...DONE\n\nNew Hysteresis:\t\t";
            valStr += String((int)MCP9802.getHyst());
        } else {
            valStr += String(hystVal[i], 1);
            MCP9802.setHyst(hystVal[i]);
            valStr += "C...DONE\n\nNew Hysteresis:\t\t";
            valStr += String(MCP9802.getHyst(), 1);
        }
        Serial.print(valStr + "C\n");
        quickDelay();
    }
}

void testSetHystF() {
    float hystVal[8] = { 134, -66, 110.3, 70.7, -54.9, 1043, -491, DEFAULT_HYST };
    String valStr = "\nCurrent Hysteresis:\t";
    valStr += String(MCP9802.getHyst(), 0);
    Serial.print(valStr + "F\n");
   for (byte i=0; i<8; i++) {
        valStr = "\nSetting Hysteresis to:\t";
        if (i == 5 || i == 6) {
            valStr += String((hystVal[i] / 16.0), 1);
            MCP9802.setHyst16(hystVal[i]);
            valStr += "F...DONE\n\nNew Hysteresis:\t\t";
            valStr += String(MCP9802.getHyst(), 1);
        } else {
           valStr += String(hystVal[i], 1);
            MCP9802.setHyst(hystVal[i]);
            valStr += "F...DONE\n\nNew Hysteresis:\t\t";
            valStr += String(MCP9802.getHyst(), 1);
        }
        Serial.print(valStr + "F\n");
        quickDelay(); 
    }
}

void testSettingLimit() {
    Serial.print(F("\nSETTING LIMIT (CELSIUS)\n"));
    testSetLimitC();
    testSwitchTempUnit();   
    Serial.print(F("\n\nSETTING LIMIT (FAHRENHEIT)\n"));
    testSetLimitF();
    testSwitchTempUnit();    
}

void testSetLimitC() {
    float limitVal[8] = { 64, -16, 35.5, 17.8, -24.5, 1510, -631, DEFAULT_LIMIT };
    String valStr = "\nCurrent Limit:\t\t";
    valStr += String(MCP9802.getLimit(), 0);
    Serial.print(valStr + "C\n");
    for (byte i=0; i<8; i++) {
        valStr = "\nSetting Limit to:\t";
        if (i < 2) {
            valStr += String((int)limitVal[i]);
            MCP9802.setLimit(limitVal[i]);
            valStr += "C...DONE\n\nNew Limit:\t\t";
            valStr += String((int)MCP9802.getLimit());
        } else if (i == 5 || i == 6) {
            valStr += String((limitVal[i] / 16.0), 1);
            MCP9802.setLimit16(limitVal[i]);
            valStr += "C...DONE\n\nNew Limit:\t\t";
            valStr += String(MCP9802.getLimit(), 1);
        } else if (i == 7) {
            valStr += String((int)limitVal[i]);
            MCP9802.setLimit((int)limitVal[i]);
            valStr += "C...DONE\n\nNew Limit:\t\t";
            valStr += String((int)MCP9802.getLimit());
        } else {
            valStr += String(limitVal[i], 1);
            MCP9802.setLimit(limitVal[i]);
            valStr += "C...DONE\n\nNew Limit:\t\t";
            valStr += String(MCP9802.getLimit(), 1);
        }
        Serial.print(valStr + "C\n");
        quickDelay();
    }
}

void testSetLimitF() {
    float limitVal[8] = { 255, -12, 213.5, 167.2, -24.7, 1872, -966, DEFAULT_LIMIT };
    String valStr = "\nCurrent Limit:\t\t";
    valStr += String(MCP9802.getLimit(), 0);
    Serial.print(valStr + "F\n");
    for (byte i=0; i<8; i++) {
        valStr = "\nSetting Limit to:\t";
        if (i == 5 || i == 6) {
            valStr += String((limitVal[i] / 16.0), 1);
            MCP9802.setLimit16(limitVal[i]);
            valStr += "F...DONE\n\nNew Limit:\t\t";
            valStr += String(MCP9802.getLimit(), 1);
        } else {
            valStr += String(limitVal[i], 1);
            MCP9802.setLimit(limitVal[i]);
            valStr += "F...DONE\n\nNew Limit:\t\t";
            valStr += String(MCP9802.getLimit(), 1);
        }
        Serial.print(valStr + "F\n");
        quickDelay();
    }
}

void testSingleConversion() {
    Serial.print(F("\nChanging Conversion Mode to SINGLE-SHOT..."));
    MCP9802.setConMode(SINGLE);
    Serial.print(F("DONE\n"));
    testGetSingleConC16();
    testGetSingleConC();  
    testSwitchTempUnit();
    testGetSingleConF16();
    testGetSingleConF();       
    testSwitchTempUnit();    
    quickDelay();
}

void testGetSingleConC16() {
    tempC16 = MCP9802.singleCon16();
    Serial.print(F("\nTemp (Cx16): \t"));
    Serial.println(tempC16);
    quickDelay();  
}

void testGetSingleConC() {
    tempC = MCP9802.getTemp();
    Serial.print(F("\nTemp (C): \t"));
    Serial.println(tempC, 4);
    quickDelay();
}

void testGetSingleConF16() {
    tempF16 = MCP9802.getTemp16();
    Serial.print(F("\nTemp (Fx16): \t"));
    Serial.println(tempF16);
    quickDelay();
}

void testGetSingleConF() {
    tempF = MCP9802.getTemp();
    Serial.print(F("\nTemp (F): \t"));
    Serial.println(tempF, 4);
    quickDelay();
}

void testAlertFunctionality() {
     MCP9802.reset();
     for (byte i=0; i< 2; i++) {
         MCP9802.setAlertMode(alertModeParams[i]);
         Serial.print(F("\nTESTING ACTIVE-"));
         Serial.print(i ? "HIGH" : "LOW");
         Serial.print(F(" SETTINGS\n\nInitial Conditions:\n"));
         testGetConditions();
         testGetAlertType();
         testGetAlertMode();
         testGetTempUnit();
         testAlertState(alertModeParams[i], OFF);
         testSimulateTestConditions();
         testGetConditions();
         testAlertState(alertModeParams[i], ON);
         Serial.print(F("\nVerifying Alert Presistance after Entering Shut-Down Mode\n"));
         Serial.print(F("\nSwitching Device to Shut-Down Mode..."));
         MCP9802.setConMode(SINGLE);
         Serial.print(F("DONE\n"));
         testAlertState(alertModeParams[i], ON);
         Serial.print(F("\nSimulating Return to Normal Conditions..."));
         MCP9802.setConMode(CONT);
         MCP9802.setLimit(tempC + 20);
         MCP9802.setHyst(tempC + 10);
         Serial.print(F("DONE\n"));
         testAlertState(alertModeParams[i], OFF);
         if (!i) printDivider();
     }
     MCP9802.reset();
}

void testGetConditions() {
     testGetTempC();
     testGetLimitC();
     testGetHystC();
}

void testAlertState(alert_mode_t alertMode, alert_state_t alertState) {
     Serial.print(F("\nEXPECTED PIN D2 STATE:\t"));
     if (alertMode) Serial.print(alertState == OFF ? "LOW (ALARM OFF)\n" : "HIGH (ALARM ON)\n");
     else Serial.print(alertState == OFF ? "HIGH (ALARM OFF)\n" : "LOW (ALARM ON)\n");
     Serial.print(F("\nACTUAL PIN D2 STATE:\t"));
     if (alertMode) Serial.print(digitalRead(PIN_D2) ? "HIGH (ALARM ON)\n" : "LOW (ALARM OFF)\n");
     else Serial.print(digitalRead(PIN_D2) ? "HIGH (ALARM OFF)\n" : "LOW (ALARM ON)\n");
}

void testSimulateTestConditions() {
     Serial.print(F("\nSimulating Alert Conditions..."));
     MCP9802.setLimit(tempC - 10);
     MCP9802.setHyst(tempC - 20);
     Serial.print(F("DONE\n"));
}

void testReset() {
    Serial.print(F("\nCurrent Settings:\n"));
    testGetConfigData();
    Serial.print(F("\nCreating New Settings..."));
    MCP9802.setAlertType(INT);
    MCP9802.setAlertMode(ACTIVE_HIGH);
    MCP9802.setFaultQueue(FQ4);
    MCP9802.setResolution(RES_11);
    MCP9802.setConMode(SINGLE);
    MCP9802.setTempUnit(FAHRENHEIT);
    MCP9802.setLimit(52.5);
    MCP9802.setHyst(49.5);   
    Serial.print(F("DONE\n\nCurrent Settings:\n"));
    testGetConfigData();
    Serial.print(F("\nResetting Device to Default Settings..."));
    MCP9802.reset();
    Serial.print(F("DONE\n\nCurrent Settings:\n"));
    testGetConfigData();
}

void printDivider() {
    Serial.print(F("\n--------------------------------\n"));
}

void quickDelay() {
    delay(50);
}

void testSwitchTempUnit() {
    MCP9802.setTempUnit((MCP9802.getTempUnit() == CELSIUS) ? FAHRENHEIT : CELSIUS);
}

