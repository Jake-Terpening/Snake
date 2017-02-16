// csnake.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "gamestate.h"
#include <iostream>
#include <string>;

using namespace std;


int main()
{
	srand(time(NULL));

	gamestate g;
	g.printstate();

	string input;
	while (input != "q")
	{
		cout << "enter commands\n";
		getline(cin, input);
		
		g.set_dir_by_str(input);
		if(g.check_collisions())
			break;
		g.update();

		g.printstate();
	}
    return 0;
}

