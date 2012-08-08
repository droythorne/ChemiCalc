//
//  ChemicalProperties.h
//  ChemiCalc
//
//  Created by Daniel Roythorne on 27/07/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ChemiCalc_ChemicalProperties_h
#define ChemiCalc_ChemicalProperties_h

namespace ChemiCalc {
    
    enum ChemicalNames {
        SavitzkiGolay = 0,
        Cu,
        O2,
        ORP,
        S,
        Pb,
        Ph,
        Conductivity
    };

    enum bTechn {
        bSW = 1, // SW-voltametry (square wave voltammetry) = 01H
        bSC = 2,  // SC-voltametry (stair case voltammetry) = 02H
        bDP = 4,  // DP-voltametry (differential pulse voltammetry) = 04H
        bAM = 8,  // Amperometry = 08H
        bPM = 10,  // Potentiometry = 10H
        bIM = 20  // Impediometry = 20H
    };

    //Retained enum name to match that in original Matlab code, chem.m
    enum stTechn {  
        SW = 1, //SW-voltametry
        AM,     //Amperometry
        PM,     //Potentiometry
        IM      //Impediometry
    };

    class ChemicalProperties {
    public:
        static const int regOverloads = 65536;
        
        ChemicalProperties (ChemicalNames name,
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
                            double sInterErr_Ar);

        ChemicalNames name_; 
        stTechn st_techn_;
        bTechn b_techn_;
        int stSensName_;             //original id used in Matlab code
        double sRf_;              //Rf=1M fedback for voltammetric channels
        double sGain_;                //Channel gain!!!
        double sT1_s_;
        double sT2_s_;
        double sT3_s_;
        double sT4_s_;
        int wT1_;
        int wT2_;
        int wT3_;
        int wT4_;
        double sE1_V_;
        double sE2_V_;
        double sE3_V_;
        double sE4_V_;
        int wE1_;
        int wE2_;
        int wE3_;
        int wE4_;    
        double sFreq_Hz_;
        double bADC_Rate_;                  //ADU ADC rate for channels
        int bNSample_;
        int SmoothTechnique_;
        double sStep_V_;
        int iW_Ar_;
        double sSlope_Ar_;                  //Channel calibration slope
        double sSlopeErr_Ar_;               //Channel calibration slope error
        double sInter_Ar_;                  //Channel calibration intercept
        double sInterErr_Ar_;               //Channel calibration intercept error
            
        double sPeriod_s() const;
        void accelerate();                  //Debug mode, five times faster than regular calculation
        double sE_Range_V();                    //Potential range for channels
        double sT_Meas_s();                     //Measurement time for channels in sec
        int wDoubleNumberSW();
        double sADC_Rate_s();
        double sADC_MesTime_s();                //Time of the bNSample ADC measurement
        double sTimeBeforeADC_s();              //Time before ADC measurement start in SW
        int iTimeBeforeADC_cl();             //Time before ADC measurement start in SW in clock
        int wT1_OverFlows();                //Number of full T1 16-bit registrer overloadsduring prec.time HalfPeriod-FulADCTime/(T1 16b overflows)
        int wT1_Reminder();                 //Reminder T1 16-bit registrer overloads
        double wADC_T2reload();
        int wE_Range();                     //ADU potential range for channels
        int wT_Meas();                      //ADU measurement range for channels 
        int wStep();
        double sStep_V();
        double sScanRate_Vs();
        double sAmpPulse_V();
        int wLoopTime();
    };
        
    inline double ChemicalProperties::sPeriod_s() const {
        return 1.0 / sFreq_Hz_;
    };

    inline double ChemicalProperties::sE_Range_V() {
        return sE4_V_ - sE3_V_;
    };

    inline double ChemicalProperties::sT_Meas_s() {
        return sT4_s_ - sT3_s_;
    };

    inline int ChemicalProperties::wDoubleNumberSW() {
        if (st_techn_ == stTechn::SW) {
            return 2 * (ceil(sE_Range_V()/sStep_V_)+1); //R_SW_Prec_Time_CD
        }
        else {
            return 2 * (ceil(sT_Meas_s()/sPeriod_s())+1); //R_SW_Prec_Time_CD
        }
    };

    inline double ChemicalProperties::sADC_Rate_s() {
        return sClPeriod_s * bADC_Rate_;
    };

    inline double ChemicalProperties::sADC_MesTime_s() {
        return bNSample_ * sADC_Rate_s();
    };

    inline double ChemicalProperties::sTimeBeforeADC_s() {
        return (sPeriod_s() / 2) - sADC_Rate_s();
    };

    inline int ChemicalProperties::iTimeBeforeADC_cl() {
        return round(sTimeBeforeADC_s() / sClPeriod_s);
    };

    inline int ChemicalProperties::wT1_OverFlows() {
        return floor(iTimeBeforeADC_cl() / regOverloads);  
    };

    inline int ChemicalProperties::wT1_Reminder() {
        return iTimeBeforeADC_cl() - (wT1_OverFlows() * regOverloads);
    };

    inline double ChemicalProperties::wADC_T2reload() {
        return regOverloads - bADC_Rate_;
    };

    inline int ChemicalProperties::wE_Range() {
        return wE4_ - wE3_;
    };

    inline int ChemicalProperties::wT_Meas() {
        return wT4_ - wT3_;
    }

    inline int ChemicalProperties::wStep() {
        if  (st_techn_ == stTechn::SW) {
            return ceil( sStep_V_ / sDAC_LSB_V);
        }
        else {
            return 0;
        }
    };

    inline double ChemicalProperties::sStep_V() {
        if  (st_techn_ == stTechn::SW) {
            return wStep() * sDAC_LSB_V;
        }
        else {
            return 0;
        }
    };

    inline double ChemicalProperties::sScanRate_Vs() {
        double temp_scanRate = 0;
        if (sT_Meas_s() != 0) {
            temp_scanRate = sE_Range_V()/sT_Meas_s();
        }
        if (sPeriod_s() != 0) {
            temp_scanRate = sStep_V()/sPeriod_s();
        }
        return temp_scanRate;
    };

    inline double ChemicalProperties::sAmpPulse_V() {
        double wAmpPulse = round(sRatioAmpToStep * sStep_V() / sDAC_LSB_V);
        return wAmpPulse * sDAC_LSB_V;
    };
} //namespace ChemiCalc
#endif
