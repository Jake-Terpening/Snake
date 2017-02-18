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

/* called when a client connects */
/* tracks clients to players*/
void openHandler(int clientID) {

	std::cout << clientID << " joined." << std::endl;

	if (players.empty()) {
		players[clientID] = "1";
	}
	else if (players.size() == 1) {
		players[clientID] = "2";
	}
	else {
		server.wsClose(clientID);
	}
}

/* called when a client disconnects */
void closeHandler(int clientID) {
	std::cout << clientID << " disconnected." << std::endl;
	players.erase(clientID);

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
	vector<string> messageReceived = split(message, ':'); //need to fill
}

int moveHandler(int clientID, string direction) {
	//given direction of client, apply game logic

	std::string playerMove = players[clientID] + direction; // (ex 1a2w means player 1 pressed a and player 2 pressed w)
	State.set_dir_by_str(playerMove);

	if (State.check_collisions()) {
		return -1;
	}

	ostringstream os; // what to send back?
	os << "STATE:";

	vector<int> clientIDs = server.getClientIDs();
	for (unsigned int i = 0; i < clientIDs.size(); i++) {
		server.wsSend(clientIDs[i], os.str());
	}

	return 0;
}

/* this could probably just merge with messageHandler */
void moveResults(int clientID, string message) {
	//if needs SETUP and has 2 players, sends clients the ok for new game
	//else should call on moveHandler to update game state and return update to client
	vector<string> messageReceived = split(message, ':');

	if (messageReceived[0] == "SETUP" && players.size() == 2) {
		// board setup
	}

	// (ex: MOVE:DIRECTION )
	else if (messageReceived[0] == "MOVE"){
		string direction = messageReceived[1];

		int moveUpdate = moveHandler(clientID, direction);

		if (moveUpdate == -1) {
			// collision, end game
		}
	}
}

/* called once per select() loop */
void periodicHandler() {
	moveResults;
}

int main(int argc, char *argv[]) {
	int port;

	cout << "Please set server port: ";
	cin >> port;

	/* set event handler */
	server.setOpenHandler(openHandler);
	server.setCloseHandler(closeHandler);
	server.setMessageHandler(messageHandler);
	server.setPeriodicHandler(periodicHandler); // to periodically check state

	/* start the chatroom server, listen to ip '127.0.0.1' and port '8000' */
	server.startServer(port);

	return 1;
}
