/*
 * Authors: Rich Davison and Rodrigo Castro Azevedo
 *
 * Description: Some random variables and functions, for lack of a better place to put them.
 *
 * -_-_-_-_-_-_-_,------,   
 * _-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
 * -_-_-_-_-_-_-~|__( ^ .^) /
 * _-_-_-_-_-_-_-""  ""   
 * 
 */

#pragma once

#include <vector>
#include <string>
#include <sstream>

//It's pi(ish)...
static const float PI = 3.14159265358979323846f;	

//It's pi...divided by 360.0f!
static const float PI_OVER_360 = PI / 360.0f;

/* Radians to degrees */
static inline double toDegrees(const double rad) {
    return rad * 180.0 / PI;
};

/* Degrees to radians */
static inline double toRadians(const double deg)	{
    return deg * PI / 180.0;
};

/* Generates a floating point random number ranging from min to max */
static inline float generateRandom(float min, float max) {
    return (((float) rand() / (RAND_MAX)) * (max - min)) + min;
}

//I blame Microsoft...
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))
/* Clamps a value to be between a minimum and a maximum value. */
#define clamp(val, min, max) (val < min ? min : (val > max ? max : val))

/* A few typedefs to make things shorter. */
typedef unsigned int uint;
//typedef uint8_t byte;

static inline std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

/* Splits a string, using a delimiter character. */
static inline std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

template<typename Iterator>
void insertionSort(Iterator first, Iterator last) {
    Iterator min = first;
    for (Iterator i = first + 1; i < last; i++) {
        if (*i < *min) min = i;
    }
    std::iter_swap(first, min);
    while (++first < last) {
        for (Iterator j = first; *j < *(j - 1); --j) {
            std::iter_swap((j - 1), j);
        }
    }
}