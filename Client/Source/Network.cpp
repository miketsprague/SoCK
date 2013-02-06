/******************************************************************************/
// TODO: Sort CASES by OCCURANCE
/******************************************************************************/
#include "stdafx.h"
#include "Client.h"
/******************************************************************************/
bool Network::Connect(const char * ipaddr) {
	client = RakNetworkFactory::GetRakPeerInterface();
	client->AllowConnectionResponseIPMigration(false);
	SocketDescriptor socketDescriptor(0, 0);
	client->Startup(8, 30, &socketDescriptor, 1);
	client->SetMaximumIncomingConnections(128);
	client->SetOccasionalPing(true);
	//return client->Connect("5.151.120.186", 12345, 0, 0);
	return client->Connect(ipaddr, 12345, 0, 0);
}
void Network::Logoff() {
	for ( std::map<SystemAddress, Character*>::iterator i = network.clients.begin(); i != network.clients.end(); ++i ) {
		network.clients.erase(i);
		delete i->second;
	}

	MsgBox.push("Logging Off!",0.5);
	RakNet::BitStream clientSend; 
	clientSend.Write((unsigned char) CLIENT_LOGOFF);
	clientSend.Write(network.client->GetInternalID());
	network.client->Send(&clientSend, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
}
void Network::ShutNetworking() {
	client->Shutdown(300);

	RakNetworkFactory::DestroyRakPeerInterface(client);
}
void Network::CheckMessages() {
	// RakSleep(30);
	for (p = client->Receive(); p; client->DeallocatePacket(p), p = client->Receive()) {
		packetIdentifier = GetPacketIdentifier(p); // We got a packet, get the identifier with our handy function

		switch (packetIdentifier) { // Check if this is a network message packet
			case ID_DISCONNECTION_NOTIFICATION: // Connection lost normally
				MsgBox.push("You have lost connection.");
				break;
			case ID_ALREADY_CONNECTED:
				MsgBox.push("It seems like you are already connected.\nIf you lost connection, please wait for the server to realise.");
				break;
			case ID_INCOMPATIBLE_PROTOCOL_VERSION:
				MsgBox.push("The rules of your network are incompatible with our server technology.\nWhat on earth are you using?");
				break;
			case ID_REMOTE_DISCONNECTION_NOTIFICATION: // Server telling the clients of another client disconnecting gracefully.  You can manually broadcast this in a peer to peer enviroment if you want.
				break;
			case ID_REMOTE_CONNECTION_LOST: // Server telling the clients of another client disconnecting forcefully.  You can manually broadcast this in a peer to peer enviroment if you want.
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION: // Server telling the clients of another client connecting.  You can manually broadcast this in a peer to peer enviroment if you want.
				break;
			case ID_CONNECTION_BANNED: // Banned from this server
				MsgBox.push("Admins are bloody annoying when they ban you.", 5);
				break;
			case ID_CONNECTION_ATTEMPT_FAILED:
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS: // Sorry, the server is full.  I don't do anything here but a real app should tell the user
				MsgBox.push("Too many people just connected before we had time to improve our network architecture.");
				break;
			case ID_MODIFIED_PACKET: // Cheater!
				MsgBox.push("We caught you hacking! \nWe'll probably contact you shortly to see if we can learn from each other.");
				break;
			case ID_INVALID_PASSWORD:
				MsgBox.push("Your main server password is incorrect.");
				break;
			case ID_CONNECTION_LOST: // Couldn't deliver a reliable packet - i.e. the other system was abnormally terminated
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED: // This tells the client they have connected
				printf("ID_CONNECTION_REQUEST_ACCEPTED to %s with GUID %s\n", p->systemAddress.ToString(true), p->guid.ToString());
				printf("My external address is %s\n", client->GetExternalID(p->systemAddress).ToString(true));
				break;
				/******************************************************************************/
			case SERVER_LOGIN_SUCCESS:
				ServerLoginSuccess(p);
				break;
			case SERVER_LOGIN_FAIL:
				ServerLoginFail(p);
				break;
			case SERVER_REGISTER_SUCCESS:
				ServerRegistrationSuccess(p);
				break;
			case SERVER_REGISTER_FAIL:
				ServerRegistrationFail(p);
				break;
				/******************************************************************************/
			case CLIENT_MOVE:
				if (Players.elms())
					ClientMove(p);
				break;
			case CLIENT_CHAT:
				ClientChat(p);
				break;
			case CLIENT_LOGOFF:
				ClientLogoff(p);
				break;
				/******************************************************************************/
		}
	}
}
unsigned char Network::GetPacketIdentifier(Packet *p) {
	if (p == 0)
		return 255;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP) {
		assert(p->length > sizeof(unsigned char) + sizeof(RakNetTime));
		return (unsigned char) p->data[sizeof(unsigned char) + sizeof(RakNetTime)];
	} else
		return (unsigned char) p->data[0];
}
/******************************************************************************/