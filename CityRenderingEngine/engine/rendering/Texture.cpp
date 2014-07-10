#include "Texture.h"

const int Texture::texColNameWhite = 1100;
const int Texture::texColNameBlack = 1101;
const int Texture::texColNameRed = 1102;
const int Texture::texColNameGreen = 1103;
const int Texture::texColNameBlue = 1104;

Texture::Texture(void) : Resource() {
	texWidth = -1;
	texHeight = -1;
	textureId = -1;
	// By default create a white colour texture
	fileName = "";
	colour = new Colour(0xFFFFFFFF);
}

Texture::Texture(const Texture &copy) : Resource(copy) {
	textureId = copy.textureId;
	texWidth = copy.texWidth;
	texHeight = copy.texHeight;
	fileName = copy.fileName;
	if (copy.colour != nullptr) {
		colour = new Colour(*(copy.colour));
	} else {
		colour = nullptr;
	}
}

Texture::Texture(const std::string &filename, int name) : Resource(name) {
	this->fileName = filename;
	this->colour = nullptr;
}

Texture::Texture(Colour &colour, int name) : Resource(name) {
	this->fileName = "";
	this->colour = new Colour(colour);
}

Texture::~Texture(void) {
    delete colour;
}

Texture &Texture::operator=(const Texture &other) {
	this->textureId = other.textureId;
	this->texWidth = other.texWidth;
	this->texHeight = other.texHeight;
	this->loaded = other.loaded;
	this->name = other.name;
	return *this;
}

void Texture::load() {
	if (!loaded) {
		// It's a texture from an image file
		if (fileName != "") {
			SDL_Surface *surface;
			int mode;
            surface = IMG_Load(fileName.c_str());
			// Could not load file
			if (!surface) {
				logSDLError(std::cout, "IMAGE_LOAD");
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
			this->texWidth = surface->w;
			this->texHeight = surface->h;
			glGenTextures(1, &textureId);

			// this reads from the sdl surface and puts it into an opengl texture
			glBindTexture(GL_TEXTURE_2D, textureId);
			glTexImage2D(GL_TEXTURE_2D, 0, mode, texWidth, texHeight, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);

			// these affect how this texture is drawn later on...
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// clean up
			SDL_FreeSurface(surface);
			//GameApp::logOpenGLError("TEX_LOAD");
		} else if (colour != nullptr) {
			// It's a colour texture
			this->texWidth = 1;
			this->texHeight = 1;
			glGenTextures(1, &textureId);

			glBindTexture(GL_TEXTURE_2D, textureId);
			Uint32 col = colour->getColour();
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &col);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			//GameApp::logOpenGLError("TEX_LOAD");
		}
        loaded = true;
		if (textureId >= 0) {
			valid = true;
        }
	}
}

void Texture::unload() {
	if (loaded) {
		glDeleteTextures(1, &textureId);
		textureId = 0;
		loaded = false;
	}
}

void Texture::bindTexture(Texture *texture, GLuint shaderProgram, TextureSlot slot) {
	texture->bindTexture(shaderProgram, slot);
}

void Texture::bindTexture(GLuint shaderProgram, TextureSlot slot) {
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
			glActiveTexture(texUnit);
			glBindTexture(GL_TEXTURE_2D, textureId);
			//GameApp::logOpenGLError(((string) "TEX_BIND ") + std::to_string((long long) textureId));
		}
	}
}

Texture *Texture::createFromText(std::string textureText, Colour &textColour, TTF_Font &font) {
	Texture *texture = new Texture();
	delete texture->colour;
	texture->colour = NULL;
	int mode;
	//Render text surface
	SDL_Color sdlColour = SDL_Color();
	sdlColour.r = textColour.red;
	sdlColour.g = textColour.green;
	sdlColour.b = textColour.blue;
	sdlColour.a = textColour.alpha;
	SDL_Surface* textSurface = TTF_RenderText_Blended(&font, textureText.c_str(), sdlColour);
	if (textSurface == NULL) {
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	} else {
		// Work out what format to tell glTexImage2D to use...
		if (textSurface->format->BytesPerPixel == 3) { // RGB 24bit
			mode = GL_RGB;
		} else if (textSurface->format->BytesPerPixel == 4) { // RGBA 32bit
			mode = GL_RGBA;
		} else { // Not a valid image
			SDL_FreeSurface(textSurface);
			return NULL;
		}
		texture->texWidth = textSurface->w;
		texture->texHeight = textSurface->h;
		glGenTextures(1, &(texture->textureId));

		// this reads from the sdl surface and puts it into an opengl texture
		glBindTexture(GL_TEXTURE_2D, texture->textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, mode, texture->texWidth, texture->texHeight, 0, mode, GL_UNSIGNED_BYTE, textSurface->pixels);

		// these affect how this texture is drawn later on...
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

		// clean up
		SDL_FreeSurface(textSurface);
		//GameApp::logOpenGLError("TEX_LOAD");

		if (texture->textureId >= 0) {
			texture->loaded = true;
		}
	}
	return texture;
}

Texture *Texture::getOrCreate(int name, const std::string &fileName, bool preLoad) {
    Texture *texture = (Texture*) ResourcesManager::getResource(name);
	if (texture != nullptr) {
		return texture;
	} else {
		texture = new Texture(fileName, name);
		ResourcesManager::addResource(texture, preLoad);
		return texture;
	}
}

Texture *Texture::getOrCreate(int name, Colour &colour, bool preLoad) {
	if (ResourcesManager::resourceExists(name)) {
		return (Texture*) ResourcesManager::getResource(name);
	} else {
		Texture *newTexture = new Texture(colour, name);
		ResourcesManager::addResource(newTexture, preLoad);
		return newTexture;
	}
}

Texture *Texture::getColourWhite() {
	return getOrCreate(Texture::texColNameWhite, Colour(0xFFFFFFFF), true);
}

Texture *Texture::getColourBlack() {
	return getOrCreate(Texture::texColNameBlack, Colour(0xFF000000), true);
}

Texture *Texture::getColourRed() {
	return getOrCreate(Texture::texColNameRed, Colour(0xFFFF0000), true);
}

Texture *Texture::getColourGreen() {
	return getOrCreate(Texture::texColNameGreen, Colour(0xFF00FF00), true);
}

Texture *Texture::getColourBlue() {
	return getOrCreate(Texture::texColNameBlue, Colour(0xFF0000FF), true);
}

void Texture::setColour(Colour &colour) {
	if (this->colour) {
		*(this->colour) = colour;
	} else {
		this->colour = new Colour(colour);
	}
}