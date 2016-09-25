/*==============================================================================================================*
 
 @file     DegreeConverter.cpp
 @author   Nadav Matalon
 @license  GNU General Public License
 
 Function collection for converting Degrees Celsuis to Fahrenheit and vice-versa
 
 Rev 1.0 - First release (15.9.16)
 
 *===============================================================================================================*
    LICENSE
 *===============================================================================================================*
 
    This library is a free software; you can redistribute it and/or modify it under the terms of the
    GNU General Public License as published by the Free Software Foundation; either version 3.0 of the
    License, or any later version. This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
    PURPOSE. See the GNU General Public License for more details.

 *==============================================================================================================*/

#include "Arduino.h"
#include "DegreeConverter.h"

/*==============================================================================================================*
    CONVERT °C TO °F (I)
 *==============================================================================================================*/

float DegreeConverter::conCtoF(int valC) {
    return (valC * 1.8) + 32;
}

/*==============================================================================================================*
    CONVERT °C TO °F (II)
 *==============================================================================================================*/

float DegreeConverter::conCtoF(float valC) {
    return (valC * 1.8) + 32;
}

/*==============================================================================================================*
    CONVERT °C TO °F (III)
 *==============================================================================================================*/

float DegreeConverter::conCtoF(double valC) {
    return (valC * 1.8) + 32;
}

/*==============================================================================================================*
    CONVERT °F TO °C (I)
 *==============================================================================================================*/

float DegreeConverter::conFtoC(int valF) {
    return (valF - 32.0) * (5.0 / 9.0);
}

/*==============================================================================================================*
    CONVERT °F TO °C (II)
 *==============================================================================================================*/

float DegreeConverter::conFtoC(float valF) {
    return (valF - 32.0) * (5.0 / 9.0);
}

/*==============================================================================================================*
    CONVERT °F TO °C (III)
 *==============================================================================================================*/

float DegreeConverter::conFtoC(double valF) {
    return (valF - 32.0) * (5.0 / 9.0);
}

/*==============================================================================================================*
    CONVERT °Cx16 TO °C
 *==============================================================================================================*/

float DegreeConverter::conC16toC(int valC16) {
    return ((float)valC16 / 16.0);
}

/*==============================================================================================================*
    CONVERT °Fx16 TO °F
 *==============================================================================================================*/

float DegreeConverter::conF16toF(int valF16) {
    return (valF16 / 16.0);
}

/*==============================================================================================================*
    CONVERT °Cx16 TO °Fx16
 *==============================================================================================================*/

int DegreeConverter::conC16toF16(int valC16) {
    return ((((long)valC16 * 18) + 5120) / 10);
}

/*==============================================================================================================*
    CONVERT °Fx16 TO °Cx16
 *==============================================================================================================*/

int DegreeConverter::conF16toC16(int valF16) {
    return (int)((long(valF16) - 512) * (5.0 / 9.0));
}

/*==============================================================================================================*
    ROUND TO NEAREST 0.5°C
 *==============================================================================================================*/

float DegreeConverter::roundC(float valC) {
    if (valC > 0) {
        return (int)(((valC) * 2.0) + 0.5) / 2.0;
    } else if (valC < 0) {
        return (int)(((valC) * 2.0) - 0.5) / 2.0;
    } else {
        return 0;
    }
}

/*==============================================================================================================*
 ROUND TO NEAREST 0.5°C
 *==============================================================================================================*/

float DegreeConverter::roundC(double valC) {
    if (valC > 0) {
        return (int)(((valC) * 2.0) + 0.5) / 2.0;
    } else if (valC < 0) {
        return (int)(((valC) * 2.0) - 0.5) / 2.0;
    } else {
        return 0;
    }
}






