/*
 * Author: Rodrigo Castro Azevedo
 * Date: 17/05/2014
 * 
 * Description: A helper class to represent a RGBA colour. This class should be primarily be used with OpenGL and SDL,
 * having methods to return the colour in hexadecimal values in the RGBA and BGRA order. This class works using integer
 * values ranging from 0 to 255, but also offers functions to parse colours using floats ranging from 0.0f to 1.0f.
 */

#pragma once

#include <stdint.h>
#include "../math/Common.h"

class Colour {
public:
    
    byte red;
    byte green;
    byte blue;
    byte alpha;

    /* Initializes a new White Colour. */
    Colour(void) : red(255), green(255), blue(255), alpha(255) {}
    Colour(const Colour &copy);
    /* Parses a hexadecimal in the format RGBA to a Colour. */
    Colour(uint32_t colour);
    /* Parses individual colour values to a Colour. Values should range from 0 to 255. */
    Colour(byte red, byte green, byte blue, byte alpha);
    /* Parses individual float colour values to a Colour. Values should range from 0.0f to 1.0f. */
    Colour(float red, float green, float blue, float alpha);
    ~Colour(void) {}

    /* Sets the colour in hexadecimal format RGBA. */
    void setColour(uint32_t colour);
    /* Sets the colour in hexadecimal format BGRA. */
    void setColourRev(uint32_t colour);
    /* Sets the colour using individual values ranging from 0 to 255. */
    void setColour(byte red, byte green, byte blue, byte alpha);
    /* Sets the colour using individual float values ranging from 0.0f to 1.0f. */
    void setColour(float red, float green, float blue, float alpha);

    /* Returns the colour in hexadecimal format RGBA. */
    uint32_t getColour();
    /* Returns the colour in hexadecimal format BGRA. */
    uint32_t getColourRev();

    Colour &operator=(const Colour &other);
};