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

	auto player = dynamic_cast<Player*>(m_owner);
	Game *game = m_owner->getWorld();

	if (event == E_EVENT::EVENT_KEYBOARD_RIGHT){
		movePlayer(m_owner->getX()+ 3500* game->getDeltaTime() ,m_owner->getY());
	}
	else if (event == E_EVENT::EVENT_KEYBOARD_LEFT){
		movePlayer(m_owner->getX() - 3500* game->getDeltaTime(),m_owner->getY());
	}
	else if (event == E_EVENT::EVENT_KEYBAORD_DOWN){
		movePlayer(m_owner->getX(),m_owner->getY() + 3500* game->getDeltaTime());
	}
	else if (event == E_EVENT::EVENT_KEYBOARD_UP){
		movePlayer(m_owner->getX(),m_owner->getY() - 3500* game->getDeltaTime());
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