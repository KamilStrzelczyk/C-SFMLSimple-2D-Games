#include "Game.h"
#include <iostream>
#include <ctime>

//Private functions
void Game::initializerVariables()
{
	this->window = nullptr;

	//Game logic
	this->endGame = false;
	this->points = 0;
	this->health = 20;
	this->enemySpawnTimerMax = 20.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax; 
	this->maxEnemies = 5;
	this->mouseHeld = false;


}

void Game::initWindow()
{
	this->videoMode.height = 800;
	this->videoMode.width = 1200;
	this->window = new sf::RenderWindow(this->videoMode, "My first game", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(60);
}

void Game::initFonts()
{
	if (this->font.loadFromFile("Kids Monday.ttf"))
	{
		std::cout << "ERROR::GAME::INITFONTS::Failded to load font!" << "\n";
	}
}

void Game::initText()
{
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(30);
	this->uiText.setFillColor(sf::Color::White);
	this->uiText.setString("NONE");
}

void Game::initEnemies()
{
	this->enemy.setPosition(10.f, 10.f);
	this->enemy.setSize(sf::Vector2f(80.f, 80.f));
	//this->enemy.setScale(sf::Vector2f(0.5f, 0.5f));
	this->enemy.setFillColor(sf::Color::Yellow);
	/*this->enemy.setOutlineColor(sf::Color::Green);
	this->enemy.getOutlineThickness();*/
}

// Consturctors / Destructors

Game::Game()
{
	this->initializerVariables();
	this->initWindow();
	this->initFonts();
	this->initText();
	this->initEnemies();

}

Game::~Game()
{
	delete this->window;
}

//Accessors


const bool Game::getWindowIsOpen() const
{ 
	return this->window->isOpen(); 
}

const bool Game::getEndGame() const
{
	return this->endGame;
}

//Functions


void Game::spawnEnemy()
{
	/*
	return void
	spawns enemies and sets their types and colors. 
	set a random type (diff)
	Spawns them at positions randomly.
	sets a random position.
	sets a random color.
	add enemy to the vector.

	*/
	this->enemy.setPosition(
		static_cast<float>(rand()% static_cast<int>(this->window->getSize().x-this->enemy.getSize().x)),
		0.f

	);


	//Randomize enemy type
	int type = rand() % 5;


	switch (type)
	{
	case 0:
		this->enemy.setSize(sf::Vector2f(15.f, 15.f));
		
			this->enemy.setFillColor(sf::Color::Red);
			break;
	case 1:
		this->enemy.setSize(sf::Vector2f(30.f, 30.f));

		this->enemy.setFillColor(sf::Color::Blue);
		break; 
	case 2:
		this->enemy.setSize(sf::Vector2f(70.f, 70.f));

		this->enemy.setFillColor(sf::Color::Cyan);
			break;
	case 3:
		this->enemy.setSize(sf::Vector2f(50.f, 50.f));

		this->enemy.setFillColor(sf::Color::Yellow);
				break; 
	case 4:
		this->enemy.setSize(sf::Vector2f(100.f, 100.f));

		this->enemy.setFillColor(sf::Color::Magenta);
					break;
	default:
		this->enemy.setSize(sf::Vector2f(10.f, 10.f));
		this->enemy.setFillColor(sf::Color::White);
		break;
	}
	

	//spawn the enemy
	this->enemies.push_back(this->enemy);

	//Remove enemies at end of screen
}

void Game::pollEvents()
{
	//Event polling
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape)
				this->window->close();
			break;
		}
	}
}

void Game::updateMousePositions()
{
	/*
	@ return void
	Updates the mous positions:
	 *Mouse position relative to window (Vector2i)
	 */
	
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(mousePosWindow);
}

void Game::updateEnemies()
{
	/*
	return void 
	Update the enemy spawn timer and spawns enemies when the toal amount of enemies is smaller than the maximum
	moves the enemies downwards
	Removes the eniemie at the edge of the screen // todo
	*/
	//Updating the timer for enemy spawning
	if (this->enemies.size() < this->maxEnemies)
	{
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
		{
			//spawn the enemy and reset the timer
			this->spawnEnemy();
			this ->enemySpawnTimer = 0.f;
		}
	else
		this->enemySpawnTimer += 1.f;
	}

	//Moving and updatin enemies
	for (int i = 0; i < this->enemies.size(); i++)
	{
		bool deleted = false;
		this->enemies[i].move(0.f, 5.f);

		if (this->enemies[i].getPosition().y > this->window->getSize().y)
		{
		this->enemies.erase(this->enemies.begin() + i);
		this->health -= 1;
		std::cout << "Health: " << this->health << "\n";
	

		}
	
	}

		 //check if clicked upon
	
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (this->mouseHeld==false)
		{
			this->mouseHeld = true;
			bool deleted = false;
			for (size_t i = 0; i < this->enemies.size() && deleted == false; i++)
			{

				if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
				{

				//gain points
				if(this->enemies[i].getFillColor()==sf::Color::Red)
				this->points += 10;
				else if(this->enemies[i].getFillColor() == sf::Color::Blue)
					this->points += 7;
				else if (this->enemies[i].getFillColor() == sf::Color::Cyan)
					this->points += 3;
				else if (this->enemies[i].getFillColor() == sf::Color::Yellow)
					this->points += 5;
				else if (this->enemies[i].getFillColor() == sf::Color::Magenta)
					this->points += 1;
				std::cout << "Points: " << this->points << "\n";

				//Delete the enemy
				deleted = true;
				this->enemies.erase(this->enemies.begin() + i);
				}

			}
		}
	}
	else 
	{
		this->mouseHeld = false;
	}
}

void Game::update()
{
	this->pollEvents();

	if (this->endGame == false)
	{
	
	this->updateMousePositions();

	this->updateText();

	this->updateEnemies();
	}
	//end game condition
	if (this->health <= 0)
		this->endGame = true;
}

void Game::renderText(sf::RenderTarget& target)
{
	target.draw(this->uiText);
}

void Game::updateText()
{
	std::stringstream ss;
	ss << "Points: " << this->points<<"\n"
	<< "Health: " << this->health << "\n";
	this->uiText.setString(ss.str()); 
}

void Game::renderEnemies(sf::RenderTarget& target)
{
	//rendering all the enemies
	for (auto& e : this->enemies)
	{
		target.draw(e);

	}
}

void Game::render()
{
	/*
	* clear odl frame
	* render new objects
	* display frame in window
	Renders the game	objects.
	*/
	this->window->clear();


		//Draw game objects
	this->renderEnemies(*this->window);
	this->renderText(*this->window);
	this->window->display();
}
