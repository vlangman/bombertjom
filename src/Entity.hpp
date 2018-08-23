#pragma once
#include <vector>

class Game;
class RenderDescription;
class Component;
class GraphicsComponent;
class PlayerInputComponent;
class MovementComponent;

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
	double mWidth;
	double mHeight;
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

	void setWidth(float _width);
	void setHeight(float _height);
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
public: 
	Player(Game *world);

	GraphicsComponent *graphics;
	PlayerInputComponent *inputHandler;
	MovementComponent *moveComponent;
	void	update();
	void	setDirection(int vertical, int horizontal);
	void	movePlayer(float DeltaTime);
};

class Wall : public Entity 
{
private: 
public:
	Wall(Game *world);
	GraphicsComponent *graphics;
	void update();
};