
/* timer.cpp
 *
 * Autor: Marko Gaćeša
 * Datum: 09.06.2007.
 * Datum: 05.05.2026.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
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
		return (static_cast<float>(timer - time) / static_cast<float>(ticksPerSecond));
	else
		return (static_cast<float>(time) / static_cast<float>(ticksPerSecond));
}

void Timer::addTime(const float seconds)
{
	if (running)
		time -= static_cast<long>(ticksPerSecond * seconds);
	else
		time += static_cast<long>(ticksPerSecond * seconds);
}
