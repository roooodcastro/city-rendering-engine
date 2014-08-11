/*
 * Author: Rodrigo Castro Azevedo
 * Date: 05/07/2014
 *
 * Description: This is just a helper file to organize and group all the named resources of the City Rendering Engine.
 * All Resources that need a fixed int name should have the name put here, so they'll not get lost.
 */

#pragma once

#include <string>

// Models (from 1)
static const int MODEL_TRIANGLE = 1;
static const int MODEL_QUAD = 2;
static const int MODEL_CUBE = 3;
static const int MODEL_SPHERE = 4; // unused
static const int MODEL_PLANE = 5;
static const int MODEL_UI_QUAD = 6;

static const int MODEL_INTERSECTION = 10;
static const int MODEL_ROAD = 11;
static const int MODEL_GROUND = 12;
static const int MODEL_SIMPLE_HOUSE = 13;

// Textures (from 1000)
static const int TEXTURE_ROAD_INTERSECTION_1 = 1000;
static const int TEXTURE_ROAD_1 = 1001;
static const int TEXTURE_GRASS = 1005;

// Materials (from 2000)
static const int MATERIAL_BASIC = 2000;

// Shaders (from 5000)
static const int SHADER_LIGHT_BASIC = 5000;
static const int SHADER_INTERFACE = 5001;
static const int SHADER_SKY_BOX = 5002;
static const int SHADER_LIGHT_ROAD = 5003;