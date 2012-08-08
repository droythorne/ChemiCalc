//
//  main.cpp
//  TestXCode
//
//  Created by Daniel Roythorne on 19/07/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include "constants.h"
#include "ChemicalProperties.h"
#include "InputParser.h"

using namespace std;
using namespace ChemiCalc;
int main(int argc, const char * argv[])
{
    
    //Assign properties to the detection channels
    ChemicalProperties chemCu = ChemicalProperties(ChemicalNames::Cu, stTechn::SW, bTechn::bSW, 1, 1e6, 1, 5, 50, 147, 160, 0.8, 0.45, -0.15, 0.6, 15, 42, 16, 0, 0.008, 15, 1.5, 0.001, 0.02, 0.001);
    ChemicalProperties chemPb = ChemicalProperties(ChemicalNames::Pb, stTechn::SW, bTechn::bSW, 2, 1e6, 1, 10, 10, 110, 120, -0.185, -0.7, -0.7, -0.3, 15, 42, 16, 0, 0.008, 15, 0.05, 0.001, 0.02, 0.001);
    ChemicalProperties chemORP = ChemicalProperties(ChemicalNames::ORP, stTechn::PM, bTechn::bIM, 3, 0, 1, 85, 85, 85, 95, 0, 0, 0, 0, 15, 42, 16, 0, 0, 15, 0.05, 0.001, 0.02, 0.001);
    ChemicalProperties chemConductivity = ChemicalProperties(ChemicalNames::Conductivity, stTechn::IM, bTechn::bIM, 4, 0, 1, 25, 25, 25, 35, 0, 0, 0, 0, 15, 42, 16, 0, 0, 15, 0.05, 0.001, 0.02, 0.001);
    ChemicalProperties chemO2 = ChemicalProperties(ChemicalNames::O2, stTechn::AM, bTechn::bAM, 5, 1e6, 1, 60, 60, 60, 70, -0.2, -0.2, -0.2, -1, 15, 42, 16, 0, 0, 15, 0.05, 0.001, 0.02, 0.001);
    ChemicalProperties chemPh = ChemicalProperties(ChemicalNames::Ph, stTechn::SW, bTechn::bSW, 6, 1e6, 1, 125, 125, 125, 135, 0.25, 0.25, 0.25, 1, 15, 42, 16, 0, 0.008, 15, 0.05, 0.001, 0.02, 0.001);
    
    //UGLY - indices of chemCu, ... in chemicals should match stSensName in constructor argument
    vector<const ChemicalProperties *> chemicals(N_Chanel);
    chemicals[0] = &chemCu;
    chemicals[1] = &chemPb;
    chemicals[2] = &chemORP;
    chemicals[3] = &chemConductivity;
    chemicals[4] = &chemO2;
    chemicals[5] = &chemPh;
    
    //Set the accelerated/debug mode
    //TODO - apply across a container of ChemicalProperties
    chemCu.accelerate();
    chemPb.accelerate();
    chemORP.accelerate();
    chemConductivity.accelerate();
    chemO2.accelerate();
    chemPh.accelerate();
    
    InputParser ip(chemicals);
    ifstream myTestFile("./resources/matlabinput.txt");
    if(myTestFile.good())
    {
            ip.readCycles(myTestFile);
    }
    return 0;
}