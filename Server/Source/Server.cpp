/******************************************************************************/
// TODO: Sort CASES by OCCURANCE
/******************************************************************************/
#include "stdafx.h"
#include "Server.h"
/******************************************************************************/
int main(void) {
	server->SetTimeoutTime(30000, UNASSIGNED_SYSTEM_ADDRESS);
	//	PacketLogger packetLogger; server->AttachPlugin(&packetLogger);

	SystemAddress clientID = UNASSIGNED_SYSTEM_ADDRESS; // Record the first client that connects to us so we can pass it to the ping function

	printf(	"********************************************************************************"
	        "Welcome to the RakNet! Server 6.4 (22th April 2010)\n"
	        "********************************************************************************");

	puts("Starting...");

	// Starting the server is very simple.  2 players allowed. 0 means we don't care about a connectionValidationInteger, and false for low priority threads
	SocketDescriptor socketDescriptor(12345, 0); // Port: 12345
	bool b = server->Startup(128, 30, &socketDescriptor, 1 );
	server->SetMaximumIncomingConnections(128);
	if (b)
		puts("Server started, waiting for connections.\n");
	else {
		puts("Server failed to start.  Terminating.");
		exit(1);
	}
	server->SetOccasionalPing(true);
	server->SetUnreliableTimeout(1000);
	server->GetSockets(sockets);

	printf(	"Ports used by RakNet Tutorial! are:\n");
	for (unsigned int i = 0; i < sockets.Size(); i++) {
		printf("\t%i. %i\n", i + 1, sockets[i]->boundAddress.port);
	}

	printf(	"\nMy IP is %s\n", server->GetLocalIP(0)); printf("My GUID is %s\n", server->GetGuidFromSystemAddress(UNASSIGNED_SYSTEM_ADDRESS).ToString());
	puts(	"\n'quit', 'stat', 'ping', 'kick', 'ban', 'unban', 'clear', 'getconnectionlist'"
	        "\n\n\tCreating account 'guest' with password 'guest'."
			"\n\tCreating account 'chris' with password 'chris'."
			"\n\tCreating account 'mike' with password 'mike'."
			"\n\tCreating account 'esenthel' with password 'esenthel'.\n\n"
	        "********************************************************************************");

	Account *account = new Account();
	account->x = account->y = account->z = 0;
	account->active = false;
	account->password = "guest";
	accounts[RakNet::RakString("guest")] = account;

	Account *account1 = new Account();
	account1->x = account1->y = account1->z = 0;
	account1->active = false;
	account1->password = "chris";
	accounts[RakNet::RakString("chris")] = account1;

	Account *account2 = new Account();
	account2->x = account2->y = account2->z = 0;
	account2->active = false;
	account2->password = "esenthel";
	accounts[RakNet::RakString("esenthel")] = account2;

	Account *account3 = new Account();
	account3->x = account3->y = account3->z = 0;
	account3->active = false;
	account3->password = "mike";
	accounts[RakNet::RakString("mike")] = account3;

	char message[2048];

	/******************************************************************************/
	while (1) { // Loop for input
		RakSleep(0); // This sleep keeps RakNet responsive

#ifdef _WIN32
		if (_kbhit()) {
			gets(message);

			if (strcmp(message, "quit") == 0) { puts("Quitting.");							break; }
			if (strcmp(message, "ping") == 0) { server->Ping(clientID);						continue; }
			if (strcmp(message, "clear") == 0) { system("CLS");								continue; }

			if (strcmp(message, "stat") == 0) {
				rss = server->GetStatistics(server->GetSystemAddressFromIndex(0));
				StatisticsToString(rss, message, 2);
				printf("%s", message);
				printf("Ping %i\n", server->GetAveragePing(server->GetSystemAddressFromIndex(0)));
				continue;
			}
			if (strcmp(message, "getconnectionlist") == 0) {
				SystemAddress systems[10];
				unsigned short numConnections = 10;
				server->GetConnectionList((SystemAddress*) &systems, &numConnections);
				for (int i = 0; i < numConnections; i++) {
					printf("%i. %s\n", i + 1, systems[i].ToString(true));
				}
				continue;
			}
			if (strcmp(message, "kick") == 0) {
				printf("Enter IP to kick.  You can use * as a wildcard: ");
				gets(message); server->CloseConnection(clientID, true, 0); printf("IP %s kicked from server.\n", message);
				continue;
			}
			if (strcmp(message, "ban") == 0) {
				printf("Enter IP to ban.  You can use * as a wildcard: ");
				gets(message); server->AddToBanList(message); printf("IP %s added to ban list.\n", message);
				continue;
			}
			if (strcmp(message, "unban") == 0) {
				printf("Enter IP to remove ban.  You can use * as a wildcard: ");
				gets(message); server->RemoveFromBanList(message); printf("IP %s removed from ban list.\n", message);
				continue;
			}

			// Message now holds what we want to broadcast
			char message2[2048];

			// Append Server: to the message so clients know that it ORIGINATED from the server all messages to all clients come from the server either directly or by being relayed from other clients
			message2[0] = 0;
			strcpy(message2, "Server: ");
			strcat(message2, message);

			// We arbitrarily pick 0 for the ordering stream, UNASSIGNED_SYSTEM_ADDRESS means don't exclude anyone from the broadcast, true means broadcast the message to everyone connected
			server->Send(message2, (const int) strlen(message2) + 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
		}
#endif
		/******************************************************************************/

		// Get a packet from either the server or the client

		for (p = server->Receive(); p; server->DeallocatePacket(p), p = server->Receive()) {
			// We got a packet, get the identifier with our handy function
			packetIdentifier = GetPacketIdentifier(p);

			// Check if this is a network message packet
			switch (packetIdentifier) {
				case ID_DISCONNECTION_NOTIFICATION: // Connection lost normally
					printf("ID_DISCONNECTION_NOTIFICATION from %s\n", p->systemAddress.ToString(true));
					break;
				case ID_CONNECTION_LOST: // Couldn't deliver a reliable packet - i.e. the other system was abnormally terminated
					printf("ID_CONNECTION_LOST from %s\n", p->systemAddress.ToString(true));
					ClientLogoff(p);
					break;
				case ID_NEW_INCOMING_CONNECTION: // Somebody connected.  We have their IP now
					printf("ID_NEW_INCOMING_CONNECTION from %s with GUID %s\n", p->systemAddress.ToString(true), p->guid.ToString());
					clientID = p->systemAddress; // Record the player ID of the client
					break;
				case ID_INCOMPATIBLE_PROTOCOL_VERSION:
					printf("ID_INCOMPATIBLE_PROTOCOL_VERSION\n");
					break;
				case ID_MODIFIED_PACKET: // Cheater!
					printf("ID_MODIFIED_PACKET\n");					
					break;
				case CLIENT_LOGIN:
					printf("CLIENT_LOGIN from %s with GUID %s\n", p->systemAddress.ToString(true), p->guid.ToString());
					ClientLogin(p);
					break;
				case CLIENT_REGISTER:
					printf("CLIENT_REGISTER from %s with GUID %s\n", p->systemAddress.ToString(true), p->guid.ToString());
					ClientRegister(p);
					break;
				case CLIENT_LOGOFF:
					printf("CLIENT_LOGOFF from %s with GUID %s\n", p->systemAddress.ToString(true), p->guid.ToString());
					ClientLogoff(p);
					break;
				case CLIENT_MOVE:
					// Bounce movement data back to the clients
					ClientMove(p);
					server->Send((const char *)p->data, p->length, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0, p->systemAddress, true);
					break;
				case CLIENT_CHAT:
					// Bounce chat data back to the clients
					server->Send((const char *)p->data, p->length, HIGH_PRIORITY, RELIABLE, 0, p->systemAddress, true);
					break;
			}
		}
	}

	server->Shutdown(300);

	RakNetworkFactory::DestroyRakPeerInterface(server); // We're done with the network

	return 0;
}
/******************************************************************************/
// If the first byte is ID_TIMESTAMP, then use 5th byte, else use the 1st byte
unsigned char GetPacketIdentifier(Packet *p) {
	if (p == 0)
		return 255;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP) {
		RakAssert(p->length > sizeof(unsigned char) + sizeof(RakNetTime));
		return (unsigned char) p->data[sizeof(unsigned char) + sizeof(RakNetTime)];
	} else
		return (unsigned char) p->data[0];
}
/******************************************************************************/
void ClientLogin(Packet *p) {
	RakNet::RakString login, password;
	StringCompressor strcomp;

	RakNet::BitStream receive(p->data, p->length, false);
	receive.IgnoreBits(sizeof(unsigned char) * 8); // ignore the packet ID

	if (strcomp.DecodeString(&login,	12, &receive)	== false)	return; // We should log these hacking events
	if (strcomp.DecodeString(&password, 12, &receive)	== false)	return;

	RakNet::RakString &reason = RakNet::RakString();

	if (!Validate(login, reason) || !Validate(password, reason)) { // Warning, keycode isn't validated
		RakNet::BitStream serverSend; serverSend.Write((unsigned char)SERVER_LOGIN_FAIL);
		strcomp.EncodeString(reason, 55, &serverSend);
		server->Send(&serverSend, HIGH_PRIORITY, RELIABLE, 0, p->systemAddress, false);
		return;
	}

	std::map<RakNet::RakString, Account*>::iterator i = accounts.find(login);
	if(i != accounts.end() && i->second->password.StrCmp(password) == 0) {
		if (!i->second->active) {
			i->second->active = true;
			clients[p->guid] = i->second;

			RakNet::BitStream serverSend; serverSend.Write((unsigned char)SERVER_LOGIN_SUCCESS);
			strcomp.EncodeString("Congratulations! You have logged in successfully!", 55, &serverSend);
			server->Send(&serverSend, HIGH_PRIORITY, RELIABLE, 0, p->systemAddress, false);
		} else {
			RakNet::BitStream serverSend; serverSend.Write((unsigned char)SERVER_LOGIN_FAIL);
			strcomp.EncodeString("It seems like you are already logged on!.", 55, &serverSend);
			server->Send(&serverSend, HIGH_PRIORITY, RELIABLE, 0, p->systemAddress, false);
		}
	} else {
		RakNet::BitStream serverSend; serverSend.Write((unsigned char)SERVER_LOGIN_FAIL);
		strcomp.EncodeString("Type in your login or password correctly.", 55, &serverSend);
		server->Send(&serverSend, HIGH_PRIORITY, RELIABLE, 0, p->systemAddress, false);
	}
}

