#include "Fruit.h"



Fruit::Fruit(sf::Vector2f position, sf::Texture * texture)
	:Entity(position, texture)
{
	this->collide = false; //Initially False
	srand(time(0)); //Random Seed
}


Fruit::~Fruit()
{
}


void Fruit::update(double deltaT)
{
	
}


//Setter
void Fruit::setRandomPos()
{
	//Spawn in a Random Location 
	int randNumX = rand() % 680 + 60; //Numbers between 60 - 740

	int randNumY = rand() % 680 + 60;

	Entity::setPos(sf::Vector2f(randNumX, randNumY));
}
