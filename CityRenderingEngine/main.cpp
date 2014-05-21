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

int main(int argc, char* argv[]) {

    // Configure engine
    ConfigurationManager::setConfigFileName("anotherFile.cfg");
    Naquadah::initialize(Naquadah::NAQUADAH_INIT_EVERYTHING);

    // Create the first Scene and start the game
    Scene *scene = new Scene(SCENE_GAME);
    Naquadah::getInstance()->setNextScene(scene);

    Naquadah::getInstance()->runGame();

    // Cleanup after the game ends
    return 0;
}