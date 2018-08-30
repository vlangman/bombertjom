// #pragma once
// #include <iostream>
// #include <SDL.h>

// class Timer
// {

// private:
// 	static Timer *	sInstance;
// 	unsigned int 	mStartTicks;
// 	float			mDeltaTime;

// public:
// 	Timer();
// 	~Timer();

// 	static Timer *	Instance(void);
// 	static void		Release(void);

// 	void	Reset(void);
// 	float	DeltaTime(void);
// 	void	update(void);

	
// };



//FUCKEN HIGH RESOLUTION MUTHERFUCKING CLOCK SUN
#pragma once
#include <ctime>
#include <ratio>
#include <chrono>
#include <iostream>

class Timer
{

private:
	static Timer *	sInstance;
	float			mDeltaTime;

	std::chrono::steady_clock::time_point mStartTicks;

public:
	Timer();
	~Timer();

	static Timer *	Instance(void);
	static void		Release(void);

	void	Reset(void);
	float	DeltaTime(void);
	void	update(void);

	
};