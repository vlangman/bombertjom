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
	lives = 3;
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
	float distance = velocity * DeltaTime;

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

	double scale = static_cast<double>(m_world->getScale());
	double x = getX()/scale;
	double y = getY()/scale;
	double Mx = static_cast<double>(nearbyint(x) * scale);
	double My = static_cast<double>(nearbyint(y) * scale);
	Builder * builder = new Builder(m_world);
	builder->createBomb(Mx,My, m_world->getScale(), m_world->getScale());
	delete builder;

	return;
}


void 	Player::killPlayer(void){
	if (lives == 0) {
		std::cout << "GAME OVER!" << std::endl;
	}
	lives--;
	setX(m_world->getPlayerStartX());
	setY(m_world->getPlayerStartY());
	setDirection(0,0);
}


// =============================== WALL =============================== //
void Wall::update() 
{

}

Wall::Wall(Game *world, bool canDestroy)
:Entity(world)
{
	mIsDestroyable = canDestroy;
	mType = ET_WALL;
}

void Wall::playerStartBlock(void){
	mType = ET_IWALL;
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

void Entity::kill(void){
	mIsAlive = false;
}

bool Entity::isDestroyable(void){
	return mIsDestroyable;
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
Bomb::Bomb(Game *world, int raduis) : Entity(world)
{
	mIsDestroyable = true;
	mRaduis = raduis;
	mType = ET_BOMB;
}

//BOMB FUNCTIONS

void	Bomb::update(void){
	timer->UpdateElapsed(m_world->getDeltaTime());
	if (timer->checkTimer(3.0f)){
		detonate();
		timer->Reset();
	}
	return;
}

void   Bomb::detonate(void){

	float scale = m_world->getScale();

	bool north,east ,south, west;
	east = west = south = north = true;

	for (int i = 1; i <= mRaduis; i++){
		double dist = static_cast<double>(i*scale);
		if (north){
			collision->lethalCollision(getX(),getY()-dist, ET_BOMB);
			north = false;
		}
		if (south){
			collision->lethalCollision(getX(),getY()+dist, ET_BOMB);
			south = false;
		}
		if (east){
			collision->lethalCollision(getX() - dist,getY(), ET_BOMB);
			east = false;
		}
		if (west){
			collision->lethalCollision(getX() + dist,getY(), ET_BOMB);
			west = false;
		}
	}
	mIsAlive = false;
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
	mIsDestroyable = true;
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

	float DeltaTime = m_world->getDeltaTime();
	float frameRate = m_world->getFrameRate();
	float velocity = 3.0f * m_world->getScale();
	float distance = velocity * DeltaTime;

	if (mDirection != 0){
		switch (mDirection){
			case 1:
				if (!collision->checkCollision(getX() + distance, getY(), ET_NONE)){
					mDirection = 0;
					break;
				}
				setX(getX() + distance);
				break;
			case 2:
				if (!collision->checkCollision(getX() - distance, getY(), ET_NONE)){
					mDirection = 0;
					break;
				}		
				setX(getX() - distance);		
				break;
			case 3:
				if (!collision->checkCollision(getX(), getY() + distance, ET_NONE)){
					mDirection = 0;
					break;
				}
				setY(getY() + distance);
				break;
			case 4:
				if (!collision->checkCollision(getX(), getY() - distance, ET_NONE)){
					mDirection = 0;
					break;
				}
				setY(getY() - distance);
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


// ============================== POWERUP ================================ //

PowerUp::PowerUp(void){
	return;
};
PowerUp::~PowerUp(void){
	return;
};

PowerUp::PowerUp(Game *world)
: Entity(world)
{
	mIsDestroyable = true;
	return;
};


void PowerUp::update(void){

};

void PowerUp::activatePowerUp(void){

};