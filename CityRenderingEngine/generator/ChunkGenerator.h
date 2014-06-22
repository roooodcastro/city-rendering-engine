/*
 * Author: Rodrigo Castro Azevedo
 * Date: 11/06/2014
 * 
 * Description: The ChunkGenerator is basically a Chunk factory, generating new Chunks on demand. It has a list of
 * CityBlock generators that will generate different types of CityBlocks, like commercial blocks, residential blocks,
 * parks, squares and some other special blocks, as landmarks. This is a instance-less class.
 *
 * To generate a Chunk, the algorithm will first choose the grid type to be applied to specific chunk areas. This will
 * be selected randomly using the City Seed. After this, a GridGenerator will generate Intersections and Roads, and the
 * resulting spaces will be filled with CityBlocks.
 */

#pragma once

#include <vector>
#include "City.h"
#include "../engine/input/FileIO.h"
#include "../engine/math/Vector2.h"

class City;
class Chunk;

class ChunkGenerator {
public:

    ~ChunkGenerator(void) {}

    /*
     * Generates a new Chunk on the specified position. A new Chunk will only be generated if the position is valid and
     * there's no Chunk already in that position. If the Chunk cannot be generated, the function will return null.
     */
    static Chunk *generateChunk(City *city, const Vector2 &position);

    /* Calculates and returns the GridLayout of the position requested. */
    //TODO: create struct/class to define the grid layouts
    static int getGridLayout(const Vector2 &position);

protected:

    ChunkGenerator(void) {}
};
