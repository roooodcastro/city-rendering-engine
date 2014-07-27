#include "Skybox.h"

Skybox::Skybox(void) : Resource() {
    this->cubemapId = 0;
    this->fileNames = std::vector<std::string>();
    this->quad = Model::getOrCreate(MODEL_QUAD, "resources/meshes/skybox.obj", false);
    this->quad->addUser();
    shader = Shader::getOrCreate(SHADER_LIGHT_BASIC, "resources/shaders/vertSkybox.glsl",
        "resources/shaders/fragSkybox.glsl", false);
    shader->addUser();
}

Skybox::Skybox(const std::vector<std::string> &fileNames, int name) : Resource(name) {
    this->cubemapId = 0;
    this->fileNames = fileNames;
    this->quad = Model::getOrCreate(MODEL_QUAD, "resources/meshes/skybox.obj", false);
    this->quad->addUser();
    this->shader = Shader::getOrCreate(SHADER_SKY_BOX, "resources/shaders/vertSkybox.glsl",
        "resources/shaders/fragSkybox.glsl", false);
    this->shader->addUser();
}

Skybox::Skybox(const std::string &posX, const std::string &negX, const std::string &posY,
    const std::string &negY, const std::string &posZ, const std::string &negZ, int name) : Resource(name) {
    this->cubemapId = 0;
    this->fileNames = std::vector<std::string>();
    this->fileNames.push_back(posX);
    this->fileNames.push_back(negX);
    this->fileNames.push_back(posY);
    this->fileNames.push_back(negY);
    this->fileNames.push_back(posZ);
    this->fileNames.push_back(negZ);
    this->quad = Model::getOrCreate(MODEL_QUAD, "resources/meshes/skybox.obj", false);
    this->quad->addUser();
    this->shader = Shader::getOrCreate(SHADER_SKY_BOX, "resources/shaders/vertSkybox.glsl",
        "resources/shaders/fragSkybox.glsl", false);
    this->shader->addUser();
}

Skybox::~Skybox(void) {
    if (quad != nullptr) {
        ResourcesManager::releaseResource(quad->getName());
        quad = nullptr;
    }
    if (shader != nullptr) {
        //ResourcesManager::releaseResource(shader->getName());
        shader = nullptr;
    }
}

void Skybox::render(Scene *scene) {
    if (quad == nullptr) {
        quad = Model::getOrCreate(MODEL_QUAD, "resources/meshes/plane.obj", false);
        quad->addUser();
        return; // Skip this frame
    }
    glDepthMask(GL_FALSE);
    scene->useShader(shader);
    glUniform1i(glGetUniformLocation(shader->getShaderProgram(), "cubeTex"), 2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapId);
    quad->draw();
    glDepthMask(GL_TRUE);
}

void Skybox::load() {
    if (!loaded) {
        if (fileNames.size() == 6) {
            glGenTextures(1, &cubemapId);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapId);
            int index = 0;
            for (auto it = fileNames.begin(); it != fileNames.end(); it++) {
                std::string fileName = *it;
                if (fileName != "") {
                    SDL_Surface *surface;
                    int mode;
                    surface = IMG_Load(fileName.c_str());
                    // Could not load file
                    if (!surface) {
                        logSDLError(std::cout, "CUBEMAP_LOAD");
                        loaded = true;
                        valid = false;
                        return;
                    }
                    // work out what format to tell glTexImage2D to use...
                    if (surface->format->BytesPerPixel == 3) { // RGB 24bit
                        mode = GL_RGB;
                    } else if (surface->format->BytesPerPixel == 4) { // RGBA 32bit
                        mode = GL_RGBA;
                    } else { // Not a valid image
                        SDL_FreeSurface(surface);
                        loaded = true;
                        valid = false;
                        return;
                    }
                    int texWidth = surface->w;
                    int texHeight = surface->h;
                    GLenum texIndex = 0;
                    switch (index) {
                        case 0: texIndex = GL_TEXTURE_CUBE_MAP_POSITIVE_X; break;
                        case 1: texIndex = GL_TEXTURE_CUBE_MAP_NEGATIVE_X; break;
                        case 2: texIndex = GL_TEXTURE_CUBE_MAP_POSITIVE_Y; break;
                        case 3: texIndex = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y; break;
                        case 4: texIndex = GL_TEXTURE_CUBE_MAP_POSITIVE_Z; break;
                        case 5: texIndex = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z; break;
                    }
                    // This reads from the sdl surface and puts it into an opengl texture
                    glTexImage2D(texIndex, 0, mode, texWidth, texHeight, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);
                    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
                    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);


                    // Clean up
                    SDL_FreeSurface(surface);
                    Naquadah::getRenderer()->logOpenGLError("CUBEMAP_CREATE");
                } else {
                    // One of the filenames isn't valid, abort the loading process and make the texture invalid.
                    loaded = true;
                    valid = false;
                    return;
                }
                index++;
            }
            if (!shader->isLoaded()) {
                shader->load();
            }
            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        }
        loaded = true;
        if (cubemapId >= 0) {
            valid = true;
        }
    }
}

void Skybox::unload() {
    if (loaded) {
        glDeleteTextures(1, &cubemapId);
        loaded = false;
    }
}

/*void Skybox::bindSkybox(Skybox *texture, GLuint shaderProgram, SkyboxSlot slot) {
    texture->bindSkybox(shaderProgram, slot);
}*/

/*void Skybox::bindTexture(GLuint shaderProgram, SkyboxSlot slot) {
    if (loaded) {
        GLuint texUnit = GL_TEXTURE0;
        GLuint texVar = -1;
        int texVal = 0;
        switch (slot) {
        case TEXTURE0:
            texVar = glGetUniformLocation(shaderProgram, "texture0");
            break;
        case TEXTURE1:
            texUnit = GL_TEXTURE1;
            texVal = 1;
            texVar = glGetUniformLocation(shaderProgram, "texture1");
            break;
        case TEXTURE2:
            texUnit = GL_TEXTURE2;
            texVal = 2;
            texVar = glGetUniformLocation(shaderProgram, "texture2");
            break;
        }
        if (texVar != -1) {
            glUniform1i(texVar, texVal);
            glActiveSkybox(texUnit);
            glBindTexture(GL_TEXTURE_2D, textureId);
            //GameApp::logOpenGLError(((string) "TEX_BIND ") + std::to_string((long long) textureId));
        }
    }
}*/

Skybox *Skybox::getOrCreate(int name, const std::vector<std::string> &fileNames, bool preLoad) {
    Skybox *texture = (Skybox*) ResourcesManager::getResource(name);
    if (texture != nullptr) {
        return texture;
    } else {
        texture = new Skybox(fileNames, name);
        ResourcesManager::addResource(texture, preLoad);
        return texture;
    }
}