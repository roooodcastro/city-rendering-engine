#include "UserInterface.h"
#include "TextItem.h"
#include "ImageItem.h"

UserInterface::UserInterface(void) {
    Vector2 windowSize = Naquadah::getWindowSize();
    items = new std::map<std::string, InterfaceItem*>();
    fpsCounter = new TextItem(Vector2(20, 10), 0, "0 FPS", 20);
    resourcesCounter = new TextItem(Vector2(20, 40), 0, "0 Resources Allocated", 20);
    entitiesCounter = new TextItem(Vector2(20, 70), 0, "0 Entities", 20);
    mousePosDisplay = new TextItem(Vector2(20, 100), 0, "MousePos: (0, 0)", 20);
    interfaceShader = new Shader("InterfaceShader", "shaders/vertUI.glsl", "shaders/fragUI.glsl");
    fadeOverlay = new ImageItem(Vector2(0, 0), 0, windowSize, new Texture(Colour(0x00000000), "FadeTex"));
}

UserInterface::UserInterface(const UserInterface &copy) {
    items = new std::map<std::string, InterfaceItem*>(*(copy.items));
    fpsCounter = new TextItem(*((TextItem*) copy.fpsCounter));
    resourcesCounter = new TextItem(*((TextItem*) copy.resourcesCounter));
    entitiesCounter = new TextItem(*((TextItem*) copy.entitiesCounter));
    mousePosDisplay = new TextItem(*((TextItem*) copy.mousePosDisplay));
    interfaceShader = new Shader(*(copy.interfaceShader));
    fadeOverlay = new ImageItem(*((ImageItem*) copy.fadeOverlay));
    fadeControl = copy.fadeControl;
    fadeStep = copy.fadeStep;
    fadeDuration = copy.fadeDuration;
}

UserInterface::~UserInterface(void) {
    items->clear();
    delete items;
    delete fpsCounter;
    delete resourcesCounter;
    delete entitiesCounter;
    delete mousePosDisplay;
    delete interfaceShader;
}

UserInterface &UserInterface::operator=(const UserInterface &other) {
    *(this->items) = *(other.items);
    *(this->fpsCounter) = *(other.fpsCounter);
    *(this->resourcesCounter) = *(other.resourcesCounter);
    *(this->entitiesCounter) = *(other.entitiesCounter);
    *(this->mousePosDisplay) = *(other.mousePosDisplay);
    *(this->interfaceShader) = *(other.interfaceShader);
    *(this->fadeOverlay) = *((ImageItem*) other.fadeOverlay);
    fadeControl = other.fadeControl;
    fadeStep = other.fadeStep;
    fadeDuration = other.fadeDuration;
    return *this;
}

void UserInterface::onMouseMoved(Vector2 &position, Vector2 &amount) {
    for (auto it = items->begin(); it != items->end(); ++it) {
        if (!(*it).second->isHidden()) {
            (*it).second->onMouseMoved(position, amount);
        }
    }
    /*if (GameApp::getInstance()->isDebugOn()) {
        mousePos = Vector2(position);
    }*/
}

void UserInterface::onMouseClick(Uint8 button, Vector2 &position) {
    for (auto it = items->begin(); it != items->end(); ++it) {
        if (!(*it).second->isHidden()) {
            (*it).second->onMouseClick(button, position);
        }
    }
}

void UserInterface::onMouseDoubleClick(Uint8 button, Vector2 &position) {
    for (auto it = items->begin(); it != items->end(); ++it) {
        if (!(*it).second->isHidden()) {
            (*it).second->onMouseDoubleClick(button, position);
        }
    }
}

void UserInterface::onMouseButtonDown(Uint8 button, Vector2 &position) {
    for (auto it = items->begin(); it != items->end(); ++it) {
        if (!(*it).second->isHidden()) {
            (*it).second->onMouseButtonDown(button, position);
        }
    }
}

void UserInterface::onMouseButtonUp(Uint8 button, Vector2 &position) {
    for (auto it = items->begin(); it != items->end(); ++it) {
        if (!(*it).second->isHidden()) {
            (*it).second->onMouseButtonUp(button, position);
        }
    }
}

void UserInterface::onMouseWheelScroll(int amount) {
    for (auto it = items->begin(); it != items->end(); ++it) {
        if (!(*it).second->isHidden()) {
            (*it).second->onMouseWheelScroll(amount);
        }
    }
}

void UserInterface::onKeyPress(SDL_Keysym key) {
    for (auto it = items->begin(); it != items->end(); ++it) {
        if (!(*it).second->isHidden()) {
            (*it).second->onKeyPress(key);
        }
    }
}

void UserInterface::onKeyDown(SDL_Keysym key) {
    for (auto it = items->begin(); it != items->end(); ++it) {
        (*it).second->onKeyDown(key);
    }
}

