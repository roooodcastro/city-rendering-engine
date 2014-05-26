#include "Colour.h"

Colour::Colour(const Colour &copy) {
    this->red = copy.red;
    this->green = copy.green;
    this->blue = copy.blue;
    this->alpha = copy.alpha;
}

Colour::Colour(uint32_t colour) {
    this->red = 0;
    this->green = 0;
    this->blue = 0;
    this->alpha = 0;
    this->setColour(colour);
}

Colour::Colour(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
    this->red = clamp(red, 0, 255);
    this->green = clamp(green, 0, 255);
    this->blue = clamp(blue, 0, 255);
    this->alpha = clamp(alpha, 0, 255);
}

Colour::Colour(float red, float green, float blue, float alpha) {
    this->red = clamp((uint8_t) (red * 255), 0, 255);
    this->green = clamp((uint8_t) (green * 255), 0, 255);
    this->blue = clamp((uint8_t) (blue * 255), 0, 255);
    this->alpha = clamp((uint8_t) (alpha * 255), 0, 255);
}

Colour &Colour::operator=(const Colour &other) {
    this->red = other.red;
    this->green = other.green;
    this->blue = other.blue;
    this->alpha = other.alpha;
    return *this;
}

void Colour::setColour(uint32_t colour) {
    alpha = clamp((uint8_t) (colour & 0x000000FF), 0, 255);
    blue = clamp((uint8_t) ((colour & 0x0000FF00) >> 8), 0, 255);
    green = clamp((uint8_t) ((colour & 0x00FF0000) >> 16), 0, 255);
    red = clamp((uint8_t) ((colour & 0xFF000000) >> 24), 0, 255);
}

void Colour::setColourRev(uint32_t colour) {
    alpha = clamp((uint8_t) (colour & 0x000000FF), 0, 255);
    red = clamp((uint8_t) ((colour & 0x0000FF00) >> 8), 0, 255);
    green = clamp((uint8_t) ((colour & 0x00FF0000) >> 16), 0, 255);
    blue = clamp((uint8_t) ((colour & 0xFF000000) >> 24), 0, 255);
}

void Colour::setColour(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
    this->red = clamp(red, 0, 255);
    this->green = clamp(green, 0, 255);
    this->blue = clamp(blue, 0, 255);
    this->alpha = clamp(alpha, 0, 255);
}

void Colour::setColour(float red, float green, float blue, float alpha) {
    this->red = clamp((uint8_t) (red * 255), 0, 255);
    this->green = clamp((uint8_t) (green * 255), 0, 255);
    this->blue = clamp((uint8_t) (blue * 255), 0, 255);
    this->alpha = clamp((uint8_t) (alpha * 255), 0, 255);
}

unsigned int Colour::getColour() {
    uint32_t colour = 0;
    colour += alpha;
    colour += (blue << 8);
    colour += (green << 16);
    colour += (red << 24);
    return colour;
}

unsigned int Colour::getColourRev() {
    uint32_t colour = 0;
    colour += alpha;
    colour += (red << 8);
    colour += (green << 16);
    colour += (blue << 24);
    return colour;
}

Vector3 Colour::getColourVec3() {
    return Vector3(red / 255.0f, green / 255.0f, blue / 255.0f);
}