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

	if (State.empty()) {
		State[clientID] = "p1";
	}
	else if (State.size() == 1) {
		State[clientID] = "p2";
	}
	else {
		server.wsClose(clientID);
	}
}

/* called when a client disconnects */
void closeHandler(int clientID) {
	std::cout << clientID << " disconnected." << std::endl;
	State.erase(clientID);

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
	vector<string> messageReceived = split(message, ); //need to fill
}

void moveHandler(int clientID, string direction) {
	//given direction of client, apply game logic
}

/* this could probably just merge with messageHandler */
void moveResults(int clientID, string message) {
	//if needs SETUP and has 2 players, sends clients the ok for new game
	//else should call on moveHandler to update game state and return update to client
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
