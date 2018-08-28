#include "gameWorld.hpp"
#include <fstream>

GameWorld::GameWorld(void){
	return;
}

GameWorld::~GameWorld(void){
	std::cout << "GameWorld decontructed" << std::endl;
	return;
}

GameWorld & GameWorld::operator=(const GameWorld & _rhs){
	this->m_map = _rhs.getMap();
	return *this;
}

GameWorld::GameWorld(const GameWorld & _gameworld){
	this->m_map = _gameworld.getMap();
	return;
}


void GameWorld::init(const  std::string level){
	std::cout << "init the game world with map " << level << std::endl;
	std::string myLine;
	
	std::ifstream myFile("../maps/" + level);
	if (myFile.is_open()) {
		while(getline(myFile, myLine)){
			std::vector<int> line;
			for (int i = 0; myLine[i]; i++){
				line.push_back(myLine[i]);
			}
			m_map.push_back(line);
			line.clear();
		}
	}
	else{
		std::cout << "an error occured opening map file" << std::endl;
		exit(1);
		return;
	}
	return;
}

std::vector<std::vector<int>> GameWorld::getMap(void) const {
	return this->m_map;
}
	