/* 
  MCP9802 LIBRARY - COMPLETE USAGE EXAMPLE
  ----------------------------------------

  INTRODUCTION
  ------------ 

  This sketch offers a complete usage illustration, as well as a rubust testing mechanism, for the MCP9802.

  The sketch exposes all available configuration settings, data reading, data setting, and alarm functionality encapsulated 
  within the MCP9802.

  INPORTANT: The current library depends on the Arduino IDE's native 'Wire' library for I2C communication between the Arduino (Master) and the MCP9802 (Slave).

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
*/

#include "MCP9802.h"

const byte PIN_D2 = 2;                                                    // Arduino PIN D2 (PIN 2) connected to the MCP9802's ALERT Pin
const byte DEV_ADDR = 0x48;                                               // DEC: 72 - I2C address of the MCP9802 (Change as needed)

byte    config;                                                           // Variables to hold configuration register data
int     tempC16, tempF16, hystC16, hystF16, limitC16, limitF16;           // Variables to hold conversion data
float   tempC, tempF, hystC, hystF, limitC, limitF;                       // Variables to hold conversion data
String  deviceMode, alertType, alertMode, conMode;                        // Variables to hold configuration data
alert_mode_t alertModeParams[2] = { ACTIVE_LOW, ACTIVE_HIGH };            // Array for Alert testing

typedef enum:byte {                                                       // Complemetary type definition to facilitate redablily of the Alert tests
    OFF = 0,
    ON  = 1
} alert_state_t;

MCP9802 mcp9802(DEV_ADDR);                                            // Constructs a new MCP9802 object with the relevant I2C address

void setup() {
    pinMode(PIN_D2, INPUT_PULLUP);                                        // Setting Arduino Digital Pin 2 to INPUT with pull-up resistors (for Alert testing)
    Serial.begin(9600);                                                   // Initiallizes the Serial Communications Port (at 9600bd)
    Wire.begin();                                                         // Initiallizes the I2C Communications bus
    while(!Serial);                                                       // Waits for Serial Port to initialize
    printDivider();
    runTests();
}

void loop() {}

void runTests() {
    Serial.print(F("\nMCP9802 LIBRARY TESTING\n"));
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
    Serial.print(mcp9802.ping() ? "Not Found\n" : "Found!\n");
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
    Serial.print(mcp9802.getAlertType() ? "INTERRUPT\n" : "COMPARATOR\n");
    quickDelay();
}

void testGetAlertMode() {
    Serial.print(F("\nALERT MODE:\t\t"));
    Serial.print(mcp9802.getAlertMode() ? "ACTIVE-HIGH\n" : "ACTIVE-LOW\n");
    quickDelay();
}

void testGetFaultQueue() {
    byte fqVal = mcp9802.getFaultQueue();
    Serial.print("\nFAULT QUEUE:\t\t");
    Serial.print(fqVal);
    Serial.print(fqVal == 1 ? " FAULT\n" : " FAULTS\n");
    quickDelay();
}

void testGetResolution() {
    Serial.print(F("\nRESOLUTION:\t\t"));
    Serial.print(mcp9802.getResolution());
    Serial.print(F("-BIT\n"));
    quickDelay();
}

void testGetConMode() {
    Serial.print(F("\nCONVERSION MODE:\t"));
    Serial.print(mcp9802.getConMode() ? "SINGLE-SHOT\n" : "CONTINUOUS\n");
    quickDelay();
}

void testGetTempUnit() {
    Serial.print(F("\nTEMPERATURE UNIT:\t"));
    Serial.print(mcp9802.getTempUnit() ? "FAHRENHEIT\n" : "CELSIUS\n");
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
    testGetTempC();
    testGetTempF();
}

void testGetTempC() {
    tempC = mcp9802.getTemp();
    Serial.print(F("\nTemp (C): \t"));
    Serial.println(tempC, 4);
    quickDelay();
}

void testGetTempF() {
    mcp9802.setTempUnit(FAHRENHEIT);
    tempF = mcp9802.getTemp();
    Serial.print(F("\nTemp (F): \t"));
    Serial.println(tempF, 4);
    mcp9802.setTempUnit(CELSIUS);
    quickDelay();
}

