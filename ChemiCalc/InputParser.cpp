//
//  InputParser.cpp
//  ChemiCalc
//
//  Created by Daniel Roythorne on 30/07/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <cmath>
#include <sstream>

#include "./constants.h"
#include "./InputParser.h"

namespace ChemiCalc {
    
    InputParser::InputParser(const vector<const ChemicalProperties*> &chemicals) 
    :   chemicals_(chemicals),
        wRefVoltAr_(iMeasCount_, 0.),
        wGrnVoltAr_(iMeasCount_, 0.),
        wTempVoltAr_(iMeasCount_, 0.),
        wErefVoltAr_(iMeasCount_, 0.),
        iRegParCount_(0),
        iMeasCycle_(0),
        iChanRec_(0),
        bChanOK_(false),
        iTV_Point_(N_Chanel, 0),
        sT_sec_(N_Chanel, vector<int>(DUMMYwDoubleNumberSW,0)),
        sV_sec_(N_Chanel, vector<int>(DUMMYwDoubleNumberSW,0)),
        iV_sec_(N_Chanel, vector<int>(DUMMYwDoubleNumberSW,0)),
        wI_Ar_(N_Chanel, vector<vector<int> >(iMeasCount_, vector<int>(DUMMYwDoubleNumberSW,0))),
        sI_Ar_(N_Chanel, vector<vector<int> >(iMeasCount_, vector<int>(DUMMYwDoubleNumberSW,0))),
        wPT_Ar_(N_Chanel, vector<vector<int> >(iMeasCount_, vector<int>(DUMMYwDoubleNumberSW,0))), 
        sPT_Ar_(N_Chanel, vector<vector<int> >(iMeasCount_, vector<int>(DUMMYwDoubleNumberSW,0))), 
        wIuP_Ar_(N_Chanel, vector<vector<int> >(iMeasCount_, vector<int>(DUMMYwDoubleNumberSW,0))), 
        sIuP_Ar_(N_Chanel, vector<vector<int> >(iMeasCount_, vector<int>(DUMMYwDoubleNumberSW,0))), 
        wIdO_Ar_(N_Chanel, vector<vector<int> >(iMeasCount_, vector<int>(DUMMYwDoubleNumberSW,0))),
        sIdO_Ar_(N_Chanel, vector<vector<int> >(iMeasCount_, vector<int>(DUMMYwDoubleNumberSW,0))),
        sIsm_Ar_(N_Chanel, vector<vector<int> >(iMeasCount_, vector<int>(DUMMYwDoubleNumberSW,0))),          //Array of smoothed signal
        sIsmMax_Ar_(N_Chanel, vector<vector<int> >(iMeasCount_, vector<int>(DUMMYwDoubleNumberSW,0))),       //Array of the max values of the smoothed signal
        sIsmMaxV_Ar_(N_Chanel, vector<vector<int> >(iMeasCount_, vector<int>(DUMMYwDoubleNumberSW,0))),      //Array of position of the max values of the smoothed signal
        sIsmAv_Ar_(N_Chanel, vector<vector<int> >(iMeasCount_, vector<int>(DUMMYwDoubleNumberSW,0))),        //Dynamic array of aver signal
        sIsmLeftMin_Ar_(N_Chanel, vector<vector<int> >(iMeasCount_, vector<int>(DUMMYwDoubleNumberSW,0))),   //Dynamic array of left min signal
        sIsmRightMin_Ar_(N_Chanel, vector<vector<int> >(iMeasCount_, vector<int>(DUMMYwDoubleNumberSW,0))),  //Dynamic array of the right min signal
        sIsmDif_Ar_(N_Chanel, vector<vector<int> >(iMeasCount_, vector<int>(DUMMYwDoubleNumberSW,0))),     
        bMeas_(false),
        bStartMeas_(false),
        bEndMeas_(false),
        iAmpSW_(0),
        sAmpSW_(0.),
        iN_SWP_(0),
        iUp_(0),
        CycleEnd_(false)
    {};
    int InputParser::readCycles(std::ifstream &file){
        std::stringstream pStrStream(std::ios_base::in | std::ios_base::out);
        while(iMeasCycle_ <= iMeasCount_) {
            while(!CycleEnd_) {
                file.get(*pStrStream.rdbuf(), ',');
                file.ignore();
                pStrStream << ",";
                headDataComma(pStrStream.str());
                pStrStream.str("");
            }
        } 
        return 0;
    };
    
