#include "GameState.h"
#include <iostream>


GameState::GameState(int map)
{

	//Initializing Textures
	this->headTexture.loadFromFile("Sprites/head.png");
	this->fruitTexture.loadFromFile("Sprites/fruit.png");

	//Initializing Head
	this->head = new Head(sf::Vector2f(420, 420), &headTexture);

	srand(time(0));
	int randNum = rand() % 680 + 60;

	//Initializing Fruit
	this->fruit = new Fruit(sf::Vector2f(randNum, randNum), &fruitTexture);
	//Initializes the Border Stuff
	if (map == 0)
		this->initBorder(); 
	if (map == 1)
		this->initBorder1();
	if (map == 2)
		this->initBorder2();

	moveFruitRand();
	this->counterTF = 0; //Counter for Tail Fruit Collision
	this->counterHF = 0; //Counter for Head Fruit Collision
	this->counterHT = 0; //Counter for Head Tail Collision
	this->counterHB = 0; //Counter for Head Border Collision
	this->pauseCounter = 0; //Counter for the Pause Button


	//Initializing the Background of the GameStates
	this->background.setPosition(sf::Vector2f(0, 0));
	this->background.setFillColor(sf::Color(80, 80, 80));

	bool success = this->font.loadFromFile("Fonts/PublicPixel-z84yD.ttf");

	this->pause = false; 
	this->pauseText.setString("Pause");
	this->pauseText.setPosition(sf::Vector2f(300, 300));
	this->pauseText.setFont(font);
	this->pauseText.setCharacterSize(60);

	this->scoreTexture.loadFromFile("Sprites/blank.png");

	//Setting the ScoreButton
	this->scoreButton = new Button(sf::Vector2f(80,60), &this->scoreTexture, sf::Vector2f(100, 40), sf::Color::Cyan,
		sf::Color::Cyan, sf::Color::Cyan, &this->font, std::to_string(State::getHolderScore()) , 40, sf::Color::Black);
	this->scoreButton->setTextPos(sf::Vector2f(700, 40));

}


GameState::~GameState()
{
	std::cout << "Deleting Head" << std::endl;
	std::cout << "Deleting Fruit" << std::endl;
	delete this->head;
	delete this->fruit;
	// where are tails?
	
	//Deallocating Memory of the Borders
	for (int i = 0; i < this->borders.size(); i++)
	{
		delete borders[i];
	}
	delete this->scoreButton; //Deallocating Button Memory 
}

//Updates all the Components of the State
void GameState::update(double deltaT)
{
	this->updateKeyBinds(deltaT); //Checking if the User Wants to Quit the GameState
	
	//If the game is not paused Update the head and fruit Otherwise DONT 
	if (this->pause == false)
	{
		this->head->update(deltaT);
		this->fruit->update(deltaT);
	}
}

//Draws all the Components of the State
void GameState::draw(sf::RenderWindow * w)
{
	//Setting the Background Size to that of the Window	(MUST BE DRAWN FRIST)
	this->background.setSize(sf::Vector2f(w->getSize().x, w->getSize().x));
	w->draw(this->background);


	this->head->draw(w);
	this->fruit->draw(w);
	
	//Drawing the Border
	for (int i = 0; i < this->borders.size(); i++)
	{
		this->borders[i]->draw(w); //Drawing the Borders to the Screen
	}

	if (pause == true)
	{
		//Draw PauseText
		w->draw(this->pauseText); 
	}

	this->scoreButton->draw(w); //Drawing the Score Button to the Screen

}

//Processes All the Components of the State (e.g. Collision Detection)
void GameState::processStuff(double deltaT, sf::Vector2f mousePos)
{
	this->head->entOutOfWindow();

	//Checking Collision Between The Fruit and the Head

	//If this is true they are Colliding 
	if (this->head->getCollider().checkCollision(this->fruit->getCollider()) == true)
	{
		if (counterHF >= 0.3)
		{
			std::cout << "Head And Fruit Are Colliding " << std::endl;
			this->head->growTail(); //Grows the Snake Tail
			moveFruitRand();
			State::setHolderScore(State::getHolderScore() + 1);
			this->scoreButton->setText(std::to_string(State::getHolderScore())); //Updating the ScoreBoard
			counterHF = 0; //Resetting the Counter
		}
	}

	//Checking if the Head Is Colliding with the Tail
	if (this->head->headToTail() == true)
	{
		if (counterHT >= 0.3)
		{
			std::cout << "Head Touched Tail " << std::endl;
			counterHT = 0; //Resetting Counter
			State::setQuit(true);//Game Over 
		}
	}

	//Checking if the Head is Colliding with the Border

	for (int i = 0; i < this->borders.size(); i++)
	{
		//If the Head and Border are colliding 
		if (this->head->getCollider().checkCollision(this->borders[i]->getCollider()) == true)
		{
			if (counterHB >= 0.3)
			{
				std::cout << "Head is Touching the Border " << std::endl;
				counterHB = 0; //Resetting Counter 
				State::setQuit(true);//Game Over
			}
		}
	}



	counterTF += deltaT; //Tail Fruit Collision Counter 
	counterHF += deltaT; //Head Fruit Collision Counter
	counterHT += deltaT; //Head Tail Collision Counter
	counterHB += deltaT; //Head Border Collision Counter

}

