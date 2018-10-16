#include "Component.hpp"
#include "GameEntity.hpp"
#include "GameWorld.hpp"
#include "../GameEngine/graphics/includes/Models.hpp"
#include <iostream>
#include <glm/gtx/string_cast.hpp>


Component::Component(GameEntity *owner)
{
	if (owner == NULL){
		std::cout << "Component::Component - owner passed is NULL" << std::endl;
	}
	m_owner = owner;

	mType = E_COMPONENT_TYPE::COMPONENT_DEFAULT;
}

E_COMPONENT_TYPE Component::getType()
{
	return mType;
}

GameEntity *Component::getOwner()
{
	return m_owner;
}

// ============================================================= GRAPHICS COMPONENT ========================================================//

GraphicsComponent::GraphicsComponent(GameEntity * owner)
:Component(owner)
{
	mType = E_COMPONENT_TYPE::COMPONENT_GRAPHICS;

	m_shader = NULL; 
	m_model = NULL;
}

void GraphicsComponent::create(GameBoi::graphics::Shader *shader, GameBoi::graphics::Model *model, glm::vec3 scale, float rotation, glm::vec3 size)
{
	m_shader = shader; 
	m_model = model;
	mScale = scale; 
	mRotation = rotation;
	mSize = size;

	m_shader->use();
}

void GraphicsComponent::setSize(glm::vec3 size){
	mSize = size;
}

void GraphicsComponent::update() 
{
	
}

glm::vec3 &GraphicsComponent::getSize()
{
	return mSize;
}

void GraphicsComponent::setRotation(float rot)
{
	mRotation = rot;
}

GameBoi::graphics::Model *GraphicsComponent::getModel()
{
	return m_model;
}

void GraphicsComponent::render() 
{
	Camera *camera = m_owner->getWorld()->getCamera();
	m_shader->use(); //KOBUS HERE IS YOUR ISSUE :) BUT THIS ISNT OPTIMAL
	m_shader->setMat4("projection", glm::perspective(glm::radians(camera->Zoom), (float)SCREEN_WIDTH/ (float)SCREEN_HEIGHT, 0.1f, 100.0f));
	m_shader->setMat4("view", camera->GetViewMatrix());
	
		// calculate the model matrix for each object and pass it to shader before drawing
	m_model->NewPostionAndScale(*m_owner->getPosition(),mSize / m_model->getSize(), mRotation);
	m_model->DrawAndSet(*m_shader, "model");
}

void GraphicsComponent::setModel(GameBoi::graphics::Model *_model){
	m_model = _model;
}


// ============================================================= PLAYER MOVEMENT COMPONENT ========================================================//


PlayerMovementComponent::PlayerMovementComponent(GameEntity *owner)
:Component(owner)
{
	mType = E_COMPONENT_TYPE::COMPONENT_MOVEMENT;
}

void PlayerMovementComponent::create(GraphicsComponent *graphics, float speed)
{
	m_graphics = graphics;
	mSpeed = speed;
}

E_ANIMATION_TYPE PlayerMovementComponent::getMoveState(void){
	return mState;
};


bool checkBombPlacememt(float x, float z, GameEntity *b) 
{
	if (nearbyint(x) == b->getPosition()->x && nearbyint(z) == b->getPosition()->z){
		return true;
	}
	return false;
}
	