    void InputParser::headDataComma(const string &headDataComma){
        string::const_iterator headDataComma_iter = headDataComma.begin();
        string stData; //String to contain hex chars
        char stHead;
        int iChData = 0;
        int iDataCharNum = 0;
        bool bPotUp = true; //Flag of positive SW wave
        
        //NB -  sVstep, eTemp, eTempV, J brought outside while scope to match MATLAB scoping behaviour;
        //      unclear whether to make member variable so lifetime is that of object and initialised 
        //      only once at construction and/or to permit client access following headDataComma execution??
        double sVstep = 0.;         
        double eTemp = 0.;          
        double eTempV = 0.;
        int J = 0; //NB - never read
        
        while(headDataComma_iter != headDataComma.end()) {
            char current_char = *headDataComma_iter;
            if (!(current_char == ',')) {
                if ((48 < current_char && current_char < 9+48) || (65 < current_char && 5+65)) {
                    if (++iChData <= iDataCharNum) {
                        stData.push_back(current_char);
                    }
                } else {
                    if ((current_char == 'r') || 
                        (current_char == '#') || 
                        (current_char == 't') || 
                        (current_char == '^') || 
                        (current_char == '*')) {
                        iDataCharNum = 2;
                    } else if ( 
                        (current_char == '{') || 
                        (current_char == 'P') || 
                        (current_char == 'R') || 
                        (current_char == 'e') || 
                        (current_char == 'V') || 
                        (current_char == 'd') ||
                        (current_char == '\\') || 
                        (current_char == '|') ||
                        (current_char == '<') || 
                        (current_char == 'M')) {
                        iDataCharNum = 4;
                    } else if (
                        (current_char == '>') || 
                        (current_char == ',') || 
                        (current_char == '4') || //NB - not sure whether this should be ASCII character EOT (end of transmission) or '4' (ASCII 52)
                        (current_char == 'w') || 
                        (current_char == 'W')) {
                        iDataCharNum = 0;
                    } else {
                        //NB - No default behavior specified in Matlab code
                    }
                    stHead = current_char;
                }
            } else {
                int stData_int = hexStringToInt(stData);
                switch (stHead) {
                    case 'R':
                        wRefVoltAr_[iMeasCycle_] = stData_int;
                        //sVrefRec not used
                        break;
                    case 'G':
                        wGrnVoltAr_[iMeasCycle_] = stData_int;
                        //sAgndRec not used
                    case 'T':
                        wTempVoltAr_[iMeasCycle_] = stData_int;
                        //sTaduRec not used
                    case 'e':
                        wErefVoltAr_[iMeasCycle_] = stData_int;
                        //sErefRec not used
                    case 'r':
                        ++iRegParCount_;
                    //NB - assignment of stData_int to stChar in cases 'r', 'd', 'P' is omitted as the variable is not used subsequently, but may need to make member variable for client access
                    case '#':
                        iChanRec_ = stData_int;
                        if (iChanRec_ < N_Chanel) bChanOK_ = true; 
                    default:
                        break;
                }
                if (bChanOK_) {
                    bStartMeas_ = false;
                    bool temp_bMeas = bMeas_;
                    int iVstep = 0;
                    if (bEndMeas_) {
                        bEndMeas_ = false;
                        bMeas_ = false;
                    }
                    switch (stHead) {
                        case 't':
                            if (!temp_bMeas) {
                                bChanOK_ = false;
                                sT_sec_[iChanRec_][iTV_Point_[iChanRec_]] = stData_int;
                            }
                            ++iTV_Point_[iChanRec_];
                            break;
                        case 'V':
                        {
                            double iVdacRec = stData_int; 
                            double sVdacRec = iVdacRec * sDAC_LSB_V - sRefElPot_V;
                            if (bStartMeas_) {
                                iUp_ = 0;
                                iVdacRec -= iAmpSW_;
                                sVdacRec -= sAmpSW_;
                                if (chemicals_[iChanRec_]->st_techn_ == stTechn::SW) {
                                    wPT_Ar_[iChanRec_][iMeasCycle_][iUp_] = iVdacRec;
                                    sPT_Ar_[iChanRec_][iMeasCycle_][iUp_] = 1000 * sVdacRec;
                                } else {
                                    wPT_Ar_[iChanRec_][iMeasCycle_][iUp_] = iUp_;
                                    sPT_Ar_[iChanRec_][iMeasCycle_][iUp_] = iUp_ * (chemicals_[iChanRec_]->sPeriod_s());
                                }
                            }
                            if (bEndMeas_) {
                                iVdacRec += iAmpSW_;
                                sVdacRec += sAmpSW_;
                            }
                            iV_sec_[iChanRec_][iTV_Point_[iChanRec_]] = iVdacRec;
                            sV_sec_[iChanRec_][iTV_Point_[iChanRec_]] = sVdacRec;
                            break;
                        }
                        case '{':
                            iN_SWP_ = stData_int;
                            break;
                        case '\\':
                            iVstep = stData_int;
                            sVstep = iVstep * sDAC_LSB_V;
                            break;
                        case '|':
                            iAmpSW_ = stData_int;
                            sAmpSW_ = iAmpSW_ * sDAC_LSB_V;
                            bStartMeas_ = true;
                            bMeas_ = true;
                            iUp_ = 0;
                            bPotUp = true;
                            break;
                        case '<':
                        {
                            double sIadcRec;
                            if (chemicals_[iChanRec_]->st_techn_ == stTechn::SW || 
                               chemicals_[iChanRec_]->st_techn_ == stTechn::AM) {
                                sIadcRec = stData_int * sDAC_LSB_V - sRefElPot_V;
                            } else {
                                sIadcRec = stData_int * sADC_LSB_V;
                            }
                            if (iUp_ < iN_SWP_ / 2) {
                                if (bPotUp) {    //Positive SW
                                    bPotUp = false;
                                    wIuP_Ar_[iChanRec_][iMeasCycle_][iUp_] = stData_int;
                                    sIuP_Ar_[iChanRec_][iMeasCycle_][iUp_] = sIadcRec;
                                } else {          //Negative SW
                                    bPotUp = true; 
                                    wIdO_Ar_[iChanRec_][iMeasCycle_][iUp_] = stData_int;
                                    sIdO_Ar_[iChanRec_][iMeasCycle_][iUp_] = sIadcRec;
                                    if (chemicals_[iChanRec_]->st_techn_ == stTechn::SW) {
                                        //Current vs. Pot
                                        wI_Ar_[iChanRec_][iMeasCycle_][iUp_] = wIuP_Ar_[iChanRec_][iMeasCycle_][iUp_] - stData_int;
                                        sI_Ar_[iChanRec_][iMeasCycle_][iUp_] = sIuP_Ar_[iChanRec_][iMeasCycle_][iUp_] - sIadcRec;
                                    } else {
                                        //Current vs. time
                                        wI_Ar_[iChanRec_][iMeasCycle_][iUp_] = (wIuP_Ar_[iChanRec_][iMeasCycle_][iUp_] - stData_int)/2;
                                        sI_Ar_[iChanRec_][iMeasCycle_][iUp_] = (sIuP_Ar_[iChanRec_][iMeasCycle_][iUp_] - sIadcRec)/2;
                                    }
                                    if (chemicals_[iChanRec_]->SmoothTechnique_ == 0) { //Savitsky-Golay
                                        int temp_iW_Ar = chemicals_[iChanRec_]->iW_Ar_;
                                        int m = (temp_iW_Ar - 1)/2;
                                        //NB - added decrement of temp_iW_Ar in conditional compared with Matlab code; test against current behaviour to check array indexing 
                                        if (iUp_ >= temp_iW_Ar - 1) {
                                            for (int i = 0; i < temp_iW_Ar; i++) {
                                                eTemp += vSG_Coef_Ar[i] * sI_Ar_[iChanRec_][iMeasCycle_][iUp_-i];
                                            } 
                                            sIsm_Ar_[iChanRec_][iMeasCycle_][iUp_-m] = eTemp; //Array of smoothed signal
                                            double eTempMax, eTempMaxV;
                                            if (iUp_ == temp_iW_Ar) { //Max signal and its position
                                                eTempMax = eTemp;
                                                eTempMaxV = sPT_Ar_[iChanRec_][iMeasCycle_][iUp_-m];
                                            } else {
                                                if (eTemp >= sIsmMax_Ar_[iChanRec_][iMeasCycle_][iUp_-m-1]) {
                                                    eTempMax = eTemp;
                                                    eTempMaxV = sPT_Ar_[iChanRec_][iMeasCycle_][iUp_-m];
                                                } else {
                                                    eTempMax = sIsmMax_Ar_[iChanRec_][iMeasCycle_][iUp_-m-1];
                                                    eTempMaxV= sIsmMaxV_Ar_[iChanRec_][iMeasCycle_][iUp_-m-1];  
                                                }
                                            }
                                            sIsmMax_Ar_[iChanRec_][iMeasCycle_][iUp_-m] = eTempMax; //Dynamic array of max signal
                                            sIsmMaxV_Ar_[iChanRec_][iMeasCycle_][iUp_-m] = eTempMaxV;
                                        }
                                    }
                                    iUp_+=1;
                                    if (chemicals_[iChanRec_]->SmoothTechnique_ == 0) {
                                        wPT_Ar_[iChanRec_][iMeasCycle_][iUp_] = wPT_Ar_[iChanRec_][iMeasCycle_][iUp_-1] + iVstep;
                                        sPT_Ar_[iChanRec_][iMeasCycle_][iUp_] = sPT_Ar_[iChanRec_][iMeasCycle_][iUp_-1] + 1000 * sVstep;
                                    } else {
                                        wPT_Ar_[iChanRec_][iMeasCycle_][iUp_] = iUp_;
                                        sPT_Ar_[iChanRec_][iMeasCycle_][iUp_] = iUp_ * (chemicals_[iChanRec_]->sPeriod_s());
                                    }
                                }
                            }
                            break;
                        }
                        case '>':
                        {
                            stHead = ' ';
                            bEndMeas_ = true;
                            iUp_ = 0;
                            bPotUp = true;
                            stData.empty();
                            iChData = 0;
                            if (chemicals_[iChanRec_]->SmoothTechnique_ == 0) { //Savitsky-Golay
                                int temp_iW_Ar = chemicals_[iChanRec_]->iW_Ar_;
                                int m = (temp_iW_Ar - 1)/2;
                                double eTempMax, eTempMaxV,eTempMinLeft, eTempMinLeftV, eTempMinRight, eTempMinRightV; 
                                if (chemicals_[iChanRec_]->st_techn_ == stTechn::SW) { //SW technique
                                    eTempMax = sIsmMax_Ar_[iChanRec_][iMeasCycle_][(iN_SWP_/2)-m-1];
                                    eTempMaxV =  sIsmMaxV_Ar_[iChanRec_][iMeasCycle_][(iN_SWP_/2)-m-1];
                                    eTempMinLeft = sIsm_Ar_[iChanRec_][iMeasCycle_][m];
                                    eTempMinLeftV = sPT_Ar_[iChanRec_][iMeasCycle_][m];
                                    eTempMinRight = eTempMax;
                                    eTempMinRightV = eTempMaxV;
                                    for(iUp_ = m; iUp_ <= (iN_SWP_/2)-m-2;iUp_++) {
                                        eTempV = sPT_Ar_[iChanRec_][iMeasCycle_][iUp_];
                                        eTemp = sIsm_Ar_[iChanRec_][iMeasCycle_][iUp_];
                                        if (eTempMaxV >= eTempV) { 
                                            //Minimum left
                                            if (eTempMinLeft >= eTemp) { 
                                                eTempMinLeft = eTemp;
                                                eTempMinLeftV = eTempV;
                                            }
                                        } else {
                                            //Minimum right
                                            if (eTempMinRight >= eTemp) {
                                                eTempMinRight = eTemp;
                                                eTempMinRightV = eTempV;
                                            }
                                        }   
                                    }
                                    double pwidthV = eTempMinRightV - eTempMinLeftV; //Peak Width
                                    bool bBLine;
                                    if (abs(eTempMinRightV - eTempMinLeftV) > 0.01) { //Peak is inside of 10mV peak width
                                        bBLine = true;
                                        eTempV = (eTempMinRight-eTempMinLeft)/(eTempMinRightV-eTempMinLeftV);
                                    } else {
                                        bBLine = false;
                                        pwidthV = pwidthV; //Peak width is less than minimal 
                                    }
                                    for (iUp_ = m-1; iUp_ <= (iN_SWP_/2)-m-2; iUp_++) { 
                                        double eTempBL;
                                        if (bBLine) { //Base line
                                            eTempBL =   eTempMinLeft + 
                                                        eTempV * (sPT_Ar_[iChanRec_][iMeasCycle_][iUp_] - eTempMinLeftV); //Dynamic array of the signal without baseline
                                        } else {//No base line
                                            eTempBL = 0; 
                                        }
                                        eTemp = sIsm_Ar_[iChanRec_][iMeasCycle_][iUp_] - eTempBL;
                                        sIsmDif_Ar_[iChanRec_][iMeasCycle_][iUp_] = eTemp;
                                    }
                                    //Concentration
                                    if (!bBLine) {
                                        eTemp = 0;   
                                    } else {
                                        double eTempBL = sIsmDif_Ar_[iChanRec_][iMeasCycle_][iUp_ + m];    
                                        eTemp = (eTempBL - chemicals_[iChanRec_]->sInter_Ar_)/chemicals_[iChanRec_]->sSlope_Ar_;
                                    }
                                } else { //Current vs. Pot
                                    //Averaging
                                    eTemp = 0;
                                    int K = 0;
                                    if (chemicals_[iChanRec_]->st_techn_ == stTechn::AM) {
                                        J = (iN_SWP_/2)- m - 32;
                                    } else {
                                        J = m;
                                    }
                                    for (iUp_ = m-1; iUp_ <= (iN_SWP_/2)-m-2; iUp_++) { 
                                        eTemp = eTemp + sIsm_Ar_[iChanRec_][iMeasCycle_][iUp_];
                                        K++;
                                    }
                                    if (K>1) { 
                                        eTemp = eTemp/K;
                                        eTempV = 0;
                                        for (iUp_ = m-1; iUp_ <= (iN_SWP_/2)-m-2; iUp_++) { 
                                            eTempV = eTempV + pow(eTemp - sIsm_Ar_[iChanRec_][iMeasCycle_][iUp_],2);
                                        }
                                        eTempV = sqrt(eTempV/(K-1));
                                        if (chemicals_[iChanRec_]->st_techn_ == stTechn::AM ||
                                            chemicals_[iChanRec_]->st_techn_ == stTechn::IM) {
                                            eTemp = (eTemp - chemicals_[iChanRec_]->sInter_Ar_)/chemicals_[iChanRec_]->sSlope_Ar_;
                                        }
                                    }
                                }
                            }
                            break;
                        }
                        default:
                            break;
                    }
                    ++iTV_Point_[iChanRec_];
                }
                switch (stHead) { 
                    case 's':
                        iChData = 0;
                        stData.empty();
                        break;
                    case 'W':
                        iMeasCycle_++;
                        CycleEnd_ = true;
                        break;
                    case 'w':
                        //TODO - Not sure how it's expected to implement this. Replace MATLAB code below with that expected for C++;
                        //fprintf(s, 'S') % send 'S' CHAR to ADuC842
                        break;
                    case 'M':
                        stHead = '<';
                        break;
                    default:
                        break;
                }
            }
            headDataComma_iter++;
        }
    };
} //namespace ChemiCalc