#include "TextItem.h"

const std::string TextItem::defaultFont = "resources/fonts/CalibriBold.ttf";
const int TextItem::defaultFontSize = 16;
const Colour TextItem::defaultColour = Colour(0xFFFFFFFF);

TextItem::TextItem(void) : InterfaceItem(ITEM_TEXT) {
	font = defaultFont;
	fontSize = defaultFontSize;
	colour = new Colour(defaultColour);
	setText("");
	fadeControl = 0;
    textChanged = true;
}

TextItem::TextItem(const TextItem &copy) : InterfaceItem(copy) {
	this->text = std::string(copy.text);
	this->font = std::string(copy.font);
	this->fontSize = copy.fontSize;
	this->colour = new Colour(*(copy.colour));
	this->fadeControl = copy.fadeControl;
	this->fadeStep = copy.fadeStep;
	this->fadeDuration = copy.fadeDuration;
    this->textChanged = copy.textChanged;
}

TextItem::TextItem(std::string text, int fontSize) : InterfaceItem(ITEM_TEXT) {
	font = defaultFont;
	fontSize = fontSize;
	colour = new Colour(defaultColour);
	setText(text);
	fadeControl = 0;
    textChanged = true;
}

TextItem::TextItem(Vector2 &position, float rotation, std::string text, int fontSize) : InterfaceItem(ITEM_TEXT, position, rotation, Vector2(SIZE_NO_RESIZE, SIZE_NO_RESIZE)) {
	font = defaultFont;
	this->fontSize = fontSize;
	colour = new Colour(defaultColour);
	setText(text);
	this->size = new Vector2(SIZE_NO_RESIZE, SIZE_NO_RESIZE);
	fadeControl = 0;
    textChanged = true;
}

TextItem::TextItem(Vector2 &position, float rotation, Vector2 &size) : InterfaceItem(ITEM_TEXT, position, rotation, size) {
	font = defaultFont;
	fontSize = defaultFontSize;
	colour = new Colour(defaultColour);
	setText("");
	fadeControl = 0;
    textChanged = true;
}

TextItem::TextItem(Vector2 &position, float rotation, Vector2 &size, std::string text, int fontSize) : InterfaceItem(ITEM_TEXT, position, rotation, size) {
	font = defaultFont;
	this->fontSize = fontSize;
	colour = new Colour(defaultColour);
	setText(text);
	fadeControl = 0;
    textChanged = true;
}

TextItem::~TextItem(void) {
	delete colour;
	colour = nullptr;
}

void TextItem::setText(std::string text) {
	this->text = text;
    textChanged = true;
}

void TextItem::setColour(Colour &colour) {
	*(this->colour) = colour;
    textChanged = true;
}

void TextItem::setFont(std::string font) {
	this->font = font;
    textChanged = true;
}

void TextItem::setFontSize(int fontSize) {
	this->fontSize = fontSize;
    textChanged = true;
}

void TextItem::reloadText() {
	// We release our current texture before creating the new one
	if (texture != nullptr) {
        texture->unload();
		delete texture;
	}
    texture = new Texture(text, *colour, font, fontSize);
    textChanged = false;
}

void TextItem::onMouseMoved(Vector2 &position, Vector2 &amount) {
}

void TextItem::onMouseClick(Uint8 button, Vector2 &position) {
}

void TextItem::onMouseDoubleClick(Uint8 button, Vector2 &position) {
}

void TextItem::onMouseButtonDown(Uint8 button, Vector2 &position) {
}

void TextItem::onMouseButtonUp(Uint8 button, Vector2 &position) {
}

/*
 * The parameter amount is positive when the wheel is scrolled away from the player,
 * and negative if scrolled toward the player.
 */
void TextItem::onMouseWheelScroll(int amount) {
}

void TextItem::onKeyPress(SDL_Keysym key) {
}

void TextItem::onKeyDown(SDL_Keysym key) {
}

void TextItem::onKeyUp(SDL_Keysym key) {
}

void TextItem::update(unsigned millisElapsed) {
	/*
	 * The fading effect doesn't work because SDL_ttf cannot render text with
	 * other alpha values. It uses the alpha channel only to create a transparent
	 * background to the text, not to make the text itself transluscent.
	 */

	if (fadeControl > 0) {
		// Fade effect
		fadeStep += millisElapsed;
		uint8_t alpha = 0;
		if (fadeControl == 1) {
			alpha = (uint8_t) (((fadeStep * 1.0f) / fadeDuration) * 256.0f);
		} else {
			alpha = (uint8_t) (256 - ((fadeStep * 1.0f) / fadeDuration) * 256.0f);
		}
		if (fadeStep >= fadeDuration) {
			if (fadeControl == 2) {
				hidden = true;
			}
			fadeControl = 0;
			fadeStep = 0;
		} else {
			// Replace the colour with a new one, with different alpha value
			if (fadeControl == 1) {
				Colour nextColour = fadeColour;
				nextColour.alpha = alpha;
				setColour(nextColour);
				hidden = false;
			} else {
				Colour nextColour = *colour;
				nextColour.alpha = alpha;
				setColour(nextColour);
			}
		}
	}
	InterfaceItem::update(millisElapsed);
}

void TextItem::draw(float millisElapsed, GLuint program) {
    reloadText();
    InterfaceItem::draw(millisElapsed, program);
}

TextItem &TextItem::operator=(const TextItem &other) {
	this->text = other.text;
	this->font = other.font;
	this->fontSize = other.fontSize;
	this->colour = other.colour;
	return *this;
}

void TextItem::startFadeIn(Colour toColour, int millis) {
	fadeStep = 0;
	fadeControl = 1;
	fadeDuration = millis;
	fadeColour = toColour;
}

void TextItem::startFadeOut(int millis) {
	fadeStep = 0;
	fadeControl = 2;
	fadeDuration = millis;
}