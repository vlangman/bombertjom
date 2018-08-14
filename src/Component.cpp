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

GraphicsComponent::GraphicsComponent(Entity *owner, E_COLOR color)
: Component(owner), mColor(color)
{

}

void GraphicsComponent::update()
{

}

E_COLOR GraphicsComponent::getColor()
{
    return mColor;
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
        m_owner->setX(m_owner->getX() + 10);
    }
    else if (event == E_EVENT::EVENT_KEYBOARD_LEFT){
        m_owner->setX(m_owner->getX() - 10);
    }
    else if (event == E_EVENT::EVENT_KEYBAORD_DOWN){
        m_owner->setY(m_owner->getY() + 10);
    }
    else if (event == E_EVENT::EVENT_KEYBOARD_UP){
        m_owner->setY(m_owner->getY() - 10);
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
