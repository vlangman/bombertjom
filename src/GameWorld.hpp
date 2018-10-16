#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include "../GameEngine/graphics/includes/Functions.hpp"
// #include "../GameEngine/graphics/includes/Shader.hpp"
#include "../GameEngine/utils/includes/timer.hpp"
// #include "../GameEngine/graphics/src/Textures.cpp"
#include "../GameEngine/graphics/includes/Camera.hpp"
// #include "../GameEngine/graphics/includes/Models.hpp"
#include "ResourceManager.hpp"
#include "Builder.hpp"
#include <irrKlang.h>

// enum E_ANIMATION_TYPE;

class GameEntity;

const int SCREEN_WIDTH = 1366;
const int SCREEN_HEIGHT = 768;

class GameWorld
{
private:
	std::vector<GameEntity*> mEntityList;
	std::vector< std::vector<int> > m_map;
	GameEntity *m_Player;
    Camera * m_camera;
    GameBoi::graphics::Window *m_window;

    ResourceManager mResourceManager;
	Builder *mEntityBuilder;
	//game loop vars
	GameBoi::Timer *m_Timer;
	float mFrameRate = 100;
	int mEntityCount = 0;
	GameEntity *m_StartingBlock;
	float mScaling;
	irrklang::ISoundEngine *mSoundEngine;



public:
	GameWorld(void);
	~GameWorld(void);
	GameWorld & operator=(const GameWorld & _rhs);
	GameWorld(const GameWorld & _gameworld);

	void runLoop();
	void draw();

	void init(const std::string level);

	void loadMap(const std::string level);
	void loadResources();
	void spawnStartEntities();
	void stepGame(float Delta);
	void renderGame(void);
	void cleanUp(void);
	void playSound(char* audioFile);
	void play3DSound(char* audioFile, glm::vec3 pos);

	void quitGame(void);

	Camera *getCamera();
	GameBoi::graphics::Window *getWindow();


	std::vector<std::vector<int> > getMap(void) const;

	//entities
	std::vector<GameEntity*> *getEntityList();
	void addEntity(GameEntity*);
	Builder* getBuilder(void);
	GameEntity *getPlayer(void);
	float getScaling(void);
	//utility
	float getDeltaTime(void);

	//gameplay functions
	void respawnPlayer();


};