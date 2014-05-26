/*
 * Author: Rodrigo Castro Azevedo
 * Date: 17/05/2014
 *
 * Description:
 */

#pragma once

#include "../math/Vector3.h"
#include "Colour.h"
#include "Shader.h"

/* The type of the Light Source. Every light must be of one of these types. */
enum LightType {
    LIGHT_POINT = 1,
    LIGHT_SPOT = 2,
    LIGHT_DIRECTIONAL = 3
};

class Light {
public:

    /* The size of the area affected my the light. Does not affect Directional Light Sources. Defaults to 1. */
    float radius;
    /* The intensity of the light source. Defaults to 1. */
    float intensity;
    /* The world position of this light. Does not affect Directional Light Sources. Defaults to (0, 0, 0). */
    Vector3 position;
    /* The diretion of the light. Does not affect Point Light Sources. Defaults to (0, 0, 0). */
    Vector3 direction;
    /* The colour of the light source. Defaults to white. */
    Colour colour;
    /* The type of the Light Source. Defaults to LIGHT_POINT. */
    LightType type;

    /* Creates a White POINT_LIGHT, in the center of the world, with radius and intensity set to 1.0f. */
    Light(void);
    /* Generic constructor. */
    Light(Colour colour, Vector3 position, Vector3 direction, float intensity, float radius, LightType type);
    /* Creates a Point Light Source. */
    Light(Colour colour, Vector3 position, float intensity, float radius);
    /* Creates a Spot Light Source. */
    Light(Colour colour, Vector3 position, Vector3 direction, float intensity, float radius);
    /* Creates a Directional Light Source. */
    Light(Colour colour, Vector3 direction, float intensity);
    ~Light(void) {}

    /*
     * This function updates the Shader Parameters related to the scene lighting in the shader. If the parameters
     * already exists, they will be only updated, but if they don't they will be created.
     */
    void updateShaderParameters(Shader *shader);

};