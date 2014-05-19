/*
 * Author: Rodrigo Castro Azevedo
 *
 * Description: This class keeps track of time. When the simulation is started, the current clock time is stored, and
 * for every frame, the time at beggining is stored to calculate the next frame's deltaT.
 */

#pragma once

#include <SDL_thread.h>
#include <iostream>
#include "Windows.h"

class GameTimer {
public:

	GameTimer(int targetTps, void (*callbackFunction)(double));
	GameTimer(const GameTimer &copy);
	~GameTimer(void);

	GameTimer &operator=(const GameTimer &other);

	/* Starts the timer, setting startTime to the current time. Start time will then be used to calculate all times in the simulation */
	void startTimer();

	/* Stops the timer, setting the variable running to false */
	void stopTimer();

	/* The actual timer run function. This will be called inside a thread started on the startTimer method */
	static int run(void *parameter);

	/*
	 * Ticks the timer, setting lastTick to the current time. This is used to calculate the deltaT
	 * between the beggining of a frame to the beggining of the previous frame
	 */
	void tick();

	double getStartTime() { return startTime; }
	double getLastTick() { return lastTick; }
	/* Returns the amount of time passed between the start of the timer to the current time */
	double getTotalTime();
	static double getTime(double start, double frequency);
	int getTickCount() { return countTicks; }
	/* Returns the amount of time, in ms passed since the last tick */
	double getDeltaT();
	/* Calculates the average FPS based on the last numTicksToCalc ticks duration */
	int calculateTicksPerSecond();
	SDL_Thread *getThread() { return thread; }

protected:

	/* The total number of ticks since the beggining of the timer */
	int countTicks;
	/* Timestamp of the start of the simulation */
	double startTime;
	/* Internal system variable to determine CPU clock */
	double frequency;
	/* Time between the beggining of last tick to the start of simulation */
	double lastTick;
	/* A list containing the time to render the last 60 frames */
	double *tickIntervalList;
	/* The sum of the last 60 frames intervals */
	double tickIntervalSum;
	/* The function that will be called when this timer ticks */
	void (*callbackFunction)(double);
	/* Whether this timer is running or not */
	bool running;
	/* The thread that will be running on this timer */
	SDL_Thread *thread;
	/* The number of ticks that are expected to be processed each second. Defaults to 60. */
	int targetTps;

};