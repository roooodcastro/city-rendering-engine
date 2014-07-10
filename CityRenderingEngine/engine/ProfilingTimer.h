/*
 * Author: Rodrigo Castro Azevedo
 * Date: 30/05/2014
 *
 * Description: This class is a Timer much like GameTimer, but instead of providing the engine with reliable fixed time
 * steps to perform logic and render updates, its main purpose here is to simple measure the time it takes to do stuff.
 *
 * It works by having an accumulator that will sum all the time being measured until it's reset to the next cycle, when
 * the measurements will start over again. Usually each cycle will correspond to each frame being measured.
 *
 * The time returned by the methods getTime(), getAverageTime() and getLastCycleTime() are in milliseconds.
 */

#pragma once

#include <SDL_thread.h>
#include <iostream>
#include "Windows.h"

class ProfilingTimer {
public:

    /* The unique identifier for this ProfilingTimer. This is used by Profiler to find this timer. */
    int id;

    ProfilingTimer(int id);
    ProfilingTimer(int id, int numCyclesToAverage);
    ~ProfilingTimer(void);

    /* Reset the measurement and start a new cycle. */
    void resetCycle();

    /* Add duration to the current cycle. */
    void addMeasure(float duration);

    /*
     * Start a new measurement for this cycle. This timer will store the timestamp of this moment, and when the funcion
     * finishMeasurement() is called, will calculate the deltaT between the two calls, effectively calculating the time
     * it took to run all lines of code between these two calls. If this funcion is called again while a measurement is
     * in place, the previous measurement will be discarded and a new one will begin from this moment.
     */
    void startMeasurement();

    /*
     * Finish the current measurement that was started with the funcion startMeasurement(), calculates and adds the
     * deltaT of this measurement to the total measurement of this cycle.
     */
    void finishMeasurement();

    /* Gets the current timestamp, relative to the start of this cycle. */
    double getTime();

    /* Returns the amount of time that was measured since the last reset. */
    float getMeasuredTime() { return measurementTime; }

    /* Returns the average duration of the last numCyclesToAverage cycles. */
    float getAverageTime() { return cycleTimeAverage; }

    /* Returns the total number of cycles measured by this timer. */
    int getNumCycles() { return numCycles; }

    /* Returns the number of measurements already taken in the current cycle. */
    int getNumMeasurementsCycle() { return numMeasurements; }

    /* Returns the total duration of the last cycle. */
    float getLastCycleTime();

    /*
     * Changes the number of cycles used to average the measurement results. If the new number is smaller than the
     * current, the oldest cycles will be discarded, but if it's bigger, the oldest N cycles will be assigned the same
     * time as the current oldest cycle, with N being the difference between the new number and the current one.
     */
    void setNumCyclesToAverage(int numCyclesToAverage);

protected:

    /* Pools information from the CPU to enable a correct and precise time measurement. */
    void configureTimer();

    /* The total number of measurements that were performed since the last time it was reset. */
    int numMeasurements;

    /* The number of previous cycles that should be used to average the measurement results. Defaults to 60. */
    int numCyclesToAverage;

    /* The total number os cycles that were calculated since the creation of this Timer. */
    int numCycles;

    /* The start time of the current measurement. This is set on the function startMeasurement(). */
    float startCurrentMeasurement;

    /* The total time that was measured since the last time it was reset. */
    float measurementTime;

    /* An array containing the duration of each of the last numCyclesToAverage cycles. */
    float *cyclesTimeList;

    /* The sum of the total duration of the last numCyclesToAverage cycles. */
    float cyclesTimeSum;

    /* The average duration of the last numCyclesToAverage cycles. */
    float cycleTimeAverage;

    /* Timestamp of the start of this cycle. */
    double startTime;

    /* Internal system variable to determine CPU clock. */
    double frequency;
};