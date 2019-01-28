/*
Author: Pindrought
Date: 11/13/2015
This is the solution for the server that you should have at the end of tutorial 2.
*/

#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996) 
#include <WinSock2.h>
#include <string>
#include <iostream>
#include <ctime>
#include <sstream>

using namespace std;

class item {
public:
	string name;
	int price;
	int amount;
	string winner;
	string lead;
	clock_t startTime;
	double secondsToDelay = 20;
	bool timerStarted;
	void startTimer()
	{
		this->startTime = clock(); //Start timer
		return;
	}
	int timer() {
		double secondsPassed = (clock() - startTime) / CLOCKS_PER_SEC;
		cout << "seconds passed: " << secondsPassed << endl;
		if (secondsPassed >= secondsToDelay)
		{
			amount--;
			this->timerStarted = false;
			this->winner = this->lead;
			cout << "------------------" << endl;
			cout << "The winner of " << name << " is " << winner << endl;
			cout << "---------------------" << endl;
		}

		return 0;
	}
};

struct client {
	string name;
	int bid;
	string item;
	SOCKET id;
};

const int max = 4;
client clients[4];
client winners[4];
item items[max];
SOCKET Connections[100];
int ConnectionCounter;

void ClientHandlerThread(int index) //index = the index in the SOCKET Connections array
{
	cout << "index " << index << endl;
	char buffer[256]; //Buffer to receive and send out messages from/to the clients
	while (true)
	{
		recv(Connections[index], buffer, 256, NULL); //get message from client
		string tmp, word;
		tmp = buffer;
		stringstream iss(tmp);

		for (int i = 0; i < 3; i++) {
			iss >> word;
			if (i == 0)
				clients[index].name = word;
			if (i == 1)
				clients[index].bid = stoi(word);
			if (i == 2)
				clients[index].item = word;
		}




		for (int i = 0; i < ConnectionCounter; i++)
		{
			for (int p = 0; p < 4; p++)
			{
				if (clients[i].item == items[p].name)
				{
					if (items[p].timerStarted == false)
					{
						items[p].startTimer();//start the timer
						items[p].timerStarted = true;
						items[p].winner = "NA";
						cout << "----------------------------" << endl;
						cout << "-------Bidding has Started for " << items[p].name << endl;
						cout << "--------------------------------" << endl;
					}
					if (clients[i].bid > items[p].price)
					{
						items[p].price = clients[i].bid;
						items[p].lead = clients[i].name;
						cout << "----------------------------" << endl;
						cout << "new highest bid: " << items[p].price << endl;
						cout << "--------------------------------" << endl;
					}
				}
			}
		}
		for (int i = 0; i < 4; i++)
		{
			if (items[i].timerStarted == true)
				items[i].timer();//increment the timer
		}

		cout << "---------------Client sent back this info: --------------------------" << endl;
		for (int i = 0; i < max; i++)
		{
			cout << "Name: " << clients[i].name << endl;
			cout << "Bid: " << clients[i].bid << endl;
			cout << "Item: " << clients[i].item << endl;
			cout << "id: " << clients[i].id << endl;
		}
		cout << endl;
		cout << "---------------Current winnners--------------------------------------" << endl;
		for (int i = 0; i < 4; i++)
		{
			cout<<"item name: "<<items[i].name << endl;
			cout <<"current winner: "<< items[i].winner << endl;
		}
		cout << endl;
		cout << endl;
		string sndTmp;
		for (int i = 0; i < max; i++)
			sndTmp = sndTmp + " " + items[i].name + " " + to_string(items[i].price) + " " + to_string(items[i].amount) + " " + items[i].lead + " " + items[i].winner;
		const char* msg = sndTmp.c_str();
		//cout << "CC---------------------------"<<ConnectionCounter << endl;
		for (int i = 0; i < ConnectionCounter; i++) //For each client connection
		{
			//if (i == index) //Don't send the chat message to the same user who sent it
			//	continue; //Skip user
			//cout << "sending" << endl;

			send(Connections[i], msg, 256, NULL);//send the chat message to this client
		}
		//Sleep(1000);
	}
}

int main()
{
	//Create the item array 
	items[0].name = "ramen";
	items[1].name = "mountain_dew";
	items[2].name = "dorritos";
	items[3].name = "bowsette";
	ConnectionCounter = 0;
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
		items[i].amount = 20;// tmp;
		items[i].lead = "notyou";
		items[i].winner = "NA";
	}

	//Winsock Startup
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
	//int ConnectionCounter = 0; //# of client connections
	for (int i = 0; i < 100; i++)
	{
		newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen); //Accept a new connection
		string sndTmp;
		for (int i = 0; i < max; i++)
			sndTmp = sndTmp + " " + items[i].name + " " + to_string(items[i].price) + " " + to_string(items[i].amount) + " " + items[i].lead + " " + items[i].winner;
		const char* msg = sndTmp.c_str();
		//cout << msg << endl;
		if (newConnection == 0) //If accepting the client connection failed
		{
			std::cout << "Failed to accept the client's connection." << std::endl;
		}
		else //If client connection properly accepted
		{
			std::cout << "Client Connected!" << std::endl;
			const char* MOTD = msg; //Create buffer with message of the day
			//cout << MOTD << endl;
			send(newConnection, MOTD, 256, NULL); //Send MOTD buffer
			Connections[i] = newConnection; //Set socket in array to be the newest connection before creating the thread to handle this client's socket.
			ConnectionCounter += 1; //Incremenent total # of clients that have connected
			cout << ConnectionCounter << endl;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(i), NULL, NULL); //Create Thread to handle this client. The index in the socket array for this thread is the value (i).
		}
	}

	system("pause");
	return 0;
}