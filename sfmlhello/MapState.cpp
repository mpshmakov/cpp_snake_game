#include "MapState.h"
#include <iostream>


MapState::MapState()
{


	bool success = this->font.loadFromFile("Fonts/PublicPixel-z84yD.ttf");
	this->backgroundTexture.loadFromFile("Sprites/maps.png");



	//Initializing the map buttons
	this->map1 = new Button(sf::Vector2f(240,235),nullptr,sf::Vector2f(350,120),
		sf::Color(75,75,75), sf::Color(55,55,55), sf::Color(40,40,40), &this->font, "Map 1",
		46,sf::Color::White);
	this->map1->setTextPos(sf::Vector2f(285, 252));

	this->map2 = new Button(sf::Vector2f(240, 373), nullptr, sf::Vector2f(350, 120),
		sf::Color(75, 75, 75), sf::Color(55, 55, 55), sf::Color(40, 40, 40), &this->font, "Map 2",
		46, sf::Color::White);
	this->map2->setTextPos(sf::Vector2f(285, 390));

	this->map3 = new Button(sf::Vector2f(240, 511), nullptr, sf::Vector2f(350, 120),
		sf::Color(75, 75, 75), sf::Color(55, 55, 55), sf::Color(40, 40, 40), &this->font, "Map 3",
		46, sf::Color::White);
	this->map3->setTextPos(sf::Vector2f(285, 528));

	//Initializing the BackGround 
	this->background.setSize(sf::Vector2f(800, 800));
	this->background.setTexture(&this->backgroundTexture);
}


MapState::~MapState()
{
	delete this->map1;
	delete this->map2;
	delete this->map3;
}



//Updates the Components in the State
void MapState::update(double deltaT)
{
	this->updateKeyBinds(deltaT); //Checking if the User wants to Quit the State 
}

//Draws the Components in the State
void MapState::draw(sf::RenderWindow * w)
{
	//Drawing the Background FIRST 
	w->draw(this->background);
	//Drawing the Buttons in the State 
	this->map1->draw(w); 
	this->map2->draw(w);
	this->map3->draw(w);
}

//Processes the Components in the State 
void MapState::processStuff(double deltaT, sf::Vector2f mousePos)
{
	this->map1->checkState(mousePos);
	this->map2->checkState(mousePos);
	this->map3->checkState(mousePos);
}

//Updates the Key presses in the State
void MapState::updateKeyBinds(double deltaT)
{
	State::checkForQuit(); //Checks if the User Wants to Quit
}

NextState MapState::switchState()
{
	if (this->map1->isPressed())
		return StartGame1;
	else if (this->map2->isPressed())
		return StartGame2;
	else if (this->map3->isPressed())
		return StartGame3;
	return NoChange;
}





