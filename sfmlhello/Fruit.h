#ifndef FRUIT_H
#define FRUIT_H

#include <SFML/Graphics.hpp>
#include "Entity.h"
#include <vector>
#include <ctime>
#include <cstdlib> // Access to rand()

class Fruit : public Entity
{
public:
	Fruit(sf::Vector2f position, sf::Texture * texture);
	~Fruit();

	void update(double deltaT) override;

	bool fruittotail();

	void setRandomPos();

private:
	bool collide; //Determines if the Fruit is Collided with the snake
};


#endif
