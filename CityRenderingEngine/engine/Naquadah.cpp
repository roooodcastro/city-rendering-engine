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

void Naquadah::initialize(unsigned int initModules) {
    if (instance == nullptr) {
        instance = new Naquadah();
    }
    ResourcesManager::initialize();
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
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_EVENTS);
    // Init keyboard and mouse managers
    Keyboard::getInstance();
    Mouse::getInstance();
}

void Naquadah::runGame() {
    gameRunning = true;

    installTimers(); // Start game timers
    while(gameRunning) {
        handleUserEvents();
        SDL_Delay(1);
    }
    GameTimer::logicTimer->stopTimer();
    GameTimer::physicsTimer->stopTimer();
    GameTimer::renderingTimer->stopTimer();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Naquadah::handleUserEvents() {
    SDL_Event e;
    bool rendered = false;
    while (SDL_PollEvent(&e)){
        switch (e.type) {
        case SDL_USEREVENT:
            if (e.user.code == USER_EVENT_RENDER && !rendered) {
                rendered = true;
                render(GameTimer::renderingTimer->getLastDeltaT());
            }
            break;
        case SDL_MOUSEMOTION:
            Mouse::recordMouseMove(e.motion);
            if (currentScene)
                currentScene->onMouseMoved(Vector2((float) e.motion.x, (float) e.motion.y),
                Vector2((float) e.motion.xrel, (float) e.motion.yrel));
            break;
        case SDL_MOUSEBUTTONDOWN:
            Mouse::recordMouseButton(e.button);
            if (currentScene)
                currentScene->onMouseButtonDown(e.button.button, Vector2((float) e.button.x, (float) e.button.y));
            break;
        case SDL_MOUSEBUTTONUP:
            Mouse::recordMouseButton(e.button);
            if (currentScene) {
                currentScene->onMouseButtonUp(e.button.button, Vector2((float) e.button.x, (float) e.button.y));
                /*if (e.button.clicks == 1)
                    currentScene->onMouseClick(e.button.button, Vector2((float) e.button.x, (float) e.button.y));
                else if (e.button.clicks == 2) {
                    currentScene->onMouseDoubleClick(e.button.button, Vector2((float) e.button.x, (float) e.button.y));
                }*/
            }
            break;
            case SDL_MOUSEWHEEL:
                if (currentScene) {
                    currentScene->onMouseWheelScroll(e.wheel.y);
                }
                break;
        case SDL_KEYDOWN:
            Keyboard::recordKey(e.key);
            if (currentScene) {
                currentScene->onKeyDown(e.key.keysym);
            }
            break;
        case SDL_KEYUP:
            Keyboard::recordKey(e.key);
            if (currentScene) {
                currentScene->onKeyUp(e.key.keysym);
                currentScene->onKeyPress(e.key.keysym);
            }
            break;
        case SDL_JOYAXISMOTION:
            if (e.jaxis.which == 0) {
                // X axis motion
                if (e.jaxis.axis == 0) {
                    // Fire axis motion event
                }
            }
            break;
        case SDL_JOYBUTTONDOWN:
            // Fire buttond own event
            break;
        case SDL_JOYBUTTONUP:
            // Fire button up event
            break;	
        case SDL_JOYDEVICEADDED:
            // Bind the joystick to the game
            break;
        case SDL_JOYDEVICEREMOVED:
            // Unbind the joystick and warn player
            break;
        case SDL_QUIT:
            gameRunning = false;
            break;
        case SDL_WINDOWEVENT:
            // Checks if user is still controlling the Game Window. If not, then automatically pauses the game.
            // A list of all possible window events can be found here: https://wiki.libsdl.org/SDL_WindowEvent
            switch (e.window.event) {
            case SDL_WINDOWEVENT_FOCUS_LOST:
                //SDL_Log("Window %d lost keyboard focus", e.window.windowID);
                //setGamePaused(true);
                break;
            }
            break;
        }
    }
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
        currentScene->update(millisElapsed);
    }
    //std::cout << GameTimer::logicTimer->getTicksPerSecond() << " TPS, " << GameTimer::renderingTimer->getTicksPerSecond() << " FPS" << std::endl;
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
        renderer->render(currentScene, millisElapsed);
    }
}

/* Callback function created to be called by the logic timer, and call the update logic function of Naquadah. */
void updateLogicTimerCallback(float millisElapsed) {
    Naquadah::getInstance()->updateLogic(millisElapsed);
}

/* Callback function created to be called by the physics timer, and call the update physics function of Naquadah. */
void updatePhysicsTimerCallback(float millisElapsed) {
    Naquadah::getInstance()->updatePhysics(millisElapsed);
}

/*
 * Callback function used by GameTimer to generate a SDL Event to tell Naquadah to render the next frame. This is
 * necessary because GameTimer runs on a separate thread, and all render calls to OpenGL must be made on the same
 * thread that OpenGL is running, which is the main thread, so we create an event from GameTimer's thread, so the
 * main thread can read it and call render from there.
 */
void renderTimerCallback(float millisElapsed) {
    // Creates a new SDL Event, that will be polled by handleUserEvents() and this will call the render method.
    // This is needed so the render call comes from the same thread that OpenGL is running (the main thread).
    SDL_Event renderEvent;
    renderEvent.type = SDL_USEREVENT;
    renderEvent.user.code = Naquadah::USER_EVENT_RENDER;
    SDL_PushEvent(&renderEvent);
}

void Naquadah::installTimers() {
    /* All timers are initialized and keep running at all times, even if some of the modules isn't loaded. The only
     * exception is the Rendering timer, which starts only when Renderer is created, due to it requiring its own
     * thread.
     */
    GameTimer::logicTimer = new GameTimer(TARGET_FPS, &updateLogicTimerCallback);
    GameTimer::physicsTimer = new GameTimer(TARGET_FPS, &updatePhysicsTimerCallback);
    GameTimer::renderingTimer = new GameTimer(TARGET_FPS, &renderTimerCallback);
    GameTimer::logicTimer->startTimer();
    GameTimer::physicsTimer->startTimer();
    GameTimer::renderingTimer->startTimer();
}

Vector2 Naquadah::getWindowSize() {
    if (instance != nullptr && instance->renderer != nullptr) {
        return instance->renderer->getWindowSize();
    }
    return Vector2();
}