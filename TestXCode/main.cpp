//
//  main.cpp
//  TestXCode
//
//  Created by Daniel Roythorne on 19/07/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <vector>
using namespace std;
int main(int argc, const char * argv[])
{

    // insert code here...
    vector<int> myVec(4);
    myVec.push_back(2);
    myVec.push_back(3);
    myVec.push_back(1);
    sort(myVec.begin(),
         myVec.end(),
         [](const int &a, const int &b) -> bool
         {
             return (a < b);
         });
    cout << "Hello, World!\n";
    return 0;
}