#pragma once
#include "Builder.hpp"
#include "timer.hpp"
#include <vector>
#include <math.h>

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
	ET_ENEMY,
	ET_IWALL,
	ET_NONE,
};

class Entity
{
protected:
	double mX;
	double mY;
	double mWidth;
	double mHeight;
	E_ENTITY_TYPE mType;
	Game	*m_world;
	bool	mIsAlive;
	bool	mIsDestroyable;
	int		id;

public:
	Entity(void);

	Entity(Game *world);
	virtual void update() = 0;
	virtual ~Entity() = 0;
	
	int    getId(void);
	double getX();
	double getY();
	bool   isDestroyable(void);
	bool   isAlive(void);

	void setX(double x);
	void setY(double y);
	void setPosition(double x, double y);
	void setWidth(float _width);
	void setHeight(float _height);
	void kill(void);

	float getWidth(void) const;
	float getHeight(void) const;

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
	int 	lives;
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
	void	killPlayer(void);
};

class Wall : public Entity 
{
private: 
public:
	Wall(Game *world, bool canDestroy);
	GraphicsComponent *graphics;
	CollisionComponent *collision;
	void update();
	void playerStartBlock(void);
};


class Bomb : public Entity 
{
private:
	int mRaduis;
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
	Bomb(Game *world, int raduis);

	//BOMB FUNCTIONS
	void	update();
	void	detonate(void);
};


class Enemy : public Entity
{
private:
	int 	mDirection;
public:
	GraphicsComponent *graphics;
	CollisionComponent *collision;
	TimerComponent * timer;

	Enemy(void);
	~Enemy(void);

	Enemy(Game *world);
	
	void update(void);

	void newDirection(void);
	void moveEnemy(void);
};


class PowerUp : public Entity
{
public:
	GraphicsComponent *graphics;
	CollisionComponent *collision;

	PowerUp(void);
	~PowerUp(void);

	PowerUp(Game *world);
	
	void update(void);
	void activatePowerUp(void);
};
