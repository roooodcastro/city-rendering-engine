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
#include "Vector2.h"

/* Maximum value that a int type can have. */
static const int MAX_INT = 2147483647;

/* Error tolerance for float type values. */
static const float EPS = 0.0000000001f;

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

static float distancePointToLineSeg(Vector2 lineA, Vector2 lineB, Vector2 point) {
    // Return minimum distance between line segment lineAlineB and point
    Vector2 line = lineB - lineA;
    const float l2 = line.x * line.x + line.y * line.y; // i.e. |w-v|^2 - avoid a sqrt
    if (l2 == 0.0f) return (lineA - point).getLength(); // v == w case
    // Consider the line extending the segment, parameterized as v + t (w - v).
    // We find projection of point p onto the line. 
    // It falls where t = [(p-v) . (w-v)] / |w-v|^2
    const float t = Vector2::dot(point - lineA, lineB - lineA) / l2;
    if (t < 0.0f) return (lineA - point).getLength(); // Beyond the 'v' end of the segment
    else if (t > 1.0f) return (lineB - point).getLength();  // Beyond the 'w' end of the segment
    const Vector2 projection = lineA + (line * t);  // Projection falls on the segment
    return (projection - point).getLength();
}

/* Checks if a point is inside a triangle defined by the vertices a, b and c. */
static bool isPointInTriangle(const Vector2 &point, const Vector2 &a, const Vector2 &b, const Vector2 &c) {
    float A = 1/2 * (-b.y * c.x + a.y * (-b.x + c.x) + a.x * (b.y - c.y) + b.x * c.y);
    float sign = A < 0 ? -1.0f : 1.0f;
    float s = (a.y * c.x - a.x * c.y + (c.y - a.y) * point.x + (a.x - c.x) * point.y) * sign;
    float t = (a.x * b.y - a.y * b.x + (a.y - b.y) * point.x + (b.x - a.x) * point.y) * sign;
    
    return s > 0 && t > 0 && (s + t) < 2 * A * sign;
}