void PlayerMovementComponent::update()
{
	auto window = this->m_owner->getWorld()->getWindow();
	auto player = this->m_owner;
	auto playerCollider = dynamic_cast<ColliderComponent*>(player->getComponent(E_COMPONENT_TYPE::COMPONENT_COLLIDER));
	float DeltaTime = player->getWorld()->getDeltaTime();
	float speed = mSpeed * this->m_owner->getWorld()->getScaling();
	float distance = DeltaTime * speed;

	if (window->isKeyPressed(GLFW_KEY_A)) {
		m_graphics->setRotation(90);
		player->setPosition(player->getPosition()->x + distance, player->getPosition()->y, player->getPosition()->z);
		playerCollider->setCollision();
		handleCollision(1.0f,0.0f,distance);
	}
	else if (window->isKeyPressed(GLFW_KEY_D)) {
		m_graphics->setRotation(270);
		player->setPosition(player->getPosition()->x - distance, player->getPosition()->y, player->getPosition()->z);
		playerCollider->setCollision();
		handleCollision(-1.0f,0.0f,distance);

	}
	else if (window->isKeyPressed(GLFW_KEY_W)) {
		m_graphics->setRotation(0);
		player->setPosition(player->getPosition()->x, player->getPosition()->y, player->getPosition()->z + distance);
		playerCollider->setCollision();
		handleCollision(0.0f,1.0f,distance);

	}
	else if (window->isKeyPressed(GLFW_KEY_S)) {
		m_graphics->setRotation(180);
		player->setPosition(player->getPosition()->x, player->getPosition()->y, player->getPosition()->z - distance);
		playerCollider->setCollision();
		handleCollision(0.0f,-1.0f,distance);

	}
	else if (window->isKeyPressed(GLFW_KEY_SPACE)) {
		bool canPlace = true;
		for (auto i : *(this->m_owner->getWorld()->getEntityList()))
		{
			if (i->getType() == E_ENTITY_TYPE::BOMB){
				if (checkBombPlacememt(player->getPosition()->x,player->getPosition()->z, i)){
					canPlace = false;
				}
			}
			if (i->getType() == E_ENTITY_TYPE::ENEMY){
				if (playerCollider->checkIntersection(player, i)){
					canPlace = false;
				}
			} 
		}
		if (canPlace){
			mState = E_ANIMATION_TYPE::ABILITY;
			//player->placeBomb();
		}
	} 
	else{
		mState = E_ANIMATION_TYPE::IDLE;
	}

}

void PlayerMovementComponent::handleCollision(float xDir, float zDir, float distance){
	auto player = this->m_owner;
	auto playerCollider = dynamic_cast<ColliderComponent*>(player->getComponent(E_COMPONENT_TYPE::COMPONENT_COLLIDER));
	auto victim = playerCollider->checkCollision();

	float xDistance = distance * xDir;
	float zDistance = distance * zDir;

	if (victim != NULL){
		if (victim->getType() == E_ENTITY_TYPE::WALL){
	 		mState = E_ANIMATION_TYPE::IDLE;
			playerCollider->clearCollision();
			player->setPosition(player->getPosition()->x - xDistance, player->getPosition()->y, player->getPosition()->z - zDistance);
		}
		else if (victim->getType() == E_ENTITY_TYPE::SPEED_UP){
			mState = E_ANIMATION_TYPE::RUN;
			mSpeed++;
			victim->kill();
			playerCollider->clearCollision();
		}
		else if (victim->getType() == E_ENTITY_TYPE::BOMB_UP){
			mState = E_ANIMATION_TYPE::RUN;
			player->setBombRange(player->getBombRange() + 1.0f);
			std::cout << "BOMB+++: " << player->getBombRange() << std::endl;
			victim->kill();
			playerCollider->clearCollision();
		}
		else if (victim->getType() == E_ENTITY_TYPE::LIFE_UP){
			mState = E_ANIMATION_TYPE::RUN;
			m_owner->setLives(m_owner->getLives() + 1);
			victim->kill();
			playerCollider->clearCollision();
		}
	}else{
		mState = E_ANIMATION_TYPE::RUN;
	}

}


float PlayerMovementComponent::getSpeed()
{
	return mSpeed;
}
void PlayerMovementComponent::setSpeed(float speed)
{
	mSpeed = speed;
}

// ============================================================= ENEMY MOVEMENT COMPONENT ========================================================//

EnemyMovementComponent::EnemyMovementComponent(GameEntity *owner)
:Component(owner)
{
	mType = E_COMPONENT_TYPE::COMPONENT_MOVEMENT;
}

void EnemyMovementComponent::create(GraphicsComponent *graphics, float speed)
{
	m_graphics = graphics;
	mSpeed = speed;
	xDirection = 1;
	zDirection = 0;
}

E_ANIMATION_TYPE EnemyMovementComponent::getMoveState(void){
	return mState;
};

