/*
 * Author: Rodrigo Castro Azevedo
 * Date: 17/05/2014
 *
 * Description: 
 */

#pragma once

#include "../math/Vector3.h"

class Light {
public:

    /* The size of the area affected my the light. Defaults to 1. */
    float radius;
    /* The intensity of the light source. Defaults to 1. */
    float intensity;
    /* The world position of this light. Defaults to (0, 0, 0). */
    Vector3 position;
    /* The diretion of the light. Does not affect point light sources. Defaults to (0, 0, 0). */
    Vector3 direction;
    /* The colour of the light source. Defaults to white. */
    Vector3 colour;

    Light(void);
    ~Light(void);

protected:

};