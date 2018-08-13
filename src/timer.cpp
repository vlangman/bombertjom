#include "timer.hpp"

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
	mTimeScale = 1.0f;
	return;
}

Timer::~Timer(void){
	return;
}

void Timer::Reset(void){
	mStartTicks = SDL_GetTicks();
	mElapsedTicks = 0;
	mDeltaTime = 0.0f;
}

float Timer::DeltaTime(void){
	return mDeltaTime;
}

void Timer::TimeScale(float t){
	mTimeScale = t;
}

float Timer::getTimeScale(void){
	return mTimeScale;
}

void Timer::Update(void){
	mElapsedTicks = SDL_GetTicks() - mStartTicks;
	//ticks in miliseconds
	mDeltaTime = mElapsedTicks * 0.001f;
}