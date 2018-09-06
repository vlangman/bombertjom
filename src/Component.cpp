#include "Component.hpp"
#include "Entity.hpp"
#include <iostream>

Component::Component(Entity *owner)
{
	if (owner == NULL){
		std::cout << "Component::Component - owner passed is NULL" << std::endl;
	}
	m_owner = owner;
}

Component::~Component()
{
	std::cout << "COMPONENT DECONSTRUCTOR CALLED" << std::endl;
}
// ================= GRAPHICS COMPONENT ============================ //

GraphicsComponent::GraphicsComponent(Entity *owner, E_COLOR color, double width, double height)
: Component(owner), mColor(color), mwidth(width), mheight(height)
{

}

void GraphicsComponent::update()
{

}

E_COLOR GraphicsComponent::getColor()
{
	return mColor;
}

double GraphicsComponent::getWidth() const{
	return mwidth;
}

double GraphicsComponent::getHeight() const{
	return mheight;
}

// ================ INPUT COMPONENT ================================= //
PlayerInputComponent::PlayerInputComponent(Entity *owner)
: Component(owner)
{

}

void PlayerInputComponent::update()
{
}

void PlayerInputComponent::handleInput(E_EVENT event)
{
	if (m_owner->getEntityType() != E_ENTITY_TYPE::ET_PLAYER){
		std::cout << "ERROR: PlayerInput components' owner is not of type PLAYER";
	}

	auto player = dynamic_cast<Player*>(m_owner);
	
	if (event == E_EVENT::EVENT_KEYBOARD_RIGHT){
		player->setDirection(0,1);
	}
	if (event == E_EVENT::EVENT_KEYBOARD_LEFT){
		player->setDirection(0,-1);
	}
	if (event == E_EVENT::EVENT_KEYBAORD_DOWN){
		player->setDirection(-1,0);
	}
	if (event == E_EVENT::EVENT_KEYBOARD_UP){
		player->setDirection(1,0);
	}
	if (event == E_EVENT::EVENT_KEYBOARD_SPACEBAR){
		player->placeBomb();
	}
	if (event == E_EVENT::EVENT_NONE){
		// player->setDirection(0,0);
	}
	
	

}



// ================ MOVEMENT COMPONENT ============================== //

MovementComponent::MovementComponent(Entity *owner)
: Component(owner)
{

}

void MovementComponent::move(double x, double y)
{
	m_owner->setPosition(x, y);
}

Entity *Component::getOwner()
{
	return m_owner;
}

void  MovementComponent::update(){
}

// ================ COLLISION COMPONENT ============================== //

void CollisionComponent::update(){

}

CollisionComponent::CollisionComponent(Entity *owner)
: Component(owner)
{

}

double  CollisionComponent::getX(){
	return m_owner->getX();
}

double  CollisionComponent::getY(){
	return m_owner->getY();
}

bool	withinSquare(double p_x, double p_y, double w_x, double w_y, double scale){
	float softness = scale/20;
	scale -= softness;
	// p_y = p_y - softness;
	//directly on top of eachother
	if (p_x == w_x && p_y == w_y){
		return true;
	}

	if (p_x == w_x && p_y < w_y + scale && w_y < p_y){
		// std::cout << "top side collision" << std::endl;
		return true;
	} else if (p_x == w_x && p_y + scale > w_y && w_y > p_y){
		// std::cout << "bottom side collision" << std::endl;
		return true;
	}

	if (p_y == w_y && p_x < w_x + scale && w_x < p_x){
		// std::cout << "left side collision" << std::endl;
		return true;
	} else if (p_y == w_y && p_x + scale > w_x && w_x > p_x){
		// std::cout << "right side collision" << std::endl;
		return true;
	}

	// if left side of player overlaps right side of wall && right side doesnt overlap left side of wall
	if (p_x < w_x + scale && p_x > w_x){
		// std::cout << "if left side of player overlaps right side of wall" << std::endl;
		if (p_y > w_y && p_y < w_y + scale){
			// std::cout << "top left corner collision" << std::endl;
			return true;
		}else if (p_y < w_y && p_y + scale > w_y){
			// std::cout << "bottom left corner collision" << std::endl;
			return true;
		}
	}
	// if right side of player overlaps left side of wall
	if (p_x + scale > w_x && p_x < w_x){
		// std::cout << "if right side of player overlaps left side of wall" << std::endl;
		if (p_y > w_y && p_y < w_y + scale){
			// std::cout << "top right corner collision" << std::endl;
			return true;
		}else if (p_y < w_y && p_y + scale > w_y){
			// std::cout << "bottom right corner collision" << std::endl;
			return true;
		}
	}

	return false;
}


bool CollisionComponent::checkCollision(double x, double y, E_ENTITY_TYPE type) {

	Game * m_game = m_owner->getWorld();
	double scale = m_game->getScale();
	E_ENTITY_TYPE ownerType = getOwner()->getEntityType();

	for (auto i: m_game->colliderList){
		if (ownerType == ET_PLAYER && i->getOwner()->getEntityType() == ET_IWALL){
			continue;
		}
		if (i->isAlive()){
			if (i->getOwner()->getId() != getOwner()->getId()){
				if (i->getOwner()->getEntityType() != type){
					if (withinSquare(x, y, i->getX(), i->getY(), scale)){
						if (i->getOwner()->getEntityType() == ET_ENEMY && ownerType == ET_PLAYER){
							Player * player = dynamic_cast<Player*>(getOwner());
							player->killPlayer();
						}
						if (i->getOwner()->getEntityType() == ET_PLAYER && ownerType == ET_ENEMY){
							Player * player = dynamic_cast<Player*>(i->getOwner());
							player->killPlayer();
						}
						return false;
					}
				}
			}
		}
	}
	return true;
}


void	CollisionComponent::lethalCollision(double x, double y, E_ENTITY_TYPE type){
	Game * m_game = m_owner->getWorld();
	double scale = m_game->getScale();
	int count = 0;

	for (auto i: m_game->colliderList)
	{
		if (i->getOwner()->getId() != getOwner()->getId() && i->isDestroyable()){
			if (i->getOwner()->getEntityType() != type){
				if (withinSquare(x, y, i->getX(), i->getY(), scale)){
					i->kill();
				}
			}
		}
		count++;
	}
}

void	CollisionComponent::kill(){
	m_owner->kill();
}

bool CollisionComponent::isDestroyable(void){
	return m_owner->isDestroyable();
}

bool	CollisionComponent::isAlive(){
	return m_owner->isAlive();
}

// ================ TIMER COMPONENT ============================== //

void	TimerComponent::update(void){
	return;
}

TimerComponent::TimerComponent(Entity *owner)
: Component(owner)
{
	Reset();
	return;
}

TimerComponent::~TimerComponent(void){
	return;
}

void TimerComponent::Reset(void) {
	mElapsedTime = 0.0f;
}

void TimerComponent::UpdateElapsed(float DeltaTime){
	//ticks in miliseconds (/1000 for microsecs)
	mElapsedTime += DeltaTime;
}


bool TimerComponent::checkTimer(float time){
	// std::cout << "component TIMER: " << mElapsedTime << std::endl;
	if (mElapsedTime > time)
		return true;
	return false;
}