void testGetHystSetting() {
    testGetHystC();
    testGetHystF();
}

void testGetHystC() {
    hystC = mcp9802.getHyst();
    Serial.print(F("\nHyst (C): \t"));
    Serial.println(hystC, 4);
    quickDelay();
}

void testGetHystF() {
    mcp9802.setTempUnit(FAHRENHEIT);
    hystF = mcp9802.getHyst();
    Serial.print(F("\nHyst (F): \t"));
    Serial.println(hystF, 4);
    mcp9802.setTempUnit(CELSIUS);
    quickDelay();
}

void testGetLimitSetting() {
    testGetLimitC();
    testGetLimitF();
}

void testGetLimitC() {
    limitC = mcp9802.getLimit();
    Serial.print(F("\nLimit (C): \t"));
    Serial.println(limitC, 4);
    quickDelay();
}

void testGetLimitF() {    
    mcp9802.setTempUnit(FAHRENHEIT);
    limitF = mcp9802.getLimit();
    Serial.print(F("\nLimit (F): \t"));
    Serial.println(limitF, 4);
    mcp9802.setTempUnit(CELSIUS);
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
        mcp9802.setAlertType(alertTypeParams[i]);
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
        mcp9802.setAlertMode(alertModeParams[i]);
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
        mcp9802.setFaultQueue(fqParams[i]);
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
        mcp9802.setResolution(resParams[i]);
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
        mcp9802.setConMode(conModeParams[i]);
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
        mcp9802.setTempUnit(tempUnitParams[i]);
        Serial.print(F("...DONE\n"));
        testGetTempUnit();
        quickDelay();
    }
}

void testSetRegData() {
    testSetHyst();
    printDivider();
    testSetLimit();
}

void testSetHyst() {
    String valStr;
    float hystVal[5] = { 57, -32, 43.5, 21.1, -11.6 };
    for (byte i=0; i<2 ; i++) {
        Serial.print(F("\nSETTING HYSTERESIS ("));
        Serial.print(mcp9802.getTempUnit() ? "FAHRENHEIT)\n" : "CELSIUS)\n");
        for (byte j=0; j<5; j++) {
            valStr = "\nCurrent Hysteresis:\t";
            valStr += String(mcp9802.getHyst(), 4);
            valStr += (mcp9802.getTempUnit() ? "F" : "C");
            Serial.print(valStr);
            valStr = "\n\nSetting Hysteresis to:\t";
            valStr += String(hystVal[j], 4);
            mcp9802.setHyst(hystVal[j]);
            valStr += (mcp9802.getTempUnit() ? "F" : "C");
            valStr += ("...DONE\n");
            Serial.print(valStr);
        }
        valStr = "\nCurrent Hysteresis:\t";
        valStr += String(mcp9802.getHyst(), 4);
        valStr += (mcp9802.getTempUnit() ? "F" : "C");
        Serial.print(valStr);
        Serial.print(F("\n"));
        mcp9802.setTempUnit(mcp9802.getTempUnit() ? CELSIUS : FAHRENHEIT);
    }
    mcp9802.setHyst(DEFAULT_HYST);
    quickDelay();
}

void testSetLimit() {
    String valStr;
    float limitVal[5] = { 64, -16, 35.5, 17.8, -24.1 };
    for (byte i=0; i<2 ; i++) {
        Serial.print(F("\nSETTING LIMIT ("));
        Serial.print(mcp9802.getTempUnit() ? "FAHRENHEIT)\n" : "CELSIUS)\n");
        for (byte j=0; j<5; j++) {
            valStr = "\nCurrent Limit:\t\t";
            valStr += String(mcp9802.getLimit(), 4);
            valStr += (mcp9802.getTempUnit() ? "F" : "C");
            Serial.print(valStr);
            valStr = "\n\nSetting Limit to:\t";
            valStr += String(limitVal[j], 4);
            mcp9802.setLimit(limitVal[j]);
            valStr += (mcp9802.getTempUnit() ? "F" : "C");
            valStr += ("...DONE\n");
            Serial.print(valStr);
        }
        valStr = "\nCurrent Limit:\t\t";
        valStr += String(mcp9802.getLimit(), 4);
        valStr += (mcp9802.getTempUnit() ? "F" : "C");
        Serial.print(valStr);
        Serial.print(F("\n"));
        mcp9802.setTempUnit(mcp9802.getTempUnit() ? CELSIUS : FAHRENHEIT);
    }
    mcp9802.setLimit(DEFAULT_LIMIT);
    quickDelay();
}

