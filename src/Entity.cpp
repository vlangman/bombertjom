#include "Entity.hpp"
#include "Component.hpp"


// =============================== PLAYER =============================== //
void Player::update()
{
	graphics->update();
}

Player::Player(Game *world)
:Entity(world)
{
	mType = ET_PLAYER;
}


void    Player::setDirection(int vertical, int horizontal){
	north = false;
	south = false;
	east = false;
	west = false;

	if (vertical < 0)
		south = true;
	else if (vertical > 0)
		north = true;

	if (horizontal < 0)
		west = true;
	else if (horizontal > 0)
		east = true;
}

void	Player::movePlayer(float DeltaTime){
	// std::cout << "MOVING WITH DELTA: " << DeltaTime << std::endl;


	float scale = 5000 * DeltaTime;
	if (east){
		// std::cout << "RIGHT" << std::endl;
		if (collision->checkCollision(getX() + scale, getY())){
			setX(getX() + scale);
		}
		
	}
	if (west){
		// std::cout << "LEFT" << std::endl;
		if (collision->checkCollision(getX() - scale, getY())){
			setX(getX() - scale);
		}
		
	}
	if (south){
		// std::cout << "DOWN" << std::endl;

		if (collision->checkCollision(getX(), getY() + scale))
		{
			setY(getY() + scale);
		}
	}
	if (north){
		// std::cout << "UP" << std::endl;
		if (collision->checkCollision(getX(), getY() - scale)){
			setY(getY() - scale);
		}
	}
	return;
}


void	Player::placeBomb(void){
	std::cout << "PLACING A BOMB at x: " << getX()<< " y: " << getY() << " scale is: " << m_world->getScale() << std::endl;
	Builder * builder = new Builder(m_world);
	builder->createBomb(getX(), getY(), m_world->getScale(), m_world->getScale());
	delete builder;

	return;
}


// =============================== WALL =============================== //
void Wall::update() 
{

}

Wall::Wall(Game *world)
:Entity(world)
{
	mType = ET_WALL;
}

// ============================== ENTITY ================================ //
Entity::Entity(void){
	std::cout << "THIS SHOULD NEVER BE CALLED!! REEEEEEEĒEĒ" << std::endl;
	return;
}

Entity::Entity(Game *world)
{
	m_world = world;
	mType = ET_NONE;
}

E_ENTITY_TYPE Entity::getEntityType()
{
	return mType;
}

void Entity::setX(double x)
{
	mX = x;
}

void Entity::setY(double y)
{
	mY = y;
}

void Entity::setPosition(double x, double y)
{
	mX = x;
	mY = y;
}

double Entity::getX()
{
	return mX;
}

double Entity::getY()
{
	return mY;
}

void Entity::setWidth(float _width){
	mWidth = _width;
}

void Entity::setHeight(float _height){
	mHeight = _height;
}

Game *Entity::getWorld(){
	return m_world;
}

// ============================== BOMB ================================ //

//canonical constructors
Bomb::Bomb(void) : Entity(){
	return;
}

Bomb::~Bomb(void){
	return;
}

Bomb::Bomb(const Bomb & _bomb){
	static_cast<void>(_bomb);
	return;
}

Bomb & Bomb::operator=(const Bomb & _rhs){
	static_cast<void>(_rhs);
	return *this;
}


//usefull constructors
Bomb::Bomb(Game *world) : Entity(world)
{
	mType = ET_BOMB;
}

//BOMB FUNCTIONS

void	Bomb::update(void){
	return;
}