
/* timer.cpp
 *
 * Autor: Marko Gacesa
 * Datum: 09.06.2007.
 */

#include "timer.h"

long Timer::timer = 0;
long Timer::ticksPerSecond = 0;

void Timer::start()
{
	if (running) return;
	time = timer - time;
	running = true;
}

void Timer::stop()
{
	if (!running) return;
	running = false;
	time = timer - time;
}

float Timer::getTime() const
{
	if (running)
		return ((float)(timer - time) / (float)ticksPerSecond);
	else
		return ((float)(time) / (float)ticksPerSecond);
}

void Timer::addTime(float seconds)
{
	if (running)
		time -= (long)(ticksPerSecond * seconds);
	else
		time += (long)(ticksPerSecond * seconds);
}
