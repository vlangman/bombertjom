#pragma once
#include "ResourceManager.hpp"
#include "GameEntity.hpp"

class GameEntity;
class GameWorld;

class Builder 
{
private:
    GameWorld *m_world;
    ResourceManager *m_resources;
    float mGameScale;
public:
    Builder(void);
    Builder(GameWorld *world);

    void setGameScale(float scale);
    void create(GameWorld *world, ResourceManager *resources);

    GameEntity *createStartingGate(double x, double y, double z);
    GameEntity *createPlayer(double x, double y, double z);
    void createEnemy(double x, double y, double z);
    void createWall(double x, double y, double z, bool canDestroy);
    void createBomb(double x, double y, double z);
    void createFloor(double x, double y, double z);
    void createPowerUp(double x, double y, double z, int type);

};