//Defines all the Keyboard Stuff for that state
void GameState::updateKeyBinds(double deltaT)
{
	State::checkForQuit(); //Updates The State::quit if the Key is pressed 

	//Pauses the Game
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		if (pauseCounter >= 0.3)
		{
			if (this->pause == false)
			{
				this->pause = true;
			}
			else
			{
				this->pause = false; //Resetting it 
			}
			std::cout << "Pausing Screen " << this->pause<<  std::endl;
			pauseCounter = 0; //Resetting the Counter
		}
	}
	pauseCounter += deltaT; 
}


//Initializes the Border Stuff


void GameState::initBorder()
{
	this->borderTexture.loadFromFile("Sprites/wall.png");

	int amountNeeded = 80;//Need 80 Borders to Cover the 800x800 Window Sizes Each Border 40x40 
	/*
		20 needed
		______ 
	20  |    | 20
		|    |
		------
		20 needed 
	*/

	//Position for the Borders where the window ends
	sf::Vector2f pos(20, 20);

	int flag = 0;//Helps update the position

	for (int i = 0; i < amountNeeded; i++)
	{
		this->borders.push_back(new Border(pos, &this->borderTexture));

		if (i < 20) //Updating the Position for the Long lines 
		{
			pos.x += 40;
			pos.y = 20; 
		}
		else if (i < 60 && i >= 20) //Updating the Position for the border Lines  |     |
		{
			if (i % 2 == 0)
			{
				//i is even
				pos.x = 20;
			}
			else
			{
				//i is odd 
				pos.x = 780;
			}
			if (flag == 2)
			{
				//Update the y POSITION
				pos.y += 40;
				flag = 0; //Resetting Flag
			}
			flag++; //Updating Flag 
		}
		else if (i >= 60)//Updating the Position for the Long lines LAST ONE
		{
			if (i == 60)
			{
				pos.x = 20;//Reset the Starting Position
				pos.y = 780;
			}
			else
			{
				pos.y = 780;
				pos.x += 40;
			}
		}
	}

}


