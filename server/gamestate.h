#pragma once
#include <utility>
#include <iostream>
#include <time.h>
#include <string>

using namespace std;

class gamestate
{
private:
	int field[10][10];		//field will be an array of arrays of ints, 0 will denote an empty space, 1 will denote a space with player 1, 2 will denote a space containing player 2, 3 will denote a space containing the food

	int xpos1;					//stores the x position of player1
	int ypos1;					//stores the y position of player1
	pair <int, int> dir1;		//stores the direction of player1
	int score1;					//stores the score of player1
	pair <int, int> tail1[100];

	int xpos2;					//stores the x position of player2
	int ypos2;					//stores the y position of player2
	pair <int, int> dir2;		//stores the direction of player2
	int score2;					//stores the score of player 2
	pair <int, int> tail2[100];

	int xpos3;		//stores the x position of the food
	int ypos3;		//stores the y position of the food

	bool game_end = false;

public:

	std::string eatFood = "0";

	gamestate() 			//default constructor creates a 10x10 board
	{
		xpos1 = 1;				//player 1 starts 1 space in from the upper right corner facing down
		ypos1 = 1;
		dir1 = make_pair(0, 1);			 
		score1 = 0;
		
		xpos2 = 8;				//player 2 starts 1 space in from the lower left corner facing up
		ypos2 = 8;	
		dir2 = make_pair(0, -1);
		score2 = 0;		


		for (int y = 0; y < 10; ++y)
		{
			for (int x = 0; x < 10; ++x)
			{
				field[x][y] = 0;
			}
		}

		field[ypos1][xpos1] = 1;
		field[ypos2][xpos2] = 2;
		set_food();
	}

	void set_food()					//sets the food to a random empty location
	{
		cout << "food was set" << endl;
		xpos3 = rand() % 10;
		ypos3 = rand() % 10;
		if (field[ypos3][xpos3] == 0)
			field[ypos3][xpos3] = 3;
		else
			set_food();
	}

	bool check_collisions()			//checks if a game ending collision will occur on the next frame
	{
		std::cout << "collisions being checked\n";
		int newx1 = xpos1 + dir1.first;
		int newy1 = ypos1 + dir1.second;
		int newx2 = xpos2 + dir2.first;
		int newy2 = ypos2 + dir2.second;

		if (newx1 < 0 || newx1 > 9 || newy1 < 0 || newy1 > 9)
		{
			cout << "\n**p1 exceeded bounds**\n";
			game_end = true;
			return true;
		} 
		if(newx2 < 0 || newx2 > 9 || newy2 < 0 || newy2 > 9)
		{
			cout << "\n**p2 exceeded bounds**\n";
			game_end = true;
			return true;
		}
		if (newy1 == newy2 && newx1 == newx2)
		{
			cout << "\n**heads collided**\n";
			game_end = true;
			return true;
		}

		if (field[newy1][newx1] == 1 || field[newy1][newx1] == 2 || field[newy2][newx2] == 1 || field[newy2][newx2] == 2)
		{
			cout << "\n**snakes collided**\n";
			game_end = true;
			return true;
		}
		return false;
	}

	int check_food()				//checks if a player will hit the food, returns the player #, 0 if neither player
	{
		int newx1 = xpos1 + dir1.first;
		int newy1 = ypos1 + dir1.second;
		int newx2 = xpos2 + dir2.first;
		int newy2 = ypos2 + dir2.second;
		if (field[newy1][newx1] == 3) 
		{
			eatFood = "1";
			return 1;
		}
		if (field[newy2][newx2] == 3) 
		{
			eatFood = "2";
			return 2;
		}
		eatFood = "0";
		return 0;
	}

