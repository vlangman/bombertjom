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


void	Game::buildGameObjects(void){

	Builder 	builder(this);
	int 		blocks_x = (this->window_x /32);
	int 		blocks_y = (this->window_y /32);
	double		increment_y = static_cast<double>(blocks_y) / 10;
	double		increment_x =  static_cast<double>(blocks_x) / 10;

	std::vector<std::vector<int>> map = this->gameWorld.getMap();
	// read map from gameworld into game entities
	 for (int x = 0; x < map.size(); x++) {
		for (int y = 0; y < map[x].size(); y++)
		{
			if (map[x][y] == 49)
			{
				Entity *wall = builder.createWall(x*32*increment_x,y*32*increment_y,32*increment_x,32* increment_y);
				addEntity(wall);
			}
		}
	}

	// Add the player
	Entity *player = builder.createPlayer(32 * increment_x, 32 * increment_y, 32 * increment_x, 32 * increment_y);
	addEntity(player);
	this->m_Player = player;
}

//game loop functions

void	Game::stepGame(float DeltaTime){
	//calulate and draw
	E_EVENT event = sdl.handleEvents();

	if (event == E_EVENT::EVENT_CLOSE_WINDOW){
		m_shouldRun = false;
	}

	for (auto i: inputHandlers)
	{
		i->handleInput(event);
	}

	//this is some yikes code xD
	auto player = dynamic_cast<Player*>(this->m_Player);
	player->movePlayer(DeltaTime);

	sdl.clearScreen();

	for (auto i : renderList)
	{
		sdl.draw(i->getOwner()->getX(),i->getOwner()->getY(),i->getWidth(),i->getHeight(),i->getColor());
	}
}


int 	Game::runLoop(void)
{

	buildGameObjects();
	m_Timer->Reset();
	float timeStep = 0.0f;
	float frameCounter = 0.0f;
	float Delta = 0.0f;

	while (m_shouldRun)
	{

		m_Timer->Update();
		Delta = m_Timer->DeltaTime();
		timeStep += Delta;
		
		while(timeStep < (1/frameRate)){
			//update timer
			m_Timer->Update();
			Delta = m_Timer->DeltaTime();
			//increment the time for last frame
			timeStep += Delta;
			//step the game with the current delta time
			stepGame(Delta);
			//increment the stepCounter
			frameCounter++;
		}
		
		sdl.displayScreen();

		float FPS = 1.0f/m_Timer->DeltaTime();
		sdl.drawFps(FPS);
		timeStep = 0;
		frameCounter = 0;
		m_Timer->Reset();
	}
	return 1;



}


void 	Game::init(int _verbose, int width, int height, bool fullscreen){
	this->verbose = _verbose;
	this->window_x = width;
	this->window_y = height;

	this->sdl.init(width, height, this->verbose);
	this->m_shouldRun = true;
	//instantiates the game world
	this->gameWorld.init("test2.map");
	// exit(1);

	//use instance to create and reset timer
	m_Timer = Timer::Instance();
	return;
}

void	Game::closeGame(void){
	Timer::Release();
	m_Timer = NULL;
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


void Game::handleEvents()
{
	E_EVENT event = sdl.handleEvents();

	if (event == E_EVENT::EVENT_CLOSE_WINDOW) {
		std::cout << "CLOSING GAME" << std::endl;
		closeGame();
		return;
	}
	
}


float Game::getDeltaTime(void){
	m_Timer->Update();
	return m_Timer->DeltaTime();
}