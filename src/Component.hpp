#pragma once
#include "game.hpp"

class Entity;

class Component 
{
protected:
    Entity *m_owner;
public:
    Component(Entity *owner);
    Entity *getOwner();

    virtual void update() = 0;
};

class GraphicsComponent : public Component
{
private: 
    double  *m_x;
    double  *m_y;
    int     *m_width;
    int     *m_height;
    E_COLOR mColor;
public:
    GraphicsComponent(Entity *owner, E_COLOR color);

    E_COLOR getColor();
    void update();
};

class MovementComponent : public Component 
{
public: 
    MovementComponent(Entity *owner);
    void move(double x, double y);
};

class PlayerInputComponent : public Component 
{
public:
    PlayerInputComponent(Entity *owner);
    void update();
    void handleInput(E_EVENT event);
};

class CollisionComponent : public Component 
{
private: 
public: 
    CollisionComponent(Entity *owner);
    void update();
};