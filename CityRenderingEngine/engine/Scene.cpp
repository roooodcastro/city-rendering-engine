#include "Level.h"

Level::Level() {
	entities = new std::map<std::string, Entity*>();
	projectionMatrix = new Matrix4();
	cameraMatrix = new Matrix4();
	lightSources = new std::vector<Light*>();
	cameraPos = new Vector3(0, 0, 0);
	cameraRotation = new Vector3(0, 0, 0);
	dragging = false;
	draggingRight = false;
	mutex = SDL_CreateMutex();
}

Level::Level(const Level &copy) {
	this->cameraPos = new Vector3(*(copy.cameraPos));
	this->cameraRotation = new Vector3(*(copy.cameraRotation));
	this->cameraMatrix = new Matrix4(*(copy.cameraMatrix));
	this->entities = new std::map<std::string, Entity*>(*(copy.entities));
	this->levelType = copy.levelType;
	this->lightSources = new std::vector<Light*>(*(copy.lightSources));
	this->projectionMatrix = new Matrix4(*(copy.projectionMatrix));
	this->userInterface = new UserInterface(*(copy.userInterface));
	this->mutex = copy.mutex;
}

Level::Level(LevelType type) {
	entities = new std::map<std::string, Entity*>();
	projectionMatrix = new Matrix4();
	cameraMatrix = new Matrix4();
	levelType = type;
	lightSources = new std::vector<Light*>();
	cameraPos = new Vector3(0, 0, 0);
	cameraRotation = new Vector3(0, 0, 0);
	dragging = false;
	draggingRight = false;
	mutex = SDL_CreateMutex();
}

Level::Level(LevelType type, UserInterface &userInterface) {
	entities = new std::map<std::string, Entity*>();
	projectionMatrix = new Matrix4();
	cameraMatrix = new Matrix4();
	levelType = type;
	this->userInterface = new UserInterface(userInterface);
	lightSources = new std::vector<Light*>();
	cameraPos = new Vector3(0, 0, 0);
	cameraRotation = new Vector3(0, 0, 0);
	dragging = false;
	draggingRight = false;
	mutex = SDL_CreateMutex();
}

Level::~Level(void) {
	if (userInterface) {
		delete userInterface;
	}
	entities->clear();
	delete entities;
	delete lightSources;
	delete cameraPos;
	delete cameraRotation;
	SDL_DestroyMutex(mutex);
}

void Level::onMouseMoved(Vector2 &position, Vector2 &amount) {
	if (userInterface) {
		userInterface->onMouseMoved(position, amount);
	}
	if (!GameApp::getInstance()->isGamePaused()) {
		for (auto it = entities->begin(); it != entities->end(); ++it) {
			(*it).second->onMouseMoved(position, amount);
		}
	}

	if (dragging) {
		this->cameraRotation->y += (amount.x / 4.0f);
		this->cameraRotation->x += (amount.y / 4.0f);
	} else if (draggingRight) {
		this->cameraPos->x += (amount.x / 20.0f);
		this->cameraPos->y -= (amount.y / 20.0f);
	}
}

void Level::onMouseClick(Uint8 button, Vector2 &position) {
	if (userInterface) {
		userInterface->onMouseClick(button, position);
	}
	if (!GameApp::getInstance()->isGamePaused()) {
		for (auto it = entities->begin(); it != entities->end(); ++it) {
			(*it).second->onMouseClick(button, position);
		}
	}
}

void Level::onMouseDoubleClick(Uint8 button, Vector2 &position) {
	if (userInterface) {
		userInterface->onMouseDoubleClick(button, position);
	}
	if (!GameApp::getInstance()->isGamePaused()) {
		for (auto it = entities->begin(); it != entities->end(); ++it) {
			(*it).second->onMouseDoubleClick(button, position);
		}
	}
}

void Level::onMouseButtonDown(Uint8 button, Vector2 &position) {
	if (userInterface) {
		userInterface->onMouseButtonDown(button, position);
	}
	if (!GameApp::getInstance()->isGamePaused()) {
		for (auto it = entities->begin(); it != entities->end(); ++it) {
			(*it).second->onMouseButtonDown(button, position);
		}
	}

	if (button == SDL_BUTTON_MIDDLE) {
		dragging = true;
	} else if (button == SDL_BUTTON_RIGHT) {
		draggingRight = true;
	}
}

void Level::onMouseButtonUp(Uint8 button, Vector2 &position) {
	if (userInterface) {
		userInterface->onMouseButtonUp(button, position);
	}
	if (!GameApp::getInstance()->isGamePaused()) {
		for (auto it = entities->begin(); it != entities->end(); ++it) {
			(*it).second->onMouseButtonUp(button, position);
		}
	}

	if (button == SDL_BUTTON_MIDDLE) {
		dragging = false;
	} else if (button == SDL_BUTTON_RIGHT) {
		draggingRight = false;
	}
}

/*
 * The parameter amount is positive when the wheel is scrolled away from the player,
 * and negative if scrolled toward the player.
 */
void Level::onMouseWheelScroll(int amount) {
	if (userInterface) {
		userInterface->onMouseWheelScroll(amount);
	}
	if (!GameApp::getInstance()->isGamePaused()) {
		for (auto it = entities->begin(); it != entities->end(); ++it) {
			(*it).second->onMouseWheelScroll(amount);
		}
	}
}

