/*
 * Class: Vector2
 * Authors: Richard Davison, Rodrigo Castro
 * Description: Simple Vector2 class.
 *
 * -_-_-_-_-_-_-_,------,
 * _-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
 * -_-_-_-_-_-_-~|__( ^ .^) /
 * _-_-_-_-_-_-_-""  ""
 *
 */

#pragma once

#include <iostream>

class Vector2 {
public:

    Vector2(void) {
        toZero();
    }

    Vector2(const float x, const float y) {
        this->x = x;
        this->y = y;
    }

    Vector2(Vector2 &copy) {
        this->x = copy.x;
        this->y = copy.y;
    }

    ~Vector2(void) {}

    float x;
    float y;

    /* Set both values of this vector to zero. */
    void toZero() {
        x = y = 0.0f;
    }

    Vector2 normalised() {
        Vector2 normalised = Vector2(*this);
        float length = getLength();
        if (length != 0.0f) {
            length = 1.0f / length;
            normalised.x = x * length;
            normalised.y = y * length;
        }
        return normalised;
    }

    
    /* Returns the calculated length of this vector. The length is the square root of the sum of the axis squared. */
    float getLength() const {
        return sqrt((x * x) + (y * y));
    }

    /* Inverts the vector. */
    void invert() {
        x = -x;
        y = -y;
    }

    /* Returns a inverse version of this vector, without modifying it. */
    inline Vector2 inverse() const {
        return Vector2(-x, -y);
    }

    /* Prints the vector to the output stream in the format "Vector2(x, y)". */
    inline friend std::ostream& operator<<(std::ostream& o, const Vector2& v){
        o << "Vector2(" << v.x << ", " << v.y << ")" << std::endl;
        return o;
    }

    /*
     * ========== Operators ==========
     */

    inline Vector2 operator=(const Vector2 &other) {
        this->x = other.x;
        this->y = other.y;
        return *this;
    }

    inline Vector2 operator+(const Vector2 &a) const {
        return Vector2(x + a.x, y + a.y);
    }

    inline Vector2 operator+=(const Vector2 &a) {
        this->x += a.x;
        this->y += a.y;
        return *this;
    }
    
    inline Vector2 operator-() const {
        return Vector2(-x, -y);
    }

    inline Vector2 operator-(const Vector2 &a) const {
        return Vector2(x - a.x, y - a.y);
    }

    inline Vector2 operator-=(const Vector2 &a) {
        this->x -= a.x;
        this->y -= a.y;
        return *this;
    }

    inline Vector2 operator*(const Vector2 &a) const {
        return Vector2(x * a.x, y * a.y);
    }

    inline Vector2 operator*(const float a) const {
        return Vector2(x * a, y * a);
    }

    inline Vector2 operator*=(const Vector2 &a) {
        this->x *= a.x;
        this->y *= a.y;
        return *this;
    }

    inline Vector2 operator*=(const float a) {
        this->x *= a;
        this->y *= a;
        return *this;
    }

    inline Vector2 operator/(const Vector2 &a) const {
        return Vector2(x / a.x, y / a.y);
    }

    inline Vector2 operator/(const float &a) const {
        return Vector2(x / a, y / a);
    }

    inline Vector2 operator/=(const Vector2 &a) {
        this->x /= a.x;
        this->y /= a.y;
        return *this;
    }

    inline Vector2 operator/=(const float &a) {
        this->x /= a;
        this->y /= a;
        return *this;
    }

    inline bool	operator==(const Vector2 &a) const { return (a.x == x && a.y == y) ? true : false; }
    inline bool	operator!=(const Vector2 &a) const { return (a.x == x && a.y == y) ? false : true; }

};