void testSingleConversion() {
    Serial.print(F("\nChanging Conversion Mode to SINGLE-SHOT..."));
    mcp9802.setConMode(SINGLE);
    Serial.print(F("DONE\n"));
    testGetSingleConC();
    mcp9802.setTempUnit(FAHRENHEIT);
    testGetSingleConF();
    mcp9802.setTempUnit(CELSIUS);
    quickDelay();
}

void testGetSingleConC() {
    tempC = mcp9802.getTemp();
    Serial.print(F("\nTemp (C): \t"));
    Serial.println(tempC, 4);
    quickDelay();
}

void testGetSingleConF() {
    tempF = mcp9802.getTemp();
    Serial.print(F("\nTemp (F): \t"));
    Serial.println(tempF, 4);
    quickDelay();
}

void testAlertFunctionality() {
     mcp9802.reset();
     for (byte i=0; i< 2; i++) {
         mcp9802.setAlertMode(alertModeParams[i]);
         Serial.print(F("\nTESTING ACTIVE-"));
         Serial.print(i ? "HIGH" : "LOW");
         Serial.print(F(" SETTINGS\n\nInitial Conditions:\n"));
         testGetTempC();
         quickDelay();
         mcp9802.setLimit(tempC + 20);
         quickDelay();
         mcp9802.setHyst(tempC + 10);
         quickDelay();
         testGetLimitC();
         testGetHystC();
         testGetAlertType();
         testGetAlertMode();
         testGetTempUnit();
         testAlertState(alertModeParams[i], OFF);
         testSimulateTestConditions();
         testGetTempC();
         testGetLimitC();
         testGetHystC();
         testAlertState(alertModeParams[i], ON);
         Serial.print(F("\nVerifying Alert Presistance after Entering Shut-Down Mode\n"));
         Serial.print(F("\nSwitching Device to Shut-Down Mode..."));
         mcp9802.setConMode(SINGLE);
         Serial.print(F("DONE\n"));
         testAlertState(alertModeParams[i], ON);
         Serial.print(F("\nSimulating Return to Normal Conditions..."));
         mcp9802.setConMode(CONT);
         mcp9802.setLimit(tempC + 20);
         mcp9802.setHyst(tempC + 10);
         Serial.print(F("DONE\n"));
         testAlertState(alertModeParams[i], OFF);
         if (!i) printDivider();
     }
     mcp9802.reset();
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
     mcp9802.setLimit(tempC - 10);
     mcp9802.setHyst(tempC - 20);
     Serial.print(F("DONE\n"));
}

void testReset() {
    Serial.print(F("\nCurrent Settings:\n"));
    testGetConfigData();
    Serial.print(F("\nCreating New Settings..."));
    mcp9802.setAlertType(INT);
    mcp9802.setAlertMode(ACTIVE_HIGH);
    mcp9802.setFaultQueue(FQ4);
    mcp9802.setResolution(RES_11);
    mcp9802.setConMode(SINGLE);
    mcp9802.setTempUnit(FAHRENHEIT);
    mcp9802.setLimit(52.5);
    mcp9802.setHyst(49.5);   
    Serial.print(F("DONE\n\nCurrent Settings:\n"));
    testGetConfigData();
    Serial.print(F("\nResetting Device to Default Settings..."));
    mcp9802.reset();
    Serial.print(F("DONE\n\nCurrent Settings:\n"));
    testGetConfigData();
}

void printDivider() {
    Serial.print(F("\n--------------------------------\n"));
}

void quickDelay() {
    delay(50);
}

