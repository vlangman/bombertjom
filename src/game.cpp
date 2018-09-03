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
	std::vector<std::vector<int>> map = this->gameWorld.getMap();
	double		increment_y = static_cast<double>(this->window_y) / map.size();
	double		increment_x =  static_cast<double>(this->window_x) / map.size();
	this->scale = increment_y;

	
	// read map from gameworld into game entities
	 for (unsigned int x = 0; x < map.size(); x++) {
		for (unsigned int y = 0; y < map[x].size(); y++)
		{
			//if the number read is a 1 (ascii 49)
			if (map[x][y] == 49)
			{
				Entity *wall = builder.createWall(x*increment_x,y*increment_y,increment_x, increment_y);
				addEntity(wall);
			}
			//if the number read is a 2 (ascii 50)

			if (map[x][y] == 50)
			{

				Entity *enemy = builder.createEnemy(x*increment_x,y*increment_y,increment_x, increment_y);
				addEntity(enemy);
			}
		}
	}

	// Add the player
	Entity *player = builder.createPlayer( increment_x,  increment_y,  increment_x,  increment_y);
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

	for (auto i: entityList){
		if (i->getEntityType() == E_ENTITY_TYPE::ET_BOMB){
			i->update();
		}
		if (i->getEntityType() == E_ENTITY_TYPE::ET_ENEMY){
			i->update();
		}
	}
	//move the player
	auto player = dynamic_cast<Player*>(this->m_Player);
	player->movePlayer(DeltaTime);

	cleanUp();	
}


int 	Game::runLoop(void)
{

	buildGameObjects();
	m_Timer->Reset();
	float timeStep = 0.0f;
	float frameCounter = 0.0f;
	float Delta = 0.0f;

	m_Timer->update();
	while (m_shouldRun)
	{
		m_Timer->update();
		Delta = m_Timer->DeltaTime();
		timeStep += Delta;

		while(timeStep < (1/frameRate)){
			//update timer sets start time to now
			m_Timer->update();
			Delta = m_Timer->DeltaTime();
			//increment the time for last frame
			timeStep += Delta;
			//step the game with the current delta time if time avaliable
			if (timeStep < (1/frameRate))
				stepGame(Delta);
			else
				break;
		}
		m_Timer->Reset();
		//reset timer so we can include how long render takes
		sdl.clearScreen();
		for (auto i : renderList)
		{
			sdl.draw(i->getOwner()->getX(),i->getOwner()->getY(),i->getWidth(),i->getHeight(),i->getColor());
		}
		sdl.displayScreen();
		// float FPS = 1.0f/timeStep;
		// sdl.drawFps(FPS);
		timeStep = 0;
		frameCounter = 0;
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
	this->gameWorld.init("dank.map");

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
		colliderList.push_back(dynamic_cast<Wall*>(entity)->collision);
	}
	else if (entity->getEntityType() == E_ENTITY_TYPE::ET_BOMB){
		renderList.push_back(dynamic_cast<Bomb*>(entity)->graphics);
	}
	else if (entity->getEntityType() == E_ENTITY_TYPE::ET_ENEMY){
		renderList.push_back(dynamic_cast<Enemy*>(entity)->graphics);
		colliderList.push_back(dynamic_cast<Enemy*>(entity)->collision);
	}
}

void	Game::cleanUp(void){

	//INPUT HANDLER LIST;
	for(std::vector<PlayerInputComponent*>::iterator it = inputHandlers.begin(); it!= inputHandlers.end(); /*it++*/){
		Entity *entity = (*it)->getOwner();

		if (!entity->isAlive()){
				delete ((*it));
				it = inputHandlers.erase(it);
		}
		else
			it++;
	}

	//RENDER LIST
	for(std::vector<GraphicsComponent*>::iterator it = renderList.begin(); it!= renderList.end(); /*it++*/){
		Entity *entity = (*it)->getOwner();

		if (!entity->isAlive()){

				delete ((*it));
				it = renderList.erase(it);
		}
		else
			it++;
	}

	//COLLIDER LIST
	for(std::vector<CollisionComponent*>::iterator it = colliderList.begin(); it!= colliderList.end(); /*it++*/){
		Entity *entity = (*it)->getOwner();

		if (!entity->isAlive()){
			delete ((*it));

			it = colliderList.erase(it);
		}
		else
			it++;
	}

	//ENTITY LIST MUST COME LAST !!!
	for (std::vector<Entity*>::iterator it= entityList.begin(); it!= entityList.end(); /*it++*/) 
	{
		if (!(*it)->isAlive()){
			delete ((*it));
			it = entityList.erase(it);
			break;
		}
		else
			it++;
	}

};



//utility

void 	Game::log(std::string message)
{
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
	return m_Timer->DeltaTime();
}

double Game::getScale(void) const{
	return  scale;
}

float	Game::getFrameRate(void) const{
	return frameRate;
}