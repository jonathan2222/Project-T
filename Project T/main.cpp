#include <iostream>
#include "SFML\Network.hpp"

const unsigned short PORT = 5000;
const std::string IPADDRESS("192.168.10.163");//change to suit your needs

std::string msgSend;

sf::TcpSocket socket;
sf::Mutex globalMutex;
bool quit = false;

void doStuff(void)
{
	static std::string oldMsg;
	while (!quit)
	{
		sf::Packet packetSend;
		globalMutex.lock();
		packetSend << msgSend;
		globalMutex.unlock();

		socket.send(packetSend);

		std::string msg;
		sf::Packet packetReceive;

		socket.receive(packetReceive);
		if ((packetReceive >> msg) && oldMsg != msg && !msg.empty())
		{
			std::cout << msg << std::endl;
			oldMsg = msg;
		}
	}
}

void server(void)
{
	sf::TcpListener listener;
	listener.listen(PORT);
	listener.accept(socket);
	std::cout << "New client connected: " << socket.getRemoteAddress() << std::endl;
}

bool client(void)
{
	if (socket.connect(IPADDRESS, PORT) == sf::Socket::Done)
	{
		std::cout << "Connected\n";
		return true;
	}
	return false;
}

void getInput(void)
{
	std::string s;
	std::cout << "\nEnter \"exit\" to quit or message to send: ";
	getline(std::cin, s);
	if (s == "exit")
		quit = true;
	globalMutex.lock();
	msgSend = s;
	globalMutex.unlock();
}

int main()
{
	sf::Thread* thread = nullptr;

	char who;
	std::cout << "Client (c) or Server (s): ";
	std::cin >> who;

	if (who == 's')
		server();
	else 
		client();

	thread = new sf::Thread(&doStuff);
	thread->launch();

	while (!quit)
	{
		getInput();
	}

	if (thread)
	{
		thread->wait();
		delete thread;
	}

	return 0;
}