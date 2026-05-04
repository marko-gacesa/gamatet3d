
/* timer.h
 *
 * Autor: Marko Gacesa
 * Datum: 09.06.2007.
 */

#ifndef _TIMER_H_
#define _TIMER_H_

class Timer
{
private:
	static long timer;
	static long ticksPerSecond;

	// Ako je Timer startovan: time sadrzi trenutak kada je Timer startovan.
	// Ako timer nije startovan: time sadrzi vreme koliko je Timer radio.

	long time;
	bool running;

public:
	Timer() : time(0L), running(false) {}

	bool isRunning() const { return running; }

	void reset() { if (running) time = timer; else time = 0L; }

	void start();
	void stop();

	float getTime() const; // u sekundama
	void addTime(float seconds);

	static void setPeriod(long milisec) { ticksPerSecond = milisec; } // vreme izmedju dva poziva progress()
	static void progress() { timer++; }
};


#endif
