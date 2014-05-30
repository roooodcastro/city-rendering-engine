#include "ProfilingTimer.h"

ProfilingTimer::ProfilingTimer(int id) {
    this->id = id;
    this->cyclesTimeList = new float[60];
    configureTimer();
}

ProfilingTimer::ProfilingTimer(int id, int numCyclesToAverage) {
    this->id = id;
    this->numCyclesToAverage = numCyclesToAverage;
    this->cyclesTimeList = new float[numCyclesToAverage];
    configureTimer();
}

ProfilingTimer::~ProfilingTimer() {
    delete[] this->cyclesTimeList;
    this->cyclesTimeList = nullptr;
}

void ProfilingTimer::configureTimer() {
    LARGE_INTEGER freq;
    freq.QuadPart = (LONGLONG) frequency;
    LARGE_INTEGER start;
    start.QuadPart = (LONGLONG) startTime;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    frequency = (double) freq.QuadPart;
    startTime = (double) start.QuadPart;
    numMeasurements = 0;
    numCycles = 0;
    cyclesTimeSum = 0;
    cycleTimeAverage = 0;
    measurementTime = 0;
    for (int i = 0; i < numCyclesToAverage; i++) {
        cyclesTimeList[i] = 0;
    }
}

double ProfilingTimer::getTime() {
    LARGE_INTEGER t;
    QueryPerformanceCounter(&t);
    return (double) ((t.QuadPart - startTime) * 1000.0f / frequency);
}

void ProfilingTimer::resetCycle() {
    cyclesTimeSum -= cyclesTimeList[numCycles % numCyclesToAverage];
    cyclesTimeSum += measurementTime;
    cyclesTimeList[numCycles % numCyclesToAverage] = measurementTime;
    cycleTimeAverage = (cyclesTimeSum / max(1, min(numCyclesToAverage, numCycles)));
    numCycles++;
    numMeasurements = 0;
    measurementTime = 0;
    // Recalibrate the timer, necessary on multi-core processors
    LARGE_INTEGER freq;
    freq.QuadPart = (LONGLONG) frequency;
    LARGE_INTEGER start;
    start.QuadPart = (LONGLONG) startTime;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    frequency = (double) freq.QuadPart;
    startTime = (double) start.QuadPart;
}

void ProfilingTimer::addMeasure(float duration) {
    measurementTime += duration;
    numMeasurements++;
}

void ProfilingTimer::startMeasurement() {
    startCurrentMeasurement = getTime();
}

void ProfilingTimer::finishMeasurement() {
    // 0.005 is the time it takes to perform this measurement, we don't want this messing with the results.
    addMeasure((getTime() - startCurrentMeasurement) - 0.0005f);
}

void ProfilingTimer::setNumCyclesToAverage(int numCyclesToAverage) {
    // Updates the variable and store its old value
    int oldNumber = this->numCyclesToAverage;
    this->numCyclesToAverage = numCyclesToAverage;
    // Creates a temporary list with the old measurement values
    float *tempList = new float[oldNumber];
    for (int i = 0; i < oldNumber; i++) {
        tempList[i] = cyclesTimeList[i];
    }
    // Update the official list
    delete[] cyclesTimeList;
    cyclesTimeList = new float[numCyclesToAverage];
    for (int i = 0; i < numCyclesToAverage; i++) {
        cyclesTimeList[i] = 0;
    }
    // Transfer the old measurements from the temp list to the updated list
    int minNumber = min(oldNumber, numCyclesToAverage);
    for (int j = minNumber; j > 0; j--) {
        cyclesTimeList[j] = tempList[j];
    }
    delete[] tempList;
}

float ProfilingTimer::getLastCycleTime() {
    return cyclesTimeList[(numCycles - 1) % numCyclesToAverage];
}