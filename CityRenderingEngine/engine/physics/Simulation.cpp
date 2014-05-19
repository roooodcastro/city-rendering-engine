#include "Simulation.h"
#include "WorldPartitioning.h"

const float Simulation::GRAVITY_EARTH = -9.78f;
const float Simulation::GRAVITY_MOON = -1.62f;
const float Simulation::GRAVITY_ZERO = 0.0f;

Simulation *Simulation::instance = NULL;

Simulation::Simulation(void) {
	running = false;
	paused = false;
	timer = new GameTimer(GameApp::TARGET_TPS, &Simulation::timerCallback);
	broadphaseDetector = new WorldPartitioning(4, 4, 4, Vector3(-100, -100, -100), Vector3(100, 100, 100));
}

Simulation::Simulation(const Simulation &copy) {
	running = copy.running;
	paused = copy.paused;
	timer = new GameTimer(*(copy.timer));
	broadphaseDetector = copy.broadphaseDetector;
}

Simulation::~Simulation(void) {
	delete timer;
	delete broadphaseDetector;
}

Simulation &Simulation::operator=(const Simulation &other) {
	running = other.running;
	paused = other.paused;
	*timer = *(other.timer);
	*broadphaseDetector = *(other.broadphaseDetector);
	return *this;
}

Simulation *Simulation::getInstance() {
	if (instance == NULL) {
		// Create if this is the first access
		instance = new Simulation();
	}
	return instance;
}

void Simulation::timerCallback(double millisElapsed) {
	Simulation *simulation = Simulation::getInstance();
	Level *level = GameApp::getInstance()->getCurrentLevel();
	if (level != NULL) {
		// Only try to simulate physics if there's an active level to get the entities from
		level->lockMutex();
		std::map<std::string, Entity*> *entities = level->getEntities();
		std::vector<PhysicalBody*> *physicalBodies = new std::vector<PhysicalBody*>();
		// Get all physical bodies from the current level
		unsigned numEntities = entities->size();
		for (unsigned i = 0; i < numEntities; i++) {
			if (i < entities->size()) {
				auto it = entities->begin();
				std::advance(it, i);
				int childIndex = 0;
				// Add all children entities as well
				std::vector<Entity*> children = Entity::getAllChildren((*it).second);
				for (unsigned i = 0; i < children.size(); i++) {
					physicalBodies->emplace_back(children[i]->getPhysicalBody());
				}
			}
		}
		if (!simulation->isPaused()) {
			// Update physics
			for (unsigned i = 0; i < physicalBodies->size(); i++) {
				// Calculate velocity, position, etc for each PhysicalBody;
				PhysicalBody *physicalBody = (*physicalBodies)[i];
				// If PhysicalBody is at rest, don't update its position
				if (!physicalBody->isAtRest((float) millisElapsed)) {
					physicalBody->update((float) millisElapsed);
				}
			}

			// Check for collisions
			simulation->getBroadphaseDetector()->performDetection(physicalBodies, (float) millisElapsed);
		}
		level->unlockMutex();
	}
}

void Simulation::startSimulation() {
	// Running will be set to false when the user exits the game
	running = true;
	paused = false;
	timer->startTimer();
}

void Simulation::stopSimulation() {
	running = false;
	timer->stopTimer();
	//SDL_WaitThread(timer->getThread(), NULL);
}

//void Simulation::addPhysicalBody(PhysicalBody *PhysicalBody) {
//	if (PhysicalBody) {
//		// If we don't have space to store the PhysicalBody, make some!
//		// I set this if to >= to always have an empty space in the array, just in case
//		if ((physicalBodies->size() + 1) >= physicalBodies->capacity()) {
//			physicalBodies->reserve(physicalBodies->capacity() + 5);
//		}
//		physicalBodies->emplace_back(PhysicalBody);
//	}
//}
//
//void Simulation::removePhysicalBody(PhysicalBody *PhysicalBody) {
//	physicalBodies->erase(std::remove(physicalBodies->begin(), physicalBodies->end(), PhysicalBody), physicalBodies->end());
//	// If vector becomes too big, shrink it
//	if (physicalBodies->size() + 10 < physicalBodies->capacity()) {
//		physicalBodies->shrink_to_fit();
//	}
//}