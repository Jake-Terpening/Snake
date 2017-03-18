//NAME              ID
//Jake Terpening    21945519
//Whitney Tran      77187709
//Daniel Williams   77402284
//Tristan Bock      50665011

//code modified from chatroom example
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include "websocket.h"
#include "gamestate.h"
#include <chrono>

using namespace std::chrono;
	
webSocket server;
gamestate State;
map<int, string> players; //keeps track of which client is which snake
std::string p1Name = "P1", p2Name = "P2";
bool gameStarted = false;

void init()	// fresh game
{
	State = gamestate();
}

/* called when a client connects */
/* tracks clients to players*/
void openHandler(int clientID) {

	std::cout << clientID << " joined." << std::endl;

	if (players.empty())
	{
		players[clientID] = "1";
	}
	else if (players.size() == 1)
	{
		players[clientID] = "2";
	}
	else
	{
		server.wsClose(clientID);
	}
}

/* called when a client disconnects */
void closeHandler(int clientID) {
	std::cout << clientID << " disconnected." << std::endl;
	players.erase(clientID);

	// Player DC, game init
	if (players.size() < 2) {
		init();
	}
}

/* Return a vector of strings split over the delimiter for message*/
vector<string> split(string message, char delimiter) {
	vector<string> result = vector<string>();

	stringstream stream(message);
	string token;

	while (getline(stream, token, delimiter))
	{
		result.push_back(token);
	}

	return result;
}

/* called when a client sends a message to the server */
void messageHandler(int clientID, string message)
{
	vector<string> messageArr = split(message, ':');
}

void moveHandler(int clientID, string direction)
{
	//given direction of client, apply game logic

	std::string playerMove = players[clientID] + direction; // (ex 1a2w means player 1 pressed a and player 2 pressed w)
	State.set_dir_by_str(playerMove);

	ostringstream os; // UPDATE:GameBoard:score1:score2
		os << "UPDATE:" << State.state_str() << ":" << State.playerSco(1) << ":" << State.playerSco(2);

	vector<int> clientIDs = server.getClientIDs();
	for (int i = 0; i < clientIDs.size(); i++) 
	{
		server.wsSend(clientIDs[i], os.str());
	}
}


void moveResults(int clientID, string message)
{
	vector<string> messageArr = split(message, ':');
	if (messageArr[0] == "START") 
		{
			if (players[clientID] == "1")
			{
				if (messageArr[1] != "")
				{
					p1Name = messageArr[1];
				}
			}

			if (players[clientID] == "2")
			{
				if (messageArr[1] != "")
				{
					p2Name = messageArr[1];
				}
			}

			if (players.size() == 2)
			{
				vector<int> clientIDs = server.getClientIDs();
				for (int i = 0; i < clientIDs.size(); i++) // START:col:row:clientSnake:p1Name:p2Name
				{
					server.wsSend(clientIDs[i], "START:" + State.state_str() + ":" + players[i] + ":" + p1Name + ":" + p2Name);
				}
				gameStarted = true;
			}
		}
	

	
	else if (messageArr[0] == "MOVE") // MOVE:DIRECTION
	{
		string direction = messageArr[1];

		moveHandler(clientID, direction);

		milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
		long long num_ms = ms.count();
		std::cout << "message sent: " << messageArr[2] << std::endl;
		std::cout << "message recieved: " << num_ms << std::endl;
		long long offset = num_ms - std::stoll(messageArr[2]);
		std::cout << "offset: " << offset << std::endl;
	}
}

/* called once per select() loop */
void periodicHandler()
{
	if (State.check_collisions())
		init();
	if (gameStarted) 
	{
		int random_delay1 = rand() % 390 + 10;
		int random_delay2 = rand() % 390 + 10;
		random_delay2 = abs(random_delay1 - random_delay2);
		int first_to_recieve = rand() % 2;
		int second_to_recieve = 1 - first_to_recieve;
		// std::cout << State.state_str() << std::endl;
		State.update();

		Sleep(random_delay1);
		server.wsSend(first_to_recieve, "START:" + State.state_str() + ":" + players[0] + ":" + p1Name + ":" + p2Name);
		Sleep(random_delay2);
		server.wsSend(second_to_recieve, "START:" + State.state_str() + ":" + players[0] + ":" + p1Name + ":" + p2Name);
		Sleep(1000);
	}
	
	
}

int main(int argc, char *argv[]) {
	int port;

	cout << "Please set server port: ";
	cin >> port;

	/* set event handler */
	server.setOpenHandler(openHandler);
	server.setCloseHandler(closeHandler);
	server.setMessageHandler(moveResults);
	server.setPeriodicHandler(periodicHandler); // to periodically check state

	/* start the chatroom server, listen to ip '127.0.0.1' and port '8000' */
	server.startServer(port);

	return 1;
}
