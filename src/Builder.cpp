#include "Builder.hpp"
#include "GameEntity.hpp"
#include "Component.hpp"
#include "GameWorld.hpp"

enum E_ANIMATION_TYPE;

Builder::Builder()
{
	
}

Builder::Builder(GameWorld *world)
{
	m_world = world;
}

void Builder::setGameScale(float scale){
	mGameScale = scale;
}

void Builder::createFloor(double x, double y, double z)
{
	GameEntity *floor = new GameEntity(m_world, false);
	floor->setPosition(glm::vec3(x,y,z));
	
	// Add a graphics component
	GraphicsComponent *graphics = new GraphicsComponent(floor);
	graphics->create(m_resources->getShader("shader_basic"), m_resources->getModel("model_floor"), glm::vec3(1.0f,1.0f,1.0f), 0, glm::vec3(1.0f, 0.1f, 1.0f));

	floor->addComponent(graphics);
	m_world->addEntity(floor);
}                                                                                   

void Builder::createWall(double x, double y, double z, bool canDestroy)
{
	GameEntity *wall = new GameEntity(m_world, canDestroy);
	wall->setType(E_ENTITY_TYPE::WALL);
	wall->setPosition(glm::vec3(x,y,z));
	
	// Add a graphics component
	GraphicsComponent *graphics = new GraphicsComponent(wall);
	if (!canDestroy){
		graphics->create(m_resources->getShader("shader_basic"), m_resources->getModel("model_wall"), glm::vec3(1.0f,1.0f,1.0f), 0, glm::vec3(1.0f,1.0f,1.0f));
	}else{
		graphics->create(m_resources->getShader("shader_basic"), m_resources->getModel("model_crate"), glm::vec3(1.0f,1.0f,1.0f), 0, glm::vec3(1.0f,1.0f,1.0f));
	}

	// Add a collider component
	ColliderComponent *collider = new ColliderComponent(wall);
	collider->create(graphics->getSize().x * mGameScale, graphics->getSize().y *mGameScale);

	//add animation component
	AnimationComponent *animation = new AnimationComponent(wall);
	animation->loadModel(m_resources->getModel("model_wall_die1"), E_ANIMATION_TYPE::DEATH);
	animation->loadModel(m_resources->getModel("model_wall_die2"), E_ANIMATION_TYPE::DEATH);
	animation->loadModel(m_resources->getModel("model_wall_die3"), E_ANIMATION_TYPE::DEATH);
	animation->loadModel(m_resources->getModel("model_wall_die4"), E_ANIMATION_TYPE::DEATH);

	animation->loadModel(m_resources->getModel("model_player_stand1"), E_ANIMATION_TYPE::RUN);
	animation->loadModel(m_resources->getModel("model_player_stand1"), E_ANIMATION_TYPE::RUN);
	animation->loadModel(m_resources->getModel("model_player_run2"), E_ANIMATION_TYPE::RUN);
	animation->loadModel(m_resources->getModel("model_player_run3"), E_ANIMATION_TYPE::RUN);
	animation->loadModel(m_resources->getModel("model_player_run2"), E_ANIMATION_TYPE::RUN);


	// set animation times
	//run , idle, death, ability
	animation->setInterval(0.15f, 0.0f, 0.2f, 0.20f);
	animation->setAnimationMode(ANIMATION_NONE);


	wall->addComponent(graphics);
	wall->addComponent(animation);

	wall->addComponent(collider);
	m_world->addEntity(wall);	
}

