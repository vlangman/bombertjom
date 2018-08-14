#include "Entity.hpp"
#include "Component.hpp"


// =============================== PLAYER =============================== //
void Player::update()
{
    graphics->update();
}

Player::Player(Game *world)
:Entity(world)
{
    mType = ET_PLAYER;
}

// =============================== WALL =============================== //
void Wall::update() 
{

}

Wall::Wall(Game *world)
:Entity(world)
{
    mType = ET_WALL;
}

// ============================== ENTITY ================================ //
Entity::Entity(Game *world)
{
    m_world = world;
    mType = ET_NONE;
}

E_ENTITY_TYPE Entity::getEntityType()
{
    return mType;
}

void Entity::setX(double x)
{
    mX = x;
}

void Entity::setY(double y)
{
    mY = y;
}

void Entity::setPosition(double x, double y)
{
    mX = x;
    mY = y;
}

double Entity::getX()
{
    return mX;
}

double Entity::getY()
{
    return mY;
}