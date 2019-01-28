/*
Author: Pindrought
Date: 11/13/2015
This is the solution for the client that you should have at the end of tutorial 1.
*/
#pragma comment(lib,"ws2_32.lib")

#include <WinSock2.h>
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>

using namespace std;
struct item {
	string name;
	int price;
	int amount;
};

item items[4];

struct client {
	string name;
	int bid;
	string item;
	int maxBid;
};

int max = rand() % 100 + 1;

int main()
{
	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) //If WSAStartup returns anything other than 0, then that means an error has occured in the WinSock Startup.
	{
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	
	SOCKADDR_IN addr; //Address to be binded to our Connection socket
	int sizeofaddr = sizeof(addr); //Need sizeofaddr for the connect function
	addr.sin_addr.s_addr = inet_addr("192.168.137.1"); //Address = localhost (this pc)
	addr.sin_port = htons(1111); //Port = 1111
	addr.sin_family = AF_INET; //IPv4 Socket

	SOCKET Connection = socket(AF_INET, SOCK_STREAM, NULL); //Set Connection socket
	if (connect(Connection, (SOCKADDR*)&addr, sizeofaddr) != 0) //If we are unable to connect...
	{
		MessageBoxA(NULL, "Failed to Connect", "Error", MB_OK | MB_ICONERROR);
		return 0; //Failed to Connect
	}
	std::cout << "Connected!" << std::endl;

	char MOTD[256];
	recv(Connection, MOTD, sizeof(MOTD), NULL); //Receive Message of the Day buffer into MOTD array

	string tmp,word;
	tmp = MOTD;
	stringstream iss(tmp);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			iss >> word;
			if (j == 0)
				items[i].name = word;

			if (j == 1)
				items[i].price = stoi(word);

			if (j == 2)
				items[i].amount = stoi(word);
		}
	}

	cout << endl;
	for (int i = 0; i < 4; i++)
	{
		cout << "Name: " << items[i].name << endl;
		cout << "Price: " << items[i].price << endl;
		cout << "Amount: " << items[i].amount << endl;
	}
	cout << endl;

	client me;
	me.name = "frank";
	srand(0);
	int randomItem = rand() % 3;
	
	me.item = items[randomItem].name;
	me.maxBid = max;

	if(me.maxBid >= items[randomItem].price + 1)
		me.bid = items[randomItem].price + 1;

	string sndTmp = me.name + "\n" + to_string(me.bid) + "\n" + me.item;
	const char* msg = sndTmp.c_str();
	send(Connection, msg, 255, NULL); 

	//std::cout << "MOTD:" << MOTD << std::endl;
	while (true)
	{
		Sleep(10);
	}
}