void ClientMove(Packet *p)
{
	RakNet::BitStream receive(p->data, p->length, false);

	RakNetTime packetTime;
	receive.IgnoreBits(sizeof(unsigned char) * 8);
	receive.Read(packetTime);
	receive.IgnoreBits(sizeof(unsigned char) * 8);

	SystemAddress addr; receive.Read(addr);
	Netpoint n; receive.ReadVector( n.pos.x, n.pos.y, n.pos.z );
				receive.ReadVector( n.vel.x, n.vel.y, n.vel.z );
				receive.ReadVector( n.ang.x, n.ang.y, n.ang.z );
				n.packetTime = packetTime;
	printf("Move recieved %d, %d, %d", n.pos.x, n.pos.y, n.pos.z);
}

void ClientRegister(Packet *p) {
	RakNet::RakString login, password;
	StringCompressor strcomp;

	RakNet::BitStream receive(p->data, p->length, false);
	receive.IgnoreBits(sizeof(unsigned char) * 8); // ignore the packet ID

	if (strcomp.DecodeString(&login,	12, &receive)	== false)	return; // We should log these hacking events
	if (strcomp.DecodeString(&password, 12, &receive)	== false)	return;

	RakNet::RakString &reason = RakNet::RakString();

	if (!Validate(login, reason) || !Validate(password, reason)) {
		RakNet::BitStream serverSend; serverSend.Write((unsigned char)SERVER_REGISTER_FAIL);
		strcomp.EncodeString(reason, 55, &serverSend);
		server->Send(&serverSend, HIGH_PRIORITY, RELIABLE, 0, p->systemAddress, false);
		return;
	}


	std::map<RakNet::RakString, Account*>::iterator j = accounts.find(login);
	if(j == accounts.end()) {
		Account *account = new Account();
		account->password = password;
		account->active = false;
		account->x = 1; account->y = 1; account->z = 1;

		accounts[login] = account; // create account

		RakNet::BitStream serverSend; serverSend.Write((unsigned char)SERVER_REGISTER_SUCCESS);
		server->Send(&serverSend, HIGH_PRIORITY, RELIABLE, 0, p->systemAddress, false);
	} else {
		RakNet::BitStream serverSend; serverSend.Write((unsigned char)SERVER_REGISTER_FAIL);
		strcomp.EncodeString("Somebody else has already taken that username.", 55, &serverSend);
		server->Send(&serverSend, HIGH_PRIORITY, RELIABLE, 0, p->systemAddress, false);
	}
}
void ClientLogoff(Packet *p) {
	// Echo the logoff back to the other clients, so they can remove the player locally
	server->Send((const char *)p->data, p->length, HIGH_PRIORITY, RELIABLE, 0, p->systemAddress, true);

	std::map<RakNetGUID, Account*>::iterator i = clients.find(p->guid);
	if (i != clients.end()) {
		i->second->active = false;
		clients.erase(i);
		printf("\nRemoving Client Successfull!\n");
	}
}
/******************************************************************************/