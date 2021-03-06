/*
 * Authors: Rich Davison & Rodrigo Castro Azevedo
 * Date: 19/05/2014
 *
 * Description: A simple 4 by 4 matrix class.
 *
 * -_-_-_-_-_-_-_,------,   
 * _-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
 * -_-_-_-_-_-_-~|__( ^ .^) /
 * _-_-_-_-_-_-_-""  ""   
 * 
 */

#pragma once

#include <iostream>
#include "common.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3.h"

class Vector3;
class Matrix3;

class Matrix4 {
public:

    Matrix4(void);
    Matrix4(Matrix3 &m);
    Matrix4(const Matrix4 &copy);
    Matrix4(float elements[16]);
    ~Matrix4(void);

    /* The array that contains the values of the matrix. */
    float values[16];

    /* Set all matrix values to zero. */
    void toZero();

    /* Sets matrix to identity matrix (1.0 down the diagonal). */
    void toIdentity();

    /* Sets the values of a single row of the matrix to the values of the provided Vector4. */
    void setRow(unsigned int row, const Vector4 &val) {
        if (row >= 0 && row <= 3) {
            int start = 4 * row;
            values[start += 4] = val.x;
            values[start += 4] = val.y;
            values[start += 4] = val.z;
            values[start += 4] = val.w;
        }
    }

    /* Sets the values of a single column of the matrix to the values of the provided Vector4. */
    void setColumn(unsigned int column, const Vector4 &val) {
        if (column >= 0 && column <= 3) {
            memcpy(&values[4 * column], &val, sizeof(Vector4));
        }
    }

    /* Returns the values of a single row as a Vector4. */
    Vector4 getRow(unsigned int row) {
        Vector4 out(0, 0, 0, 1);
        if (row >= 0 && row <= 3) {
            int start = 4 * row;
            out.x = values[start += 4];
            out.y = values[start += 4];
            out.z = values[start += 4];
            out.w = values[start += 4];
        }
        return out;
    }

    /* Returns the values of a single column as a Vector4. */
    Vector4 getColumn(unsigned int column) {
        Vector4 out(0, 0, 0, 1);
        if (column >= 0 && column <= 3) {
            memcpy(&out, &values[4 * column], sizeof(Vector4));
        }
        return out;
    }

    /* Gets the OpenGL position vector (floats 12, 13 and 14). */
    Vector3 getPositionVector() const;

    /* Sets the OpenGL position vector (floats 12, 13 and 14). */
    void setPositionVector(const Vector3 in);

    /* Gets the OpenGL scale vector (floats 1, 5 and 10). */
    Vector3 getScalingVector() const;

    /* Sets the OpenGL scale vector (floats 1, 5 and 10). */
    void setScalingVector(const Vector3 &in);

    /* Creates a rotation matrix that rotates all axis by the degree of each element of the vector degrees. */
    static Matrix4 Rotation(Vector3 degrees);

    /* Creates a rotation matrix that rotates by 'degrees' around the 'axis'. Analogous to glRotatef. */
    static Matrix4 Rotation(float degrees, const Vector3 &axis);

    /* Creates a scaling matrix (puts the 'scale' vector down the diagonal). Analogous to glScalef. */
    static Matrix4 Scale(const Vector3 &scale);

    /* Creates a translation matrix, with 'translation' vector at floats 12 13, and 14. Analogous to glTranslatef. */
    static Matrix4 Translation(const Vector3 &translation);

    /* Returns a inversed copy of the matrix. */
    static Matrix4 inverse(const Matrix4 &input);

    /*
     * Creates a perspective matrix, with 'znear' and 'zfar' as the near and far planes, using 'aspect' and 'fov' as
     * the aspect ratio and vertical field of vision, respectively.
     */
    static Matrix4 Perspective(float znear, float zfar, float aspect, float fov);

    /* Creates an orthographic matrix with 'znear' and 'zfar' as the near and far planes, and so on */
    static Matrix4 Orthographic(float znear, float zfar, float right, float left, float top, float bottom);

    /*
     * Builds a view matrix suitable for sending straight to the vertex shader. Puts the camera at 'from', with
     * 'lookingAt' centered on the screen, with 'up' as the...up axis (pointing towards the top of the screen)
     */
    static Matrix4 buildViewMatrix(const Vector3 &from, const Vector3 &lookingAt, const Vector3 up = Vector3(0, 1, 0));

    /*
     * ========== Operators ==========
     */

    /* Multiplies 'this' matrix by matrix 'a'. Performs the multiplication in 'OpenGL' order (ie, backwards) */
    inline Matrix4 operator*(const Matrix4 &a) const{	
        Matrix4 out;
        //Students! You should be able to think up a really easy way of speeding this up...
        for (unsigned int r = 0; r < 4; ++r) {
            int r4 = r * 4;
            for (unsigned int c = 0; c < 4; ++c) {
                int cr4 = c + r4;
                out.values[cr4] = 0.0f;
                for (unsigned int i = 0; i < 4; ++i) {
                    out.values[cr4] += this->values[c + (i * 4)] * a.values[(r4) + i];
                }
            }
        }
        return out;
    }

    inline Vector3 operator*(const Vector3 &v) const {
        Vector3 vec;
        float temp;
        vec.x = v.x * values[0] + v.y * values[4] + v.z * values[8]  + values[12];
        vec.y = v.x * values[1] + v.y * values[5] + v.z * values[9]  + values[13];
        vec.z = v.x * values[2] + v.y * values[6] + v.z * values[10] + values[14];
        temp =  v.x * values[3] + v.y * values[7] + v.z * values[11] + values[15];

        vec.x = vec.x / temp;
        vec.y = vec.y / temp;
        vec.z = vec.z / temp;
        return vec;
    };

    inline Vector4 operator*(const Vector4 &v) const {
        return Vector4(
            v.x * values[0] + v.y * values[4] + v.z * values[8]  + v.w * values[12],
            v.x * values[1] + v.y * values[5] + v.z * values[9]  + v.w * values[13],
            v.x * values[2] + v.y * values[6] + v.z * values[10] + v.w * values[14],
            v.x * values[3] + v.y * values[7] + v.z * values[11] + v.w * values[15]
        );
    };

    inline bool operator==(const Matrix4 &other) const {
        bool diff = false;
        for (int i = 0; i < 16; i++) {
            if (values[i] != other.values[i]) {
                diff = true;
                break;
            }
        }
        return !diff;
    };
    
    inline bool operator!=(const Matrix4 &other) const {
        bool diff = false;
        for (int i = 0; i < 16; i++) {
            if (values[i] != other.values[i]) {
                diff = true;
                break;
            }
        }
        return diff;
    };

    /* Handy string output for the matrix. Can get a bit messy, but better than nothing! */
    inline friend std::ostream& operator<<(std::ostream& o, const Matrix4& m){
        o << "Matrix4(";
        o << "\t"   << m.values[0] << ", " << m.values[4] << ", " << m.values[8] << ", " << m.values[12] << std::endl;
        o << "\t\t" << m.values[1] << ", " << m.values[5] << ", " << m.values[9] << ", " << m.values[13] << std::endl;
        o << "\t\t" << m.values[2] << ", " << m.values[6] << ", " << m.values[10] << ", "<< m.values[14] << std::endl;
        o << "\t\t" << m.values[3] << ", " << m.values[7] << ", " << m.values[11] << ", "<< m.values[15] << ")" <<std::endl;
        return o;
    }
};