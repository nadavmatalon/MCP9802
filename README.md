
# MCP9802 AVR DRIVER LIBRARY

## Introduction

The MCP9802 is a 9 to 12-Bit Single-Channel Temperature Sensor with Hysteresis & Alert capabilities and hardware I2C interface.

This library contains a robust driver for the MCP9802 which exposes all its Configuration, Hysteresis, Limit & Alert Settings; working in degrees Celsius or Fahrenheit; and, Integer or Floating-Point data manipulation.

## Repository Contents

* **MCP9802.h** - Header file of the library.
* **MCP9802.cpp** - Compilation file of the library.
* **DegreeConverter.h** - Header file for Library helper methods.
* **DegreeConverter.cpp** - Compilation file for Library helper methods.
* **/examples** - 
- **/MCP9802_Test/MCP9802_Test.ino** - A basic sketch for testing the MCP9802 is hooked-up and operating correctly.
- **MCP9802_Usage/MCP9802_Usage.ino** - A much more detailed sketch offering a complete usage illustration, as well as a rubust testing mechanism.
- **/MCP9802_Info/MCP9802_Info.ino** - A short sketch showing how to generate a printable device information String of the MCP9802's current settings (i.e. Configuration, Limit &anp; Hysteresis registers).
* **/extras** - Complementary documentation (End-User License, etc.)
* **keywords.txt** - Keywords for this library which will be highlighted in sketches within the Arduino IDE. 
* **library.properties** - General library properties for the Arduino package manager.
* **README.md** - The readme file for this library.
* **library.json** - JSON file for the Arduino package manager.

## HOOK-UP

* __PIN 1 (VCC)__ - Connect VCC to the Arduino's 5V output
* __PIN 2 (GND)__ - Connect GND to the Arduino's GND
* __PIN 3 (ALERT)__ - Connect ALERT to the Arduino's Digital Pin 2
* __PIN 4 (SCL)__ - Conntect SCL to the Arduino's PIN A5 with a 2K2 pull-up resistor
* __PIN 5 (SDA)__ - Conntect SDA to the Arduino's PIN A4 with a 2K2 pull-up resistor
* __DECOUPING__: Connect a 0.1uF Ceramic Capacitor between the MCP9802's VCC & GND PINS

## General Notes

__1. I2C Communications Library__

This library uses the '[WSWire](https://github.com/steamfire/WSWireLib/tree/master/Library/WSWire)' library for I2C communication between the contoller IC (Master) and the MCP9802 (Slave), so it is __NECESSARY__ to have it installed prior to using the current libraty. 

Alternatively, if you wish to use the '[Wire](https://github.com/arduino/Arduino/tree/master/hardware/arduino/avr/libraries/Wire)' - or any other I2C library for that matter - simply change the following line the the __MCP9802.h__ file:
```
#include <WSWire.h>
```
to this:
```
#include <Wire.h>  // or to whatever I2C library name you are using.
```
As noted above, whichever library you intend to use for this purpose __must be alredy installed__ for the MCP9802 library to work.

__2. Shutdown & Conversion Mode__

The first bit of the configuration byte controls the device mode of operation, namely: ON, in which the device operates in 'CONTINUOUS' mode,
or OFF - or more precisely HYBERNATE (as I2C communication remains active), in which the device operates in 'SINGLE-SHOT' nmode. As such, setting 
the 'CONVERSION MODE' of the device to 'CONTINUOUS' will effectively ensure that it is 'ON', while setting it to 'SINGLE-SHOT' mode will turn it OFF 
(or more accurately, put it in hybernate mode).

__3. Hysteresis & Limit Registers Resolution__

The Temperature register has a setteble range of 9 to 12-BIT (0.5 to 0.0625 degrees Celsius respectively). However, both the LIMIT and HYSTERESIS 
registers only have a 9-BIT fixed resolution. This means these registers can only be set with a maximum resolution of 0.5 degrees Celsius. 
Hence, while the relevant functions (e.g. setTempC(); ) will happily accept any float value within the premmitted parameter range (-55C to 125C) 
for either of these two registers, this float value will be automatically rounded to the nearest 0.5C.

__4. Degrees Celsius & Fahrenheit__

The libraty offers the option of getting/setting all termperature values (Abmient [read-only], Limit [read-write] and/or Hysteresis [read-write]) 
in either degrees Celsuis or Fahrenheit. These can be obtained in a floating point format or, if prefered, in a x16 integer format to speed-up 
conversion calculations and save memory space. 

__5. Degress Fahrenheit Precision Limitations__

