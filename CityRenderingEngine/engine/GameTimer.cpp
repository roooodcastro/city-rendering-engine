#include "GameTimer.h"

GameTimer *GameTimer::logicTimer = nullptr;
GameTimer *GameTimer::renderingTimer = nullptr;
GameTimer *GameTimer::physicsTimer = nullptr;

GameTimer::GameTimer(int targetTps, std::function<void(float)> callback) {
    running = false;
    tickCount = 0;
    this->targetTps = targetTps;
    tickIntervalSum = 0;
    tickIntervalList = new float[targetTps];
    tpsAverage = 0;
    for (int i = 0; i < targetTps; i++) {
        tickIntervalList[i] = 0;
    }
    thread = nullptr;
    this->callback = callback;
}

GameTimer::GameTimer(const GameTimer &copy) {
    this->tickCount = copy.tickCount;
    this->startTime = copy.startTime;
    this->lastTick = copy.lastTick;
    this->frequency = copy.frequency;
    this->targetTps = copy.targetTps;
    this->tickIntervalSum = copy.tickIntervalSum;
    this->tickIntervalList = new float[copy.targetTps];
    for (int i = 0; i < targetTps; i++)
        this->tickIntervalList[i] = copy.tickIntervalList[i];
    this->callback = copy.callback;
}


GameTimer::~GameTimer(void) {
    delete[] tickIntervalList;
}

GameTimer &GameTimer::operator=(const GameTimer &other) {
    this->tickCount = other.tickCount;
    this->startTime = other.startTime;
    this->lastTick = other.lastTick;
    this->frequency = other.frequency;
    this->targetTps = other.targetTps;
    this->tickIntervalSum = other.tickIntervalSum;
    delete[] tickIntervalList;
    this->tickIntervalList = new float[other.targetTps];
    for (int i = 0; i < targetTps; i++)
        this->tickIntervalList[i] = other.tickIntervalList[i];
    this->callback = other.callback;
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
    // Update the list of samples to calculate TPS
    tickIntervalSum -= tickIntervalList[tickCount % targetTps];
    tickIntervalSum += (float) millisElapsed;
    tickIntervalList[tickCount % targetTps] = (float) millisElapsed;
    tpsAverage = (float) (1 / ((tickIntervalSum / targetTps) / 1000.0f));
    tickCount++;
}

double GameTimer::getTime(double start, double frequency) {
    LARGE_INTEGER t;
    QueryPerformanceCounter(&t);
    return (double) ((t.QuadPart - start) * 1000.0f / frequency);
}

double GameTimer::getTotalTime() {
    return GameTimer::getTime(startTime, frequency);
}

float GameTimer::getDeltaT() {
    LARGE_INTEGER t;
    QueryPerformanceCounter(&t);
    return (float) ((t.QuadPart - lastTick) * 1000.0f / frequency);
}

int GameTimer::run(void *parameter) {
    GameTimer *instance = (GameTimer*) parameter;
    double targetMillis = 1000.0 / instance->targetTps;
    while (instance->running) {
        double millisElapsed = instance->getDeltaT();
        if (millisElapsed >= targetMillis) {
            instance->tick();
            // If there's a callback function, call it passing millisElapsed as parameter
            if (instance->callback) {
                instance->callback((float) millisElapsed);
            }
        }
    }
    return 0;
}