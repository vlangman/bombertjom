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


void GameWorld::init(void){
	std::cout << "init the game world" << std::endl;
	return;
}

std::vector<std::vector<int>>* GameWorld::getMap(void) const{
	return this->m_map;
}


// void GameWorld::readMap(const std::string mapName){
	
// 	std::string 	myLine;
// 	int 			lineCount;
// 	int 			colsCount;


// 	std::ifstream	myFile("../maps/" + mapName);
// 	int count = 0;


// 	if (myFile.is_open()){

// 		while (std::getline(myFile, myLine))
//    			++lineCount;

//    		for (int i = 0; myLine[i]; i++){
//    			colsCount++;
//    		}

// 		this->m_map->resize(lineCount, std::vector<int>(colsCount, 0));
	
// 		while(std::getline(myFile, myLine)){
		
// 			for(int i = 0; myLine[i]; i++){
// 				m_map[count][i] = std::atoi(myLine[i]);
// 			}
// 			count++;
// 		}

// 	}
// 	else{
// 		std::cout << "an error occured opening file" << std::endl;
// 		return;
// 	}
// }

	