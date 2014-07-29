#pragma once

#include "../Naquadah.h"
#include "Colour.h"
#include "../ResourcesManager.h"
#include "Material.h"

class Material;

enum TextureSlot {
	TEXTURE0,
	TEXTURE1,
	TEXTURE2
};

struct TextFontAttr {
    std::string fontFilename;
    int fontSize;
};

class Texture : public Resource {
public:
	Texture(void);
	Texture(const Texture &copy);
	Texture(const std::string &filename, int name);

    /*
	 * Creates a text texture, which is basically a text rendered into an image. This is the only type of texture that
     * should not be added to the Resource Manager, because texts usually change a lot and adding and removing them
     * from the manager every frame or so is kind of redundant.
	 */
    Texture(const std::string &text, const Colour &colour, const std::string &fontFilename, int fontSize);
    Texture(Colour &colour, int name);
	virtual ~Texture(void);

	Texture &operator=(const Texture &other);

	/*
	 * Reads an image file from disk and buffers it to the OpenGL context. Returns the OpenGL reference value and
     * writes the width and height of the texture to the two last parameters.
	 */
	virtual void load();

	/* Unloads texture data from OpenGL. */
	virtual void unload();

	/*
	 * These functions will look in the resource manager to see if the texture already exists, returning them if so,
     * or creating them if they're not present in the manager. The preLoad flag determines if the Texture should be
     * buffered to the GPU now, instead of being able to do this later.
	 */
	static Texture *getOrCreate(int name, const std::string &fileName, bool preLoad);
	static Texture *getOrCreate(int name, Colour &colour, bool preLoad);

	/*
	 * Binds an already loaded texture to the specified texture unit. Call this function right before rendering a model
     * that uses a texture.
	 */
	static void bindTexture(Texture *texture, GLuint shaderProgram, TextureSlot slot);

	/* Binds this texture into the specified texture unit. */
	void bindTexture(GLuint shaderProgram, TextureSlot slot);

	GLuint getTextureId() { return textureId; }
	int getTextureWidth() { return texWidth; }
	int getTextureHeight() { return texHeight; }

    void setColour(const Colour &colour) { this->colour = colour; }

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
	static const int texColNameWhite;
	static const int texColNameBlack;
	static const int texColNameRed;
	static const int texColNameGreen;
	static const int texColNameBlue;

protected:

	/* The OpenGL ID for the loaded texture */
	GLuint textureId;

	/* Texture size parameters */
	int texWidth;
	int texHeight;

	/* The filename to load the texture from */
	std::string fileName;

    /* If it's a text texture, this is its text. */
    std::string text;

    /* If it's a text texture, this is the font that will be used to render the text. */
    TextFontAttr fontAttr;

	/* Or the colour, if it's a colour texture, or a text texture. */
	Colour colour;
};