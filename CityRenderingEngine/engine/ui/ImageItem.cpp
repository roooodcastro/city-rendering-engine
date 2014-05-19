#include "ImageItem.h"

ImageItem::ImageItem(void) : InterfaceItem(ITEM_IMAGE) {

}

ImageItem::ImageItem(Vector2 &position, float rotation, Vector2 &size, Texture *image) : InterfaceItem(ITEM_IMAGE, position, rotation, size) {
	texture = image;
}

ImageItem::~ImageItem() {

}

void ImageItem::onMouseMoved(Vector2 &position, Vector2 &amount) {
}

void ImageItem::onMouseClick(Uint8 button, Vector2 &position) {
}

void ImageItem::onMouseDoubleClick(Uint8 button, Vector2 &position) {
}

void ImageItem::onMouseButtonDown(Uint8 button, Vector2 &position) {
}

void ImageItem::onMouseButtonUp(Uint8 button, Vector2 &position) {
}

/*
 * The parameter amount is positive when the wheel is scrolled away from the player,
 * and negative if scrolled toward the player.
 */
void ImageItem::onMouseWheelScroll(int amount) {
}

void ImageItem::onKeyPress(SDL_Keysym key) {
}

void ImageItem::onKeyDown(SDL_Keysym key) {
}

void ImageItem::onKeyUp(SDL_Keysym key) {
}

