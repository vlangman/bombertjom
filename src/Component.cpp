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
	if (event == E_EVENT::EVENT_NONE){
		player->setDirection(0,0);
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
	if (p_x > w_x && p_x < w_x + scale){
		//possible x collision since horizontal match
		if (p_y > w_y && p_y < w_y + scale){
			//check vertical match
			//x is within and y is within collide
			return true;
		}
	}
	if (p_x + scale > w_x && p_x + scale < w_x + scale){
		if (p_y + scale > w_y && p_y + scale < w_y + scale){
			return true;
		}
	}

	if (p_y > w_y && p_y < w_y + scale){
		if(p_x > w_x && p_x < w_x + scale){
			return true;
		}
	}

	if (p_y + scale > w_y && p_y + scale < w_y + scale){
		if(p_x + scale > w_x && p_x + scale < w_x + scale){
			return true;
		}
	}

	return false;
}


bool CollisionComponent::checkCollision(double x, double y){
	
	Game * m_game = m_owner->getWorld();
	double xInc = m_game->getWindowX() / 10.0f;
	double yInc = m_game->getWindowY() / 10.0f;

	for (auto i: m_game->colliderList)
	{
		if (withinSquare(x, y, i->getX(), i->getY(), yInc)){
			std::cout << "within square" << std::endl;
		}
	}

}

