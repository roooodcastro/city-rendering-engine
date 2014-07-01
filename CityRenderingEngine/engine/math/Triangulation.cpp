#include "Triangulation.h"

float Triangulation::calculateArea(const std::vector<Vector2> &polygon) {
    int numSides = (int) polygon.size();
    float area = 0.0f;
    for (int p = numSides - 1, q = 0; q < numSides; p = q++) {
        area += polygon.at(p).x * polygon.at(q).y - polygon.at(q).x * polygon.at(p).y;
    }
    return area * 0.5f;
}

bool Triangulation::isInsideTriangle(const Vector2 &point, const Vector2 &a, const Vector2 &b, const Vector2 &c) {
    float cCrossAP, bCrossCP, aCrossBP;
    Vector2 diffA = c - b;
    Vector2 diffB = a - c;
    Vector2 diffC = b - a;
    Vector2 diffAP = point - a;
    Vector2 diffBP = point - b;
    Vector2 diffCP = point - c;
    aCrossBP = diffA.x * diffBP.y - diffA.y * diffBP.x;
    cCrossAP = diffC.x * diffAP.y - diffC.y * diffAP.x;
    bCrossCP = diffB.x * diffCP.y - diffB.y * diffCP.x;
    return ((aCrossBP >= 0.0f) && (bCrossCP >= 0.0f) && (cCrossAP >= 0.0f));
};

bool Triangulation::snip(const std::vector<Vector2> &polygon, int u, int v, int w, int n, int *V) {
    Vector2 a, b, c, point;

    a = Vector2(polygon.at(V[u]).x, polygon.at(V[u]).y);
    b = Vector2(polygon.at(V[v]).x, polygon.at(V[v]).y);
    c = Vector2(polygon.at(V[w]).x, polygon.at(V[w]).y);

    if (EPS > (((b.x - a.x) * (c.y - a.y)) - ((b.y - a.y) * (c.x - a.x)))) return false;

    for (int p = 0; p < n; p++) {
        if ((p == u) || (p == v) || (p == w)) continue;
        point = Vector2(polygon.at(V[p]).x, polygon.at(V[p]).y);
        if (isInsideTriangle(point, a, b, c)) return false;
    }
    return true;
}

bool Triangulation::triangulate(const std::vector<Vector2> &polygon, std::vector<Vector2> &result) {
    /* Allocate and initialize list of vertices in polygon */
    int numSides = (int) polygon.size();
    if (numSides < 3) return false;

    int *vertexIndices = new int[numSides];

    /* Order vertexIndices so we get conter-clockwise indexes on it. */
    if (0.0f < calculateArea(polygon)) {
        for (int v = 0; v < numSides; v++)
            vertexIndices[v] = v;
    } else {
        for(int v = 0; v < numSides; v++)
            vertexIndices[v] = (numSides - 1) - v;
    }
    int nv = numSides;

    /* Remove nv-2 Vertices, creating 1 triangle every time */
    int count = 2 * nv;   /* error detection */

    for (int m = 0, v = nv - 1; nv > 2;) {
        /* If we loop, it is probably a non-simple polygon */
        if (0 >= (count--)) {
            /* Triangulate: ERROR - probable bad polygon! */
            return false;
        }

        /* Three consecutive vertices in current polygon, <u,v,w> */
        int u = v;
        if (nv <= u)
            u = 0; // Previous
        v = u + 1;
        if (nv <= v)
            v = 0; // New v
        int w = v + 1;
        if (nv <= w)
            w = 0; // Next

        if (snip(polygon, u, v, w, nv, vertexIndices)) {
            int a, b, c, s, t;
            /* True names of the vertices */
            a = vertexIndices[u];
            b = vertexIndices[v];
            c = vertexIndices[w];
            /* Output Triangle */
            result.push_back(polygon.at(a));
            result.push_back(polygon.at(b));
            result.push_back(polygon.at(c));
            m++;
            /* Remove v from remaining polygon */
            for (s = v, t = v + 1; t < nv; s++, t++) {
                vertexIndices[s] = vertexIndices[t];
            }
            nv--;
            /* Resest error detection counter */
            count = 2 * nv;
        }
    }
    delete vertexIndices;
    return true;
}