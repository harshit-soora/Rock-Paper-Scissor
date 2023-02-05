// Copyright (c) 2023-2024, Harshit Soora.

#include <cassert>
#include <iostream>
#include <chrono>
#include <thread>
#include <time.h>
#include <map>
#include <fstream>
#include <vector>
#include <cstring>

// Pass this a argument output file to launch test mode
constexpr char debug[] = "test";

/*
* This is the implementaion of a Game class
* This include computer sign generation, computing winner and maintaining rules
*/
class Game
{
	private:
		bool m_bexitGame = true;
		std::map<char, std::string> m_actionDict; 
		std::ostream& m_log = std::cout;

		int generateSign()
		{
			int choice = rand() % 3;
			return choice;
		}

	public:
		// We don't expect default constructor for Game class so marked it deleted
		Game() = delete;

		Game(std::ostream& logger) : 
			m_log(logger)
		{
			m_bexitGame = false;
			srand(time(0));
			m_actionDict[0] = "ROCK";
			m_actionDict[1] = "PAPER";
			m_actionDict[2] = "SCISSOR";
		}

		int computeWinner(int userSign, int compSign, bool effect)
		{
			int i;
			int flag = 5;
			while(flag && effect)
			{
				// This is added purely to create some user experience
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
				m_log << ".";
				flag--;
			}

			flag = 1;
			if(userSign == compSign) {
				flag = 0;
			}
			else if((userSign+1)%3 == compSign) {
				flag = -1;
			}

			if(flag == 1) {
				m_log << "\n---YOU ARE THE WINNER---\n";
			}
			else if(flag == 0) {
				m_log << "\n---ITS A DRAW---\n";
			}
			else {
				m_log << "\n---YOU LOSE---\n";
			}
			m_log << "You: ";
			m_log << m_actionDict[userSign];
			m_log << "\nComputer: ";
			m_log << m_actionDict[compSign];
			m_log << "\n";

			return flag;
		}

		void promptRules()
		{
			m_log << "R [Rock]\n";
			m_log << "P [Paper]\n";
			m_log << "S [Scissor]\n";
			m_log << "Q [Quit]\n";
		}

		int parseInput(char userInput)
		{
			int userSign = -1;
			if(userInput == 'Q') {
				m_bexitGame = true;
			}
			else if(userInput == 'R') {
				userSign = 0;
			}
			else if(userInput == 'P') {
				userSign = 1;
			}
			else if(userInput == 'S') {
				userSign = 2;
			}
			else {
				m_log << "You have pressed a wrong input, try following - \n";
				promptRules();
			}
			return userSign;
		}

		void play()
		{
			int computerSign = generateSign();
			char userInput;
			int userSign;
			std::cin >> userInput;

			userSign = parseInput(userInput);
			if(userSign == -1) {
				return;
			}
			computeWinner(userSign, computerSign, true);
		}

		bool exitGame()
		{
			return m_bexitGame;
		}

		~Game()
		{
			m_actionDict.clear();
			m_bexitGame = true;
		}
};


/*
* This is the implementaion file of a Tester class
* This will check Game's result again hard coded results
*/
class Tester
{
	private:
		Game *m_game;
		const std::string m_testFile = "test_output.txt";
		std::ofstream m_testLog;
		std::vector<std::vector<int>> m_cache;

	public:
		Tester()
		{
			m_testLog.open(m_testFile); 
			m_game = new Game(m_testLog);

			// hardcode output to test m_cache[i][j]
			// 0 Rock, 1 Paper, 2 Scissor
			// 0 draw, 1 win for i(user), -1 win for j(computer)
			m_cache.resize(3, std::vector<int>(3, 0));
			m_cache[0][1] = -1;
			m_cache[1][2] = -1;
			m_cache[2][0] = -1;

			m_cache[1][0] = 1;
			m_cache[2][1] = 1;
			m_cache[0][2] = 1;
		}

		void test()
		{
			int out;
			for(int i=0;i<3;i++)
			{
				for(int j=0;j<3;j++)
				{
					// since ofstream is derived from ostream, so we can use its object :: Polymorphism
					out = m_game->computeWinner(i, j, false);
					assert(out==m_cache[i][j]);
				}
			}
		}

		~Tester()
		{
			m_testLog.close();
			m_cache.clear();
			m_game = nullptr;
		}
};


int main(int argc, char **argv)
{
	//Pass "test" as arg to enable testing
	if(argc>1 && strcmp(argv[1], debug) == 0){
		std::cout << "You ran test mode, open test_output.txt to see the results\n";
		Tester* t = new Tester();
		t->test();
		delete t;
	}
	else{
		Game* g = new Game(std::cout);

		// Write a welcome note with rules
		std::cout << "WELCOME PLAYER\n";
		std::cout << "Here are some rules\n";
		std::cout << "You have to give a letter input representing your choice -\n";
		g->promptRules();
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		std::cout << "Are you ready?\n";
		
		// Launch Game
		while(!g->exitGame())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			std::cout << "\nPress your choice to begin\n";
			g->play();
		}
		std::cout << "\nSee you again, bye.";

		delete g;
	}

	return 0;
}
