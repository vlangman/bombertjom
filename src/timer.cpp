// #include "timer.hpp"

// Timer *Timer::sInstance = NULL;

// Timer *Timer::Instance(void){
// 	if (sInstance == NULL){
// 		sInstance = new Timer();
// 	}
// 	return sInstance;
// }

// void Timer::Release(void){
// 	delete sInstance;
// 	sInstance = NULL;
// }

// Timer::Timer(void){
// 	Reset();
// 	return;
// }

// Timer::~Timer(void){
// 	return;
// }

// void Timer::Reset(void) {
// 	mStartTicks = SDL_GetTicks();
// 	mDeltaTime = 0.0f;
// }

// float Timer::DeltaTime(void){
// 	return mDeltaTime;
// }

// void Timer::update(void){
// 	//ticks in miliseconds (/1000 for microsecs)
// 	mDeltaTime = (SDL_GetTicks() - mStartTicks) * 0.001f;
// 	if (mDeltaTime < 0.001f)
// 		mDeltaTime = 0.001f;
// }



//FUCKEN HIGH RESOLUTION MUTHERFUCKING CLOCK SUN

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
	return;
}

Timer::~Timer(void){
	return;
}

void Timer::Reset(void) {
	mStartTicks = std::chrono::steady_clock::now();
	mDeltaTime = 0.0f;
}

float Timer::DeltaTime(void){
	return mDeltaTime;
}

void Timer::update(void){
	std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
	std::chrono::duration<float> time = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - mStartTicks);
	mDeltaTime = time.count();
}

