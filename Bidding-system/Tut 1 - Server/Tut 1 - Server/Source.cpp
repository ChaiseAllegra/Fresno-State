/*
Author: Pindrought
Date: 11/13/2015
This is the solution for the server that you should have at the end of tutorial 1.
*/

#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <string>
#include <ctime>
#include <sstream>

using namespace std;
struct client {
	string name;
	int bid;
	string item;
	SOCKET id;
};
struct item{
	string name;
	int price;
	int amount;
};

const int max=4;
client clients[4];
item items[max];
SOCKET connected[4];

int main()
{
	//Create the item array 
	items[0].name = "ramen";
	items[1].name = "mountain_dew";
	items[2].name = "dorritos";
	items[3].name = "bowsette";

	for (int i = 0; i < max; i++)
	{
		srand(0);
		int tmp = rand() % 10 + 1;
		items[i].price = tmp;

		clients[i].bid = 0;
		clients[i].name = "no";
		clients[i].item = "none";

		srand(0);
		tmp = rand() % 10 + 1;
		items[i].amount = tmp;
	}
	//-------------------------------------------------------------------------//
	int numConnected = 0;
	bool insert = true;
	while(true)
	{
	//WinSock Startup

	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) //If WSAStartup returns anything other than 0, then that means an error has occured in the WinSock Startup.
	{
		MessageBoxA(NULL, "WinSock startup failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	SOCKADDR_IN addr; //Address that we will bind our listening socket to
	int addrlen = sizeof(addr); //length of the address (required for accept call)
	addr.sin_addr.s_addr = inet_addr("192.168.137.1"); //Broadcast locally
	addr.sin_port = htons(1111); //Port
	addr.sin_family = AF_INET; //IPv4 Socket

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL); //Create socket to listen for new connections
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr)); //Bind the address to the socket
	listen(sListen, SOMAXCONN); //Places sListen socket in a state in which it is listening for an incoming connection. Note:SOMAXCONN = Socket Oustanding Max Connections

	SOCKET newConnection; //Socket to hold the client's connection
	newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen); //Accept a new connection
	string sndTmp;
	for (int i = 0; i < max; i++)
		sndTmp = sndTmp + "\n" + items[i].name + "\n" + to_string(items[i].price) + "\n" + to_string(items[i].amount);

	const char* msg = sndTmp.c_str();

	cout << msg << endl;

	if (newConnection == 0) //If accepting the client connection failed
		std::cout << "Failed to accept the client's connection." << std::endl;

	else //If client connection properly accepted
	{
		std::cout << "Client Connected!" << std::endl;
		cout << "connection id " << newConnection << endl;
		insert = true;
		for (int x : connected)
		{
			if (x == newConnection)
				insert = false;
		}
		if (insert)
		{
			connected[numConnected] = newConnection;
			clients[numConnected].id = newConnection;
			if(numConnected<max)
				numConnected++;
		}
		const char* MOTD = msg; //Create buffer with message of the day
		send(newConnection, MOTD, 255, NULL); //Send MOTD buffer
	}

	char incMsg[256];
	recv(newConnection, incMsg, sizeof(incMsg), NULL); //Receive Message of the Day buffer into MOTD array

	cout << "Client Message: " << incMsg << endl << endl;

	string tmp, word;
	tmp = incMsg;
	stringstream iss(tmp);

	for (int i = 0; i < max; i++) {
		for (int j = 0; j < 3; j++) {
			if (clients[i].id == newConnection)
			{
				iss >> word;
				if (j == 0)
					clients[i].name = word;

				if (j == 1)
					clients[i].bid = stoi(word);

				if (j == 2)
					clients[i].item = word;
			}
		}
	}
	
	for (int i = 0; i < max; i++)
	{
		cout << "Name: " << clients[i].name << endl;
		cout << "Bid: " << clients[i].bid << endl;
		cout << "Item: " << clients[i].item << endl;
		cout << "id: " << clients[i].id << endl;
	}
	cout << endl;
	Sleep(8000);
}
	system("pause");
	return 0;
}