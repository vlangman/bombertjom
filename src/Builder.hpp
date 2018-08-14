#pragma once

class Entity;
class Game;

class Builder 
{
private:
    Game *m_world;
public:
    Builder(Game *world);
    Entity *createPlayer(double x, double y);
    Entity *createWall(double x, double y);
};