	void update()					//updates each frame
	{
		check_collisions();
		std::cout << "game_end: " << game_end << std::endl;
		if (check_food() == 1)
		{
			score1++;
			set_food();
		}
		else if (check_food() == 2)
		{
			score2++;
			set_food();
		}

		for (int i = score1; i > 0; --i)		//update player1 tail
		{
			tail1[i] = tail1[i - 1];
			cout << tail1[i].first << " " << tail1[i].second << endl;
		}
		tail1[0] = make_pair(xpos1, ypos1);

		for (int i = score2; i > 1; --i)		//update player2 tail
		{
			tail2[i] = tail2[i - 1];
		}
		tail2[0] = make_pair(xpos2, ypos2);

		xpos1 = xpos1 + dir1.first;				//updates player1 head
		ypos1 = ypos1 + dir1.second;

		xpos2 = xpos2 + dir2.first;				//updates player2 head
		ypos2 = ypos2 + dir2.second;

		for (int y = 0; y < 10; ++y)			//updates the field
		{
			for (int x = 0; x < 10; ++x)
			{
				field[x][y] = 0;
			}
		}
		field[ypos1][xpos1] = 1;
		field[ypos2][xpos2] = 2;
		field[ypos3][xpos3] = 3;
		for (int i = 0; i < score1; ++i)
		{
			cout << tail1[i].first << " " << tail1[i].second;
			field[tail1[i].second][tail1[i].first] = 1;
		}
		for (int i = 0; i < score2; ++i)
		{
			field[tail2[i].second][tail2[i].first] = 2;
		}
	}

	void printstate()					//prints the field
	{
		cout << "\nplayer1 score: " << score1 << " | player2 score: " << score2 << "\n\n";
		for (int i = 0; i < 22; ++i)
		{
			cout << "_";
		}
		cout << "\n";
		for (int rows= 0; rows < 10; ++rows)
		{
			cout << "|";
			for (int cols= 0; cols < 10; ++cols)
			{
				cout << " " << field[rows][cols];
			}
			cout << " |\n";
		}
		for (int i = 0; i < 22; ++i)
		{
			cout << "_";
		}
		cout << "\n\n";
	}

	std::string foodLoc()		// return coor in string
	{
		return std::to_string(xpos3) + "," + std::to_string(ypos3);
	}

	std::string playerLoc(int player)		// return player pos in string
	{
		if (player == 1)
			return std::to_string(xpos1) + "," + std::to_string(ypos1);
		else if (player == 2)
			return std::to_string(xpos2) + "," + std::to_string(ypos2);
	}

	std::string playerSco(int player)		// return player score in string
	{
		if (player == 1)
			return std::to_string(score1);
		else if (player == 2)
			return std::to_string(score2);
	}

	std::string colRow()		// return player score in string
	{
		return to_string(24) + ":" + to_string(24);
	}

	void set_dir_by_str(string s)		//sets directions using a string input (ex 1a2w means player 1 pressed a and player 2 pressed w)
	{
		if (s.find("1a") != std::string::npos)			//player 1 direction
			dir1 = make_pair(-1, 0);
		if (s.find("1w") != std::string::npos)
			dir1 = make_pair(0, -1);
		if (s.find("1d") != std::string::npos)
			dir1 = make_pair(1, 0);
		if (s.find("1s") != std::string::npos)
			dir1 = make_pair(0, 1);

		if (s.find("2a") != std::string::npos)			//player 2 direction
			dir2 = make_pair(-1, 0);
		if (s.find("2w") != std::string::npos)
			dir2 = make_pair(0, -1);
		if (s.find("2d") != std::string::npos)
			dir2 = make_pair(1, 0);
		if (s.find("2s") != std::string::npos)
			dir2 = make_pair(0, 1);
	}



	string state_str()					//returns a string representing the state for main to read (X=border, 0=empty, 1=player1, 2=player2, 3=food, -=newline)
	{

		ostringstream str;
		if (!game_end)
		{
			for (int i = 0; i < 12; ++i)	//Top border
			{
				str << "X";
			}
			str << "-";
			for (int rows = 0; rows < 10; ++rows)
			{
				str << "X";								//left border
				for (int cols = 0; cols < 10; ++cols)
				{
					str << field[rows][cols];			//field
				}
				str << "X-";							//right border
			}
			for (int i = 0; i < 12; ++i)
			{
				str << "X";								//bottom border
			}
		}
		else
		{
			for (int rows = 0; rows < 12; ++rows)		//if game has ended should print a black canvas
			{
				str << "X";
				for (int cols = 0; cols < 12; ++cols)
				{
					str << "X";
				}
				str << "-";
			}
		}
		return str.str();		
	}
};