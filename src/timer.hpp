#pragma once
#include <iostream>
#include <SDL.h>

class Timer
{

private:
	static Timer *	sInstance;
	unsigned int 	mStartTicks;
	float			mDeltaTime;

public:
	Timer();
	~Timer();

	static Timer *	Instance(void);
	static void		Release(void);

	void	Reset(void);
	float	DeltaTime(void);
	void	Update(void);

	
};