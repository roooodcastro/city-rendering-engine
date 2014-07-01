/*
 * Author: John W. Ratcliff [jratcliff@verant.com]
 * Date: 29/06/2014
 *
 * Description: Static class to triangulate any contour/polygon efficiently. Does not support polygons with holes.
 * Uses STL vectors to represent a dynamic array of vertices. This code snippet was submitted to FlipCode.com by
 * John W. Ratcliff (jratcliff@verant.com) on July 22, 2000.  I did not write the original code/algorithm for this
 * triangulator, in fact, I can't even remember where I found it in the first place. However, I did rework it into the
 * following black-box static class so you can make easy use of it in your own code.
 *
 * This code was adapted from here: http://www.flipcode.com/archives/Efficient_Polygon_Triangulation.shtml
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <vector>
#include <string.h>
#include "Common.h"

class Triangulation {
public:

    // triangulate a contour/polygon, places results in STL vector
    // as series of triangles.
    static bool triangulate(const std::vector<Vector2> &polygon, std::vector<Vector2> &result);

    /* Calculates the area of the provided polygon. */
    static float calculateArea(const std::vector<Vector2> &polygon);

    /* Returns true if a given point is inside the triangle defined by the points A, B and C, and flase otherwise. */
    static bool isInsideTriangle(const Vector2 &point, const Vector2 &a, const Vector2 &b, const Vector2 &c);

private:

    static bool snip(const std::vector<Vector2> &polygon, int u, int v, int w, int n, int *V);

};