GameEntity *Builder::createPlayer(double x, double y, double z)
{
	GameEntity *player = new GameEntity(m_world, true);
	player->setPosition(glm::vec3(x,y,z));
	player->setType(E_ENTITY_TYPE::PLAYER);
	player->setLives(3);
	player->setBombRange(1.0f);

	
	// Add a graphics component
	GraphicsComponent *graphics = new GraphicsComponent(player);
	graphics->create(m_resources->getShader("shader_basic"), m_resources->getModel("model_player_stand1"), glm::vec3(1.0f,1.0f,1.0f), 0, glm::vec3(0.5f, 1.0f, 0.5f));

	//add animation component
	AnimationComponent *animation = new AnimationComponent(player);
	animation->loadModel(m_resources->getModel("model_player_stand1"), E_ANIMATION_TYPE::IDLE);
	animation->loadModel(m_resources->getModel("model_player_stand2"), E_ANIMATION_TYPE::IDLE);
	animation->loadModel(m_resources->getModel("model_player_stand3"), E_ANIMATION_TYPE::IDLE);
	animation->loadModel(m_resources->getModel("model_player_stand2"), E_ANIMATION_TYPE::IDLE);

	animation->loadModel(m_resources->getModel("model_player_run1"), E_ANIMATION_TYPE::RUN);
	animation->loadModel(m_resources->getModel("model_player_run2"), E_ANIMATION_TYPE::RUN);
	animation->loadModel(m_resources->getModel("model_player_run3"), E_ANIMATION_TYPE::RUN);
	animation->loadModel(m_resources->getModel("model_player_run2"), E_ANIMATION_TYPE::RUN);

	animation->loadModel(m_resources->getModel("model_player_place1"), E_ANIMATION_TYPE::ABILITY);
	animation->loadModel(m_resources->getModel("model_player_place2"), E_ANIMATION_TYPE::ABILITY);
	animation->loadModel(m_resources->getModel("model_player_place1"), E_ANIMATION_TYPE::ABILITY);
	animation->loadModel(m_resources->getModel("model_player_place2"), E_ANIMATION_TYPE::ABILITY);
	animation->loadModel(m_resources->getModel("model_player_place3"), E_ANIMATION_TYPE::ABILITY);
	animation->loadModel(m_resources->getModel("model_player_place3"), E_ANIMATION_TYPE::ABILITY);

	animation->loadModel(m_resources->getModel("model_player_stand1"), E_ANIMATION_TYPE::DEATH);
	animation->loadModel(m_resources->getModel("model_player_die1"), E_ANIMATION_TYPE::DEATH);
	animation->loadModel(m_resources->getModel("model_player_die2"), E_ANIMATION_TYPE::DEATH);
	animation->loadModel(m_resources->getModel("model_player_die3"), E_ANIMATION_TYPE::DEATH);
	animation->loadModel(m_resources->getModel("model_player_die4"), E_ANIMATION_TYPE::DEATH);


	// set animation times
	//run , idle, death, ability
	animation->setInterval(0.10f, 0.25f, 0.15f, 0.1f);
	animation->setAnimationMode(IDLE);



	// Add a movement component
	PlayerMovementComponent *movement = new PlayerMovementComponent(player);
	movement->create(graphics, 3.0f);

	// Add a collider component
	ColliderComponent *collider = new ColliderComponent(player);
	collider->create(graphics->getSize().x* mGameScale*0.5f, graphics->getSize().y *mGameScale*0.5f);

	player->addComponent(graphics);
	player->addComponent(movement);
	player->addComponent(collider);
	player->addComponent(animation);

	m_world->addEntity(player);
	return player;
}

void Builder::createEnemy(double x, double y, double z)
{
	GameEntity *enemy = new GameEntity(m_world, true);
	enemy->setPosition(glm::vec3(x,y,z));
	enemy->setType(E_ENTITY_TYPE::ENEMY);
	
	// Add a graphics component
	GraphicsComponent *graphics = new GraphicsComponent(enemy);
	graphics->create(m_resources->getShader("shader_basic"), m_resources->getModel("model_enemy"), glm::vec3(1.0f,1.0f,1.0f), 0, glm::vec3(0.8f, 1.0f, 0.8f));

	// Add a movement component
	EnemyMovementComponent *movement = new EnemyMovementComponent(enemy);
	movement->create(graphics, 1.2f);

	// Add a collider component
	ColliderComponent *collider = new ColliderComponent(enemy);
	collider->create(graphics->getSize().x *mGameScale*0.8f, graphics->getSize().y*mGameScale*0.8f);

	//add animation component
	AnimationComponent *animation = new AnimationComponent(enemy);

	animation->loadModel(m_resources->getModel("model_monster1_run1"), E_ANIMATION_TYPE::RUN);
	animation->loadModel(m_resources->getModel("model_monster1_run2"), E_ANIMATION_TYPE::RUN);
	animation->loadModel(m_resources->getModel("model_monster1_run3"), E_ANIMATION_TYPE::RUN);
	animation->loadModel(m_resources->getModel("model_monster1_run2"), E_ANIMATION_TYPE::RUN);


	animation->loadModel(m_resources->getModel("model_monster1_die1"), E_ANIMATION_TYPE::DEATH);
	animation->loadModel(m_resources->getModel("model_monster1_die2"), E_ANIMATION_TYPE::DEATH);
	animation->loadModel(m_resources->getModel("model_monster1_die3"), E_ANIMATION_TYPE::DEATH);
	animation->loadModel(m_resources->getModel("model_monster1_die4"), E_ANIMATION_TYPE::DEATH);

	//run , idle, death, ability
	animation->setInterval(0.10f, 0.25f, 0.15f, 0.20f);
	animation->setAnimationMode(RUN);

	enemy->addComponent(graphics);
	enemy->addComponent(movement);
	enemy->addComponent(collider);
	enemy->addComponent(animation);


	m_world->addEntity(enemy);

}