As the MCP9802 was designed primerily to work in a degree Celsuis scheme, all Fahrenheit values obtained (or custom set by the user) 
can only represent approximations of the precise Celsius values generated or stored by the device. This limitation is particualarly 
noticable when setting the LIMIT or HYSTERESIS registers to custom Fahrenheit values, as a double operation takes place, namely: 
rounding the given value to the nearest 0.5 degree Celisus and the subsequent conversion of this figure to the equivalent Fahrenheit value.

__6. Alert Output__

The MCP9802's Alert functionality is based on an 'open collector' architecture which means it requires a pull-up resistor in order to work 
(this is true for both Alert Types, i.e. 'ACTIVE-LOW' and 'ACTIVE-HIGH). For the purposes of this testing sketch, the Atmega's (weak) internal 
pull-up resistor is used and so the only connection needed in this context is between the MCP9802's ALERT pin and the Arduino's Digital Pin D2. 
However, for any real-life use of the device, it is highly recommended to implement a suitable external pull-up resistor (typically 10K) 
hooked-up betweem the ALERT pin and VCC.

__7. Device Information String__

It is now possible to extend the MCP9802 Library to include a function for generating a pritable device information string showing all the 
relevant details about the devices current Configuration, Limit & Hysteresis settings. As the additional functionality comes at the cost 
of increased memory usage, it was implemented as an optional add-on rather than added directly to the core MCP9802 Library. See the 
[MCP9802_Info](https://github.com/nadavmatalon/MCP9802/blob/master/examples/MCP9802_Info/MCP9802_Info.ino) example sketch for detailed explanation and an actual usage demo.

## I2C ADDRESSES

Each ADS1110 has 1 of 8 possible I2C addresses (factory hardwired & recognized by its specific part number & top marking 
on the package itself):

| PART NO.        | BIN      | HEX  | DEC | MARKING |
|-----------------|----------|------|-----|---------|
| MCP9802A0T-M/OT | 01001000 | 0x48 | 72  | JKNN    |
| MCP9802A1T-M/OT | 01001001 | 0x49 | 73  | JLNN    |
| MCP9802A2T-M/OT | 01001010 | 0x4A | 74  | JMNN    |
| MCP9802A3T-M/OT | 01001011 | 0x4B | 75  | JPNN    |
| MCP9802A4T-M/OT | 01001100 | 0x4C | 76  | JQNN    |
| MCP9802A5T-M/OT | 01001101 | 0x4D | 77  | JRNN    |
| MCP9802A6T-M/OT | 01001110 | 0x4E | 78  | JSNN    |
| MCP9802A7T-M/OT | 01001111 | 0x4F | 79  | JTNN    |


## LIBRARY INSTALLATION & SETUP

Begin by installing the library either by using the Arduino IDE's Installation Wizard (Arduino Version >1.5) or simply download the library's ZIP folder from GITHUB, extract it, and copy the extraxcted folder to your Arduino 'libraries' folder.

Next, include the library at the top of the sketch as follows:

```
#include <MCP9802.h>
```

At this point you can construct a new MPC9802 object(s) by using the following command (at the top of the sketch after the 'include' line):

```
MCP9802 device_name(device_address);
```

>__NOTE__: replace the '__device_name__' above with a name of your choice. Also, make sure to replace the variable '__device_address__' with 
the specific I2C address of your device - see I2C ADDRESSES section above.


## LIBRARY METHODS

With the library installed & included in the sketch, and an MCP9802 object initiallized, the following functions are available 
(see the usage example sketch for a detailed implementation):

__Note About Methods' Return Values:__  
All 'get' methods return some sort of value (e.g. temp reading, hysteresis setting, etc.), while all 'set' methods return nothing. Nevertheless, ALL methods implicitly update the library's __I2C _comBuffer__ (=communication buffer) after each I2C transmission. The reason for this functional design is to maintain structural coherance between the 'get' and 'set' methods. As 'get' methods cannot return both the desired value and the I2C transmission's result simultaniously. Consequently, if the relevant value hasn't been obtained by a particular 'get' method, the user can simply check the content of the _comBuffer to see what error occured. Similarly, it is possible to check if a particular setting has been successfully applied via a 'set' method either by preforming the corresponding 'get' method - e.g. getHystC() after using setHystC() - or by checking the content of the _comBuffer (0 indicates a successful transmission, 1-6 indicate an error as listed below). 

__ping();__                                  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Searches for the MCP9802 at the pre-defined I2C Bus address &amp; returns byte with the relevant success/error code, as follows:  
0 ... Success (no error)  
1 ... Buffer overflow  
2 ... Address sent, NACK received  
3 ... Data send, NACK received  
4 ... Other error (lost bus arbitration, bus error, etc.)  
5 ... Timed-out while trying to become Bus Master  
6 ... Timed-out while waiting for data to be sent<br>
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;byte  

__getTempC16();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Returns current temperature reading in degrees Celsius times 16  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;int

__getTempC();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Returns current temperature reading in degrees Celsius  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;float

__getTempF16();__  
Parameters:&nbsp;&nbsp;&nbsp;None   
Description:&nbsp;&nbsp;&nbsp;Returns current temperature reading in degrees Fahrenheit times 16  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;int

__getTempF();__  
Parameters:&nbsp;&nbsp;&nbsp;None   
Description:&nbsp;&nbsp;&nbsp;Returns current temperature reading in degrees Fahrenheit    
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;float

__getHystC16();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Returns the current Hysteresis register value in degrees Celsius times 16  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;int

__getHystC();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Returns the current Hysteresis register value in degrees Celsius   
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;float

__getHystF16();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Returns the current Hysteresis register setting in degrees Fahrenheit times 16    
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;int

__getHystF();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Returns the current Hysteresis register setting in degrees Fahrenheit   
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;float

__getLimitC16();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Returns the current Limit register value in degrees Celsius times 16 
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;int

__getLimitC();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Returns the current Limit register value in degrees Celsius    
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;float

__getLimitF16();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Returns the current Limit register value in degrees Fahrenheit times 16  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;int

__getLimitF();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Returns the current Limit register value in degrees Fahrenheit   
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;float

__getAlertType();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Returns the current alert type setting (0 = COMPARATOR / 1 = INTERRUPT)  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;byte

__getAlertMode();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Returns the current alert mode setting (0 = ACTIVE-LOW / 1 = ACTIVE-HIGH)  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;None

__getFaultQueue();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Returns the current Fault Queue setting (1 / 2 / 4 / 6 fault readings - i.e. below Hysteresis or above the Limit - before activating the alert)  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;byte
__getResolution();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Returns the current Resolution setting (9 = 9-BIT / 10 = 10-BIT / 11 = 11-BIT / 12 = 12-BIT)  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;byte

__getConMode();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Returns the current Conversion Mode setting (0 = CONTINUOUS / 1 = SINGLE-SHOT)  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;byte

__setHyst16();__  
Parameters:&nbsp;&nbsp;&nbsp;int (range: -880 to +2000)  
Description:&nbsp;&nbsp;&nbsp;Sets the Hysteresis register value in degrees Celsius times 16   
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;None

__setHystC();__  
Parameters:&nbsp;&nbsp;&nbsp;int / float (range: -55.0 to +125.0)  
Description:&nbsp;&nbsp;&nbsp;Sets the Hysteresis register value in degrees Celsius  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;None

__setHystF16();__  
Parameters:&nbsp;&nbsp;&nbsp;int (range: 1072 - to +4112)  
Description:&nbsp;&nbsp;&nbsp;Sets the Hysteresis register value in degrees Fahrenheit times 16  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;None

__setHystF();__  
Parameters:&nbsp;&nbsp;&nbsp;int / float (range: -67.0 to +257.0)  
Description:&nbsp;&nbsp;&nbsp;Sets the Hysteresis register value in degrees Fahrenheit  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;None

__setLimitC16();__  
Parameters:&nbsp;&nbsp;&nbsp;int (range: -880 to +2000)  
Description:&nbsp;&nbsp;&nbsp;Sets the Limit register value in degrees Celsius times 16  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;None

__setLimitC();__  
Parameters:&nbsp;&nbsp;&nbsp;int / float (range: -55.0  to +125.0)  
Description:&nbsp;&nbsp;&nbsp;Sets the Limit register value in degrees Celsius   
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;None

__setLimitF16();__  
Parameters:&nbsp;&nbsp;&nbsp;int (range: -1072 to 4112)  
Description:&nbsp;&nbsp;&nbsp;Sets the Limit register value in degrees Fahrenheit times 16  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;None

__setLimitF();__  
Parameters:&nbsp;&nbsp;&nbsp;int / float (range: -67.0 to +257.0)  
Description:&nbsp;&nbsp;&nbsp;Sets the Limit register value in degrees Fahrenheit  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;None

__setAlertType();__  
Parameters:&nbsp;&nbsp;&nbsp;COMP / INT  
Description:&nbsp;&nbsp;&nbsp;Sets the alert output type (COMPARATOR / INTERRUPT)  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;None

__setAlertMode();__  
Parameters:&nbsp;&nbsp;&nbsp;ACTIVE_LOW / ACTIVE_HIGH  
Description:&nbsp;&nbsp;&nbsp;Sets the alert output mode (ACTIVE-LOW / ACTIVE-HIGH)  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;None

__setFaultQueue();__  
Parameters:&nbsp;&nbsp;&nbsp;FQ1 / FQ2 / FQ4 / FQ6  
Description:&nbsp;&nbsp;&nbsp;Sets the Fault Queue value (FQ1 = 1 Fault / FQ2 = 2 Faults / FQ4 = 4 Faults / FQ6 = 6 Faults)  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;None

__setResolution();__  
Parameters:&nbsp;&nbsp;&nbsp;RES_9 / RES_10 / RES_11 / RES_12  
Description:&nbsp;&nbsp;&nbsp;Sets the Resolution value (RES_9 = 9-BIT / RES_10 = 10-BIT / RES_11 = 11-BIT / RES_12 = 12-BIT)  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;None

__setConMode();__  
Parameters:&nbsp;&nbsp;&nbsp;CONT / SINGLE  
Description:&nbsp;&nbsp;&nbsp;Sets the Conversion Mode (CONTINUOUS / SINGLE-SHOT)  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;None

__reset();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Resets the device to power-up default settings (except for the TEMP Register which is Read-Only), as follows:
* REG POINTER:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0x00 (TEMP REGISTER)
* CONFIG:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0x00
* DEVICE MODE:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ON
* ALERT OUTPUT TYPE:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;COMPARATOR
* ALERT OUTPUT MODE:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ACTIVE-LOW
* FAULT-QUEUE:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1 FAULT
* RESOLUTION:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;9-BIT
* CONVERSION MODE:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;CONTINUOUS
* LIMIT (DATA):&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0xA000 (80°C)
* HYST (DATA):&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0x9600 (75°C)<br>

Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;None

__singleConC16();__  
Parameters:&nbsp;&nbsp;&nbsp;None   
Conditions:&nbsp;&nbsp;&nbsp;&nbsp;Works only in __Single-Shot__ mode  
Description:&nbsp;&nbsp;&nbsp;Carries out a single conversion &amp; returns a temperature reading in degrees Celsius times 16  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;int

__singleConC();__  
Parameters:&nbsp;&nbsp;&nbsp;None   
Conditions:&nbsp;&nbsp;&nbsp;&nbsp;Works only in __Single-Shot__ mode  
Description:&nbsp;&nbsp;&nbsp;Carries out a single conversion &amp; returns a temperature reading in degrees Celsius  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;float

__singleConF16();__  
Parameters:&nbsp;&nbsp;&nbsp;None   
Conditions:&nbsp;&nbsp;&nbsp;&nbsp;Works only in __Single-Shot__ mode  
Description:&nbsp;&nbsp;&nbsp;Carries out a single conversion &amp; returns a temperature reading in degrees Fahrenheit times 16  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;int

__singleConF();__  
Parameters:&nbsp;&nbsp;&nbsp;None   
Conditions:&nbsp;&nbsp;&nbsp;&nbsp;Works only in __Single-Shot__ mode  
Description:&nbsp;&nbsp;&nbsp;Carries out a single conversion &amp; returns a temperature reading in degrees Fahrenheit  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;float

__getComResult();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;Returns the latest I2C Communication result (see Success/Error codes above)  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;byte

__Destructor__  
If, for whatever reason, you wish to destruct an existing MCP9802 object, you can use the following line to do so:

```
~MCP9802 device_name();
```
>__NOTE__: replace the '__device_name__' above with the name of your MCP9802 device.

###Extended Functionality*

(* requires including the 'MCP9802InfoStr_h' file at the top of the sketch)

__MCP9802InfoStr();__  
Parameters:&nbsp;&nbsp;&nbsp;Name of an initialized MCP9802 instance  
Description:&nbsp;&nbsp;Returns printable string containing detailed information about the device's current settings 
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;String

## RUNNING THE EXAMPLE SKETCHES

1) Hook-up the MCP9802 to the Arduino as explained above.
2) If you like, connect a LED with an appropriate series resistor between the MCP9802's ALERT pin (PIN 3) and ground
3) Upload the relevant example sketch to the Arduino.
4) Open the Serial Communications Window (make sure the baud-rate is set to 9600 or change them in the sketch to match your Serial Port buad rate).


## BUG REPORTS

Please report any issues/bugs/suggestions at the 'Issues' section of this Github repository.


## LICENSE

This library is a free software; you can redistribute it and/or modify it under the terms of the 
GNU General Public License as published by the Free Software Foundation; either version 3.0 of 
the License, or any later version.This program is distributed in the hope that it will be useful, 
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
PARTICULAR PURPOSE. See the GNU General Public License for more details.


