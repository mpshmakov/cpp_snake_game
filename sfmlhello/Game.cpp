#include "Game.h"
#include "GameState.h"
#include <iostream>
#include <cstdlib>
#include <ctime>


Game::Game() :
	window(sf::VideoMode(800, 800), "SnakeGame", sf::Style::Close)
{
	this->deltaT = 0; //Initially ZERO

	this->initStates(); //Initializing All the States in the Game

	this->score = 0; //Saves the Score Acts as a Middle Man to bridge the gap between GameState and LostState

}



Game::~Game()
{
	//Deallocating Memory Allocated from States
	for (int i = 0; i < this->states.size(); i++)
	{
		delete states.top();

		states.pop(); //Go to the Next One 
	}
}

//Public Function
void Game::start()
{
	this->run(); //Runs the Main Loop 
}




void Game::run()
{
	sf::Clock clock;

	//Main GameLoop
	while (this->window.isOpen())
	{
		this->deltaT = clock.restart().asSeconds()/2; //Gets the Speed of Our Specific Computer

		//Event Loop Handler
		sf::Event evnt;
		while (this->window.pollEvent(evnt))
		{
			if (evnt.type == sf::Event::Closed)
			{
				this->window.close(); 
			}
		}

		this->window.clear(); //Clears the Window
		
		Game::update(); //Updates all the Components in the Game
		Game::processStuff(); //Processes All the Components in the Game
		Game::draw(); //Draws all the Components in the Game

		this->window.display(); //Displays the Window
	}

}


void Game::update()
{
	//Updating the Current State	Ensures its not Empty
	if (this->states.empty() == false)
	{
		this->states.top()->update(this->deltaT);
	}
}


void Game::draw()
{
	//Drawing the Current State		Ensures its not Empty
	if (!this->states.empty())
	{
		this->states.top()->draw(&this->window);
	}
}

void Game::processStuff()
{

	//Processing the Current State
	if (!this->states.empty())
	{
		//Process Everything First
		this->states.top()->processStuff(this->deltaT, sf::Vector2f(sf::Mouse::getPosition(this->window))); 
		
	}

	//Talking about the Lost state Stuff
	if (this->states.size() <= 1)
	{

		NextState next = this->states.top()->switchState();
		if (next != NoChange)
		{
			this->score = this->states.top()->State::getHolderScore();//Saving the Score 

			delete this->states.top();
			this->states.pop();
			
			if (next == MapList)
			{
				this->states.push(new MapState());
			}
			else if (next == Lost)
			{
				this->states.push(new LostState());
				this->states.top()->State::setHolderScore(this->score);
			}
			else if (next == StartGame1)
			{
				this->states.push(new GameState(0));
			}
			else if (next == StartGame2)
			{
				this->states.push(new GameState(1));
			}
			else if (next == StartGame3)
			{
				this->states.push(new GameState(2));
			}


		}

	}
}


//Initiallizes all the States of the Game
void Game::initStates()
{
	//this->states.push(new LostState()); //If the Player Loses Switch to this State 
	//this->states.push(new GameState());  //Inital Game State 
	this->states.push(new MapState()); //Map list
}