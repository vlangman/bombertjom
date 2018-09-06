#pragma once

class Entity;
class Game;

class Builder 
{
private:
    Game *m_world;
public:
    Builder(Game *world);
    Entity *createPlayer(double x, double y, double width, double height);
    Entity *createWall(double x, double y, double  width, double height, bool canDestroy);
    Entity *createStartBlock(double x, double y, double  width, double height);
    Entity *createBomb(double x, double y, double  width, double height);
    Entity *createEnemy(double x, double y, double  width, double height);
};

