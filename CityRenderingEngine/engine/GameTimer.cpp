#include "GameTimer.h"

GameTimer::GameTimer(int targetTps, void (*callbackFunction)(double)) {
	running = false;
	countTicks = 0;
	this->targetTps = targetTps;
	tickIntervalSum = 0;
	tickIntervalList = new double[targetTps];
	for (int i = 0; i < targetTps; i++) {
		tickIntervalList[i] = 0;
	}
	thread = NULL;
	this->callbackFunction = callbackFunction;
}

GameTimer::GameTimer(const GameTimer &copy) {
	this->countTicks = copy.countTicks;
	this->startTime = copy.startTime;
	this->lastTick = copy.lastTick;
	this->frequency = copy.frequency;
	this->targetTps = copy.targetTps;
	this->tickIntervalSum = copy.tickIntervalSum;
	this->tickIntervalList = new double[copy.targetTps];
	for (int i = 0; i < targetTps; i++) {
		this->tickIntervalList[i] = 0;
	}
	this->callbackFunction = copy.callbackFunction;
}


GameTimer::~GameTimer(void) {
	delete[] tickIntervalList;
}

GameTimer &GameTimer::operator=(const GameTimer &other) {
	this->countTicks = other.countTicks;
	this->startTime = other.startTime;
	this->lastTick = other.lastTick;
	this->frequency = other.frequency;
	this->targetTps = other.targetTps;
	this->tickIntervalSum = other.tickIntervalSum;
	return *this;
}

void GameTimer::startTimer() {
	LARGE_INTEGER freq;
	freq.QuadPart = (LONGLONG) frequency;
	LARGE_INTEGER start;
	start.QuadPart = (LONGLONG) startTime;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);
	frequency = (double) freq.QuadPart;
	startTime = (double) start.QuadPart;
	lastTick = startTime;
	running = true;
	thread = SDL_CreateThread(&GameTimer::run, "", this);
}

void GameTimer::stopTimer() {
	running = false;
}

void GameTimer::tick() {
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);
	double millisElapsed = getDeltaT();
	lastTick = (double) t.QuadPart;
	// Update the list of samples to calculate FPS
	tickIntervalSum -= tickIntervalList[countTicks % targetTps];
	tickIntervalSum += millisElapsed;
	tickIntervalList[countTicks % targetTps] = millisElapsed;
	countTicks++;
}

double GameTimer::getTime(double start, double frequency) {
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);
	return (double) ((t.QuadPart - start) * 1000.0f / frequency);
}

double GameTimer::getTotalTime() {
	return GameTimer::getTime(startTime, frequency);
}

double GameTimer::getDeltaT() {
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);
	return (double) ((t.QuadPart - lastTick) * 1000.0f / frequency);
}

int GameTimer::calculateTicksPerSecond() {
	return (int) (1 / ((tickIntervalSum / targetTps) / 1000.0f));
}

int GameTimer::run(void *parameter) {
	GameTimer *instance = (GameTimer*) parameter;
	// If this timer has a callback function, execute it when needed, if not, just completely skip this subthread
	if (instance->callbackFunction != NULL) {
		double targetMillis = 1000.0 / instance->targetTps;
		while (instance->running) {
			double millisElapsed = instance->getDeltaT();
			if (millisElapsed >= targetMillis) {
				instance->tick();
				// Call the callback function passing the millisElapsed
				(*(instance->callbackFunction))(millisElapsed);
			}
		}
	}
	return 0;
}