void EnemyMovementComponent::handleCollision(float xDir, float zDir, float distance){
	auto enemy = this->m_owner;
	auto enemyCollider = dynamic_cast<ColliderComponent*>(enemy->getComponent(E_COMPONENT_TYPE::COMPONENT_COLLIDER));
	auto victim = enemyCollider->checkCollision();

	float xDistance = distance * xDir;
	float zDistance = distance * zDir;

	if (victim != NULL){
		if (victim->getType() == E_ENTITY_TYPE::WALL){
			enemy->setPosition(enemy->getPosition()->x - xDistance, enemy->getPosition()->y, enemy->getPosition()->z - zDistance);
			this->xDirection = 0;
			this->zDirection = 0;
		}
		if (victim->getType() == E_ENTITY_TYPE::ENEMY){
			enemy->setPosition(enemy->getPosition()->x - xDistance, enemy->getPosition()->y, enemy->getPosition()->z - zDistance);
			this->xDirection = 0;
			this->zDirection = 0;
		}
		if (victim->getType() == E_ENTITY_TYPE::BOMB){
			enemy->setPosition(enemy->getPosition()->x - xDistance, enemy->getPosition()->y, enemy->getPosition()->z - zDistance);
			this->xDirection = 0;
			this->zDirection = 0;
		}
		if (victim->getType() == E_ENTITY_TYPE::PLAYER){
			victim->kill();
			this->xDirection = 0;
			this->zDirection = 0;
		}
		if (victim->getType() == E_ENTITY_TYPE::INVISIBLEWALL){
			enemy->setPosition(enemy->getPosition()->x - xDistance, enemy->getPosition()->y, enemy->getPosition()->z - zDistance);
			this->xDirection = 0;
			this->zDirection = 0;
		}
		enemyCollider->clearCollision();
	}
	mState = E_ANIMATION_TYPE::RUN;

}

void EnemyMovementComponent::update()
{
	auto window = this->m_owner->getWorld()->getWindow();
	auto enemy = this->m_owner;
	auto enemyCollider = dynamic_cast<ColliderComponent*> (enemy->getComponent(E_COMPONENT_TYPE::COMPONENT_COLLIDER));
	float DeltaTime = enemy->getWorld()->getDeltaTime();
	float speed = mSpeed * this->m_owner->getWorld()->getScaling();
	float distance = DeltaTime * speed;

	// east
	if (xDirection > 0 && zDirection == 0) {
		m_graphics->setRotation(90);
		enemy->setPosition(enemy->getPosition()->x + distance, enemy->getPosition()->y, enemy->getPosition()->z);
		enemyCollider->setCollision();
		handleCollision(1.0f,0.0f,distance);
	}
	// west
	else if (xDirection < 0 && zDirection == 0) {
		m_graphics->setRotation(270);
		enemy->setPosition(enemy->getPosition()->x - distance, enemy->getPosition()->y, enemy->getPosition()->z);
		enemyCollider->setCollision();
		handleCollision(-1.0f,0.0f,distance);

	}
	//north
	else if (xDirection == 0 && zDirection > 0) {
		m_graphics->setRotation(0);
		enemy->setPosition(enemy->getPosition()->x, enemy->getPosition()->y, enemy->getPosition()->z + distance);
		enemyCollider->setCollision();
		handleCollision(0.0f,1.0f,distance);

	}
	//south
	else if (xDirection == 0 && zDirection < 0) {
		m_graphics->setRotation(180);
		enemy->setPosition(enemy->getPosition()->x, enemy->getPosition()->y, enemy->getPosition()->z - distance);
		enemyCollider->setCollision();
		handleCollision(0.0f,-1.0f,distance);
	}
	//player needs a new direction
	else {
		enemyCollider->clearCollision();
		possibleMoves();
		randomDirection();
	}
}

