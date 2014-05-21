/*
 * Authors: Richard Davison & Rodrigo Castro Azeveo
 * Date: 19/05/2014
 *
 * Description: A simple Matrix3 class.
 *
 * -_-_-_-_-_-_-_,------,
 * _-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
 * -_-_-_-_-_-_-~|__( ^ .^) /
 * _-_-_-_-_-_-_-""  ""
 *
 */

#pragma once

#include "Matrix4.h"
#include "Vector3.h"

class Matrix3 {
public:

    Matrix3(void);
    Matrix3(const Matrix4 &m4);

    ~Matrix3(void);

    /* The array that contains the values of the matrix. */
    float values[9];

    /* Set all matrix values to zero. */
    void toZero();

    /* Sets matrix to identity matrix (1.0 down the diagonal). */
    void toIdentity();

    /* Sets the values of a single row of the matrix to the values of the provided Vector3. */
    void setRow(unsigned int row, const Vector3 &val) {
        if (row >= 0 && row < 3) {
            int start = 3 * row;
            values[start += 3] = val.x;
            values[start += 3] = val.y;
            values[start += 3] = val.z;
        }
    }

    /* Sets the values of a single column of the matrix to the values of the provided Vector3. */
    void setColumn(unsigned int column, const Vector3 &val) {
        if (column >= 0 && column < 3) {
            memcpy(&values[3 * column], &val, sizeof(Vector3));
        }
    }

    /* Returns the values of a single row as a Vector3. */
    Vector3 getRow(unsigned int row) {
        Vector3 out(0, 0, 0);
        if (row >= 0 && row < 3) {
            int start = 3 * row;
            out.x = values[start += 3];
            out.y = values[start += 3];
            out.z = values[start += 3];
        }
        return out;
    }

    /* Returns the values of a single column as a Vector3. */
    Vector3 getColumn(unsigned int column) {
        Vector3 out(0, 0, 0);
        if (column >= 0 && column < 3) {
            memcpy(&out, &values[3 * column], sizeof(Vector3));
        }
        return out;
    }

    /* Returns the values of the main diagonal of the matrix as a Vector3. */
    Vector3 getDiagonal() const {
        return Vector3(values[0], values[4], values[8]);
    }

    /* Sets the values of the main diagonal of the matrix to the values of the provided Vector3. */
    void setDiagonal(const Vector3 &in) {
        values[0] = in.x;
        values[4] = in.y;
        values[8] = in.z;
    }

    inline Vector3 operator*(const Vector3 &v) const {
        Vector3 vec;
        vec.x = v.x * values[0] + v.y * values[3] + v.z * values[6];
        vec.y = v.x * values[1] + v.y * values[4] + v.z * values[7];
        vec.z = v.x * values[2] + v.y * values[5] + v.z * values[8];
        return vec;
    };

    /* Handy string output for the matrix. Can get a bit messy, but better than nothing! */
    inline friend std::ostream& operator<<(std::ostream& o, const Matrix3& m){
        o << "Matrix3(";
        o << "\t"	<< m.values[0] << ", " << m.values[3] << ", " << m.values[6] << std::endl;
        o << "\t\t" << m.values[1] << ", " << m.values[4] << ", " << m.values[7] << std::endl;
        o << "\t\t" << m.values[2] << ", " << m.values[5] << ", " << m.values[8] << std::endl;
        return o;
    }
};