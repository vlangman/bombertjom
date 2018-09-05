#pragma once
#include <iostream>
#include "sdlLib.hpp"
#include "gameWorld.hpp"
#include "timer.hpp"
#include <vector>
#include <unistd.h>


class PlayerInputComponent;
class GraphicsComponent;
class CollisionComponent;
class TimerComponent;
class Entity;

class Game
{
	private:

		int				window_x;
		int				window_y;
		bool			m_shouldRun;
		bool 			verbose;
		SdlLib			sdl;
		Timer*			m_Timer;
		float 			frameRate = 100;
		GameWorld 		gameWorld;
		Entity 			*m_Player;
		double			scale;
		int				entityCount;

	public:
		std::vector<Entity*> entityList;
		std::vector<PlayerInputComponent*> inputHandlers;
		std::vector<GraphicsComponent*> renderList;
		std::vector<CollisionComponent*> colliderList;

		//canonical constructors
		Game(void);
		~Game(void);
		Game & operator=(const Game & _rhs);
		Game(const Game & _game);
	
		//getters and setters
		int getWindowX(void) const;
		int getWindowY(void) const;
		int getVerbose(void) const;
		double	getScale(void) const;
		int		newEntity(void);
		float	getFrameRate(void) const;

		//game loop functions
		int 	runLoop(void);
		void 	init(int _verbose = 0, int width = 500, int height = 500, bool fullscreen = 0);
		void 	handleEvents(void);
		void	closeGame(void);
		void	stepGame(float DeltaTime);
		void	cleanUp(void);

		//utility
		void	log(std::string message);
		float	getDeltaTime(void);
		void	buildGameObjects(void);
		void	addEntity(Entity *entity);
		
		

};