void EnemyMovementComponent::possibleMoves(void){
	
	auto enemy = this->m_owner;
	auto enemyCollider = dynamic_cast<ColliderComponent*>(enemy->getComponent(E_COMPONENT_TYPE::COMPONENT_COLLIDER));
	GameEntity *victim;
	glm::vec3 *originalPosition = enemy->getPosition();
	float originalX = originalPosition->x;
	float originalY = originalPosition->y;
	float originalZ = originalPosition->z;
	float scaling = 1.0f * this->m_owner->getWorld()->getScaling();


	mPossibleMoves.clear();


	//east
	enemy->setPosition(originalX + scaling, originalY, originalZ);
	enemyCollider->setCollision();
	victim = enemyCollider->checkCollision();
	if (victim != NULL){
		if (victim->getType() != E_ENTITY_TYPE::WALL && victim->getType() != E_ENTITY_TYPE::BOMB && victim->getType() != E_ENTITY_TYPE::ENEMY){
			mPossibleMoves.push_back(E_DIRECTIONS::EAST);
		}
	}else {
		mPossibleMoves.push_back(E_DIRECTIONS::EAST);
	}

	//west
	enemy->setPosition(originalX - scaling, originalY, originalZ);
	enemyCollider->setCollision();
	victim = enemyCollider->checkCollision();
	if (victim != NULL)
	{
		if (victim->getType() != E_ENTITY_TYPE::WALL && victim->getType() != E_ENTITY_TYPE::BOMB && victim->getType() != E_ENTITY_TYPE::ENEMY){
			mPossibleMoves.push_back(E_DIRECTIONS::WEST);
		}
	} else {
		mPossibleMoves.push_back(E_DIRECTIONS::WEST);
	}

	//south
	enemy->setPosition(originalX, originalY, originalZ - scaling);
	enemyCollider->setCollision();
	victim = enemyCollider->checkCollision();
	if (victim != NULL){
		if (victim->getType() != E_ENTITY_TYPE::WALL && victim->getType() != E_ENTITY_TYPE::BOMB && victim->getType() != E_ENTITY_TYPE::ENEMY){
			mPossibleMoves.push_back(E_DIRECTIONS::SOUTH);
		}
	}
	else {
		mPossibleMoves.push_back(E_DIRECTIONS::SOUTH);
	}

	//north
	enemy->setPosition(originalX , originalY, originalZ + scaling);
	enemyCollider->setCollision();
	victim = enemyCollider->checkCollision();
	if (victim != NULL){
		if (victim->getType() != E_ENTITY_TYPE::WALL && victim->getType() != E_ENTITY_TYPE::BOMB && victim->getType() != E_ENTITY_TYPE::ENEMY){
			mPossibleMoves.push_back(E_DIRECTIONS::NORTH);
		}
	} else {
		mPossibleMoves.push_back(E_DIRECTIONS::NORTH);
	}

	enemy->setPosition(originalX, originalY, originalZ);

	return;
}

void EnemyMovementComponent::randomDirection(void){
	int randomIndex;
	E_DIRECTIONS type;

	srand(time(NULL));
	if (mPossibleMoves.size() == 0){
		xDirection = 0;
		zDirection = 0;
		return;
	}

	randomIndex = static_cast<int>(rand() % mPossibleMoves.size());
	type = static_cast<E_DIRECTIONS>(mPossibleMoves[randomIndex]);

	switch(type){
		case NORTH:
			xDirection= 0;
			zDirection = 1;
			break;
		case EAST:
			xDirection = 1;
			zDirection = 0;
			break;
		case SOUTH:
			xDirection = 0;
			zDirection = -1;
			break;
		case WEST:
			xDirection = -1;
			zDirection = 0;
			break;
	}

};

float EnemyMovementComponent::getSpeed()
{
	return mSpeed;
}
void EnemyMovementComponent::setSpeed(float speed)
{
	mSpeed = speed;
}




// ============================================================= COLLIDER COMPONENT ========================================================//

ColliderComponent::ColliderComponent(GameEntity *owner)
:Component(owner)
{
	mType = E_COMPONENT_TYPE::COMPONENT_COLLIDER;
	m_Collision = NULL;
}

void ColliderComponent::create(float xSize, float ySize)
{
	mXSize = xSize; 
	mYSize = ySize;
}


void ColliderComponent::update()
{

}

float ColliderComponent::getXSize()
{
	return mXSize;
}

float ColliderComponent::getYSize()
{
	return mYSize;
}