void GameState::initBorder1()
{
	this->borderTexture.loadFromFile("Sprites/wall.png");

	int amountNeeded = 124;

	//Position for the Borders where the window ends
	sf::Vector2f pos(20, 20);

	int flag = 0;//Helps update the position

	for (int i = 0; i < amountNeeded; i++)
	{
		this->borders.push_back(new Border(pos, &this->borderTexture));

		if (i < 20) //Updating the Position for the Long lines 
		{
			pos.x += 40;
			pos.y = 20;
		}
		else if (i < 60 && i >= 20) //Updating the Position for the border Lines  |     |
		{
			if (i % 2 == 0)
			{
				//i is even
				pos.x = 20;
			}
			else
			{
				//i is odd 
				pos.x = 780;
			}
			if (flag == 2)
			{
				//Update the y POSITION
				pos.y += 40;
				flag = 0; //Resetting Flag
			}
			flag++; //Updating Flag 
		}
		else if (i >= 60 && i<80)//Updating the Position for the Long lines LAST ONE
		{
			if (i == 60)
			{
				pos.x = 20;//Reset the Starting Position
				pos.y = 780;
			}
			else
			{
				pos.y = 780;
				pos.x += 40;
			}
		}
		else if (i >= 80 && i < 84) {
			if (i == 80) {
				pos.y = 340;
				pos.x = 380;
			}
			else {
				pos.y += 40;
				
			}
		}
		else if (i >= 84 && i < 88) {
			if (i == 84) {
				pos.y = 340;
				pos.x = 420;
			}
			else {
				pos.y += 40;
				pos.x = 420;
			}
		}
		else if (i >= 88 && i < 90) {
			if (i == 88) {
				pos.x = 340;
				pos.y = 380;
			}
			else {
				pos.x += 120;
				pos.y = 380;
			}
		}
		else if (i >= 90 && i < 92) {
			if (i == 90) {
				pos.x = 340;
				pos.y = 420;
			}
			else {
				pos.x += 120;
				pos.y = 420;
			}
		}
		else if (i >= 92 && i < 108) { // Walls on top and bottom in the middle
			if (i == 92) {
				pos.x = 380;
				pos.y = 60;
			}
			else if (i >92 && i<96) {
				pos.y += 40;
			}
			else if (i == 96) {
				pos.y = 660;
			}
			else if (i > 96 && i < 100) {
				pos.y += 40;
			}

			else if (i == 100) {
				pos.x = 420;
				pos.y = 60;
			}
			else if (i > 100 && i < 104) {
				pos.y += 40;
			}
			else if (i == 104) {
				pos.y = 660;
			}
			else if (i > 104 && i < 108) {
				pos.y += 40;
			}
		}
		else if (i >= 108 && i < 124) { // walls in the middle on the left and right side
			if (i == 108) {
				pos.y = 380;
				pos.x = 60;
			}
			else if (i >108 && i<112) {
				pos.x += 40;
			}
			else if (i == 112) {
				pos.x = 660;
			}
			else if (i > 112 && i < 116) {
				pos.x += 40;
			}

			else if (i == 116) {
				pos.y = 420;
				pos.x = 60;
			}
			else if (i > 116 && i < 120) {
				pos.x += 40;
			}
			else if (i == 120) {
				pos.x = 660;
			}
			else if (i > 120 && i < 124) {
				pos.x += 40;
			}
		}
	}
}



void GameState::initBorder2() //make this one without borders
{
	this->borderTexture.loadFromFile("Sprites/wall.png");

	int amountNeeded = 72;

	//Position for the Borders where the window ends
	sf::Vector2f pos(20, 20);

	int flag = 0;//Helps update the position

	for (int i = 0; i < amountNeeded; i++)
	{
		this->borders.push_back(new Border(pos, &this->borderTexture));

		if (i == 0) {
			pos.x = 20;
			pos.y = 380;
		}
		else if (i >= 1 && i < 10) {
			pos.x += 40;
		}
		else if (i == 10) {
			pos.x = 500;
		}
		
		else if (i == 11) {
			pos.x = 20;
			pos.y = 20;
		}
		else if (i >= 12 && i < 14)
			pos.x += 40;
		else if (i == 14)
			pos.x = 260;
		else if (i >= 15 && i < 24)
			pos.x += 40;
		else if (i == 24) {
			pos.x = 20;
			pos.y = 500;
		}
		else if (i >= 25 && i < 44)
			pos.x += 40;
		else if (i == 44) {
			pos.x = 20;
			pos.y = 60;
		}
		else if (i == 45) {
			pos.x = 540;
			pos.y = 500;
		}
		else if (i >= 46 && i < 53)
			pos.y += 40;
		else if (i == 53) {
			pos.y = 20;
			pos.x = 380;
		}
		else if (i >= 54 && i < 63)
			pos.y += 40;
		else if (i == 63) {
			pos.x = 500;
			pos.y = 380;
		}
		else if (i >= 64 && i < 72) {
			pos.x += 40;
		}
	}
}

void GameState::moveFruitRand() {
	while (true) {
		this->fruit->setRandomPos(); //Moving Fruit
		bool coll = 0;

		if (this->head->checkFruitCollision(this->fruit->getCollider())==false) {
			coll = checkBorderCollision();
			if (coll == true)
				std::cout << "fruit and wall are colliding" << std::endl;
		}
		else {
			std::cout << "fruit and tail are colliding" << std::endl;
			coll = true;
		}

		if (!coll)
			break;

	}

}

bool GameState::checkBorderCollision() 
{
	for (int i = 0; i < this->borders.size(); i++)
	{
		const Collider& fruitcoll = fruit->getCollider();
		if (borders[i]->getCollider().checkCollision(fruitcoll) == true)
		{
			return true;
		}

	}
	return false;
}

NextState  GameState::switchState()
{
	if (getQuit())
		return Lost;
	else
		return NoChange;
}