#include "Builder.hpp"
#include "Entity.hpp"
#include "Component.hpp"

Builder::Builder(Game *world)
{
	m_world = world;
}

Entity *Builder::createPlayer(double x, double y, double width, double height)
{
	Player *player = new Player(m_world);

	// init graphics component
	player->graphics = new GraphicsComponent(player, E_COLOR::COLOR_RED, width, height);
	// init input handler
	player->inputHandler = new PlayerInputComponent(player);

	player->moveComponent = new MovementComponent(player);

	player->moveComponent->move(x, y);

	return player;
}

Entity *Builder::createWall(double x, double y, double width, double height)
{
	Wall *wall = new Wall(m_world);

	wall->setWidth(width);
	wall->setHeight(height);
	// init graphics component
	wall->graphics = new GraphicsComponent(wall, E_COLOR::COLOR_GREY, width, height);
	wall->setPosition(x,y);
	return wall;
}