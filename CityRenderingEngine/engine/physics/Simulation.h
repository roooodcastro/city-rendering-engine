/*
 * Author: Rodrigo Castro Azevedo
 *
 * Description: This class is the main physics simulation class. It dictates the rhythm of the physics engine.
 * It stores a vector of all physical bodies that will be subject to physics and collision detection,
 * and for each frame iterates over it to process everything.
 *
 * The position and velocity calculations are done using Verlet integration.
 */

#pragma once

#include <vector>
#include "GameApp.h"
#include "GameTimer.h"
#include "PhysicalBody.h"

class BroadphaseDetector;
class PhysicalBody;

class Simulation {
public:

	~Simulation(void);
	Simulation &operator=(const Simulation &other);

	static Simulation *getInstance();

	/* Resets the simulation, deleting it */
	static void resetSimulation() { delete instance; instance = NULL; }

	/* Starts the simulation */
	void startSimulation();
	/* Stops the simulation */
	void stopSimulation();

	GameTimer *getTimer() { return timer; }
	void setBroadphaseDetector(BroadphaseDetector *broadphaseDetector) { this->broadphaseDetector = broadphaseDetector; }
	BroadphaseDetector *getBroadphaseDetector() { return broadphaseDetector; }
	void setPaused(bool paused) { this->paused = paused; }
	bool isPaused() { return paused; }

	/* Sets the gravity value for all entities */
	void setGravity(float gravity) { this->gravity = gravity; }
	float getGravity() { return gravity; }

	static const float GRAVITY_EARTH;
	static const float GRAVITY_MOON;
	static const float GRAVITY_ZERO;

protected:

	/* Protected constructors, as this is a singleton class */
	Simulation(void);
	Simulation(const Simulation &copy);

	/* This is the function that the timer will call every tick, to update the simulation */
	static void timerCallback(double millisElapsed);

	/* Game timer controlling the physics */
	GameTimer *timer;
	/* Broadphase detector, to partition the world and reduce number of collision checks */
	BroadphaseDetector *broadphaseDetector;
	/* The gravity acceleration for this simulation. Default: 0. Earth's gravity: -9.78m/s*s */
	float gravity;

	/* Simulation control flags */
	bool running;
	bool paused;

	/* The singleton instance of this class */
	static Simulation *instance;
};