bool DoBoxesIntersect(GameEntity *a, GameEntity *b) 
{

	auto aCollider = dynamic_cast<ColliderComponent*> (a->getComponent(E_COMPONENT_TYPE::COMPONENT_COLLIDER));
	auto bCollider = dynamic_cast<ColliderComponent*> (b->getComponent(E_COMPONENT_TYPE::COMPONENT_COLLIDER));

  return (abs(a->getPosition()->x - b->getPosition()->x) * 2 < (aCollider->getXSize()/1.2f + bCollider->getXSize())) &&
		 (abs(a->getPosition()->z - b->getPosition()->z) * 2 < (aCollider->getYSize()/1.2f + bCollider->getYSize()));
}

void ColliderComponent::clearCollision(void){
	m_Collision = NULL;
}

bool ColliderComponent::checkIntersection(GameEntity * _a, GameEntity * _b){
	return (DoBoxesIntersect(_a, _b));
}

bool ColliderComponent::setCollision(void){
	bool collision = false;

	for (auto i : *(m_owner->getWorld()->getEntityList()))
	{
		if (i->getId() != m_owner->getId() && i->getComponent(E_COMPONENT_TYPE::COMPONENT_COLLIDER)){
			if (DoBoxesIntersect(m_owner, i))
			{
				m_Collision = i;
				collision = true;
			}
		}
	}
	if (!collision){
		m_Collision = NULL;
	}
}

GameEntity *ColliderComponent::checkCollision(void){
	return m_Collision;
};




// ================ TIMER COMPONENT ============================== //

void	TimerComponent::update(void){
	UpdateElapsed(m_owner->getWorld()->getDeltaTime());
	return;
}

TimerComponent::TimerComponent(GameEntity *owner)
: Component(owner)
{
	mType = E_COMPONENT_TYPE::COMPONENT_TIMER;
	Reset();
	return;
}

TimerComponent::~TimerComponent(void){
	return;
}

void TimerComponent::Reset(void) {
	// std::cout << "component TIMER: " << mElapsedTime << std::endl;
	mElapsedTime = 0.0f;
}

void TimerComponent::UpdateElapsed(float DeltaTime){
	//ticks in miliseconds (/1000 for microsecs)
	mElapsedTime += DeltaTime;
}


bool TimerComponent::checkTimer(float time){
	
	if (mElapsedTime >= time)
		return true;
	return false;
}


// ================ ANIMATION COMPONENT ============================== //

//cononical start
// AnimationComponent::AnimationComponent(void){
// 	return;
// };
AnimationComponent::~AnimationComponent(void){
	return;
};
// AnimationComponent::AnimationComponent(const AnimationComponent & animu){
// 	static_cast<void>(animu);
// 	return;
// };
// AnimationComponent & operator=(const AnimationComponent &rhs){
// 	static_cast<void>(rhs);
// 	return *this;
// };
//cononical start

AnimationComponent::AnimationComponent(GameEntity *_owner)
:Component(_owner)
{
	mType = E_COMPONENT_TYPE::COMPONENT_ANIMATION;
	timer = new TimerComponent(_owner);
}

void AnimationComponent::setAnimationMode(E_ANIMATION_TYPE _mode){
	if (mAnimationState == ABILITY && _mode == IDLE){
        if (mCurrentAnimation == m_models_ability.size()-1){
            mAnimationState = _mode;
            mCurrentAnimation = 0;
            timer->Reset();
        }
    }
    else if (mAnimationState != _mode){
        mAnimationState = _mode;
        mCurrentAnimation = 0;
        timer->Reset();
    }
}

void AnimationComponent::create()
{

}

bool AnimationComponent::isCurrentAnimationComplete(void){
	switch(mAnimationState){
		case RUN:
			return (mCurrentAnimation == m_models_run.size() - 1);
			break;
		case IDLE:
			return (mCurrentAnimation == m_models_idle.size() - 1);
			break;
		case DEATH:
			return (mCurrentAnimation == m_models_death.size() - 1);
			break;
		case ABILITY:
			return (mCurrentAnimation == m_models_ability.size() - 1);
			break;
	}
	return false;
}

