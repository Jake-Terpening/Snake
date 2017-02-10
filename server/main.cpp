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

using namespace std;

webSocket server;
map<int, int> playerScores;

/* called when a client connects */
void openHandler(int clientID) {
	vector<int> clientIDs = server.getClientIDs();

	// only one client can connect
	if (clientIDs.size() == 1) {
		std::cout << clientID << " joined." << std::endl;
		playerScores[1] = 0;
		playerScores[2] = 0;
	}

	// this disconnects anyone else
	else {
		server.wsClose(clientID);
	}
}

/* called when a client disconnects */
void closeHandler(int clientID) {
	std::cout << clientID << " disconnected." << std::endl;

	vector<int> clientIDs = server.getClientIDs();
}

/* called when a client sends a message to the server */
void messageHandler(int clientID, string message) {
	ostringstream os;

	// receive a string indicating who scored, mark on map, sends back updated score
	if (message == "1") {
		++playerScores[1];
		std::cout << clientID << " scored." << std::endl;
		os << "SCORE:" << playerScores[1];
	}
	else {
		++playerScores[2];
		std::cout << clientID << " scored." << std::endl;
		os << "SCORE:" << playerScores[2];
	}

}

/* called once per select() loop */
void periodicHandler() {
	static time_t next = time(NULL) + 10;
	time_t current = time(NULL);
	if (current >= next) {
		ostringstream os;
		string timestring = ctime(&current);
		timestring = timestring.substr(0, timestring.size() - 1);
		os << timestring;

		vector<int> clientIDs = server.getClientIDs();
		for (int i = 0; i < clientIDs.size(); i++)
			server.wsSend(clientIDs[i], os.str());

		next = time(NULL) + 10;
	}
}

int main(int argc, char *argv[]) {
	int port;

	cout << "Please set server port: ";
	cin >> port;

	/* set event handler */
	server.setOpenHandler(openHandler);
	server.setCloseHandler(closeHandler);
	server.setMessageHandler(messageHandler);
	//server.setPeriodicHandler(periodicHandler);

	/* start the chatroom server, listen to ip '127.0.0.1' and port '8000' */
	server.startServer(port);

	return 1;
}
