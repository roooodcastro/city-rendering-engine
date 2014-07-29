#pragma once

#include "InterfaceItem.h"

class TextItem : public InterfaceItem {
public:

	TextItem(void);
	TextItem(const TextItem &copy);
	TextItem(std::string text, int fontSize);
	TextItem(Vector2 &position, float rotation, std::string text, int fontSize);
	TextItem(Vector2 &position, float rotation, Vector2 &size);
	TextItem(Vector2 &position, float rotation, Vector2 &size, std::string text, int fontSize);
	virtual ~TextItem(void);

	/* Mouse events */
	virtual void onMouseMoved(Vector2 &position, Vector2 &amount); // Will fire every time the mouse moves
	virtual void onMouseClick(Uint8 button, Vector2 &position); // Will fire once a mouse button is released
	virtual void onMouseDoubleClick(Uint8 button, Vector2 &position); // Will fire on a double click
	virtual void onMouseButtonDown(Uint8 button, Vector2 &position); // Will fire in every tick that a button is down
	virtual void onMouseButtonUp(Uint8 button, Vector2 &position); // Will fire every time a mouse button is released
	virtual void onMouseWheelScroll(int amount); // Will fire every time the mouse wheel scrolls

	/* Keyboard events */
	virtual void onKeyPress(SDL_Keysym key); // Will fire every time a key is released
	virtual void onKeyDown(SDL_Keysym key); // Will fire in every tick that a key is down
	virtual void onKeyUp(SDL_Keysym key); // Will fire every time a key is released

	/* General getters and setters */
	void setText(std::string text);
	std::string getText() { return text; }
	void setColour(Colour &colour);
	Colour *getColour() { return colour; }
	void setFont(std::string font);
	std::string getFont() { return font; }
	void setFontSize(int fontSize);
	int getFontSize() { return fontSize; }

	/* We set these to Arial 16, white */
	static const std::string defaultFont;
	static const int defaultFontSize;
	static const Colour defaultColour;

	virtual TextItem &operator=(const TextItem &other);

	/* Gameloop funcions */
	virtual void update(unsigned millisElapsed);
    virtual void draw(float millisElapsed, GLuint program);

	/*
	 * Start a fadeIn effect from transparent to colour, for the specified duration.
	 * Will set the TextItem colour to whatever colour is provided. If the TextItem
	 * is hidden, it will become visible.
	 */
	void startFadeIn(Colour toColour, int millis);
	/* Start a fadeOut effect to transparent, for the specified duration. Will hide the TextItem after finished */
	void startFadeOut(int millis);

	/* Checks if the interface is fading, no matter in which direction. */
	bool isFading() { return fadeControl > 0; }

protected:

	/*
	 * This function will be called everytime an attribute of this text is modified,
	 * because as the text is rendered as a texture, we can't just go and edit it
	 * directly, instead we always have to rebuild it, no matter how small the change is.
	 */
	void reloadText();

	std::string text; // The text label, what will be written
	std::string font; // The text font filename (including the path)
	int fontSize; // The font size, in points
	Colour *colour; // The colour of the text. If a background colour is needed, an ImageItem should be put behind this text with the colour
	/* Describes if the textItem is fading in (1), fading out (2), or not fading (0). Defaults to 0. */
	int fadeControl;
	/* Internal variable used to control the fading effect */
	int fadeStep;
	/* Internal variable used to control the fading effect */
	int fadeDuration;
	/* Stores the colour that the FadeIn is supposed to achieve */
	Colour fadeColour;
    /* A flag inditating if the text has changed. Defaults to true. */
    bool textChanged;
};