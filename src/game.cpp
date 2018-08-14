#include "game.hpp"
#include "Builder.hpp"
#include "Entity.hpp"
#include "Component.hpp"

//canonical constructors
Game::Game(void){
	this->verbose = false;
	m_shouldRun = true;
	return;
}


Game::~Game(void){
	return;
}


Game & Game::operator=(const Game & _rhs){
	this->window_x = _rhs.getWindowX();
	this->window_y =  _rhs.getWindowY();
	return *this;
}


Game::Game(const Game & _game){
	this->window_x = _game.getWindowX();
	this->window_y =  _game.getWindowY();
	return;
}


//getters and setters
	
int Game::getWindowX(void) const{
	return this->window_x;
}

int Game::getWindowY(void) const{
	return this->window_y;
}

int Game::getVerbose(void) const{
	return this->window_y;
}


//game loop functions

int 	Game::runLoop(void)
{
	//handle any user input

	//update all objects (phys calcs)

	//render changes to the display
	Builder builder(this);

	// Add the player
	Entity *player = builder.createPlayer(30,30);
	addEntity(player);

	// Add some walls
	for (int i = 0; i < 20; i++){
		Entity *wall = builder.createWall(0,i * 30);
		addEntity(wall);
	}

	while (m_shouldRun)
	{
		E_EVENT event = sdl.handleEvents();

		if (event == E_EVENT::EVENT_CLOSE_WINDOW){
			m_shouldRun = false;
		}

		for (auto i: inputHandlers)
		{
			i->handleInput(event);
		}

		sdl.clearScreen();
		
		for (auto i : renderList)
		{
			sdl.draw(i->getOwner()->getX(),i->getOwner()->getY(),32,32,i->getColor());
		}
		sdl.displayScreen();
	}
	return 1;
}

void 	Game::init(int _verbose, int width, int height, bool fullscreen){
	this->verbose = _verbose;
	this->window_x = width;
	this->window_y = height;

	this->sdl.init(width, height, this->verbose);

	return;
}

void	Game::closeGame(void){
	exit(1);
}

void	Game::addEntity(Entity *entity)
{
	entityList.push_back(entity);

	if (entity->getEntityType() == E_ENTITY_TYPE::ET_PLAYER){
		renderList.push_back(dynamic_cast<Player*>(entity)->graphics);
		inputHandlers.push_back(dynamic_cast<Player*>(entity)->inputHandler);
	}
	else if (entity->getEntityType() == E_ENTITY_TYPE::ET_WALL){
		renderList.push_back(dynamic_cast<Wall*>(entity)->graphics);
	}
}

// void	cleanup();


//utility

void 	Game::log(std::string message){
	if (this->verbose){
		std::cout << message << std::endl;
	}
	return;
}