void Level::onKeyPress(SDL_Keysym key) {
	if (userInterface) {
		userInterface->onKeyPress(key);
	}
	if (!GameApp::getInstance()->isGamePaused()) {
		for (auto it = entities->begin(); it != entities->end(); ++it) {
			(*it).second->onKeyPress(key);
		}
	}
	if (levelType == LEVEL_GAME) {
		if (key.sym == SDLK_ESCAPE) {
			GameApp::getInstance()->setGamePaused(!GameApp::getInstance()->isGamePaused());
		}
	}
}

void Level::onKeyDown(SDL_Keysym key) {
	Keyboard::getInstance()->keyDown(key);
	// Now we propagate this event to the interface items
	if (userInterface) {
		userInterface->onKeyDown(key);
	}
	if (!GameApp::getInstance()->isGamePaused()) {
		for (auto it = entities->begin(); it != entities->end(); ++it) {
			(*it).second->onKeyDown(key);
		}
	}
}

void Level::onKeyUp(SDL_Keysym key) {
	Keyboard::getInstance()->keyUp(key);
	// Now we propagate this event to the interface items
	if (userInterface) {
		userInterface->onKeyUp(key);
	}
	if (!GameApp::getInstance()->isGamePaused()) {
		for (auto it = entities->begin(); it != entities->end(); ++it) {
			(*it).second->onKeyUp(key);
		}
	}
}

bool Level::isEntityInLevel(std::string name) {
	return entities->find(name) != entities->end();
}

void Level::addEntity(Entity *entity, std::string name) {
	if (entity && name != "") {
		entities->insert(std::pair<std::string, Entity*>(name, entity));
	}
}

bool Level::removeEntity(std::string name) {
	if (isEntityInLevel(name)) {
		return entities->erase(name) > 0;
	}
	return false;
}

void Level::processLevelTick(unsigned int millisElapsed) {
	lockMutex();
	userInterface->update(millisElapsed);
	if (!GameApp::getInstance()->isGamePaused()) {
		unsigned numEntities = entities->size();
		for (unsigned i = 0; i < numEntities; i++) {
			if (i < entities->size()) {
				auto it = entities->begin();
				std::advance(it, i);
				(*it).second->update(millisElapsed);
			}
		}
		calculateCameraMatrix();
	}
	unlockMutex();
}

void Level::drawLevel(unsigned int millisElapsed) {
	// Should first draw the interface using a ortographic projection,
	// then switch to a perspective projection and draw all entities
	GLuint program;

	// Draw Entities
	for (auto it = entities->begin(); it != entities->end(); ++it) {
		// We first get the right shader to use with this entity
		Entity *entity = (*it).second;
		Shader shader = *(GameApp::getInstance()->getDefaultShader());
		if (entity->getCustomShader()) {
			shader = *(entity->getCustomShader());
		}
		program = shader.getShaderProgram();
		if (glIsProgram(program) != GL_TRUE) {
			std::cout << "GLSL program has become invalid!" << std::endl;
		} else {
			glUseProgram(program);
		}
		GameApp::logOpenGLError("USE_PROGRAM @ RENDER_ENTITY");
		// Then we update the shader matrices
		glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, false, (float*) &(entity->getModelMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, false, (float*) cameraMatrix);
		glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, false, (float*) projectionMatrix);
		applyShaderLight(program);
		entity->draw(millisElapsed);
	}

	// Draw Interface
	if (userInterface != NULL) {
		program = userInterface->getInterfaceShader()->getShaderProgram();
		glUseProgram(program);
		glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, false, (float*) &(Matrix4::Translation(Vector3(0, 0, 1.0f))));
		glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, false, (float*) &(Matrix4::Orthographic(-1, 1, 1920.0f, 0, 1080, 0)));
		userInterface->draw(millisElapsed);
	}
}

void Level::addLightSource(Light &lightSource) {
	// If we don't have space to store the item, make some!
	// I set this if to >= to always have an empty space in the array, just in case
	if ((lightSources->size() + 1) >= lightSources->capacity()) {
		lightSources->reserve(lightSources->capacity() + 10);
	}
	lightSources->emplace_back(&lightSource);
}

void Level::removeLightSource(Light &lightSource) {
	lightSources->erase(std::remove(lightSources->begin(), lightSources->end(), &lightSource), lightSources->end());
}

void Level::applyShaderLight(GLuint program) {
	unsigned lightCount = lightSources->size();
	if (lightCount > 0) {
		// We first build the arrays to be passed to the shader
		Vector3 *positions = new Vector3[lightCount];
		Vector3 *colours = new Vector3[lightCount];
		float *radii = new float[lightCount];

		for (unsigned i = 0; i < lightCount; i++) {
			positions[i] = (*lightSources)[i]->position;
			colours[i] = (*lightSources)[i]->colour;
			radii[i] = (*lightSources)[i]->radius;
		}

		GLuint n1 = glGetUniformLocation(program, "lightColour[]");
		glUniform3fv(n1, lightSources->size(), (float*) colours);
		GLuint n2 = glGetUniformLocation(program, "lightPos[]");
		glUniform3fv(glGetUniformLocation(program, "lightPos[]"), lightSources->size(), (float*) positions);
		glUniform1fv(glGetUniformLocation(program, "lightRadius[]"), lightSources->size(), (float*) radii);
		glUniform3fv(glGetUniformLocation(program, "cameraPos"), 1, (float*) cameraPos);

		delete[] positions;
		delete[] colours;
		delete[] radii;
	}
	glUniform1i(glGetUniformLocation(program, "lightCount"), (int) lightCount);
	GameApp::logOpenGLError("APPLY_SHADER_LIGHT");
}

void Level::lockMutex() {
	SDL_mutexP(mutex);
}

void Level::unlockMutex() {
	SDL_mutexV(mutex);
}