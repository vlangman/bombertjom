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
	double   mwidth;
	double   mheight;
	E_COLOR mColor;
public:
	GraphicsComponent(Entity *owner, E_COLOR color, double width, double height);

	E_COLOR getColor();
	void update();
	double getHeight(void) const;
	double getWidth(void) const;

};

class MovementComponent : public Component 
{
public: 
	MovementComponent(Entity *owner);
	void move(double x, double y);
	void update();
};

class PlayerInputComponent : public Component 
{
public:
	PlayerInputComponent(Entity *owner);
	void update();
	void handleInput(E_EVENT event);
	// void movePlayer(double x, double y);
};

class CollisionComponent : public Component 
{
private:
	
public:
	CollisionComponent(Entity *owner);
	void	update();
	bool	checkCollision(double x, double y);
	double	getX(void);
	double	getY(void);
};