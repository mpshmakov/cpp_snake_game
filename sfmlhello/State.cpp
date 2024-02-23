#include "State.h"
#include <iostream>


State::State()
{
	this->quit = false; //Initially False

	this->holderScore = 0; //Initially Zero 
}


State::~State()
{
}


void State::checkForQuit()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		setQuit(true); //Setting Quit to True
	}
}


//Setter
void State::setQuit(bool newQuit)
{
	this->quit = newQuit;
}


//Getter
bool State::getQuit() const
{
	return this->quit;
}


void State::setHolderScore(int newScore)
{
	this->holderScore = newScore;
}

int State::getHolderScore() const
{
	return this->holderScore;
}

NextState State::switchState()
{
	return NoChange; //HardCoded to Not switch State
}
