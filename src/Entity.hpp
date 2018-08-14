#pragma once
#include <vector>

class Game;
class RenderDescription;
class Component;
class GraphicsComponent;
class PlayerInputComponent;

enum E_ENTITY_TYPE 
{
    ET_PLAYER,
    ET_WALL,
    ET_NONE
};

class Entity
{
protected:
    double mX;
    double mY;
    E_ENTITY_TYPE mType;
    Game *m_world;

public:
    Entity(Game *world);
    virtual void update() = 0;
    
    double getX();
    double getY();

    void setX(double x);
    void setY(double y);
    void setPosition(double x, double y);

    Game *getWorld();

    E_ENTITY_TYPE getEntityType();
};

class Player : public Entity 
{
private:
public: 
    Player(Game *world);

    GraphicsComponent *graphics;
    PlayerInputComponent *inputHandler;
    void update();
};

class Wall : public Entity 
{
private: 
public:
    Wall(Game *world);
    GraphicsComponent *graphics;
    void update();
};