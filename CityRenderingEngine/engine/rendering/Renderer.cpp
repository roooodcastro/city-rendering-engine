#include "Renderer.h"

Renderer::Renderer(void) {
    // Reads some configuration from the config file
    std::string gameTitle = ConfigurationManager::getInstance()->readString("gameTitle", "Game");
    std::string resolution = ConfigurationManager::getInstance()->readString("resolution", "1280x720");
    std::vector<std::string> resSplit = split(resolution, 'x');
    windowSize = Vector2(1280, 720);
    if (resSplit.size() == 2) {
        windowSize.x = (float) atoi(resSplit[0].c_str());
        windowSize.y = (float) atoi(resSplit[1].c_str());
    }

    // Initialize SDL graphics functionalities
    SDL_Init(SDL_INIT_EVERYTHING); // TODO: change this
    window = SDL_CreateWindow(gameTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int) windowSize.x,
        (int) windowSize.y, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        //logSDLError(std::cout, "WINDOW_INIT");
    }

    // Init and configure OpenGL
    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // TODO: read driver data to check supported opengl versions
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    glContext = SDL_GL_CreateContext(window);
    if (glContext == nullptr) {
        //logSDLError(std::cout, "OPENGL_CONTEXT_INIT");
    }
    SDLRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (SDLRenderer == nullptr) {
        //logSDLError(std::cout, "RENDERER_INIT");
    }
    if (TTF_Init() == -1) {
        //logSDLError(std::cout, "TTF_INIT");
    }
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
        //logSDLError(std::cout, "IMG_INIT");
    }
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        //logSDLError(std::cout, "MIXER_INIT");
    }
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "OGLRenderer::OGLRenderer(): Cannot initialise GLEW!" << std::endl;
    }

    GLenum err_code = glGetError();
    while (GL_NO_ERROR != err_code) {
        printf("OpenGL Error @ CONTEXT_INIT: %i", err_code);
        err_code = glGetError();
    }

    currentShader = nullptr;

    // We initialize the primitive meshes that will be used by the interface
    Model::initializePrimitiveMeshes();
}

Renderer::~Renderer(void) {
}

void Renderer::render(Scene *scene, float millisElapsed) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Draw current scene
    if (scene != nullptr)
        scene->render(this, millisElapsed);
    // Swap buffers
    SDL_GL_SwapWindow(window);
}

GLuint Renderer::createShaderProgram() {
    GLuint program = glCreateProgram();
    return program;
}

bool Renderer::compileShader(GLuint program, GLenum shaderType, const char *shaderCode) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCode, nullptr);
    glCompileShader(shader);
    GLint shaderCompiled = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderCompiled);
    return (shaderCompiled == GL_TRUE);
}

bool Renderer::linkProgram(GLuint program) {
    glLinkProgram(program);
    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    return (linkStatus == GL_TRUE);
}

void Renderer::bindAttributeLocation(GLuint program, GLuint location, std::string attrName) {
    glBindAttribLocation(program, location, attrName.c_str());
}

bool Renderer::useShader(Shader *shader) {
    if (currentShader == nullptr || currentShader->getShaderProgram() != shader->getShaderProgram()) {
        glUseProgram(shader->getShaderProgram());
        this->currentShader = shader;
        return (glIsProgram(shader->getShaderProgram()) == GL_TRUE);
    }
    return currentShader != nullptr;
}

bool Renderer::updateShaderMatrix(std::string matrixName, Matrix4 *matrix) {
    GLuint location = glGetUniformLocation(currentShader->getShaderProgram(), matrixName.c_str());
    if (location != -1) {
        glUniformMatrix4fv(location, 1, false, (float*) matrix);
        return true;
    } else return false;
}