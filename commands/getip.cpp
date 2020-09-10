#include "getip.h"
#include "save/oui.h"
#include "../main.h"

using namespace std;

// Functions gets all the devices on the current network and saves them to targets.txt
void get() {
    // Create the file targets.txt and if it exist already, replace it
    ofstream reset;
    reset.open("targets.txt");
    reset.close();

    system("sudo arp-scan --localnet > /dev/null"); // Finds all devices on the network
    string ret = returnCmd("sudo arp -a"); // List the found ARP devices
    vector<string> arpRet = split(ret, '\n'); // Each device is one element of the vector
    
    regex macr("(([a-f\\d]{1,2}\\:){5}[a-f\\d]{1,2})"); // Mac adress regular expression
    regex ipr("[0-9]+(?:\\.[0-9]+){3}"); // IPv4 regular expression
    smatch m;
    
    vector<string> ouiVec = split(oui, '\n'); // Convert the OUI string to a vector
    
    // Go over each found device
    for(int i = 0; i < arpRet.size(); i++) {
        string line = arpRet.at(i);
        string mac;
        string ip;

        // Find the Mac Address in the return line
        if(regex_search(line, m, macr)) {
            mac = m[0];
            // Change the format of the Mac Address to match the OUI Format
            if(mac.length() <= 16){
                vector<string> vmac = split(mac, ':');
                mac = "";
                for(int x = 0; x < vmac.size(); x++) {
                    if(vmac[x].size() < 2){
                        vmac[x] = "0" + vmac[x];
                    }
                    mac = mac + vmac[x] + ":";
                }
                mac.pop_back();
            }
        }
        else mac = "Unknown"; // Could not find an address in the line
        
        if(regex_search(line, m, ipr)) ip = m[0]; // Find the IPv4 address in the line
        
        // Reformat the MAC Address
        vector<string> man = split(mac.substr(0, mac.length() - 9), ':');
        string y = man[0] +  man[1] + man[2];
        transform(y.begin(), y.end(), y.begin(), ::toupper); // All caps...

        string manufacturer = "";

        // Check if the MAC Address has a match in OUI
        for(int i = 0; i < ouiVec.size(); i++){
            string x = ouiVec[i];

            if(x.find(y) != string::npos){
                manufacturer = x;
                break;
            }
        }
 
        // Set the manufacturer
        if(manufacturer == "") manufacturer = "Unknown";
        else manufacturer = manufacturer.substr(7, manufacturer.length());
        
        // Print info on the screen and add each device to targets.txt
        cout << i + 1 << ".\t" << mac << "\t" << ip << "\t"<< manufacturer << "\n";
        ofstream targets;
        targets.open("targets.txt", fstream::app);
        targets << mac + "\n" + ip + "\n";
        targets.close();
    }
}
