//
//  InputParser.h
//  ChemiCalc
//
//  Created by Daniel Roythorne on 30/07/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ChemiCalc_InputParser_h
#define ChemiCalc_InputParser_h

#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>

#include "ChemicalProperties.h"

using std::string;
using std::vector;

namespace ChemiCalc {

//This parser is horribly intertwined with the raw data that it exports - should really factor out the returned
//data structures to another class.
    class InputParser {
    
    public:
        explicit InputParser(const vector<const ChemicalProperties*> &chemicals);
        //readCycles with getToComma method 
        //that reads all char of input untill a comma is reached. Returns 0 when successful, 1 otherwise
        int readCycles(std::ifstream &file);
        const vector<const ChemicalProperties*>& chemicals_;
        vector<int> wRefVoltAr_;
        vector<int> wGrnVoltAr_;
        vector<int> wTempVoltAr_;
        vector<int> wErefVoltAr_;
        int iRegParCount_;
        vector<int> iTV_Point_;
        vector<vector<int> > sT_sec_;
        vector<vector<int> > sV_sec_;
        vector<vector<int> > iV_sec_;
        vector<vector<vector<int> > > wI_Ar_; 
        vector<vector<vector<int> > > sI_Ar_; 
        vector<vector<vector<int> > > wPT_Ar_; 
        vector<vector<vector<int> > > sPT_Ar_; 
        vector<vector<vector<int> > > wIuP_Ar_; 
        vector<vector<vector<int> > > sIuP_Ar_; 
        vector<vector<vector<int> > > wIdO_Ar_;
        vector<vector<vector<int> > > sIdO_Ar_;
        vector<vector<vector<int> > > sIsm_Ar_;          //Array of smoothed signal
        vector<vector<vector<int> > > sIsmMax_Ar_;       //Array of the max values of the smoothed signal
        vector<vector<vector<int> > > sIsmMaxV_Ar_;      //Array of position of the max values of the smoothed signal
        vector<vector<vector<int> > > sIsmAv_Ar_;        //Dynamic array of aver signal
        vector<vector<vector<int> > > sIsmLeftMin_Ar_;   //Dynamic array of left min signal
        vector<vector<vector<int> > > sIsmRightMin_Ar_;  //Dynamic array of the right min signal
        vector<vector<vector<int> > > sIsmDif_Ar_;       //Dynamic array of the signal without baseline
        
    private:
        //Determines whether a ASCII character is 0..9, A..F
        bool isHex(const char &c);
        //Converts hex strings to integers
        void headDataComma(const string &headDataComma);
        int hexStringToInt(const string &str);
        static const int iMeasCount_ = 4;
        int iMeasCycle_;
        int iChanRec_;
        bool bChanOK_;
        bool bMeas_;
        bool bStartMeas_;
        bool bEndMeas_;
        int iAmpSW_;
        double sAmpSW_;
        int iN_SWP_;
        int iUp_;
        bool CycleEnd_;
    };
    
    inline bool InputParser::isHex(const char &c) {
        if ((48 <=c && c <= 9+48) || (65 <= c && c <= 5+65)) 
            return true;
        else 
            return false;
    };
    
    inline int InputParser::hexStringToInt(const string &str) {
        char * end = 0;
        return strtol(str.c_str(), &end, 16);
    };
}

#endif
