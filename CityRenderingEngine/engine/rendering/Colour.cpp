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

Colour::Colour(byte red, byte green, byte blue, byte alpha) {
    this->red = clamp(red, 0, 255);
    this->green = clamp(green, 0, 255);
    this->blue = clamp(blue, 0, 255);
    this->alpha = clamp(alpha, 0, 255);
}

Colour &Colour::operator=(const Colour &other) {
    this->red = other.red;
    this->green = other.green;
    this->blue = other.blue;
    this->alpha = other.alpha;
    return *this;
}

void Colour::setColour(uint32_t colour) {
    alpha = clamp((byte) (colour & 0x000000FF), 0, 255);
    blue = clamp((byte) ((colour & 0x0000FF00) >> 8), 0, 255);
    green = clamp((byte) ((colour & 0x00FF0000) >> 16), 0, 255);
    red = clamp((byte) ((colour & 0xFF000000) >> 24), 0, 255);
}

void Colour::setColourRev(uint32_t colour) {
    alpha = clamp((byte) (colour & 0x000000FF), 0, 255);
    red = clamp((byte) ((colour & 0x0000FF00) >> 8), 0, 255);
    green = clamp((byte) ((colour & 0x00FF0000) >> 16), 0, 255);
    blue = clamp((byte) ((colour & 0xFF000000) >> 24), 0, 255);
}

void Colour::setColour(byte red, byte green, byte blue, byte alpha) {
    this->red = clamp(red, 0, 255);
    this->green = clamp(green, 0, 255);
    this->blue = clamp(blue, 0, 255);
    this->alpha = clamp(alpha, 0, 255);
}

void Colour::setColour(float red, float green, float blue, float alpha) {
    this->red = clamp((byte) (red * 255), 0, 255);
    this->green = clamp((byte) (green * 255), 0, 255);
    this->blue = clamp((byte) (blue * 255), 0, 255);
    this->alpha = clamp((byte) (alpha * 255), 0, 255);
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