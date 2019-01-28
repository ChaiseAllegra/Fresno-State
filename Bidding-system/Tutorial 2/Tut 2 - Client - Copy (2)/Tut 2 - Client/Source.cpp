/*
Author: Pindrought
Date: 11/13/2015
This is the solution for the client that you should have at the end of tutorial 2.
*/

#pragma comment(lib,"ws2_32.lib") //Required for WinSock
#pragma warning(disable:4996) 
#include <WinSock2.h> //For win sockets
#include <string> //For std::string
#include <iostream> //For std::cout, std::endl, std::cin.getline
#include <sstream>
#include <ctime>

using namespace std;
bool choose = true;
struct item {
	string name;
	int price;
	int amount;
	string lead;
	string winner;
};

item items[4];
struct winnings {
	string item;
	int owned;
};
struct client {
	string name;
	int bid;
	string item;
	int maxBid;
	winnings wins[4];
};

int max = rand() % 100 + 1;
client me;

SOCKET Connection;//This client's connection to the server
const char* msg;
void ClientThread()
{
	for (int i = 0; i < 4; i++) {
		items[i].name = "empty";
		items[i].price = 0;
		items[i].amount = 0;
	}
	char buffer[256]; //Create buffer to hold messages up to 256 characters
	//while (true)
	//{
		recv(Connection, buffer, 256, NULL); //receive buffer
		//cout << "--------The server sent this info: -----------------------" << endl;
		//std::cout << buffer << std::endl; //print out buffer

		string tmp, word;
		tmp = buffer;
		//cout << buffer << endl;
		stringstream iss(tmp);

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 5; j++) {
				iss >> word;
				//cout << word << endl;
				if (j == 0)
					items[i].name = word;
				if (j == 1)
					items[i].price = stoi(word);
				if (j == 2)
					items[i].amount = stoi(word);
				if (j == 3)
					items[i].lead = word;
				if (j == 4)
					items[i].winner = word;
			}
		}
		/*cout << "Items array now conatins the following " << endl;
		for (int i = 0; i < 4; i++)
		{
			cout << "Name: " << items[i].name << endl;
			cout << "Price: " << items[i].price << endl;
			cout << "Amount: " << items[i].amount << endl;
		}
		cout << "-------------------------------------------------" << endl << endl;
		*/

		//choose a random item to bid on
		if (choose)
		{
			srand(NULL);
			int randomItem = rand() % 3;
			me.item = items[randomItem].name;
			choose = false;
		}
		//random chance they dont bid
		srand(NULL);
		int doBid = rand() % 10+1;

		for (int i = 0; i < 4; i++)//looks at current highest price and raises it 
		{
			//cout << items[i].name << endl;
			if (me.item == items[i].name)
			{
				if (items[i].price >= me.bid && items[i].price < me.maxBid&&items[i].lead!=me.name&&doBid>3)
				{
					me.bid = items[i].price + 1;
					cout << "setting the price"<<me.bid << endl;
				}
			}
			if (items[i].winner == me.name)//add the the clients number of items they own
			{
				for (int j = 0; j < 4; j++)
				{
					if (items[i].name == me.wins[j].item)
					{
						me.wins[j].owned++;
						choose = true;
					}
				}
			}
		}

		cout << "------------client information------------" << endl;
		cout << me.name << endl;
		for (int i = 0; i < 4; i++)
		{
			if (me.wins[i].owned > 0)
			{
				cout << me.wins[i].item << endl;
				cout << me.wins[i].owned << endl;
			}

		}
		cout << "--------------------------" << endl;
		
	//}
	//Sleep(2000);
}

int main()
{
	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	
	SOCKADDR_IN addr; //Address to be binded to our Connection socket
	int sizeofaddr = sizeof(addr); //Need sizeofaddr for the connect function
	addr.sin_addr.s_addr = inet_addr("192.168.137.1"); //Address = localhost (this pc)
	addr.sin_port = htons(1111); //Port = 1111
	addr.sin_family = AF_INET; //IPv4 Socket

	Connection = socket(AF_INET, SOCK_STREAM, NULL); //Set Connection socket
	if (connect(Connection, (SOCKADDR*)&addr, sizeofaddr) != 0) //If we are unable to connect...
	{
		MessageBoxA(NULL, "Failed to Connect", "Error", MB_OK | MB_ICONERROR);
		return 0; //Failed to Connect
	}

	me.name = "Client3";


	me.item = "ramen";//items[randomItem].name;
	me.maxBid = 20;
	me.bid = 2;
	me.wins[0].item = "ramen";
	me.wins[1].item = "mountain_dew";
	me.wins[2].item = "dorritos";
	me.wins[3].item = "bowsette";

	std::cout << "Connected!" << std::endl;
	//CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL); //Create the client thread that will receive any data that the server sends.
	
	char buffer[256];
	while (true)
	{
		ClientThread();
		string sndTmp = me.name + " " + to_string(me.bid) + " " + me.item;
		msg = sndTmp.c_str();
		//cout << msg << endl;

		send(Connection, msg, 256, NULL);
	}
	/*
		cout << "above ur line" << endl;
		std::cin.getline(buffer, sizeof(buffer));
		send(Connection, buffer, sizeof(buffer), NULL);
		cout << "in the while" << endl;
		Sleep(10);
		
	}*/
	return 0;
}

