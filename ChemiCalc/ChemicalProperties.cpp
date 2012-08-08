//
//  ChemicalProperties.cpp
//  ChemiCalc
//
//  Created by Daniel Roythorne on 29/07/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <cmath>
#include "constants.h"
#include "ChemicalProperties.h"


namespace ChemiCalc {
    
    ChemicalProperties::ChemicalProperties 
                    (   ChemicalNames name,
                        stTechn st_techn,
                        bTechn b_techn,
                        int stSensName,
                        double sRf,
                        double sGain, 
                        double sT1_s,
                        double sT2_s,
                        double sT3_s,
                        double sT4_s,
                        double sE1_V,
                        double sE2_V,
                        double sE3_V,
                        double sE4_V,
                        double sFreq_Hz,
                        double bADC_Rate,             
                        int bNSample,
                        int SmoothTechnique,
                        double sStep_V,
                        int iW_Ar,
                        double sSlope_Ar,                
                        double sSlopeErr_Ar,             
                        double sInter_Ar,                
                        double sInterErr_Ar)
    : name_(name),
    st_techn_(st_techn),
    b_techn_(b_techn),
    stSensName_(stSensName),
    sRf_(sRf),
    sGain_(sGain), 
    sT1_s_(sT1_s),
    sT2_s_(sT2_s),
    sT3_s_(sT3_s),
    sT4_s_(sT4_s),
    sE1_V_(sE1_V),
    sE2_V_(sE2_V),
    sE3_V_(sE3_V),
    sE4_V_(sE4_V),
    sFreq_Hz_(sFreq_Hz),
    bADC_Rate_(bADC_Rate),             
    bNSample_(bNSample),
    SmoothTechnique_(SmoothTechnique),
    sStep_V_(sStep_V),
    iW_Ar_(iW_Ar),
    sSlope_Ar_(sSlope_Ar),                
    sSlopeErr_Ar_(sSlopeErr_Ar),             
    sInter_Ar_(sInter_Ar),                
    sInterErr_Ar_(sInterErr_Ar) {
        if (st_techn_ == stTechn::SW) {
            sT4_s_ = sT3_s_ + ceil((wDoubleNumberSW() - 1) * sPeriod_s()/2);
        }
        wT1_ = round(sT1_s_);
        wT2_ = round(sT2_s_);
        wT3_ = round(sT3_s_);
        wT4_ = round(sT4_s_);
        if (st_techn_ != 3 && st_techn_ != 4) {
            wE1_ = round((sE1_V_ + sRefElPot_V) / sDAC_LSB_V);
            wE2_ = round((sE2_V_ + sRefElPot_V) / sDAC_LSB_V);
            wE3_ = round((sE3_V_ + sRefElPot_V) / sDAC_LSB_V);
            wE4_ = round((sE4_V_ + sRefElPot_V) / sDAC_LSB_V);
            //Recalculate sE?_V_
            sE1_V_ = (wE1_ * sDAC_LSB_V) - sRefElPot_V;
            sE2_V_ = (wE2_ * sDAC_LSB_V) - sRefElPot_V;
            sE3_V_ = (wE3_ * sDAC_LSB_V) - sRefElPot_V;
            sE4_V_ = (wE4_ * sDAC_LSB_V) - sRefElPot_V;
        }
        else {
            wE1_ = round(sE1_V_ / sDAC_LSB_V);
            wE2_ = round(sE2_V_ / sDAC_LSB_V);
            wE3_ = round(sE3_V_ / sDAC_LSB_V);
            wE4_ = round(sE4_V_ / sDAC_LSB_V);
            //Recalculate sE?_V_
            sE1_V_ = (wE1_ * sDAC_LSB_V);
            sE2_V_ = (wE2_ * sDAC_LSB_V);
            sE3_V_ = (wE3_ * sDAC_LSB_V);
            sE4_V_ = (wE4_ * sDAC_LSB_V);
        }
    };
    
    void ChemicalProperties::accelerate() {
        sT1_s_ /= 5;
        sT2_s_ /= 5;
        sT3_s_ /= 5;
        sT4_s_ /= 5;
    };
} //namespace ChemiCalc