/*
 * Author: Rodrigo Castro Azevedo
 * Date: 19/07/2014
 *
 * Description: This class wraps some of the necessary functionality to make Skyboxes work. Each Scene can contain up
 * to one Skybox, that will be rendered on each frame. This Skybox must have all six textures set in roder to render
 * properly. On the functions that use as parameter a vector of filenames, the order of the cube faces is this:
 * Positive X, Negative X, Positive Y, Negative Y, Positive Z, Negative Z.
 *
 * As it holds six textures, this is also a Resource.
 */

#pragma once

#include "../Naquadah.h"
#include "../ResourcesManager.h"

class Scene;
class Model;
class Colour;

class Skybox : public Resource {
public:
	Skybox(void);
	Skybox(const std::vector<std::string> &fileNames, int name);
    /* Constructor with the separate six texture file names. */
    Skybox(const std::string &posX, const std::string &negX, const std::string &posY,
        const std::string &negY, const std::string &posZ, const std::string &negZ, int name);
	virtual ~Skybox(void);

    /* Renders the Skybox. */
    void render(Scene *scene);

	/* Reads the texture's image files from disk and buffers it to the OpenGL context. */
	virtual void load();

	/* Unloads texture data from OpenGL. */
	virtual void unload();

	/*
	 * These functions will look in the resource manager to see if the texture already exists, returning them if so,
     * or creating them if they're not present in the manager. The preLoad flag determines if the Texture should be
     * buffered to the GPU now, instead of being able to do this later.
	 */
	static Skybox *getOrCreate(int name, const std::vector<std::string> &fileNames, bool preLoad);

	/*
	 * Binds an already loaded texture to the specified texture unit. Call this function right before rendering a model
     * that uses a texture.
	 */
	//static void bindTexture(Texture *texture, GLuint shaderProgram, TextureSlot slot);

	/* Binds this texture into the specified texture unit. */
	//void bindTexture(GLuint shaderProgram, TextureSlot slot);

	//GLuint getTextureId() { return textureId; }
	//int getTextureWidth() { return texWidth; }
	//int getTextureHeight() { return texHeight; }

	/*
	 * Log an SDL error with some error message to the output stream of our choice
	 * @param os The output stream to write the message to
	 * @param msg The error message to write, format will be msg error: SDL_GetError()
	 */
	static void logSDLError(std::ostream &os, const std::string &msg) {
		os << msg << " error: " << SDL_GetError() << std::endl;
	}

protected:

	/* The OpenGL ID for the loaded cubemap texture. */
	GLuint cubemapId;

	/* The filenames os the six faces of the cubemap. */
	std::vector<std::string> fileNames;

    /* The specialized Skybox shader. */
    Shader *shader;

    /* The quad mesh used by the Skybox. */
    Model *quad;
};