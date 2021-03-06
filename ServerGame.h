#pragma once
#include "ServerNetwork.h"
#include "NetworkData.h"
class ServerGame
{
public:

    ServerGame(void);
    ~ServerGame(void);

    void update();
	void receiveFromClients();

	void sendActionPackets(char* content,int wisperId = -1);

private:

   // IDs for the clients connecting for table in ServerNetwork 
    static unsigned int client_id;

   // The ServerNetwork object 
    ServerNetwork* network;

	 char network_data[MAX_PACKET_SIZE];


};

