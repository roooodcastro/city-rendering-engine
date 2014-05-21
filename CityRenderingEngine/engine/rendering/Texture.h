#pragma once

#include "../Naquadah.h"
#include "../ResourcesManager.h"
#include "Material.h"

class Material;
class Colour;

enum TextureSlot {
	TEXTURE0,
	TEXTURE1,
	TEXTURE2
};

class Texture : public Resource {
public:
	Texture(void);
	Texture(const Texture &copy);
	Texture(const char *filename, const char *name);
	Texture(Colour &colour, const char *name);
	virtual ~Texture(void);

	Texture &operator=(const Texture &other);

	/*
	 * Reads an image file from disk and buffers it to the OpenGL context.
	 * Returns the OpenGL reference value and writes the width and height
	 * of the texture to the two last parameters
	 */
	virtual void load();

	/*
	 * Unloads texture data from OpenGL
	 */
	virtual void unload();

	/*
	 * These functions will look in the resource manager to see if the texture already
	 * exists, returning them if so, or creating them if they're not present in the manager.
	 */
	static Texture *getOrCreate(const char *name, const char *fileName);
	static Texture *getOrCreate(const char *name, Colour &colour);

	/*
	 * Creates a text texture, which is basically a text rendered into an image.
	 * This is the only type of texture that should not be added to the Resource Manager,
	 * because texts usually change a lot and adding and removing them from the manager
	 * every frame or so is not desirable.
	 */
	static Texture *createFromText(std::string textureText, Colour &textColour, TTF_Font &font);

	/*
	 * Binds an already loaded texture to the specified texture unit.
	 * Call this function right before rendering a model that uses a texture
	 */
	static void bindTexture(Texture *texture, GLuint shaderProgram, TextureSlot slot);

	/*
	 * Binds this texture into the specified texture unit
	 */
	void bindTexture(GLuint shaderProgram, TextureSlot slot);

	GLuint getTextureId() { return textureId; }
	int getTextureWidth() { return texWidth; }
	int getTextureHeight() { return texHeight; }
	bool isTextureValid() { return textureId >= 0 && textureId < 4000000 && loaded; }

	void setColour(Colour &colour);

	/*
	 * Log an SDL error with some error message to the output stream of our choice
	 * @param os The output stream to write the message to
	 * @param msg The error message to write, format will be msg error: SDL_GetError()
	 */
	static void logSDLError(std::ostream &os, const std::string &msg) {
		os << msg << " error: " << SDL_GetError() << std::endl;
	}

	/*
	 * A little colour texture palette and the methods to retrieve the textures.
	 */
	static Texture *getColourWhite();
	static Texture *getColourBlack();
	static Texture *getColourRed();
	static Texture *getColourGreen();
	static Texture *getColourBlue();
	static const char *texColNameWhite;
	static const char *texColNameBlack;
	static const char *texColNameRed;
	static const char *texColNameGreen;
	static const char *texColNameBlue;

protected:

	/* The OpenGL ID for the loaded texture */
	GLuint textureId;

	/* Texture size parameters */
	int texWidth;
	int texHeight;

	/* The filename to load the texture from */
	const char *fileName;
	/* Or the colour, if it's a colour texture */
	Colour *colour;

};