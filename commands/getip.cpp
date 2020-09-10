#include "getip.h"
#include "save/oui.h"
#include "../main.h"

using namespace std;

void get(){
    ofstream reset;
    reset.open("targets.txt");
    reset.close();
    system("sudo arp-scan --localnet > /dev/null");
    string ret = returnCmd("sudo arp -a");
    vector<string> arpRet = split(ret, '\n');
    
    regex macr("(([a-f\\d]{1,2}\\:){5}[a-f\\d]{1,2})");
    regex ipr("[0-9]+(?:\\.[0-9]+){3}");
    smatch m;
    
    vector<string> ouiVec = split(oui, '\n');
    
    for(int i = 0; i<arpRet.size(); i++){
        string line = arpRet.at(i);
        string mac;
        string ip;
        if(regex_search(line, m, macr)){
            mac = m[0];
            if(mac.length() <= 16){
                vector<string> vmac = split(mac, ':');
                mac = "";
                for(int x = 0; x < vmac.size(); x++){
                    if(vmac[x].size() < 2){
                        vmac[x] = "0" + vmac[x];
                    }
                    mac = mac + vmac[x] + ":";
                }
                mac.pop_back();
            }
        }
        else{
            mac = "Unknown";
        }
        if(regex_search(line, m, ipr)){
            ip = m[0];
        }
        vector<string> man = split(mac.substr(0, mac.length() - 9), ':');

        string y = man[0] +  man[1] + man[2];
        string manufacturer = "";
        transform(y.begin(), y.end(), y.begin(), ::toupper);

        for(int i = 0; i < ouiVec.size(); i++){
            string x = ouiVec[i];

            if(x.find(y) != string::npos){
                manufacturer = x;
                break;
            }
        }
 
        if(manufacturer == ""){
            manufacturer = "Unknown";
        }
        else{
            manufacturer = manufacturer.substr(7, manufacturer.length());
        }
        cout << i + 1 << ".\t" << mac << "\t" << ip << "\t"<< manufacturer << "\n";
        ofstream targets;
        targets.open("targets.txt", fstream::app);
        targets << mac + "\n" + ip + "\n";
        targets.close();
    }
}
