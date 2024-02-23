#ifndef MAPSTATE_H
#define MAPSTATE_H
#include <SFML/Graphics.hpp>
#include "State.h"
#include "Button.h"
#include <string>

/*
	This state will be displayed in the beginnging and after the lostState with the map selection 
*/



class MapState :
	public State
{
public:
	MapState();
	~MapState();


	//Updates the Components in the State
	void update(double deltaT) override;
	
	//Draws the Components in the State
	void draw(sf::RenderWindow * w) override;

	//Processes the Components in the State 
	void processStuff(double deltaT, sf::Vector2f mousePos) override;

	//Updates the Key presses in the State
	void updateKeyBinds(double deltaT) override;

	NextState switchState() override;

private:
	sf::Font font; //Default Font
	
	Button* map1; 
	Button* map2;
	Button* map3;
	sf::RectangleShape background; 
	sf::Texture backgroundTexture;

};

#endif