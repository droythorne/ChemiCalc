//
//  main.cpp
//  TestXCode
//
//  Created by Daniel Roythorne on 19/07/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <vector>
#include <regex>
using namespace std;
int main(int argc, const char * argv[])
{

    // insert code here...
    vector<int> myVec(4);
    myVec.push_back(2);
    myVec.push_back(3);
    myVec.push_back(1);
    regex integer("(\\+|-)?[[:digit:]]+");
    //int n = [] (int x, int y) { return x + y; }(5, 4);
    //cout << n << endl;
    cout << "Hello, World!\n";
    return 0;
}