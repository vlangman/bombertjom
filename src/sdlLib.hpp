#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>
#include "timer.hpp"

class GraphicsComponent;

enum E_COLOR 
{
	COLOR_YELLOW,
	COLOR_RED,
	COLOR_GREY
};

enum E_EVENT 
{
	EVENT_NONE,
	EVENT_CLOSE_WINDOW,
	EVENT_KEYBOARD_RIGHT,
	EVENT_KEYBOARD_LEFT,
	EVENT_KEYBOARD_UP,
	EVENT_KEYBAORD_DOWN,
	EVENT_KEYBOARD_1,
	EVENT_KEYBOARD_2,
	EVENT_KEYBOARD_3,
	EVENT_KEYBOARD_SPACEBAR,
};

class SdlLib {
	private:
		int 		window_x;
		int 		window_y;
		bool		verbose;

		SDL_Window *window;
		SDL_Renderer* renderer;
	public:
	/*
		default constructors and deconstructors for canonical form
	*/
		SdlLib(void);
		~SdlLib(void);
		SdlLib(const SdlLib & _sfml);
		SdlLib & operator=(const SdlLib & _sfml);

	/*
		custom constructors for Height an Width
	*/

		SdlLib(const int _W, const int _H);

	/* 
		VIRTUAL INHERITED ABSTRACT FUCNTIONS
	*/
		void init(int width, int height, bool _verbose);
		
		void draw(int x, int y, double width, double height, E_COLOR color);
		void draw(GraphicsComponent *graphics);

		void clearScreen();
		void displayScreen();
		E_EVENT handleEvents();

	/* 
		GETTER AND SETTERS
	*/
		int getWindowX(void) const;
		int getWindowY(void) const;

	//utility
		void log(std::string message);
		void drawFps(float fps);
};
