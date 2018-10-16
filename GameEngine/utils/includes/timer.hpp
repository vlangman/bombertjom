//FUCKEN HIGH RESOLUTION MUTHERFUCKING CLOCK SUN
#pragma once
#include <ctime>
#include <ratio>
#include <chrono>
#include <iostream>

namespace  GameBoi{
	class Timer
	{

	private:
		static Timer *  sInstance;
		float			mDeltaTime;
		float			mElapsedTime;

		std::chrono::steady_clock::time_point mStartTicks;

	public:
		Timer();
		~Timer();

		static Timer *  Instance(void);
		static void     Release(void);

		void    Reset(void);
		float   DeltaTime(void);
		void    update(void);

	};
}
