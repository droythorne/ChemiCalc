//
//  constants.h
//  TestXCode
//
//  Created by Daniel Roythorne on 27/07/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
//  Initializations utilizing cmath should not be affected by static initialization order ambiguity (as in stdlib)
//  but not 100% certain of this, so potentially move in to implementation file later.
//

#ifndef _constants_h
#define ChemiCalc_constants_h

#include <cmath>

namespace ChemiCalc {
       
    const double freqSW = 15;                                       //Frequency of square wave voltametry
    const double periodSW = 1. / freqSW;                            //Period of square wave
    const double sClFreq_MHz = 16.777216; 
    const double sClPeriod_s = 1e-6 / sClFreq_MHz;
    const double sTimerT0_s = sClPeriod_s * 256;                    // Timer T0 mod 2, 8 bit reload - delay
    const double bNumTimerT0_1ms = 66;                              //1ms in timer overflow
    const double sTimerT0_1ms = sTimerT0_s * bNumTimerT0_1ms;       //1ms in timer T0
    const double wNumTimerT0_1s = 993;                              //1s in timer overflow
    const double sTimerT0_1s = sTimerT0_s * wNumTimerT0_1s;         //1s in timer T0
    const int N_Chanel = 6;                     //Number of channels
    const int N_Ch_Par = 28;                    //Number of registers with channel parameters
    const int N_Point = 10;                     //Number of points for each Channel in the protocol mode
    const double sVrefInt = 2.5;                //Internal reference voltage
    const double sVrefExt = 4.096;              //External reference voltage
    const double sV_Ref_V = sVrefExt;
    const double sRefElPot_V = sV_Ref_V/2;      //Reference Electrode Potential
    const int bAdcDigCap = 12;                  //ADC digit capacity
    const int bDacDigCap = 16;                  //ADC digit capacity
    const double sADC_LSB_V = sV_Ref_V / (pow(2, bAdcDigCap)-1);    //ADC LSB
    const double sDAC_LSB_V = sV_Ref_V / (pow(2, bDacDigCap)-1);    //DAC LSB
    const int wRefElPot = round(sRefElPot_V / sDAC_LSB_V);          //ADU Reference Electrode Potential in sDAC_LSB
    const double bADC_Rate_T2Cl = 42;                               //T2 clocks for ADC conversion
    const double sADC_Rate_T2s = sClPeriod_s * bADC_Rate_T2Cl;      //T2 clocks for ADC conversion
    const double sRatioAmpToStep = 6.25;                            //Ratio Amp to Step of SW for SW voltammetry
    const int iSerNumSensors = 5;
    const int iRegParCount = 0;
    enum PCFAddress {
        b_PCF_1 = 44,
        b_PCF_2 = 46
    };
    const double sEmax_V = 1.5;     //Max potential for the system
    const double sEmin_V = -1.5;    //Min potential for the system
    const double sTmax_s = 300;     //Max time acseptable for the systyem s
    const double sFmax_Hz = 40;     //Max frequency corresponding SW period and sampling
    const double sFmin_Hz = 1;      //Min frequency corresponding SW period and sampling
    
    const double vSG_Coef_Ar[15] = {    -0.070588235,   
                                        -0.011764706,  
                                        0.03800905,  
                                        0.078733032,   
                                        0.11040724,   
                                        0.133031674,
                                        0.146606335,   
                                        0.151131222,   
                                        0.146606335, 
                                        0.133031674,   
                                        0.11040724, 
                                        0.078733032,   
                                        0.03800905,
                                        -0.011764706,   
                                        -0.070588235};
    
    //TODO - remove this after resolving the issue with Matlab initialisation of InputParser 3D arrays
    const int DUMMYwDoubleNumberSW = 1;  
} //namespace ChemiCalc


#endif