void Builder::createBomb(double x, double y, double z)
{
	GameEntity *bomb = new GameEntity(m_world, false);
	bomb->setPosition(glm::vec3(x,y,z));
	bomb->setType(E_ENTITY_TYPE::BOMB);
	bomb->setBombRange(m_world->getPlayer()->getBombRange() * mGameScale);

	// Add a graphics component
	GraphicsComponent *graphics = new GraphicsComponent(bomb);
	graphics->create(m_resources->getShader("shader_basic"), m_resources->getModel("model_bomb"), glm::vec3(1.0f,1.0f,1.0f), 0, glm::vec3(1.0f, 1.0f, 1.0f));

	// Add a collider component
	ColliderComponent *collider = new ColliderComponent(bomb);
	collider->create(graphics->getSize().x*mGameScale, graphics->getSize().y*mGameScale);

	//add a timer component
	TimerComponent *timer = new TimerComponent(bomb);

	bomb->addComponent(timer);
	bomb->addComponent(graphics);
	bomb->addComponent(collider);
	m_world->addEntity(bomb);
}


GameEntity *Builder::createStartingGate(double x, double y, double z){
	GameEntity *wall = new GameEntity(m_world, false);
	wall->setType(E_ENTITY_TYPE::INVISIBLEWALL);
	wall->setPosition(glm::vec3(x,y,z));
	
	GraphicsComponent *graphics = new GraphicsComponent(wall);
	graphics->create(m_resources->getShader("shader_basic"), m_resources->getModel("model_wall"), glm::vec3(1.0f,1.0f,1.0f), 0, glm::vec3(1.25f, 1.25f, 1.25f));


	// Add a collider component
	ColliderComponent *collider = new ColliderComponent(wall);
	collider->create(graphics->getSize().x*mGameScale*1.25f, graphics->getSize().y*mGameScale*1.25f);

	wall->addComponent(collider);
	// wall->addComponent(graphics);

	m_world->addEntity(wall);
	return wall;
}


void  Builder::createPowerUp(double x, double y, double z, int type){
	GameEntity *powerup = new GameEntity(m_world, false);

	powerup->setPosition(glm::vec3(floor(x),y,floor(z)));

	GraphicsComponent *graphics = new GraphicsComponent(powerup);

	if (type == 1){
		powerup->setType(SPEED_UP);
		graphics->create(m_resources->getShader("shader_basic"), m_resources->getModel("powerup_speed"), glm::vec3(1.0f,1.0f,1.0f), 0, glm::vec3(1.0f, 1.0f, 1.0f));
	} else if (type == 2){
		powerup->setType(LIFE_UP);
		graphics->create(m_resources->getShader("shader_basic"), m_resources->getModel("powerup_life"), glm::vec3(1.0f,1.0f,1.0f), 0, glm::vec3(1.0f, 1.0f, 1.0f));
	} else if (type == 3){
		powerup->setType(BOMB_UP);
		graphics->create(m_resources->getShader("shader_basic"), m_resources->getModel("powerup_bomb"), glm::vec3(1.0f,1.0f,1.0f), 0, glm::vec3(1.0f, 1.0f, 1.0f));
	}


	// Add a collider component
	ColliderComponent *collider = new ColliderComponent(powerup);
	collider->create(graphics->getSize().x*mGameScale, graphics->getSize().y*mGameScale);

	powerup->addComponent(collider);
	powerup->addComponent(graphics);


	m_world->addEntity(powerup);
}


void Builder::create(GameWorld *world, ResourceManager *resources)
{
	m_world = world;
	m_resources = resources;
}
