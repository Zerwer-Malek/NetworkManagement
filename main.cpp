#include "main.h"
#include "commands/getip.h"
#include "commands/mitm.h"
#include "commands/list.h"
#include "commands/id.h"
#include "commands/save/oui.h"

// Function takes a c string and return the output (C)
string returnCmd(const char* cmd) {
    array<char, 128> buffer;
    string output;
    shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw runtime_error("Command Failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            output += buffer.data();
    }
    return output;
}

// Spits a string at desired character and returns in a vector of strings
vector<string> split(string input, char c) {
    stringstream ss(input);
    string s;
    vector<string> out;
    while (getline(ss, s, c)) {
        out.push_back(s);
    }
    return out;
}

// Takes file reads contents and returns them as a string
string filetostr(string name) {
    ifstream file(name);
    stringstream buffer;
    buffer << file.rdbuf();
    string i = buffer.str();
    return i;
}

// Header function for clearing and starting program
void header() {
    system("clear");
    cout << "\t\t\t\tMiTM Toolkit\n";
}

// Help function that displays all available commands
void help() {
    cout << "clear - Clear screen and display header\n"
            "dos - DOS a target from targets.txt\n"
            "dosall - Dos everyone listed on targets.txt\n"
            "exit - Exit the program\n"
            "get - Update, display and save ARP cache (Saved to targets.xt)\n"
            "help - Display this help menu\n"
            "list - List ips in targets.txt\n"
            "mitm - Perform a mitm with a target from targets.txt\n"
            "newid - Spoof mac to a x:RR:RR:RR where x is a random"
            "choice from macchoices.txt and RR is a random hex number from 10"
            " to 255 (10 , FF)\n"
            "routermac - Set mac to resemble routers mac\n";
        }

/*
 * Main function:
 * Starts with header funtion on startup
 * Runs main terminal loop
 * Prints >>> and takes string input arg
 * Test if arg is a command and then run corresponding function
*/
int main(){
    header();
    while(true) {
        cout << ">>> ";
        string arg;
        cin >> arg;

        if(arg == "get") get();
        else if(arg == "help") help();
        else if(arg == "clear") header();
        else if(arg == "exit") exit(0);
        else if(arg == "mitm") findIp(false);
        else if(arg == "dos") findIp(true);
        else if(arg == "dosall") dosall(1.0);
        else if(arg == "list") list();
        else if(arg == "newid") newid();
        else if(arg == "routermac") routermac();
        else cout << "Command not found!\n";
    }
}
