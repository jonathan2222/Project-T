#include <iostream>
#include "SFML\Network.hpp"

const unsigned char PORT = 55002;
const std::string IPADDRESS("192.168.10.163");

void server(void)
{
	// Create a socket and bind it to the port 55002
	sf::UdpSocket socket;
	socket.bind(PORT);
	// Receive a message from anyone
	char buffer[1024];
	std::size_t received = 0;
	sf::IpAddress sender;
	unsigned short port;
	socket.receive(buffer, sizeof(buffer), received, sender, port);
	std::cout << sender.toString() << " said: " << buffer << std::endl;
	// Send an answer
	std::string message = "Welcome " + sender.toString();
	socket.send(message.c_str(), message.size() + 1, sender, port);
}

bool client(void)
{
	// Create a socket and bind it to the port 55001
	sf::UdpSocket socket;
	socket.bind(PORT);
	// Send a message to 192.168.1.50 on port 55002
	std::string message = "Hi, I am " + sf::IpAddress::getLocalAddress().toString();
	socket.send(message.c_str(), message.size() + 1, "192.168.1.50", PORT);
	// Receive an answer (most likely from 192.168.1.50, but could be anyone else)
	char buffer[1024];
	std::size_t received = 0;
	sf::IpAddress sender;
	unsigned short port;
	socket.receive(buffer, sizeof(buffer), received, sender, port);
	std::cout << sender.toString() << " said: " << buffer << std::endl;
}

int main()
{
	char who;
	std::cout << "Client (c) or Server (s): ";
	std::cin >> who;

	if (who == 's')
		server();
	else 
		client();

	return 0;
}