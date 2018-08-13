#pragma once
#include <iostream>
#include <SDL.h>

class Timer
{

private:
	static Timer *	sInstance;
	unsigned int 	mStartTicks;
	unsigned int 	mElapsedTicks;
	float			mDeltaTime;
	float			mTimeScale;

public:
	Timer();
	~Timer();

	static Timer *	Instance(void);
	static void		Release(void);

	void	Reset(void);
	float	DeltaTime(void);

	void	TimeScale(float t);
	float	getTimeScale(void);

	void	Update(void);

	
};