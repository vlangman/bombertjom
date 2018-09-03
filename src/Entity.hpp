#pragma once
#include "Builder.hpp"
#include "timer.hpp"
#include <vector>

class Game;
class RenderDescription;
class Component;
class GraphicsComponent;
class CollisionComponent;
class TimerComponent;
class PlayerInputComponent;
class MovementComponent;

enum E_ENTITY_TYPE 
{
	ET_PLAYER,
	ET_WALL,
	ET_BOMB,
	ET_NONE
};

class Entity
{
protected:
	double mX;
	double mY;
	double mWidth;
	double mHeight;
	E_ENTITY_TYPE mType;
	Game *m_world;
	bool	mIsAlive;

public:
	Entity(void);

	Entity(Game *world);
	virtual void update() = 0;
	virtual ~Entity() = 0;
	
	double getX();
	double getY();

	void setX(double x);
	void setY(double y);
	void setPosition(double x, double y);

	void setWidth(float _width);
	void setHeight(float _height);
	float getWidth(void) const;
	float getHeight(void) const;
	bool  isAlive(void);

	Game *getWorld();

	E_ENTITY_TYPE getEntityType();
};

class Player : public Entity 
{
private:
	bool	north;
	bool	south;
	bool	west;
	bool	east;
public: 
	Player(Game *world);

	GraphicsComponent *graphics;
	PlayerInputComponent *inputHandler;
	MovementComponent *moveComponent;
	CollisionComponent *collision;

	void	update();
	void	setDirection(int vertical, int horizontal);
	void	movePlayer(float DeltaTime);
	void	placeBomb(void);
};

class Wall : public Entity 
{
private: 
public:
	Wall(Game *world);
	GraphicsComponent *graphics;
	CollisionComponent *collision;
	void update();
};


class Bomb : public Entity 
{
public:

	//COMPONENTS
	GraphicsComponent *graphics;
	CollisionComponent *collision;
	TimerComponent * timer;

	//canonical constructors
	Bomb(void);
	~Bomb(void);
	Bomb(const Bomb & _bomb);
	Bomb & operator=(const Bomb & _rhs);

	//usefull constructors
	Bomb(Game *world);

	//BOMB FUNCTIONS
	void	update();
};
