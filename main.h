#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

//Functions that can be used in every file to be delared here
string returnCmd(const char* cmd);
vector<string> split(string input, char c);
string filetostr(string name);

#endif
