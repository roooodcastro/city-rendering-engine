/*
 * Authors: Richard Davison & Rodrigo Castro Azevedo
 * Date: 16/05/2014
 *
 * Description: Vector3 class to represent values such as position, rotation, scaling, etc.
 *
 * -_-_-_-_-_-_-_,------,
 * _-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
 * -_-_-_-_-_-_-~|__( ^ .^) /
 * _-_-_-_-_-_-_-""  ""
 *
 */

#pragma once

#include <cmath>
#include <iostream>

class Vector3 {
public:

    Vector3(void) {
        toZero();
    }

    Vector3(const Vector3 &copy) {
        this->x = copy.x;
        this->y = copy.y;
        this->z = copy.z;
    }

    Vector3(const float x, const float y, const float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    ~Vector3(void) {}

    float x;
    float y;
    float z;

    /* Normalises this vector. */
    void normalise() {
        float length = getLength();

        if (length != 0.0f) {
            length = 1.0f / length;
            x = x * length;
            y = y * length;
            z = z * length;
        }
    }

    /* Returns a normalised version of this vector, without modifying it. */
    Vector3 normalised() {
        Vector3 normalised = Vector3(*this);
        float length = getLength();

        if (length != 0.0f) {
            length = 1.0f / length;
            normalised.x = x * length;
            normalised.y = y * length;
            normalised.z = z * length;
        }
        return normalised;
    }

    /* Set all three values of this vector to zero. */
    void toZero() {
        x = y = z = 0.0f;
    }

    /* Returns the calculated length of this vector. The length is the square root of the sum of the axis squared. */
    float getLength() const {
        return sqrt((x * x) + (y * y) + (z * z));
    }

    /* Inverts the vector. */
    void invert() {
        x = -x;
        y = -y;	
        z = -z;	
    }

    /* Returns a inverse version of this vector, without modifying it. */
    Vector3 inverse() const {
        return Vector3(-x, -y, -z);
    }

    /* Performs and returns the dot operator between two vectors. Both vectors remain unmodified. */
    static float dot(const Vector3 &a, const Vector3 &b) {
        return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
    }

    /* Performs and returns the cross operator between two vectors. Both vectors remain unmodified. */
    static Vector3 cross(const Vector3 &a, const Vector3 &b) {
        return Vector3((a.y*b.z) - (a.z*b.y), (a.z*b.x) - (a.x*b.z), (a.x*b.y) - (a.y*b.x));	
    }

    /* Prints the vector to the output stream in the format "Vector3(x, y, z)". */
    inline friend std::ostream& operator<<(std::ostream& o, const Vector3& v) {
        o << "Vector3(" << v.x << ", " << v.y << ", " << v.z <<")" << std::endl;
        return o;
    }

    /*
     * These methods return normalized vectors that point in the named direction.
     */

    /* Returns a unit vector pointing upwards. */
    static Vector3 up() { return Vector3(0, 1, 0); }

    /* Returns a unit vector pointing downwards. */
    static Vector3 down() { return Vector3(0, -1, 0); }
    
    /* Returns a unit vector pointing left. */
    static Vector3 left() { return Vector3(-1, 0, 0); }
    
    /* Returns a unit vector pointing right. */
    static Vector3 right() { return Vector3(1, 0, 0); }
    
    /* Returns a unit vector pointing forwards. */
    static Vector3 forward() { return Vector3(0, 0, 1); }
    
    /* Returns a unit vector pointing backwards. */
    static Vector3 backward() { return Vector3(0, 0, -1); }
    
    /*
     * ========== Operators ==========
     */

    inline Vector3 operator+(const Vector3 &a) const {
        return Vector3(x + a.x, y + a.y, z + a.z);
    }

    inline Vector3 operator-(const Vector3 &a) const {
        return Vector3(x - a.x, y - a.y, z - a.z);
    }

    inline Vector3 operator-() const {
        return Vector3(-x, -y, -z);
    }

    inline void operator+=(const Vector3 &a) {
        x += a.x;
        y += a.y;
        z += a.z;
    }

    inline void operator-=(const Vector3 &a) {
        x -= a.x;
        y -= a.y;
        z -= a.z;
    }

    inline Vector3 operator*(const float a) {
        return Vector3(x * a, y * a, z * a);
    }

    inline Vector3 operator*(const Vector3 &a) {
        return Vector3(x * a.x, y * a.y, z * a.z);
    }

    inline Vector3 operator/(const Vector3 &a) {
        return Vector3(x / a.x, y / a.y, z / a.z);
    }

    inline Vector3 operator/(const float v) {
        return Vector3(x / v, y / v, z / v);
    }

    inline Vector3 operator=(const Vector3 &other) {
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
        return *this;
    }

    inline bool	operator==(const Vector3 &a) const { return (a.x == x && a.y == y && a.z == z) ? true : false; }
    inline bool	operator!=(const Vector3 &a) const { return (a.x == x && a.y == y && a.z == z) ? false : true; }
};