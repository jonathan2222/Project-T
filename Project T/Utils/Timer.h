#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer
{
public:
	Timer();
	virtual ~Timer();

	void restart();
	void update();

	float getTime();
	float getDeltaTime();

private:
	float dt;
	std::chrono::time_point<std::chrono::steady_clock> startTime;
	std::chrono::time_point<std::chrono::steady_clock> preTime;
	std::chrono::time_point<std::chrono::steady_clock> currTime;
};

#endif
