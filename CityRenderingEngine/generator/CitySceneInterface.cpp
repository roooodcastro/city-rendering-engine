#include "CitySceneInterface.h"
#include "CityScene.h"
#include "../engine/ui/TextItem.h"

CitySceneInterface::CitySceneInterface(void) : UserInterface() {
    addItem(new TextItem(Vector2(10, 10), 0, "FPS", 18), "fpsCounter");
    addItem(new TextItem(Vector2(10, 29), 0, "Chunks", 18), "chunksCounter");
    addItem(new TextItem(Vector2(10, 70), 0, "XYZ", 18), "positionDebug");
    addItem(new TextItem(Vector2(10, 89), 0, "Facing", 18), "facingDebug");
}

CitySceneInterface::CitySceneInterface(const CitySceneInterface &copy) : UserInterface(copy) {
    addItem(new TextItem(Vector2(10, 10), 0, "FPS", 18), "fpsCounter");
    addItem(new TextItem(Vector2(10, 29), 0, "Chunks", 18), "chunksCounter");
    addItem(new TextItem(Vector2(10, 70), 0, "XYZ", 18), "positionDebug");
    addItem(new TextItem(Vector2(10, 89), 0, "Facing", 18), "facingDebug");
}

CitySceneInterface::~CitySceneInterface(void) {
}

void CitySceneInterface::update(unsigned millisElapsed) {
    int fps = (int) (GameTimer::renderingTimer->getTicksPerSecond() + 0.5f);
    int tps = (int) (GameTimer::logicTimer->getTicksPerSecond() + 0.5f);
    CityScene *cityScene = (CityScene*) Naquadah::getInstance()->getCurrentScene();
    std::ostringstream fpsText;
    std::ostringstream chunksText;
    std::ostringstream positionText;
    std::ostringstream facingText;

    fpsText << "FPS: " << fps << ", TPS: " << tps;
    chunksText << "Chunks: " << cityScene->getCity()->getChunks()->size();
    Vector3 cameraPos = cityScene->getCamera()->getPosition();
    Vector3 cameraRot = cityScene->getCamera()->getRotation();
    positionText << "XYZ: " << cameraPos.x << " / " << cameraPos.y << " / " << cameraPos.z;
    facingText << "Facing (XY rotation): " << cameraRot.x << " / " << cameraRot.y;

    ((TextItem*) getItem("fpsCounter"))->setText(fpsText.str());
    ((TextItem*) getItem("chunksCounter"))->setText(chunksText.str());
    ((TextItem*) getItem("positionDebug"))->setText(positionText.str());
    ((TextItem*) getItem("facingDebug"))->setText(facingText.str());

    UserInterface::update(millisElapsed);

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
}

void CitySceneInterface::draw(unsigned millisElapsed) {
    UserInterface::draw(millisElapsed);
}