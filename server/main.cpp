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

using namespace std;

webSocket server;
gamestate State;
map<int, string> players; //keeps track of which client is which snake
std::string p1Name = "P1", p2Name = "P2";

void init()		// fresh game
{
	State = gamestate();
}

/* called when a client connects */
/* tracks clients to players*/
void openHandler(int clientID) {

	std::cout << clientID << " joined." << std::endl;

	if (players.empty()) {
		players[clientID] = "1";
		std::cout << "Players mapped 1" << std::endl;
	}
	else if (players.size() == 1) {
		players[clientID] = "2";
		std::cout << "Players mapped 2" << std::endl;
	}
	else {
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

	while (getline(stream, token, delimiter)) {
		result.push_back(token);
	}

	return result;
}

/* called when a client sends a message to the server */
void messageHandler(int clientID, string message) {
	vector<string> messageArr = split(message, ':');
	std::cout << "messageHandler" << std::endl;
}

int moveHandler(int clientID, string direction) {
	//given direction of client, apply game logic

	std::string playerMove = players[clientID] + direction; // (ex 1a2w means player 1 pressed a and player 2 pressed w)
	State.set_dir_by_str(playerMove);

	std::cout << playerMove << std::endl;

	if (State.check_collisions()) {
		return -1;
	}

	std::cout << "UPDATE:" << State.foodLoc() << ":" << State.playerLoc(1) << ":"
		<< State.playerLoc(2) << ":" << State.playerSco(1) << ":" << State.playerSco(2)
		<< ":" << State.eatFood << std::endl;

	ostringstream os; // STATE:foodLoc:player1Loc:player2Loc:score1:score2:FoodEaten(0/1/2)
		os << "UPDATE:" << State.foodLoc() << ":" << State.playerLoc(1) << ":"
		<< State.playerLoc(2) << ":" << State.playerSco(1) << ":" << State.playerSco(2)
		<< ":" << State.eatFood;

	vector<int> clientIDs = server.getClientIDs();
	for (int i = 0; i < clientIDs.size(); i++) {
		server.wsSend(clientIDs[i], os.str());
	}

	return 0;
}


void moveResults(int clientID, string message) {
	//if needs SETUP and has 2 players, sends clients the ok for new game
	//else should call on moveHandler to update game state and return update to client
	vector<string> messageArr = split(message, ':');

	std::cout << "moveResult" << std::endl;

	// (ex: START:playerNameInput )
	if (messageArr[0] == "START") 
		{

			std::cout << "Reached START" << std::endl;
			if (players[clientID] == "1" && messageArr[1] != "") // Set custom name. If none, default name used
				p1Name = messageArr[1];

			if (players[clientID] == "2" && messageArr[1] != "")
				p2Name = messageArr[1];
		}
		if (players.size() == 2) 
			{
				std::cout << "START:" << State.colRow() << ":" << players[clientID] << ":" << p1Name << ":" << p2Name << std::endl;

				// START:col:row:clientSnake:p1Name:p2Name
				vector<int> clientIDs = server.getClientIDs();
				for (int i = 0; i < clientIDs.size(); i++) {
					server.wsSend(clientIDs[i], "START:" + State.colRow() + ":" + players[i] + ":" + p1Name + ":" + p2Name);
				}
			}
	

	// (ex: MOVE:DIRECTION )
	else if (messageArr[0] == "MOVE"){
		std::cout << "Reached MOVE" << std::endl;
		string direction = messageArr[1];

		int moveUpdate = moveHandler(clientID, direction);

		if (moveUpdate == -1) {
			ostringstream os;
			os << "RESET" ;		// Reset due to collision

			vector<int> clientIDs = server.getClientIDs();
			for (int i = 0; i < clientIDs.size(); i++) {
				server.wsSend(clientIDs[i], os.str());
			}
			init();
		}
	}
}

/* called once per select() loop */
void periodicHandler() {
	moveResults; // How to call this?
}

int main(int argc, char *argv[]) {
	int port;

	cout << "Please set server port: ";
	cin >> port;

	/* set event handler */
	server.setOpenHandler(openHandler);
	server.setCloseHandler(closeHandler);
	server.setMessageHandler(moveResults);
	//server.setPeriodicHandler(periodicHandler); // to periodically check state

	/* start the chatroom server, listen to ip '127.0.0.1' and port '8000' */
	server.startServer(port);

	return 1;
}
