/*
 * Author: Rodrigo Castro Azevedo
 * Date: 20/05/2014
 *
 * Description: This class keeps track of time. When the timer is started, the current clock time is stored, and on
 * every frame, the time at beggining is stored to calculate the next frame's deltaT. It's used by the engine to manage
 * the update, render and physics update framerates, independently of each other.
 *
 * This class allows a callback function to be assigned to it, so everytime the timer ticks the callback will be
 * called. This function must have one float parameter, that tells the number of milliseconds that passed since the
 * last tick. If the callback function is not present, the GameTimer will only measure time, returning its readings
 * when requested.
 *
 * When a GameTimer is started, a new thread is started to run the timer, and the callback function, if present. The
 * timer will run asynchronously on a separate thread from the main program, killing the thread once it has stopped.
 * If this threading capability is not desired, there is a variable called threading that can be set to false, before
 * calling startTimer, that will disable this. The timer will then run on the same thread as the startTimer call was
 * made, and will only exit once it stops. If the timer is started with threading set to false and no callback function
 * is assigned, it won't do anything until one of its functions is called from outside.

 * Naquadah works with 3 GameTimers, one for each main loops that it has: game logic, graphics rendering and physics
 * simulation. This class holds each of these 3 timers as static variables, to be used by any part of the engine at any
 * time. For example, the deltaT of the last physics update could be accessed from the game logic loop.
 */

#pragma once

#include <SDL_thread.h>
#include <iostream>
#include <functional>
#include "Windows.h"

class GameTimer {
public:

    static GameTimer *logicTimer;
    static GameTimer *renderingTimer;
    static GameTimer *physicsTimer;

    /*
     * Creates a new GameTimer that when started will measure time and tick targetTps times each second. A tick occurs
     * each 1000/targetTps milliseconds, and can be compared to a game "frame". The callback parameter is a function
     * that can be set to be called on every tick, so if there is a callback function present, it will be called
     * targetTps times per second. If the execution of this function exceeds the interval between ticks, the number of
     * ticks per seconds (TPS) will drop, and the GameTimer will try to keep it as close as possible to the targetTps.
     */
    GameTimer(int targetTps, void (*callbackFunction)(float));
    GameTimer(const GameTimer &copy);
    ~GameTimer(void);

    GameTimer &operator=(const GameTimer &other);

    /*
     * Starts the timer, setting startTime to the current time. startTime will then be used as a base to calculate all
     * times of the timer.
     */
    void startTimer();

    /* Stops the timer, setting the variable running to false. */
    void stopTimer();

    /*
     * The actual timer run function. This will be called inside a thread started on the startTimer method. This should
     * be called only by the GameTimer class, inside a separate thread. parameter is the GameTimer instance that will
     * be ran. This function serves to time the execution of the timer, ticking it when needed, and calling the
     * callback function if present.
     */
    static int run(void *parameter);

    /*
     * Ticks the timer, setting lastTick to the current time. This is used to calculate the deltaT between the
     * beggining of a frame to the beggining of the previous frame. This function will also update the calculated TPS
     * value.
     */
    void tick();

    /* Returns the timestamp, in milliseconds, on the moment that this timer was started. */
    double getStartTime() { return startTime; }

    /* Returns the timestamp, in milliseconds, on the moment that the last tick happened. */
    double getLastTick() { return lastTick; }

    /* Returns the amount of time passed between the start of the timer to the current time. */
    double getTotalTime();

    /*
     * Returns the amount of time, in milliseconds, passed since start, calculated using the provided processor
     * frequency.
     */
    static double getTime(double start, double frequency);

    /* Returns the number of ticks processed since the start of this timer. */
    int getTickCount() { return tickCount; }

    /* Returns the amount of time, in milliseconds, passed since the last tick. */
    float getDeltaT();

    /* Returns the amount of time, in milliseconds passed between the start and end of the last tick. */
    float getLastDeltaT() { return (float) lastTickDeltaT; }

    /* Returns the average TPS over the last second. */
    float getTicksPerSecond() { return tpsAverage; }

    /* Returns the SDL_Thread used to run this timer. Should be used with care. */
    SDL_Thread *getThread() { return thread; }

    /* Turns the threading calability on or off. */
    void setThreading(bool threading) {
        this->threading = threading;
    }
    
    /* Returns true if this timer have threading capabilities, or false otherwise. */
    bool haveThreading() { return threading; }

protected:

    /* The total number of ticks since the beggining of the timer. */
    int tickCount;

    /* Timestamp of the start of the simulation. */
    double startTime;

    /* Internal system variable to determine CPU clock. */
    double frequency;

    /* Time between the beggining of last tick to the start of simulation. */
    double lastTick;

    /* Duration of the last tick, in milliseconds. */
    double lastTickDeltaT;

    /* Variables to help calculate a smooth ticks per second number, averaging it for one second. */
    float *tickIntervalList; // A list containing the running time of the last 60 ticks.
    float tickIntervalSum; // The sum of the last 60 ticks intervals.
    float tpsAverage; // The TPS averaged over the last second.

    /* The function that will be called when this timer ticks. */
    void (*callback)(float);

    /* Whether this timer is running or not. */
    bool running;

    /* Set this to false if this timer shouldn't run on a new thread. Defaults to true. */
    bool threading;

    /* The thread that will be running on this timer. */
    SDL_Thread *thread;

    /* The number of ticks that are expected to be processed each second. Defaults to 60. */
    int targetTps;
};