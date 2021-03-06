/*
 * Author: Rodrigo Castro Azevedo
 * Date: 28/06/2014
 *
 * Description: A class with static methods to calculate geometry related algorithms.
 */

#pragma once

#include <vector>
#include "Common.h"
#include "Vector2.h"
#include "Vector3.h"

class Geom {
public:

    /*
     * This function insets a clockwise polygon by amount. It moves all vertices to perform an inset to the polygon, or
     * an outset if a negative amount is provided. If the polygon provided have its vertices ordered CCW, a positive
     * amount will outset the polygon, and a negative will inset it. This function returns the corrected polygon, and
     * doesn't modify the original polygon.
     */
    static std::vector<Vector2> insetPolygon(std::vector<Vector2> polygon, float amount) {
        int numVertices = (int) polygon.size();
        std::vector<Vector2> insetPolygon = std::vector<Vector2>();

        // Polygon must have at least three vertices to be inset.
        if (polygon.size() < 3) return polygon;

        // Inset the first vertex, using the last one to complete the loop.
        Vector2 lastVertex = polygon.at(numVertices - 1);
        insetPolygon.push_back(Geom::insetVertex(lastVertex, polygon.at(0), polygon.at(1), amount));

        for (int i = 1; i < numVertices - 1; i++) {
            Vector2 previous = polygon.at(i - 1);
            Vector2 current = polygon.at(i);
            Vector2 next = polygon.at(i + 1);
            insetPolygon.push_back(Geom::insetVertex(previous, current, next, amount));
        }
        // Finally, inset the last vertex, using the first one to complete the loop.
        insetPolygon.push_back(Geom::insetVertex(polygon.at(numVertices - 2), lastVertex, polygon.at(0), amount));
        return insetPolygon;
    }

    /*
     * Auxiliary function to the insetPolygon function. This function will calculate the new position of a vertex,
     * based on the previous and next vertices of the polygon. It will return the new position for the point vertex.
     */
    static Vector2 insetVertex(Vector2 previous, Vector2 point, Vector2 next, float amount) {
        Vector2 prevCopy = Vector2(previous);
        Vector2 nextCopy = Vector2(next);
        Vector2 pointCopy1 = Vector2(point);
        Vector2 pointCopy2 = Vector2(point);

        // Calculate length of line segments.
        Vector2 line1 = (point - previous);
        Vector2 line2 = (next - point);
        float length1 = line1.getLength();
        float length2 = line2.getLength();

        // Exit if either segment is zero-length, and return the original point
        if (length1 == 0.0f || length2 == 0.0f) return point;

        // Inset each of the two line segments.
        float insetX =  line1.y / length1 * amount;
        float insetY = -line1.x / length1 * amount;
        prevCopy.x += insetX;
        prevCopy.y += insetY;
        pointCopy1.x += insetX;
        pointCopy1.y += insetY;

        insetX =  line2.y / length2 * amount;
        insetY = -line2.x / length2 * amount;
        nextCopy.x += insetX;
        nextCopy.y += insetY;
        pointCopy2.x += insetX; 
        pointCopy2.y += insetY;

        // If inset segments connect perfectly, return the connection point.
        if (pointCopy1 == pointCopy2) {
            return pointCopy1;
        }

        // Return the intersection point of the two inset segments (if any).
        Vector2 intersection = Geom::lineIntersection(prevCopy, pointCopy1, pointCopy2, nextCopy);
        if (intersection.x != MAX_INT) {
            return intersection;
        }
        // If the lines doesn't intersect, it means they are opposed 180�, so return the original point.
        return point;
    }

    /*
     * Calculates and returns the intersection point between two lines. Line 1 is the line between the points line1A
     * and line1B, and line 2 is the line between the points line2A and line2B. If the lines doesn't intersect (if they
     * are parallel, this function will return Vector2(MAX_INT, MAX_INT).
     */
    static Vector2 lineIntersection(Vector2 line1A, Vector2 line1B, Vector2 line2A, Vector2 line2B) {
        Vector2 x = line2A - line1A;
        Vector2 d1 = line1B - line1A;
        Vector2 d2 = line2B - line2A;

        float cross = d1.x * d2.y - d1.y * d2.x;
        if (std::abs(cross) < /*EPS*/ 1e-8)
            return Vector2((float) MAX_INT, (float) MAX_INT);

        float t1 = (x.x * d2.y - x.y * d2.x) / cross;
        return line1A + d1 * t1;
    }

    /*
     * Calculates and returns the intersection point between two lines. Line 1 is the line between the points line1A
     * and line1B, and line 2 is the line between the points line2A and line2B. If the lines doesn't intersect (if they
     * are parallel, this function will return Vector2(MAX_INT, MAX_INT).
     */
    static Vector2 lineSegmentIntersection(Vector2 lineA, Vector2 lineB, Vector2 segA, Vector2 segB) {
        // First check if the segment intersects the line. To do this, we check if the end points of the segment are
        // on different sides of the line, if they are both on the same side, they do not intersect.
        int signA = sign((lineB.x - lineA.x) * (segA.y - lineA.y) - (lineB.y - lineA.y) * (segA.x - lineA.x));
        int signB = sign((lineB.x - lineA.x) * (segB.y - lineA.y) - (lineB.y - lineA.y) * (segB.x - lineA.x));
        if (signA != signB) {
            // If they intersect, we can use the generic lineIntersection algorithm to find the exact point.
            return lineIntersection(lineA, lineB, segA, segB);
        }
        // In case they don't intersect
        return Vector2((float) MAX_INT, (float) MAX_INT);
    }

    /* Checks and returns true if a given point is within the specified line. */
    static bool isPointOnLine(Vector2 lineA, Vector2 lineB, Vector2 point) {
        float xDiff = fabs(lineA.x - lineB.x);
        float yDiff = fabs(lineA.y - lineB.y);
        if (xDiff < EPS && yDiff < EPS) //Not a line
            return false;
        if(yDiff < EPS) //No gradient
            return (fabs(lineA.y - point.y) < EPS) ? true : false;
        if(xDiff < EPS) //Infinite gradient
            return (fabs(lineA.x - point.x) < EPS) ? true : false;
        float a = (lineB.y - lineA.y) / (lineB.x - lineA.x);
        float b = lineA.y - a * lineA.x;
        if (fabs(point.y - (a * point.x + b)) < 0.1f) {
            return true;
        }
        return false;
    }

    /* Calculates and returns the normal (perpendicular vector) of a line between A and B. */
    static Vector2 vec2Normal(const Vector2 &a, const Vector2 &b) {
        float dx = b.x - a.x;
        float dy = b.y - a.y;
        return Vector2(dy, -dx);
    }

protected:

    Geom(void) {};
    ~Geom(void) {};

};