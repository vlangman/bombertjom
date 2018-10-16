#include "GameWorld.hpp"
#include "GameEntity.hpp"
#include "Font.hpp"
#include "../GameEngine/utils/includes/Skybox.hpp"
#include <fstream>

GameWorld::GameWorld(void){
	// setup the builder
	mEntityBuilder = new Builder();
	mEntityBuilder->create(this, &mResourceManager);
	// setup the camera
	m_camera = new Camera(glm::vec3(4.0f, 6.0f, 0.0f));
	m_camera->LookAt(glm::vec3(4.0f, 1.0f, 4.0f));
	// setup the window
	m_window = new GameBoi::graphics::Window("GameBoi!", SCREEN_WIDTH, SCREEN_HEIGHT);


}

void GameWorld::quitGame(void){
	for (auto i : mEntityList){
		std::cout << "killing entity: " << i->getType() << std::endl;
		i->destroy();
	}
	cleanUp();
	exit(0);
}

void GameWorld::stepGame(float DeltaTime){
	
	//exit the game
	if (m_window->isKeyPressed(GLFW_KEY_ESCAPE)) {
		quitGame();
		// break;
	}
	m_camera->Position = glm::vec3(m_Player->getPosition()->x, m_Player->getPosition()->y + 4.0f*mScaling , m_Player->getPosition()->z - 3.0f*mScaling);
	for (auto i : mEntityList)
	{
		i->update();
	}
	cleanUp();
	return;
}

void GameWorld::renderGame(void){
	for (auto i : mEntityList)
	{
		auto entityGraphics = dynamic_cast<GraphicsComponent*>(i->getComponent(COMPONENT_GRAPHICS));
		if (entityGraphics != NULL){
			entityGraphics->render();
		}
	}
}

void GameWorld::runLoop() 
{

	m_Timer->Reset();

	float 	timeStep = 0.0f;
	float 	Delta = 0.0f;
	float 	renderTime = 0;
	int 	frameCount = 0;
	float 	sumTimes = 0.0f;
	int 	engineTick = 0.0f;
	int 	dropcounter = 0;
	int 	last = 0;
	std::string fps = "0";
	std::string engine = "0";

	m_Timer->update();
	GameBoi::graphics::Fonts font("../Resources/Fonts/Chicago3D.ttf");
	GameBoi::graphics::Fonts font2("../Resources/Fonts/Chicago3D.ttf");
	// PlayerMovementComponent* playerMovement = dynamic_cast<PlayerMovementComponent*>(m_Player->getComponent(COMPONENT_PLAYERMOVEMENT));

	GameBoi::graphics::Shader skyboxShader("../GameEngine/graphics/src/shaders/skybox.vert", "../GameEngine/graphics/src/shaders/skybox.frag");
	std::vector<std::string> faces
        {
            "../Resources/skybox/space/red/bkg1_right1.png",
            "../Resources/skybox/space/red/bkg1_left2.png",
            "../Resources/skybox/space/red/bkg1_top3.png",
            "../Resources/skybox/space/red/bkg1_bottom4.png",
            "../Resources/skybox/space/red/bkg1_front5.png",
            "../Resources/skybox/space/red/bkg1_back6.png"
        };
   
    GameBoi::graphics::Skybox Box(faces);
	skyboxShader.setInt("skybox", 0);
  
	while (!m_window->closed())
	{
		m_Timer->update();
		Delta = m_Timer->DeltaTime();
		renderTime = Delta;

		//catch up by dropping the frame rate to compensate for long render times or speeding it up when leftover ticks
		if (renderTime < 1.0f/mFrameRate){
			dropcounter--;
			// std::cout << "\033[0;32m";
			if (dropcounter == -mFrameRate && mFrameRate <= 120){
				last+=2;
				mFrameRate+=2.0f + last;
				dropcounter = 0;
			}
		}
		else{
			// std::cout << "\033[0;31m";
			dropcounter++;
			if (dropcounter > mFrameRate)
				dropcounter -= mFrameRate/2;
			mFrameRate -= 2.0f;
			last = 0;
		}
		
		while(timeStep < (1.0f/mFrameRate)){
			//update timer sets start time to now
			stepGame(Delta);
			timeStep += Delta;
			m_Timer->update();
			Delta = m_Timer->DeltaTime();
			engineTick++;
		}
		m_Timer->Reset();
		//clear screen before render
		m_window->clear();
		//reset timer so we can include how long render takes
		//render it all
		renderGame();
    
		// draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();
		glm::mat4 view = m_camera->GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(m_camera->Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        view = glm::mat4(glm::mat3(m_camera->GetViewMatrix())); // remove translation from the view matrix
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);

        Box.Draw();

		font.RenderText(*mResourceManager.getShader("shader_text"), fps + " FPS", 0, SCREEN_HEIGHT - 50, 1, glm::vec3(1, 1, 0), SCREEN_WIDTH, SCREEN_HEIGHT);
		font2.RenderText(*mResourceManager.getShader("shader_text"),engine + " engine", SCREEN_WIDTH - 400, SCREEN_HEIGHT - 50, 1, glm::vec3(1, 1, 0), SCREEN_WIDTH, SCREEN_HEIGHT);

		m_window->update();

		sumTimes += timeStep;
		if (sumTimes > 1.0f){
			fps = std::to_string(frameCount);
			engine = std::to_string(engineTick);
			std::cout << "FPS[" << frameCount/sumTimes << "]/ENGINE[" << engineTick/sumTimes << "]"<< std::endl;
			// std::cout << "PLAYER X:" << m_Player->getPosition()->x << " Z: " << m_Player->getPosition()->z << std::endl;
			// std::cout << "\033[0m";
			sumTimes = 0;
			engineTick = 0;
			frameCount = 0;
		}
		timeStep = 0;
		frameCount++;
	}

}