void AnimationComponent::loadModel(GameBoi::graphics::Model * _model, E_ANIMATION_TYPE _type){
	switch(_type){
		case RUN:
			m_models_run.push_back(_model);
			break;
		case IDLE:
			m_models_idle.push_back(_model);
			break;
		case DEATH:
			m_models_death.push_back(_model);
			break;
		case ABILITY:
			m_models_ability.push_back(_model);
			break;
	}
};


void  AnimationComponent::update(){
	timer->update();

	E_ENTITY_TYPE type = m_owner->getType();
	Component *movementComponent;

	GraphicsComponent *graphics;
	
	if (mAnimationState != DEATH)
	{
		if (type == PLAYER){
			movementComponent = m_owner->getComponent(E_COMPONENT_TYPE::COMPONENT_MOVEMENT);
			setAnimationMode(dynamic_cast<PlayerMovementComponent*>(movementComponent)->getMoveState());
		}else if (type == ENEMY){
			movementComponent = m_owner->getComponent(E_COMPONENT_TYPE::COMPONENT_MOVEMENT);
			setAnimationMode(dynamic_cast<EnemyMovementComponent*>(movementComponent)->getMoveState());
		}
	}
	if (mAnimationState == RUN){
		
		if (mCurrentAnimation > m_models_run.size() - 1 || m_models_run.size() == 1){
			mCurrentAnimation = 0;
		}

		if (timer->checkTimer(mRunInterval)){
			// if (timer->checkTimer(0.5f))
			graphics = dynamic_cast<GraphicsComponent*>(m_owner->getComponent(COMPONENT_GRAPHICS));
			graphics->setModel(m_models_run[mCurrentAnimation]);
			mCurrentAnimation++;
			if (type == PLAYER)
				this->getOwner()->getWorld()->playSound("../Resources/audio/Player_Footsteps.wav");
			// if (type == ENEMY) {
			// 	glm::vec3 *position = getOwner()->getPosition();
			// 	this->getOwner()->getWorld()->play3DSound("../Resources/audio/Water_Footsteps.wav", *position);
			// }
			timer->Reset();
			
		}
	}
	else if (mAnimationState == IDLE){		
		if (mCurrentAnimation >= m_models_idle.size() - 1|| m_models_run.size() == 1){
			mCurrentAnimation = 0;
		}
		if (timer->checkTimer(mIdleInterval)){
			graphics = dynamic_cast<GraphicsComponent*>(m_owner->getComponent(COMPONENT_GRAPHICS));
			graphics->setModel(m_models_idle[mCurrentAnimation]);
			mCurrentAnimation++;
			timer->Reset();
		}
	}
	else if (mAnimationState == ABILITY){
		if (mCurrentAnimation >= m_models_ability.size() - 1|| m_models_run.size() == 1){
			mCurrentAnimation = 0;
		}
		if (timer->checkTimer(mAbilityInterval)){
			
			graphics = dynamic_cast<GraphicsComponent*>(m_owner->getComponent(COMPONENT_GRAPHICS));
			graphics->setModel(m_models_ability[mCurrentAnimation]);
			mCurrentAnimation++;
			timer->Reset();
		}
		if (isCurrentAnimationComplete()) {
			this->getOwner()->getWorld()->playSound("../Resources/audio/place.wav");
			m_owner->placeBomb();
		}
	}
	else if (mAnimationState == DEATH){
		//death animation can only play once so dont reset
		if (mCurrentAnimation <= m_models_death.size() - 1){
			if (timer->checkTimer(mDeathInterval)){
				graphics = dynamic_cast<GraphicsComponent*>(m_owner->getComponent(COMPONENT_GRAPHICS));
				graphics->setModel(m_models_death[mCurrentAnimation]);
				mCurrentAnimation++;
				timer->Reset();
			}
		}
		
	}
	return;
}

E_ANIMATION_TYPE AnimationComponent::getCurrentAnimation(void){
	return mAnimationState;
}

void AnimationComponent::setInterval(float run, float idle, float death, float ability){
	mRunInterval = run;
	mIdleInterval = idle;
	mDeathInterval = death;
	mAbilityInterval = ability;
}

