/******************************************************************************/
#include "stdafx.h"
#include "Client.h"
/******************************************************************************/
void ServerLoginSuccess(Packet *p) {
	MsgBox.freeze = false;
	MsgBox.push("Welcome to the RakNet Tutorial!");
	logwin.fadeOut();
	StateGame.set(0.5);
}
void ServerLoginFail(Packet *p) {
	MsgBox.freeze = false;
	RakNet::RakString reason;
	RakNet::BitStream receive(p->data, p->length, false);
	receive.IgnoreBits(sizeof(unsigned char) * 8); // ignore the packet ID
	if (StringCompressor().DecodeString(&reason, 55, &receive) == false) return;
	MsgBox.push(reason.C_String(), 2);
}
void ServerRegistrationFail(Packet *p) {
	MsgBox.freeze = false;
	RakNet::RakString reason;
	RakNet::BitStream receive(p->data, p->length, false);
	receive.IgnoreBits(sizeof(unsigned char) * 8); // ignore the packet ID
	if (StringCompressor().DecodeString(&reason, 55, &receive) == false) return;
	MsgBox.push(reason.C_String(), 2);
}
void ServerRegistrationSuccess(Packet *p) {
	MsgBox.freeze = false;
	MsgBox.push("Congratulations! You have successfully registered!");
	regwin.fadeOut();
	titwin.fadeIn();
}
/******************************************************************************/
void ClientMove(Packet *p) {
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

	std::map<SystemAddress, Character*>::iterator i = network.clients.find(addr);
	if(i != network.clients.end()) { // update player
		i->second->push(n);
	} else { // create player
		Game::ObjParams &player = *Game::Objs("Obj/chr/Human/0.obj");
		Character * client = new Character();
		client->create(player);

		network.clients[addr] = client;
	}
}
void ClientChat(Packet *p) {
	RakNet::RakString message;
	StringCompressor strcomp;
	RakNet::BitStream receive(p->data, p->length, false);
	receive.IgnoreBits(sizeof(unsigned char) * 8); // ignore the packet ID
	SystemAddress addr; receive.Read(addr);
	if (strcomp.DecodeString(&message,	50, &receive)	== false)	return; // We should log these hacking events

	std::map<SystemAddress, Character*>::iterator i = network.clients.find(addr);
	if(i != network.clients.end()) {
		i->second->pushChat(S+message.C_String());
	}
}
void ClientLogoff(Packet *p) {
	RakNet::BitStream receive(p->data, p->length, false);
	receive.IgnoreBits(sizeof(unsigned char) * 8);
	SystemAddress addr; receive.Read(addr);

	std::map<SystemAddress, Character*>::iterator i = network.clients.find(addr);
	if(i != network.clients.end()) {
		network.clients.erase(i);
		delete i->second;
	}
}
/******************************************************************************/