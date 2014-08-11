/*
 * Author: Ken Perlin & Rodrigo Castro Azevedo
 * Date: 03/08/2014
 * 
 * Description: This static class is a random noise generator based on the Perlin algorithm.
 * This is an adaptation of the original Java code taken from Perlin's website: http://mrl.nyu.edu/~perlin/noise/
 */

#pragma once

#include <cmath>

class Perlin {
public:

    /*
     * Calculates and returns a random float between 0 and 1. The input X and Y should be the X and Z positions of the
     * centre of the CityBlock.
     */
    static float getCityBlockDensity(float posX, float posY) {
        float noiseScale = 3000.0f;
        float noiseHeight = 17389.0f; // 2000th prime number
        float offset = 100000.0f; // Offset so the mirroring that occurs on (0, 0) won't be easily visible
        // TODO: Multiply noiseHeight with the city Seed, to generate different cities for each seed

        float posNoiseX = (abs(posX) / noiseScale) + offset;
        float posNoiseZ = (abs(posY) / noiseScale) + offset;
        /* The 0.2 offset is to increase the density a little, as the normal range for perlin noise goes only from
         * -sqrt(0.5) to +aqrt(0.5), which is approximately -0.7 to 0.7.
         */
        return getNoiseAt(posNoiseX, noiseHeight, posNoiseZ) + 0.2f;
    }

    /*
     * Perlin noise function. This is the implementation of the Perlin noise algorithm, and returns the noise value of
     * the given coordinate.
     */
    static float getNoiseAt(float x, float y, float z) {
        int intX = (int) x & 255;
        int intY = (int) y & 255;
        int intZ = (int) z & 255;
        x -= (int) x;
        y -= (int) y;
        z -= (int) z;
        float u = fade(x);
        float v = fade(y);
        float w = fade(z);
        int a = p[intX] + intY, aa = p[a] + intZ, ab = p[a + 1] + intZ,
            b = p[intX + 1] + intY, ba = p[b] + intZ, bb = p[b + 1] + intZ; // Hash coordinates of the 8 cube corners

        return lerp(w,
                    lerp(v,
                         lerp(u, grad(p[aa], x, y, z), grad(p[ba], x - 1, y, z)),
                         lerp(u, grad(p[ab], x, y - 1, z), grad(p[bb], x - 1, y - 1, z))
                    ),
                    lerp(v,
                         lerp(u, grad(p[aa + 1], x, y, z - 1), grad(p[ba + 1], x - 1, y, z - 1)),
                         lerp(u, grad(p[ab + 1], x, y - 1, z - 1), grad(p[bb + 1], x - 1, y - 1, z - 1))
                    )
               );
    }

   static float fade(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }

   static float lerp(float t, float a, float b) { return a + t * (b - a); }

   /* Converts low 4 bits of code into 12 gradient directions. */
   static float grad(int hash, float x, float y, float z) {
      int h = hash & 15;
      float u = h < 8 ? x : y;
      float v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
      return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
   }

   const static int p[];

protected:

    /* This is a static class. */
    Perlin(void) {}
    ~Perlin(void) {}

    static bool parametersLoaded;
};