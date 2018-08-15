#include "Builder.hpp"
#include "Entity.hpp"
#include "Component.hpp"

Builder::Builder(Game *world)
{
    m_world = world;
}

Entity *Builder::createPlayer(double x, double y)
{
    Player *player = new Player(m_world);

    // init graphics component
    player->graphics = new GraphicsComponent(player, E_COLOR::COLOR_RED);
    // init input handler
    player->inputHandler = new PlayerInputComponent(player);

    player->setPosition(x, y);

    return player;
}

Entity *Builder::createWall(double x, double y)
{
    Wall *wall = new Wall(m_world);

    // init graphics component
    wall->graphics = new GraphicsComponent(wall, E_COLOR::COLOR_GREY);
    std::cout << "X: " << x << " Y: " << y << std::endl;
    wall->setPosition(x,y);
    return wall;
}