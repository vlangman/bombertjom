#include "GameEntity.hpp"
#include "Component.hpp"
#include "GameWorld.hpp"
#include "Builder.hpp"

std::vector<Component*> *GameEntity::getComponents()
{
	return &mComponentList;
}

Component *GameEntity::getComponent(E_COMPONENT_TYPE type)
{
	for (auto i : mComponentList){
		if (i->getType() == type){
			return i;
		}
	}
	return NULL;
}

void GameEntity::addComponent(Component *component)
{
	mComponentList.push_back(component);
}

GameEntity::GameEntity(GameWorld *world, bool canDestroy)
{
	m_world = world;
	mCanDestroy = canDestroy;
	mIsAlive = true;
	mPosition = glm::vec3(1.0f, 1.0f, 1.0f);
	mType = E_ENTITY_TYPE::NONE;
}


GameEntity::~GameEntity(void){
	//kill component list
	for (auto i : mComponentList){
		// std::cout << "DELETING COMPONENT: " << i->getType() << std::endl;
		delete i;
		// std::cout << "DONE "<< std::endl;

	}
	// for(std::vector<Component*>::iterator it = mComponentList.begin(); it != mComponentList.end(); it++){
	// 	Component *component = (*it);
	// 	// std::cout << "KILLING COMPONENT: " << component->getType() << std::endl;
	// 	delete ((*it));
	// 	// std::cout << "erasing " <<std::endl;
	// 	it = mComponentList.erase(it);
	// }
}

// void GameEntity::releaseComponents(void){
// 	for (auto i : mComponentList){
// 		if (i->getType() != COMPONENT_GRAPHICS && i->getType()!= COMPONENT_ANIMATION){
// 			delete i;
// 		}
// 	}
// }

void GameEntity::setId(int id){
	mId = id;
}

int GameEntity::getId(void) const{
	return mId;
}

void GameEntity::setType(E_ENTITY_TYPE type)
{
	mType = type;
}

E_ENTITY_TYPE GameEntity::getType()
{
	return mType;
}

GameWorld *GameEntity::getWorld()
{
	return m_world;
}

glm::vec3 *GameEntity::getPosition()
{
	return &mPosition;
}

void GameEntity::setPosition(float x, float y, float z)
{
	mPosition = glm::vec3(x,y,z);
}

void GameEntity::update()
{
	//ONLY RESPONSIBLE FOR MOVING ENTITY && TIMER CHECKS NOT RENDERING
	if (isAlive()){
		for (auto i : mComponentList) {
	  		i->update();
		}
	}
	//if entity is a bomb check the timer
	if (getType() == E_ENTITY_TYPE::BOMB){
		TimerComponent * timer = dynamic_cast<TimerComponent*>(getComponent(E_COMPONENT_TYPE::COMPONENT_TIMER));
		if (timer->checkTimer(3.0f))
		{
			detonateBomb();
			kill();
		}
	}
}

void GameEntity::setPosition(glm::vec3 position)
{
	mPosition = position;
}

void GameEntity::placeBomb(void){
	if (getType() != E_ENTITY_TYPE::PLAYER){
		std::cout << "ENTITY OF TYPE: "<< getType() << " cannot place bomb" << std::endl;
		return;
	}
	Builder * builder = getWorld()->getBuilder();
	double xpos = static_cast<double>(nearbyint(getPosition()->x));
	double zpos = static_cast<double>(nearbyint(getPosition()->z));

	std::cout << "PLACING BOMB AT xPOS: " << xpos << " ZPOS: " <<  xpos << std::endl;
	builder->createBomb(xpos, getPosition()->y, zpos);
	return;
}


void GameEntity::detonateBomb(){
	if (getType() != E_ENTITY_TYPE::BOMB){
		std::cout << "ENTITY OF TYPE: "<< getType() << " cannot detonate" << std::endl;
		return;
	}
	bool left = 0;
	bool right = 0;
	bool up = 0;
	bool down = 0;

	int bombRaduis = static_cast<int>(getBombRange());
	float originalX = getPosition()->x;
	float originalY = getPosition()->y;
	float originalZ = getPosition()->z;

	auto bombCollider = dynamic_cast<ColliderComponent*> (getComponent(E_COMPONENT_TYPE::COMPONENT_COLLIDER));
	//set position to pos +/- bomb raduis i.e 1.0f 
	m_world->playSound("../Resources/audio/explosion.wav");
	for (int raduis = 1; raduis <= bombRaduis; raduis++){
		if (!left){
			setPosition(originalX + static_cast<float>(raduis), originalY, originalZ);
			bombCollider->setCollision();
			if (bombCollider->checkCollision()!= NULL)
			{
				left = true;
				std::cout << "BLOCKED LEFT SIDE" << std::endl;
				auto victim = bombCollider->checkCollision();
				if (victim->isDestroyable()){
					victim->kill();
				}
			}
			setPosition(originalX, originalY, originalZ);
		}
		if (!right){
			setPosition(originalX - static_cast<float>(raduis), originalY, originalZ);
			bombCollider->setCollision();
			if (bombCollider->checkCollision()!= NULL)
			{
				right = true;
				std::cout << "BLOCKED RIGHT SIDE" << std::endl;

				auto victim = bombCollider->checkCollision();
				if (victim->isDestroyable()){
					victim->kill();
				}
			}
			setPosition(originalX, originalY, originalZ);
		}
		if (!up){
			setPosition(originalX , originalY, originalZ + static_cast<float>(raduis));
			bombCollider->setCollision();
			if (bombCollider->checkCollision()!= NULL)
			{
				up = true;
				std::cout << "BLOCKED TOP SIDE" << std::endl;

				auto victim = bombCollider->checkCollision();
				if (victim->isDestroyable()){
					victim->kill();
				}
			}
			setPosition(originalX, originalY, originalZ);
		}
		if (!down){
			setPosition(originalX , originalY, originalZ - static_cast<float>(raduis));
			bombCollider->setCollision();
			if (bombCollider->checkCollision()!= NULL)
			{
				down = true;
				std::cout << "BLOCKED BOTTOM SIDE" << std::endl;

				auto victim = bombCollider->checkCollision();
				if (victim->isDestroyable()){
					victim->kill();
				}
			}
			bombCollider->clearCollision();
			setPosition(originalX, originalY, originalZ);
		}
	}
}

void GameEntity::kill(void){
		mIsAlive = false;
}

int GameEntity::getLives(void){
	return mLives;
}

void GameEntity::setLives(int _lives){
	mIsAlive = true;
	this->mLives = _lives;
}

void GameEntity::setBombRange(float range){
	mBombRange = range;
}

float GameEntity::getBombRange(void){
	return mBombRange;
}

bool GameEntity::isAlive(void){

	return mIsAlive;
}

bool GameEntity::isDestroyable(void){
	return mCanDestroy;
}

void GameEntity::randomDrop(void){
	Builder *bobDatBuilder = m_world->getBuilder();

	int randomIndex = static_cast<int>(rand() % 10 + 1);
	if (randomIndex == 3){
		bobDatBuilder->createPowerUp(getPosition()->x,getPosition()->y, getPosition()->z, 1);
	} else if (randomIndex == 5){
		bobDatBuilder->createPowerUp(getPosition()->x,getPosition()->y, getPosition()->z, 2);
	}else if (randomIndex == 7){
		bobDatBuilder->createPowerUp(getPosition()->x,getPosition()->y, getPosition()->z, 3);
	}

}


void GameEntity::destroy(void){
	mIsAlive = false;
}