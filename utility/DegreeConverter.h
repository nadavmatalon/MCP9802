/*==============================================================================================================*
 
 @file     DegreeConverter.h
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

#ifndef DegreeConverter_h
#define DegreeConverter_h

class DegreeConverter {
    public:
        static float conCtoF(int valC);
        static float conCtoF(float valC);
        static float conCtoF(double valC);

        static float conFtoC(int valF);
        static float conFtoC(float valF);
        static float conFtoC(double valF);

        static float conC16toC(int valC16);
        static float conF16toF(int valF16);
        
        static int conC16toF16(int valC16);
        static int conF16toC16(int valF16);
        
        static float roundC(float valC);
        static float roundC(double valC);
};

#endif