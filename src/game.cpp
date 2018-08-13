#include "game.hpp"

//canonical constructors
Game::Game(void){
	this->verbose = false;
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

int 	Game::runLoop(void){

	while (m_shouldRun)
	{
		mTimer->Update();
		handleEvents();

		if (mTimer->DeltaTime() >= (1.0f/ frameRate)){
			//if the delta time is greater than 1/framerate
			//then you can draw otherwise wait
			std::cout << "DeltaTime: " << mTimer->DeltaTime() << std::endl;

			//if render done reset the game timer
			mTimer->Reset();
		}

		sdl.clearScreen();
		// cleanup the game;
	}
	return 0;
}

void 	Game::init(int _verbose, int width, int height, bool fullscreen){
	this->verbose = _verbose;
	this->window_x = width;
	this->window_y = height;

	this->sdl.init(width, height, this->verbose);
	this->m_shouldRun = true;

	//use instance cuase it creates and resets timer
	mTimer = Timer::Instance();
	return;
}

void	Game::closeGame(void){
	Timer::Release();
	mTimer = NULL;
	exit(1);
}

// void	addEntity();

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
