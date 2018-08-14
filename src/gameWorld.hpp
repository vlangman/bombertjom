#pragma once
#include <iostream>
#include <vector>

class GameWorld
{

private:
	std::vector<std::vector<int>> *m_map;

public:
	GameWorld();
	~GameWorld();

	GameWorld & operator=(const  GameWorld & _rhs);
	GameWorld(const GameWorld & _gameworld);


	//game world functions
	void init(void);
	std::vector<std::vector<int>> *getMap(void) const;


	//utility

	void readMap(const std::string mapName);

	
};