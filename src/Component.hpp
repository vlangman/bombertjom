#pragma once
#include "game.hpp"
#include "Entity.hpp"


class Entity;

class Component 
{
protected:
	Entity *m_owner;
public:
	Component(Entity *owner);
	Entity *getOwner();
	
	~Component(void);
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
	bool	checkCollision(double x, double y, E_ENTITY_TYPE type);
	void	lethalCollision(double x, double y, E_ENTITY_TYPE type);
	double	getX(void);
	double	getY(void);
	void	kill(void);
	bool 	isAlive(void);
};


class TimerComponent : public Component 
{
private:
	float	mElapsedTime;

public:

	TimerComponent(Entity *owner);
	~TimerComponent(void);

	void	Reset(void);
	bool	checkTimer(float time);

	//update the clock with latest tick from game loop
	void	UpdateElapsed(float DeltaTime);
	void	update(void);
};
