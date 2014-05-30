/*
 * Author: Rodrigo Castro Azevedo
 * Date: 14/05/2014
 *
 * Description: Main file for the City Rendering Engine. The engine will be loaded
 * in this file and the rendered environment will be setup and loaded.
 *
 * The engine used is called Naquadah, which is also the name of its main class.
 *
 * To compile it as Release and get rid of the console window, change to Windows on the linker system settings
 * and add this to the linker command line: /ENTRY:"mainCRTStartup"
 */

#include <stdlib.h>
#include <iostream>
#include <SDL.h>
#include "engine/Naquadah.h"

#include "engine/ResourcesManager.h"
#include "generator/City.h"

int main(int argc, char* argv[]) {

    // Configure engine
    //ConfigurationManager::setConfigFileName("anotherFile.cfg");
    Naquadah::initialize(Naquadah::NAQUADAH_INIT_EVERYTHING);

    // Create the first Scene and start the game
    Scene *scene = new Scene();
    Naquadah::getInstance()->setNextScene(scene);

    //Shader *shader = Shader::getOrCreate("LightShader", "resources/shaders/vertNormal.glsl", "resources/shaders/fragLight.glsl");
    //float *time = new float(0.0f);
    //ShaderParameter *shaderParameter = new ShaderParameter("time", PARAMETER_FLOAT, (void*) time);
    //shader->addShaderParameter(shaderParameter);
    //Model *model = Model::getOrCreate("Teapot", "resources/meshes/teapot.obj");
    //Entity *teapot = new Entity(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1));
    //teapot->setShader(shader);
    //teapot->setModel(model);
    //scene->addEntity(teapot, "Teapot");

    scene->setLightSource(new Light(Colour(1.0f, 1.0f, 0.9f, 1.0f), Vector3(0, 50, 0), Vector3(0.2f, -0.5f, 0.1f), 0.95f, 0, LIGHT_DIRECTIONAL));
    scene->setCameraPosition(Vector3(1000, -600, 1000));
    scene->setCameraRotation(Vector3(15, 135, 0));

    City *city = City::generateManhattanGrid(20, 20);
    std::vector<CityBlock*> *cityBlocks = city->getCityBlocks();
    for (unsigned i = 0; i < cityBlocks->size(); i++) {
        CityBlock *cityBlock = cityBlocks->at(i);
        std::vector<Building*> *buildings = cityBlock->getBuildings();
        for (unsigned j = 0; j < buildings->size(); j++) {
            std::string iString = std::to_string((long long) i);
            std::string jString = std::to_string((long long) j);
            scene->addEntity(buildings->at(j), "Building_" + iString + "_" + jString);
        }
    }

    Naquadah::getInstance()->runGame();

    // Cleanup after the game ends
    return 0;
}