GameWorld::~GameWorld(void){
	std::cout << "GameWorld decontructed" << std::endl;
	return;
}

GameWorld & GameWorld::operator=(const GameWorld & _rhs){
	this->m_map = _rhs.getMap();
	return *this;
}

GameWorld::GameWorld(const GameWorld & _gameworld){
	this->m_map = _gameworld.getMap();
	return;
}

GameEntity *GameWorld::getPlayer(void){
	return m_Player;
}

void GameWorld::loadMap(const std::string level)
{
	std::cout << "init the game world with map " << level << std::endl;
	std::string myLine;
	
	std::ifstream myFile("../Resources/maps/" + level);
	if (myFile.is_open()) {
		while(getline(myFile, myLine)){
			std::vector<int> line;
			for (int i = 0; myLine[i]; i++){
				line.push_back(myLine[i]);
			}
			m_map.push_back(line);
			line.clear();
		}
	}
	else{
		std::cout << "an error occured opening map file" << std::endl;
		exit(1);
		return;
	}
	return;
}

void GameWorld::loadResources()
{
	// Shaders
	mResourceManager.loadShader("shader_basic", "../GameEngine/graphics/src/shaders/basic.vert", "../GameEngine/graphics/src/shaders/basic.frag");
	mResourceManager.loadShader("shader_text", "../GameEngine/graphics/src/shaders/text.vert", "../GameEngine/graphics/src/shaders/text.frag");

	//players models
	//standing
	mResourceManager.loadModel("model_player_stand1", "../Resources/player/bboi_stand1.obj");
	mResourceManager.loadModel("model_player_stand2", "../Resources/player/bboi_stand2.obj");
	mResourceManager.loadModel("model_player_stand3", "../Resources/player/bboi_stand3.obj");


	//running
	mResourceManager.loadModel("model_player_run1", "../Resources/player/bboi_run1.obj");
	mResourceManager.loadModel("model_player_run2", "../Resources/player/bboi_run2.obj");
	mResourceManager.loadModel("model_player_run3", "../Resources/player/bboi_run3.obj");

	//dying 
	mResourceManager.loadModel("model_player_die1", "../Resources/player/bboi_die1.obj");
	mResourceManager.loadModel("model_player_die2", "../Resources/player/bboi_die2.obj");
	mResourceManager.loadModel("model_player_die3", "../Resources/player/bboi_die3.obj");
	mResourceManager.loadModel("model_player_die4", "../Resources/player/bboi_die4.obj");


	//enemies models
	//running
	mResourceManager.loadModel("model_monster1_run1", "../Resources/enemy/monster1_run1.obj");
	mResourceManager.loadModel("model_monster1_run2", "../Resources/enemy/monster1_run2.obj");
	mResourceManager.loadModel("model_monster1_run3", "../Resources/enemy/monster1_run3.obj");

	//dying 
	mResourceManager.loadModel("model_monster1_die1", "../Resources/enemy/monster1_die1.obj");
	mResourceManager.loadModel("model_monster1_die2", "../Resources/enemy/monster1_die2.obj");
	mResourceManager.loadModel("model_monster1_die3", "../Resources/enemy/monster1_die3.obj");
	mResourceManager.loadModel("model_monster1_die4", "../Resources/enemy/monster1_die4.obj");


	//ability (bomb placement)
	mResourceManager.loadModel("model_bomb", "../Resources/bomb/bomb1.obj");
	mResourceManager.loadModel("model_player_place1", "../Resources/player/bboi_bomb1.obj");
	mResourceManager.loadModel("model_player_place2", "../Resources/player/bboi_bomb2.obj");
	mResourceManager.loadModel("model_player_place3", "../Resources/player/bboi_bomb3.obj");


	//walls & crates & death 
	mResourceManager.loadModel("model_wall", "../Resources/space/space_wall.obj");
	mResourceManager.loadModel("model_crate", "../Resources/space/space_crate.obj");

	mResourceManager.loadModel("model_wall_die1", "../Resources/space/space_crate_die1.obj");
	mResourceManager.loadModel("model_wall_die2", "../Resources/space/space_crate_die2.obj");
	mResourceManager.loadModel("model_wall_die3", "../Resources/space/space_crate_die3.obj");
	mResourceManager.loadModel("model_wall_die4", "../Resources/space/space_crate_die4.obj");



	mResourceManager.loadModel("model_bomb", "../Resources/bomb/bomb1.obj");


	mResourceManager.loadModel("model_enemy", "../Resources/enemy/monster1_run1.obj");
	mResourceManager.loadModel("powerup_speed", "../Resources/powerups/powerup_speed.obj");
	mResourceManager.loadModel("powerup_bomb", "../Resources/powerups/powerup_bomb.obj");
	mResourceManager.loadModel("powerup_life", "../Resources/powerups/powerup_fire.obj");
	mResourceManager.loadModel("model_portal1", "../Resources/portals/space_portal1.obj");

}

