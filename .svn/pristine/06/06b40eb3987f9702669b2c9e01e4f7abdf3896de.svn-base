/******************************************************************************/
//#include "MessageIdentifiers.h"
//#include "RakNetworkFactory.h"
//#include "RakPeerInterface.h"
//#include "RakNetStatistics.h"
//#include "StringCompressor.h"
//#include "RakNetTypes.h"
//#include "BitStream.h"
//#include "RakSleep.h"
//#include "RakString.h"
//#include "RakPeer.h"
//#include "PacketLogger.h"
#include <assert.h>
#include <cstdio>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <map>
/******************************************************************************/
#ifdef _WIN32
#include "Kbhit.h"
#endif
#if defined(_CONSOLE_2)
#include "Console2SampleIncludes.h"
#endif
#ifdef _CONSOLE_2
_CONSOLE_2_SetSystemProcessParams
#endif
/******************************************************************************/
unsigned char GetPacketIdentifier(Packet *p);
unsigned char packetIdentifier;
/******************************************************************************/
void ClientLogin(Packet *p);
void ClientMove(Packet *p);
void ClientRegister(Packet *p);
void ClientLogoff(Packet *p);
/******************************************************************************/
struct Account {
	RakNet::RakString password;
	float x, y, z;
	bool active;
};
/******************************************************************************/
std::map<RakNet::RakString,	Account*>			accounts;	// <login, Account*>
std::map<RakNetGUID,		Account*>			clients;		// only ever in memory
/******************************************************************************/
DataStructures::List<RakNetSmartPtr<RakNetSocket>> sockets;
RakPeerInterface *server = RakNetworkFactory::GetRakPeerInterface();
RakNetStatistics *rss;
RakNet::BitStream output;
Packet *p;
/******************************************************************************/
#include "../../RakNet/Bridge.h"
#include "../../RakNet/NetPoint.h"
#include "Tools.h"
//#include "vld.h" // not required, Visual Leak Detector plugin for Visual Studio
/******************************************************************************/