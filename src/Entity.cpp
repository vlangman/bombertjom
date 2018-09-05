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
	float frameRate = m_world->getFrameRate();
	float velocity = 5.0f * m_world->getScale();
	// std::cout << "Velocity: " << velocity << std::endl;
	float distance = velocity * DeltaTime;
	// std::cout << "SCALE: " <<  1.0f - DeltaTime<< std::endl;
	
	// float distance = velocity * DeltaTime;
	// float scale = DeltaTime * 100000.0f/m_world->getScale() * 3;
	//maybe check if next move is greater that game get scale
	// if (scale > m_world->getScale()){
	// 	std::cout << "HUGE SCALING " << scale << std::endl;
	// 	scale =  m_world->getScale() - 1.0f;
	// 	// exit(1);
	// }


	if (east){
		// std::cout << "RIGHT" << std::endl;
		if (collision->checkCollision(getX() + distance, getY(), ET_BOMB)){
			setX(getX() + distance);
		}
		
	}
	if (west){
		// std::cout << "LEFT" << std::endl;
		if (collision->checkCollision(getX() - distance, getY(), ET_BOMB)){
			setX(getX() - distance);
		}
	}
	if (south){
		// std::cout << "DOWN" << std::endl;
		if (collision->checkCollision(getX(), getY() + distance, ET_BOMB))
		{
			setY(getY() + distance);
		}
	}
	if (north){
		// std::cout << "UP" << std::endl;
		if (collision->checkCollision(getX(), getY() - distance, ET_BOMB)){
			setY(getY() - distance);
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

Entity::~Entity(void){
	return;
}

Entity::Entity(void){
	std::cout << "THIS SHOULD NEVER BE CALLED!! REEEEEEEĒEĒ... call the (Game * world) entity constructor pls boss" << std::endl;
	return;
}

Entity::Entity(Game *world)
{
	m_world = world;
	mType = ET_NONE;
	mIsAlive = true;
	id = world->newEntity();
}

bool Entity::isAlive(void){
	return mIsAlive;
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

int	Entity::getId(void){
	return id;
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
	timer->UpdateElapsed(m_world->getDeltaTime());
	if (timer->checkTimer(3.0f)){
		mIsAlive = false;
	}
	return;
}

// ============================== ENEMY ================================ //

Enemy::Enemy(void){
	return;
}

Enemy::~Enemy(void){
	return;
}

Enemy::Enemy(Game * world)
: Entity(world)
{
	mType = ET_ENEMY;
	mDirection = 0;
	return;
}

void Enemy::update(void){
	timer->UpdateElapsed(m_world->getDeltaTime());
	moveEnemy();
	return;
}


void Enemy::newDirection(void){

	std::vector<int> validMoves;
	float frameRate = m_world->getFrameRate();
	float DeltaTime = m_world->getDeltaTime();
	if (DeltaTime > 1.0f/frameRate){
		DeltaTime = 1.0f/frameRate;
	}

	float scale = DeltaTime * 100000.0f;
	if (scale > m_world->getScale()){
		scale =  m_world->getScale() - 1.0f;
	}
	if (collision->checkCollision(getX() + scale, getY(), ET_NONE)){
		validMoves.push_back(1);
	}
	if (collision->checkCollision(getX() - scale, getY(), ET_NONE)){
		validMoves.push_back(2);
	}
	if (collision->checkCollision(getX(), getY() + scale, ET_NONE)){
		validMoves.push_back(3);
	}
	if (collision->checkCollision(getX(), getY() - scale, ET_NONE)){
		validMoves.push_back(4);
	}

	srand(time(NULL));
	int iSecret = static_cast<int>((rand() % 4) + 1) ;  // rand() %  validMoves.size() + 1;
	// std::cout << "RANDOM MOVE DIRECTION: " << iSecret << std::endl;
	mDirection = iSecret;


	return;
}


void	Enemy::moveEnemy(){

	float frameRate = m_world->getFrameRate();
	float DeltaTime = m_world->getDeltaTime();
	if (DeltaTime > 1.0f/frameRate){
		DeltaTime = 1.0f/frameRate;
	}
	float scale = 0.05 * 100.0f;

	if (mDirection != 0){
		switch (mDirection){
			case 1:
				if (!collision->checkCollision(getX() + scale, getY(), ET_NONE)){
					mDirection = 0;
					break;
				}
				if (timer->checkTimer(0.05f)){
					setX(getX() + scale);
					timer->Reset();
				}
				break;
			case 2:
				if (!collision->checkCollision(getX() - scale, getY(), ET_NONE)){
					mDirection = 0;
					break;
				}
				if (timer->checkTimer(0.05f)){
					setX(getX() - scale);
					timer->Reset();
				}
				break;
			case 3:
				if (!collision->checkCollision(getX(), getY() + scale, ET_NONE)){
					mDirection = 0;
					break;
				}
				if (timer->checkTimer(0.05f)){
					setY(getY() + scale);
					timer->Reset();
				}
			
				break;
			case 4:
				if (!collision->checkCollision(getX(), getY() - scale, ET_NONE)){
					mDirection = 0;
					break;
				}
				if (timer->checkTimer(0.05f)){
					setY(getY() - scale);
					timer->Reset();
				}
				break;
		}
	}
	else {
		if (timer->checkTimer(0.2f)){
			newDirection();
			timer->Reset();
		}
		
	}
}
