#include "globals.h"

#define PORT 45674

class cNetwork 
{
public:
	cNetwork();
	~cNetwork();
	bool StartServer();
	void Update();
	bool ConnectToServer(std::string sIP, std::string sPort);
	bool Send(std::string sText);
	void Disconnect();

private:
	sf::SocketTCP client;
	sf::IPAddress clientAddress;

	sf::SocketTCP listener;
	sf::SelectorTCP selector;
};