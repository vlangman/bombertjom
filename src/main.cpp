#include "../GameEngine/graphics/includes/Functions.hpp"
#include "../GameEngine/graphics/includes/Shader.hpp"
#include "../GameEngine/utils/includes/timer.hpp"
#include "../GameEngine/graphics/src/Textures.cpp"
#include "../GameEngine/graphics/includes/Camera.hpp"
#include "../GameEngine/graphics/includes/Models.hpp"

#include "Entity.hpp"
#include "ResourceManager.hpp"
#include "GameWorld.hpp"

#include <iostream>
	
	
int main() 
{
	GameWorld *gameWorld = new GameWorld();

	gameWorld->init("dank.map");

	gameWorld->runLoop();

    return 0;
}