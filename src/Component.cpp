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
	if (event == E_EVENT::EVENT_KEYBOARD_SPACEBAR){
		player->placeBomb();
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


bool CollisionComponent::checkCollision(double x, double y){
	
	Game * m_game = m_owner->getWorld();
	double scale = m_game->getScale();

	for (auto i: m_game->colliderList)
	{
		if (withinSquare(x, y, i->getX(), i->getY(), scale)){
			return false;
		}
	}
	return true;
}


// ================ TIMER COMPONENT ============================== //

TimerComponent *TimerComponent::sInstance = NULL;

TimerComponent *TimerComponent::Instance(Entity *owner){
	if (sInstance == NULL){
		sInstance = new TimerComponent(owner);
	}
	return sInstance;
}

void TimerComponent::Release(void){
	delete sInstance;
	sInstance = NULL;
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
	mStartTicks = SDL_GetTicks();
	mDeltaTime = 0.0f;
}

float TimerComponent::DeltaTime(void){
	return mDeltaTime;
}

void TimerComponent::update(void){
	//ticks in miliseconds (/1000 for microsecs)
	mDeltaTime = (SDL_GetTicks() - mStartTicks) * 0.001f;
	if (mDeltaTime < 0.001f)
		mDeltaTime = 0.001f;
}