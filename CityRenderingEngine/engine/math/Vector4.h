/*
 * Authors: Rich Davison & Rodrigo Castro Azevedo
 * Date: 19/05/2014
 *
 * Description: Simple Vector4 class.
 *
 * -_-_-_-_-_-_-_,------,   
 * _-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
 * -_-_-_-_-_-_-~|__( ^ .^) /
 * _-_-_-_-_-_-_-""  ""   
 *
 */

#pragma once

#include "Vector3.h"

class Vector4 {
public:

    Vector4(void) {
        x = y = z = w = 1.0f;
    }

    Vector4(const Vector4 &copy) {
        this->x = copy.x;
        this->y = copy.y;
        this->z = copy.z;
        this->w = copy.w;
    }

    Vector4(float x, float y, float z, float w) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    /* Creates a Vector4 using the values of a Vector3. The member 'w' is initialized with 1.0f. */
    Vector4(Vector3 &vecThree) {
        this->x = vecThree.x;
        this->y = vecThree.y;
        this->z = vecThree.z;
        this->z = 1.0f;
    }

    ~Vector4(void) {}

    float x;
    float y;
    float z;
    float w;
    
    /* Set all four values of this vector to zero. */
    void toZero() {
        x = y = z = w = 0.0f;
    }

    /* Returns the calculated length of this vector. The length is the square root of the sum of the axis squared. */
    float getLength() const {
        return sqrt((x * x) + (y * y) + (z * z) + (w * w));
    }
    
    /* Prints the vector to the output stream in the format "Vector4(x, y, z, w)". */
    inline friend std::ostream& operator<<(std::ostream& o, const Vector4& v) {
        o << "Vector4(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")" << std::endl;
        return o;
    }

    /*
     * ========== Operators ==========
     */

    inline Vector4 operator+(const Vector4 &a) const {
        return Vector4(x + a.x, y + a.y, z + a.z, w + a.w);
    }

    inline Vector4 operator-(const Vector4 &a) const {
        return Vector4(x - a.x, y - a.y, z - a.z, w - a.w);
    }

    inline Vector4 operator-() const {
        return Vector4(-x, -y, -z, -w);
    }

    inline void operator+=(const Vector4 &a) {
        x += a.x;
        y += a.y;
        z += a.z;
        w += a.w;
    }

    inline void operator-=(const Vector4 &a) {
        x -= a.x;
        y -= a.y;
        z -= a.z;
        w -= a.w;
    }

    inline Vector4 operator*(const float a) {
        return Vector4(x * a, y * a, z * a, w * a);
    }

    inline Vector4 operator*(const Vector4 &a) {
        return Vector4(x * a.x, y * a.y, z * a.z, w * a.w);
    }

    inline Vector4 operator/(const Vector4 &a) {
        return Vector4(x / a.x, y / a.y, z / a.z, w / a.w);
    }

    inline Vector4 operator/(const float v) {
        return Vector4(x / v, y / v, z / v, w / v);
    }

    inline Vector4 operator=(const Vector4 &other) {
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
        this->w = other.w;
        return *this;
    }

    inline bool	operator==(const Vector4 &a) const { return (a.x == x && a.y == y && a.z == z && a.w == w) ? true : false; }
    inline bool	operator!=(const Vector4 &a) const { return (a.x == x && a.y == y && a.z == z && a.w == w) ? false : true; }
    
    // Hacky stuff, don't use!
    /* Enables indexing this vector like an array. */
    float &operator[](int index) {
        switch (index) {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        case 3:
            return w;
        default:
            return w;
        }
    }
};