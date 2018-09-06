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
	player->graphics = new GraphicsComponent(player, E_COLOR::COLOR_GREEN, width, height);
	// init input handler
	player->inputHandler = new PlayerInputComponent(player);

	std::cout << "BUILT PLAYER" << std::endl;
	player->collision = new CollisionComponent(player);

	player->moveComponent = new MovementComponent(player);

	player->moveComponent->move(x, y);

	return player;
}

Entity *Builder::createWall(double x, double y, double width, double height, bool canDestroy)
{
	Wall *wall = new Wall(m_world, canDestroy);


	wall->setWidth(width);
	wall->setHeight(height);
	// init graphics component
	if (canDestroy)
		wall->graphics = new GraphicsComponent(wall, E_COLOR::COLOR_GREY, width, height);
	else
		wall->graphics = new GraphicsComponent(wall, E_COLOR::COLOR_BLACK, width, height);
	wall->collision = new CollisionComponent(wall);
	wall->setPosition(x,y);
	return wall;
}



Entity *Builder::createStartBlock(double x, double y, double width, double height)
{
	std::cout << "creating start block" << std::endl;

	Wall *wall = new Wall(m_world, false);
	wall->playerStartBlock();

	wall->setWidth(width);
	wall->setHeight(height);
	// init graphics component

	wall->graphics =  new GraphicsComponent(wall, E_COLOR::COLOR_PURPLE, width, height);

	wall->collision = new CollisionComponent(wall);
	wall->setPosition(x,y);

	return wall;

}


Entity *Builder::createBomb(double x, double y, double width, double height){

	Bomb *bomb = new Bomb(m_world, 1);

	bomb->setWidth(width);
	bomb->setHeight(height);

	bomb->graphics = new GraphicsComponent(bomb, E_COLOR::COLOR_RED, width, height);
	bomb->timer = new TimerComponent(bomb);
	bomb->collision = new CollisionComponent(bomb);

	bomb->setPosition(x,y);

	m_world->addEntity(bomb);
	return bomb;

}


Entity *Builder::createEnemy(double x, double y, double width, double height){
	Enemy *enemy = new Enemy(m_world);

	enemy->setWidth(width);
	enemy->setHeight(height);

	enemy->graphics = new GraphicsComponent(enemy, E_COLOR::COLOR_BLUE, width, height);
	enemy->timer = new TimerComponent(enemy);
	enemy->collision = new CollisionComponent(enemy);
	
	enemy->setPosition(x, y);
	return (enemy);
}