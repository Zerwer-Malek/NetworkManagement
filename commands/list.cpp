//Import headers
#include "list.h"
#include "../main.h"
#include "save/oui.h"

using namespace std;

// Function lists all the devices in targets.txt
void list() {
    // Converts file contents of targets.txt to string and splits into a vector string at index \n
    vector<string> lines = split(filetostr("targets.txt"), '\n');
    
    // Declare both macs and ips lists
    vector<string> macs;
    vector<string> ips;
    
    // Declare integer for choosing target
    int choice;
    
    /* 
     * Loop through lines and sort odd lines(ips) from even lines(macs) and then append to proper list
     * Note: (starts at 0)
    */
    for(int i = 0; i < lines.size(); i++) {
        if(i % 2 == 0){
            macs.push_back(lines[i]);
        }
        else{
            ips.push_back(lines[i]);
        }
    }
    // Converts the oui string into a string vector
    vector<string> ouiVec = split(oui, '\n');
    
    // Loop through oui vector to find manufacturer match
    for(int i = 0; i < ips.size();i++) {
        // Split mac to get numbers and reformat to preform search
        vector<string> man = split(macs[i].substr(0, macs[i].length() - 9), ':');
        string y = man[0] +  man[1] + man[2];
        transform(y.begin(), y.end(), y.begin(), ::toupper);

        string manufacturer = ""; // Declare manufacturer string
        
        // Check if the line contains same MAC Address
        for(int i = 0; i < ouiVec.size(); i++) {
            string x = ouiVec[i];
            
            if(x.find(y) != string::npos) {
                manufacturer = x;
                break;
            }
        }
        
        // Set the manufacturer
        if(manufacturer == "") manufacturer = "Unknown";
        else manufacturer = manufacturer.substr(7, manufacturer.length());
        
        cout << i + 1 << ".\t" << macs[i] << "\t" << ips[i] << "\t"<< manufacturer << "\n";
    }
    
}
