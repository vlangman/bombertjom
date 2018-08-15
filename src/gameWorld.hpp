#pragma once
#include <vector>
#include <iostream>
#include <fstream>

class Entity;

class GameWorld
{
private:
	std::vector<Entity*> mEntityList;
	std::vector< std::vector<int> > m_map;

public:
	GameWorld(void);
	~GameWorld(void);
	GameWorld & operator=(const GameWorld & _rhs);
	GameWorld(const GameWorld & _gameworld);

	void update();
	void draw();

	void init(const std::string level);
	std::vector<std::vector<int>> getMap(void) const;

	//entities
	std::vector<Entity*> *getEntityList();
	void addEntity(Entity*);

};