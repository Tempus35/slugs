#include "cNetwork.h"

cNetwork::cNetwork() {

}

cNetwork::~cNetwork() {
	client.Close();	
}

bool cNetwork::StartServer() {
	if (!listener.Listen(PORT))
		return false;

	selector.Add(listener);

	return true;
}

void cNetwork::Update() {
	unsigned int iSockets = selector.Wait();

  for (unsigned int i = 0; i < iSockets; i++)
  {
		sf::SocketTCP socket = selector.GetSocketReady(i);

		if (socket == listener) {
			//client connected !
			listener.Accept(client, &clientAddress);		
			selector.Add(client);

		} else {
			//receive message or packet
			sf::Packet Packet;
      if (socket.Receive(Packet) == sf::Socket::Done)
      {
          std::string Message;
          Packet >> Message;
          //Message
      }
		}
  }
}

bool cNetwork::ConnectToServer(std::string sIP, std::string sPort){
	if (client.Connect(PORT, sIP.c_str()) != sf::Socket::Done) 
		return false;

	return true;
}

bool cNetwork::Send(std::string sText) {
	sf::Packet Packet;
	Packet << sText;
	if (client.Send(Packet) != sf::Socket::Done) 
		return false;

	return true;
}

void cNetwork::Disconnect() {
	client.Close();
}