void GameWorld::spawnStartEntities()
{
	float scaling = mResourceManager.getScaling();
	scaling *= 2.0f;
	std::cout << "Largest x = "  << scaling << std::endl;
	mScaling = scaling;
	mEntityBuilder->setGameScale(scaling);
	std::vector<std::vector<int>> map = this->m_map;
	// read map from gameworld into game entities
	 for (unsigned int x = 0; x < map.size(); x++) {
		for (unsigned int y = 0; y < map[x].size(); y++) {
			//if the number read is a 1 (ascii 49)
			if (map[x][y] == 49) {
				mEntityBuilder->createWall(x*scaling,0,y*scaling, false);
			}
			// //if the number read is 2 destroyable wall
			if (map[x][y] == 50) {
				mEntityBuilder->createWall(x*scaling,0,y*scaling, true);
			}
			//if the number read is 3
			if (map[x][y] == 51) {
				mEntityBuilder->createEnemy(x*scaling,0.1,y*scaling);
			}
			// //if the number read is 4
			if (map[x][y] == 52) {
				//create the starting block for the player
				m_StartingBlock = mEntityBuilder->createStartingGate(x*scaling,0.1,y*scaling);
			}
		}
	}
	if (!m_StartingBlock){
		std::cout << "PLAYER MISSING FROM MAP" << std::endl;
		exit(1);
	}
	m_Player = mEntityBuilder->createPlayer(m_StartingBlock->getPosition()->x,m_StartingBlock->getPosition()->y,m_StartingBlock->getPosition()->z);
}

