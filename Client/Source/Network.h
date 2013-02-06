/******************************************************************************/
struct Network {
	RakNetStatistics *rss;
	RakPeerInterface *client;
	SystemAddress clientID;
	Packet* p;

	std::map<SystemAddress, Character*> clients;

	unsigned char packetIdentifier;
	unsigned char GetPacketIdentifier(Packet *p);

	bool Connect(const char * ipaddr);
	void Logoff();
	void ShutNetworking();
	void CheckMessages();
};
/******************************************************************************/