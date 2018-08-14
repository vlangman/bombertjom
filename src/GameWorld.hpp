#pragma once
#include <vector>

class Entity;

class GameWorld
{
private:
    std::vector<Entity*> mEntityList;
public:
    void update();
    void draw();

    std::vector<Entity*> *getEntityList();
    void addEntity(Entity*);
};