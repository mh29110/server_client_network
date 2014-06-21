#include "stdafx.h"
#include "ClientGame.h"


ClientGame::ClientGame(void)
{
	network = new ClientNetwork();

	//send init packet ;
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet ;
	packet.packet_type = INIT_CONNECTION;
	packet.wisperId = -1;
	strcpy_s(packet.words,"heeee");
	packet.serialize(packet_data);
	NetworkService::sendMessage(network->ConnectSocket,packet_data,packet_size);
}


ClientGame::~ClientGame(void)
{
}

void ClientGame::sendActionPackets()
{
    // send action packet
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.packet_type = BROAD_CAST;

	strcpy_s(packet.words,"127.0.0.1");

    packet.serialize(packet_data);

    NetworkService::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void ClientGame::update()
{
    Packet packet;
    int data_length = network->receivePackets(network_data);

    if (data_length <= 0) 
    {
        //no data recieved
        return;
    }

    int i = 0;
    while (i < (unsigned int)data_length) 
    {
        packet.deserialize(&(network_data[i]));
        i += sizeof(Packet);

        switch (packet.packet_type) {

            case ACTION_EVENT:

                printf("client received action event packet from server\n");

                //sendActionPackets();

                break;
			case BROAD_CAST:
				
				printf("client received broadcast event packet from server\n {--");

				char* str ;
				if(packet.wisperId == NetworkService::SystemId)
				{
					str = "system broadcast :";
					printf("%s say: %s --}",str, packet.words);
				}
				else
				{
					printf("%d say: %s --}",packet.wisperId, packet.words);
				}

				//sendActionPackets();
				

				break;

			default:

                printf("error in packet types\n");

                break;
        }
    }
}