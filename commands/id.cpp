#include "id.h"
#include "../main.h"

using namespace std;

void newid(){
    string choices = "a8:bb:cf\n"
                     "a8:5c:2c\n"
                     "4c:57:ca\n"
                     "78:67:d7\n"
                     "a0:99:9b\n"
                     "fc:fc:48\n"
                     "b4:18:d1\n"
                     "d4:a3:3d\n"
                     "e8:80:2e\n"
                     "2c:f0:ee\n"
                     "98:ca:33\n"
                     "84:29:99\n"
                     "d0:c5:f3\n";
    vector<string> choiceV= split(choices, '\n');
    string mac;
    mac = choiceV[rand() % choiceV.size()] + ":a3:d4:62";
    string cmd ="sudo launchctl unload -w  "
    "/System/Library/LaunchDaemons/com.apple.netbiosd.plist "
    "&& sudo ifconfig en0 ether " + mac + ""
    " && sudo ifconfig en0 down && sudo ifconfig en0 up";
    system(cmd.c_str());
    
    cout << "Mac changed to " << mac << endl;
}

void routermac(){
    vector<string> lines = split(filetostr("targets.txt"), '\n');
    string mac = lines[0].substr(0, 15);
    mac = mac + "4f";
    cout << "Mac set to " << mac << endl;
    mac = "sudo launchctl unload -w "
    "/System/Library/LaunchDaemons/com.apple.netbiosd.plist"
    " && sudo ifconfig en0 ether " + mac +
    " && sudo ifconfig en0 down && sudo ifconfig en0 up";
    system(mac.c_str());
    
}
