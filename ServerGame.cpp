#include "stdafx.h"
#include "ServerGame.h"
unsigned int ServerGame::client_id; 

ServerGame::ServerGame(void)
{
    // id's to assign clients for our table
    client_id = 0;

    // set up the server network to listen 
    network = new ServerNetwork(); 
}

void ServerGame::update() 
{

    // get new clients
    if(network->acceptNewClient(client_id))
    {
        printf("client %d has been connected to the server\n",client_id); 

        client_id++;
    }

	 receiveFromClients();
}

void ServerGame::receiveFromClients()
{
    Packet packet;

    // go through all clients
    std::map<unsigned int, SOCKET>::iterator iter;

    for(iter = network->sessions.begin(); iter != network->sessions.end(); iter++)
    {
        // get data for that client
        int data_length = network->receiveData(iter->first, network_data);

        if (data_length <= 0) 
        {
            //no data recieved
            continue;
        }

        int i = 0;
        while (i < (unsigned int)data_length) 
        {
            packet.deserialize(&(network_data[i]));
            i += sizeof(Packet);

            switch (packet.packet_type) {

                case INIT_CONNECTION:

					iter;
					printf("server received init packet from client; your id is : %d: %s\n",iter->first,packet.words);
					sendActionPackets("this is a system broadcast ,welcome to this world!\n",-1);
					
					sendActionPackets(packet.words,iter->first);
                    break;

                case BROAD_CAST:

                    printf("server received broad cast!client %d,%s\n" ,iter->first,packet.words);
					sendActionPackets(packet.words,iter->first);
                    break;

                default:

                    printf("error in packet types\n");

                    break;
            }
        }
    }
}

ServerGame::~ServerGame(void)
{
}

void ServerGame::sendActionPackets(char* content,int wisperId)
{
	//send action packet

	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;
	packet.packet_type = BROAD_CAST;
	packet.wisperId = NetworkService::SystemId;
	strcpy_s(packet.words,content);

	packet.serialize(packet_data);
	network->sendToAll(packet_data,packet_size);
}