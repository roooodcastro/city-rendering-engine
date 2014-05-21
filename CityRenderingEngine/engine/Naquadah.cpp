#include "Naquadah.h"

Naquadah *Naquadah::instance = nullptr;
int Naquadah::TARGET_FPS = 60;

Naquadah::Naquadah(void) {
    gameRunning = false;
    currentScene = nullptr;
    nextScene = nullptr;
    profiling = false;
    renderer = nullptr;
    simulation = nullptr;
}

Naquadah::~Naquadah(void) {
    if (currentScene != nullptr) {
        delete currentScene;
        currentScene = nullptr;
    }
    if (nextScene != nullptr) {
        delete nextScene;
        nextScene = nullptr;
    }
}

Naquadah *Naquadah::getInstance() {
    return instance;
}

void Naquadah::initialize(int initModules) {
    if (instance == nullptr) {
        instance = new Naquadah();
    }
    bool initEverything = ((initModules >> 31) > 0);
    if ((initModules >> 1) > 0 || initEverything) {
        // Init Physics
        instance->simulation = new Simulation();
        initModules -= 2; // Remove physics bit from the code
    }
    if (initModules > 0 || initEverything) {
        // Init Graphics
        instance->renderer = new Renderer();
    }
    // Init only core SDL features
    SDL_Init(SDL_INIT_EVERYTHING);

    ResourcesManager::initialize();
    // We initialize the primitive meshes that will be used by the interface
    Model::initializePrimitiveMeshes();
}

void Naquadah::runGame() {
    gameRunning = true;
    installTimers(); // Start game timers
    while(gameRunning) {
        handleUserEvents();
    }
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Naquadah::handleUserEvents() {

}

void Naquadah::updateLogic(float millisElapsed) {
    // Switch levels if requested
    if (nextScene) {
        if (currentScene) {
            currentScene->onFinish();
            delete currentScene;
        }
        this->currentScene = nextScene;
        this->currentScene->onStart();
        nextScene = nullptr;
    }
    if (currentScene) {
        //currentScene->update(millisElapsed);
    }
}

void Naquadah::updatePhysics(float millisElapsed) {
    // If we have the simulation, update the physics
    if (simulation != nullptr) {
        //simulation->update(millisElapsed);
    }
}

void Naquadah::render(float millisElapsed) {
    // If we have the renderer, render the scene
    if (renderer != nullptr) {
        renderer->render(millisElapsed);
    }
}

void Naquadah::installTimers() {
    // All timers are initialized and keep running at all times, even if some of the modules isn't loaded
    GameTimer::logicTimer = new GameTimer(TARGET_FPS, std::bind(&Naquadah::updateLogic, this, std::placeholders::_1));
    GameTimer::physicsTimer = new GameTimer(TARGET_FPS, std::bind(&Naquadah::updatePhysics, this, std::placeholders::_1));
    GameTimer::renderingTimer = new GameTimer(TARGET_FPS, std::bind(&Naquadah::render, this, std::placeholders::_1));
}

Vector2 Naquadah::getWindowSize() {
    if (instance != nullptr && instance->renderer != nullptr) {
        return instance->renderer->getWindowSize();
    }
    return Vector2();
}