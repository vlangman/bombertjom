#pragma once
#include <iostream>
#include "sdlLib.hpp"
#include "timer.hpp"
#include "gameWorld.hpp"
#include <vector>


class PlayerInputComponent;
class GraphicsComponent;
class Entity;

class Game
{
	private:

		int		window_x;
		int		window_y;
		bool	m_shouldRun;
		bool 	verbose;
		SdlLib	sdl;
		Timer*	mTimer;
		const int frameRate = 60;
		GameWorld gameWorld;

	public:
		std::vector<Entity*> entityList;
		std::vector<PlayerInputComponent*> inputHandlers;
		std::vector<GraphicsComponent*> renderList;		

		//canonical constructors
		Game(void);
		~Game(void);
		Game & operator=(const Game & _rhs);
		Game(const Game & _game);
	
		//getters and setters
		int getWindowX(void) const;
		int getWindowY(void) const;
		int getVerbose(void) const;

		//game loop functions
		int 	runLoop(void);
		void 	init(int _verbose = 0, int width = 500, int height = 500, bool fullscreen = 0);
		
		void 	handleEvents(void);
		void	closeGame(void);
		
		void	addEntity(Entity *entity);
		// void	cleanup();

		//utility
		void log(std::string message);

};