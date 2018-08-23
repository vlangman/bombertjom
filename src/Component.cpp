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

void PlayerInputComponent::movePlayer(double x, double y)
{
	auto player = dynamic_cast<Player*>(m_owner);
	player->moveComponent->move(x, y);
}

void PlayerInputComponent::update()
{
}

void PlayerInputComponent::handleInput(E_EVENT event)
{
	if (m_owner->getEntityType() != E_ENTITY_TYPE::ET_PLAYER){
		std::cout << "ERROR: PlayerInput components' owner is not of type PLAYER";
	}
	Game *game = m_owner->getWorld();
	float offset = game->getDeltaTime();
	
	if (offset < 1.0f/60){
		offset = 1.0f/60 * 500;
	}
	// std::cout << "OFFSET: " << offset << std::endl;

	auto player = dynamic_cast<Player*>(m_owner);
	

	if (event == E_EVENT::EVENT_KEYBOARD_RIGHT){
		movePlayer(m_owner->getX() + offset , m_owner->getY());
	}
	else if (event == E_EVENT::EVENT_KEYBOARD_LEFT){
		movePlayer(m_owner->getX() - offset , m_owner->getY());
	}
	else if (event == E_EVENT::EVENT_KEYBAORD_DOWN){
		movePlayer(m_owner->getX(), m_owner->getY() + offset);
	}
	else if (event == E_EVENT::EVENT_KEYBOARD_UP){
		movePlayer(m_owner->getX(), m_owner->getY() - offset);
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

void  MovementComponent::update()
{
}