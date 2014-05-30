/*
 * Author: Rodrigo Castro Azevedo
 * Date: 30/05/2014
 *
 * Description: This class is intended to be used for debugging and profiling purposes, mainly to calculate and detect
 * possible bottlenecks in the engine and game. It should be used only during develpment, and removed completely from
 * the game for release. This class is instance-less, and all of its methods and variables are static.
 *
 * It contains a set of tools to measure performance and keep track of errors and warnings that might happen during
 * runtime. The main performance measurement tool is the ProfilingTimer class, which provides an easy and useful way to
 * measure the time it takes to perform certain operation, and it's able to average this duration over a certain period
 * of time.
 */

#pragma once

#include <map>
#include <string>
#include <algorithm>
#include "ProfilingTimer.h"

class Profiler {
public:

    /*
     * Returns a ProfilingTimer with the provided id. If Profiler is not initialized or the requested timer does not
     * exist, this function will return null.
     */
    static ProfilingTimer *getTimer(int id);

    /*
     * Adds a new ProfilingTimer to Profiler. If the timer is null or have an id that's already in Profiler, the new
     * timer will not be added.
     */
    static void addProfilingTimer(ProfilingTimer *timer);

    /* Removes the timer with id from the Profiler timers. */
    static void removeProfilingTimer(int id);

    /* Starts the Profiler, allocating the necessary memory. */
    static void startProfiler();

    /* Stops and closes the Profiler. All of its data is destroyed and the memory released. */
    static void stopProfiler();

protected:

    Profiler(void) {}
    ~Profiler(void) {}

    /* Indicated if the Profiler is currently initialized. */
    static bool initialized;

    static std::map<int, ProfilingTimer*> *profilingTimers;
};