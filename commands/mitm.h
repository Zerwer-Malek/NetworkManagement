#ifndef MITM_H
#define MITM_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <signal.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <tins/arp.h>
#include <tins/network_interface.h>
#include <tins/utils.h>
#include <tins/ethernetII.h>
#include <tins/packet_sender.h>

using namespace Tins;
using namespace std;

/*
 * Functions to preform an ARP Spoof
 * requires two packets to be send:
 * - One for the router
 * - One for the victim
*/

void findIp(bool dos);
void dosall(float speed);

#endif