void UserInterface::onKeyUp(SDL_Keysym key) {
    // The f3 key will be our debug key, showing debug info on screen
    if (key.sym == SDLK_F3) {
        //GameApp::getInstance()->setDebug(!GameApp::getInstance()->isDebugOn());
    }
    for (auto it = items->begin(); it != items->end(); ++it) {
        if (!(*it).second->isHidden()) {
            (*it).second->onKeyUp(key);
        }
    }
}

void UserInterface::update(float millisElapsed) {
    // Update inner items
    for (auto it = items->begin(); it != items->end(); ++it) {
        (*it).second->update(millisElapsed);
    }
    // Update debug text
    //if (GameApp::getInstance()->getFramesCount() % (GameApp::TARGET_FPS / 10) == 0 && GameApp::getInstance()->isDebugOn()) {
    //	int fps = GameApp::getInstance()->getFps();
    //	int resources = ResourcesManager::getResourcesCount();
    //	int numEntities = 0;
    //	std::map<std::string, Entity*> *entities = GameApp::getInstance()->getCurrentLevel()->getEntities();
    //	for (auto it = entities->begin(); it != entities->end(); ++it) {
    //		numEntities += 1;
    //		numEntities += (*it).second->getAllChildren((*it).second).size();
    //	}
    //	// Don't ask me why I had to put "long long" there, it's just the way it is...
    //	((TextItem*) fpsCounter)->setText(to_string((long long) fps) + " FPS");
    //	((TextItem*) resourcesCounter)->setText(to_string((long long) resources) + " Resources Allocated");
    //	((TextItem*) entitiesCounter)->setText(to_string((long long) numEntities) + " Entities");
    //	((TextItem*) mousePosDisplay)->setText("MousePos: (" + to_string((long long) mousePos.x) + ", " + to_string((long long) mousePos.y) + ")");
    //	fpsCounter->update(millisElapsed);
    //	resourcesCounter->update(millisElapsed);
    //	entitiesCounter->update(millisElapsed);
    //	mousePosDisplay->update(millisElapsed);
    //}
    // Fade Effect
    if (fadeControl > 0) {
        fadeStep += (int) millisElapsed;
        uint8_t alpha = 0;
        if (fadeControl == 1) {
            alpha = (uint8_t) (256 - ((fadeStep * 1.0f) / fadeDuration) * 256.0f);
        } else {
            alpha = (uint8_t) (((fadeStep * 1.0f) / fadeDuration) * 256.0f);
        }
        if (fadeStep >= fadeDuration) {
            if (fadeControl == 1) {
                fadeOverlay->setHidden(true);
            }
            fadeControl = 0;
            fadeStep = 0;
        } else {
            // Replace the colour with a new one, with different alpha value
            fadeOverlay->getTexture()->unload();
            fadeOverlay->getTexture()->setColour(Colour(0, 0, 0, alpha));
            fadeOverlay->getTexture()->load();
        }
        fadeOverlay->update(millisElapsed);
    }
}

void UserInterface::draw(float millisElapsed) {
    for (auto it = items->begin(); it != items->end(); ++it) {
        (*it).second->draw(millisElapsed, interfaceShader->getShaderProgram());
    }
    /*if (Naquadah::getInstance()->isDebugOn()) {
        fpsCounter->draw(millisElapsed, interfaceShader->getShaderProgram());
        resourcesCounter->draw(millisElapsed, interfaceShader->getShaderProgram());
        entitiesCounter->draw(millisElapsed, interfaceShader->getShaderProgram());
        mousePosDisplay->draw(millisElapsed, interfaceShader->getShaderProgram());
    }*/
    fadeOverlay->draw(millisElapsed, interfaceShader->getShaderProgram());
}

void UserInterface::addItem(InterfaceItem *item, std::string name) {
    if (item && name != "") {
        items->insert(std::pair<std::string, InterfaceItem*>(name, item));
    }
}

bool UserInterface::removeItem(InterfaceItem *item, std::string name) {
    if (isItemInInterface(name)) {
        return items->erase(name) > 0;
    }
    return false;
}

InterfaceItem *UserInterface::getItem(std::string name) {
    if (isItemInInterface(name)) {
        return items->at(name);
    }
    return NULL;
}

bool UserInterface::isItemInInterface(std::string name) {
    return items->find(name) != items->end();
}

void UserInterface::startFadeIn(int millis) {
    fadeStep = 0;
    fadeControl = 1;
    fadeDuration = millis;
    fadeOverlay->setHidden(false);
}

void UserInterface::startFadeOut(int millis) {
    fadeStep = 0;
    fadeControl = 2;
    fadeDuration = millis;
    fadeOverlay->setHidden(false);
}