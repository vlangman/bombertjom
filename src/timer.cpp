/*#include "timer.hpp"

Timer *Timer::sInstance = NULL;

Timer *Timer::Instance(void){
	if (sInstance == NULL){
		sInstance = new Timer();
	}
	return sInstance;
}

void Timer::Release(void){
	delete sInstance;
	sInstance = NULL;
}

Timer::Timer(void){
	Reset();
	return;
}

Timer::~Timer(void){
	return;
}

void Timer::Reset(void) {
	mStartTicks = SDL_GetTicks();
	mDeltaTime = 0.0f;
}

float Timer::DeltaTime(void){
	return mDeltaTime;
}

void Timer::Update(void){
	//ticks in miliseconds (/1000 for microsecs)
	mDeltaTime = (SDL_GetTicks() - mStartTicks) * 0.001f;
	if (mDeltaTime < 0.001f)
		mDeltaTime = 0.001f;
}*/