void GameWorld::init(const  std::string level)
{
	loadMap(level);
	loadResources();
	spawnStartEntities();
	mSoundEngine = irrklang::createIrrKlangDevice();
	// mSoundEngine->play2D("../Resources/audio/loop.XM", GL_TRUE);
	m_Timer = GameBoi::Timer::Instance();
}

void  GameWorld::playSound(char * audioFile){
	mSoundEngine->play2D(audioFile);
	return;
}

void  GameWorld::play3DSound(char * audioFile, glm::vec3 pos){
	irrklang::vec3df position(pos.x, pos.y, pos.z);
	mSoundEngine->play3D(audioFile, position);
	return;
}

float GameWorld::getScaling(void){
	return mScaling;
}

std::vector<std::vector<int> > GameWorld::getMap(void) const {
	return this->m_map;
}
	

Camera *GameWorld::getCamera()
{
	return m_camera;
}

std::vector<GameEntity*> *GameWorld::getEntityList()
{
	return &mEntityList;
}

void GameWorld::addEntity(GameEntity* entity)
{
	mEntityCount++;
	entity->setId(mEntityCount);
	mEntityList.push_back(entity);
}

GameBoi::graphics::Window *GameWorld::getWindow()
{
	return m_window;
}


float GameWorld::getDeltaTime(void){
	return m_Timer->DeltaTime();
}


Builder * GameWorld::getBuilder(void){
	return this->mEntityBuilder;
}

void GameWorld::respawnPlayer(){
	int lives = m_Player->getLives();
	lives--;
	if (lives < 0){
		std::cout << "GAME OVER MOTHERFUCKER!" << std::endl;
	} else {
		m_Player->setLives(lives);
		m_Player->setPosition(m_StartingBlock->getPosition()->x,m_StartingBlock->getPosition()->y,m_StartingBlock->getPosition()->z);
	}
};


void GameWorld::cleanUp(void){
	for(std::vector<GameEntity*>::iterator it = mEntityList.begin(); it!= mEntityList.end(); /*it++*/){
		GameEntity *entity = (*it);

		if (!entity->isAlive()){
	
			E_ENTITY_TYPE type = entity->getType();
		
			AnimationComponent * animation = dynamic_cast<AnimationComponent*>(entity->getComponent(E_COMPONENT_TYPE::COMPONENT_ANIMATION));
			GraphicsComponent * graphics = dynamic_cast<GraphicsComponent*>(entity->getComponent(E_COMPONENT_TYPE::COMPONENT_GRAPHICS));

			if (animation != NULL){
				if (animation->getCurrentAnimation() == E_ANIMATION_TYPE::DEATH){
					if (animation->isCurrentAnimationComplete()){

						if (type == PLAYER){
							animation->setAnimationMode(IDLE);
							respawnPlayer();
						}
						else{
							if (type == WALL || type == ENEMY){
								entity->randomDrop(); 
							}
							delete ((*it));
							it = mEntityList.erase(it);
						}
					}else{
						animation->update();
						graphics->update();
					}
				}else{
					animation->setAnimationMode(E_ANIMATION_TYPE::DEATH);
				}
				it++;
			} else{
				if (type == WALL || type == ENEMY){
					entity->randomDrop(); 
				}
				delete (*it);
				it = mEntityList.erase(it);
			}
		}
		else
			it++;
	}
	return;
}

