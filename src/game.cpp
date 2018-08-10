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

	//handle any user input

	//update all objects (phys calcs)

	//render changes to the display

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

// void	addEntity();

// void	cleanup();


//utility

void 	Game::log(std::string message){
	if (this->verbose){
		std::cout << message << std::endl;
	}
	return;
}
