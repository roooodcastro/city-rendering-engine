#include "Matrix4.h"

Matrix4::Matrix4(void) {
    toIdentity();
}

Matrix4::Matrix4(Matrix3 &m) {
    values[0] = m.values[0];
    values[1] = m.values[1];
    values[2] = m.values[2];
    values[3] = 0.0f;
    values[4] = m.values[3];
    values[5] = m.values[4];
    values[6] = m.values[5];
    values[7] = 0.0f;
    values[8] = m.values[6];
    values[9] = m.values[7];
    values[10] = m.values[8];
    values[11] = 0.0f;
    values[12] = 0.0f;
    values[13] = 0.0f;
    values[14] = 0.0f;
    values[15] = 1.0f;
}

Matrix4::Matrix4(const Matrix4 &copy) {
    memcpy(this->values, copy.values, 16 * sizeof(float));
}

Matrix4::Matrix4(float elements[16]) {
    memcpy(this->values, elements, 16 * sizeof(float));
}

Matrix4::~Matrix4(void)	{
    toIdentity();
}

void Matrix4::toIdentity() {
    toZero();
    values[0]  = 1.0f;
    values[5]  = 1.0f;
    values[10] = 1.0f;
    values[15] = 1.0f;
}

void Matrix4::toZero() {
    for(int i = 0; i < 16; i++)	{
        values[i] = 0.0f;
    }
}

Vector3 Matrix4::getPositionVector() const {
    return Vector3(values[12], values[13], values[14]);
}

void Matrix4::setPositionVector(const Vector3 in) {
    values[12] = in.x;
    values[13] = in.y;
    values[14] = in.z;
}

Vector3 Matrix4::getScalingVector() const {
    return Vector3(values[0], values[5], values[10]);
}

void Matrix4::setScalingVector(const Vector3 &in) {
    values[0] = in.x;
    values[5] = in.y;
    values[10] = in.z;		
}

Matrix4 Matrix4::Perspective(float znear, float zfar, float aspect, float fov) {
    Matrix4 m;

    const float h = 1.0f / tan(fov * PI_OVER_360);
    float neg_depth = znear - zfar;

    m.values[0] = h / aspect;
    m.values[5] = h;
    m.values[10] = (zfar + znear) / neg_depth;
    m.values[11] = -1.0f;
    m.values[14] = 2.0f * (znear * zfar) / neg_depth;
    m.values[15] = 0.0f;

    return m;
}

// http://www.opengl.org/sdk/docs/man/xhtml/glOrtho.xml
Matrix4 Matrix4::Orthographic(float znear, float zfar, float right, float left, float top, float bottom)	{
    Matrix4 m;

    m.values[0] = 2.0f / (right - left);
    m.values[5] = 2.0f / (top - bottom);
    m.values[10] = -2.0f / (zfar - znear);
    m.values[12] = -(right + left) / (right - left);
    m.values[13] = -(top + bottom) / (top - bottom);
    m.values[14] = -(zfar + znear) / (zfar - znear);
    m.values[15] = 1.0f;

    return m;
}

Matrix4 Matrix4::buildViewMatrix(const Vector3 &from, const Vector3 &lookingAt, const Vector3 up) {
    Matrix4 r;
    r.setPositionVector(Vector3(-from.x, -from.y, -from.z));
    Matrix4 m;
    Vector3 f = (lookingAt - from);
    f.normalise();

    Vector3 s = Vector3::cross(f, up);
    Vector3 u = Vector3::cross(s, f);

    m.values[0] = s.x;
    m.values[4] = s.y;
    m.values[8] = s.z;

    m.values[1] = u.x;
    m.values[5] = u.y;
    m.values[9] = u.z;

    m.values[2] = -f.x;
    m.values[6] = -f.y;
    m.values[10] = -f.z;

    return m * r;
}

Matrix4 Matrix4::Rotation(Vector3 degrees) {
    Matrix3 mX, mY, mZ, out;
    out.toIdentity();
    // Rotate X
    if (degrees.x != 0) {
        float cosX = cos((float) toRadians(degrees.x));
        float sinX = sin((float) toRadians(degrees.x));
        mX.values[0] = 1.0f;
        mX.values[4] = cosX;
        mX.values[5] = sinX;
        mX.values[7] = -sinX;
        mX.values[8] = cosX;
        out = out * mX;
    }
    // Rotate Y
    if (degrees.y != 0) {
        float cosY = cos((float) toRadians(degrees.y));
        float sinY = sin((float) toRadians(degrees.y));
        mY.values[0] = cosY;
        mY.values[2] = -sinY;
        mY.values[4] = 1.0f;
        mY.values[6] = sinY;
        mY.values[8] = cosY;
        out = out * mY;
    }

    // Rotate Z
    if (degrees.z != 0) {
        float cosZ = cos((float) toRadians(degrees.z));
        float sinZ = sin((float) toRadians(degrees.z));
        mZ.values[0] = cosZ;
        mZ.values[1] = sinZ;
        mZ.values[3] = -sinZ;
        mZ.values[4] = cosZ;
        mZ.values[8] = 1.0f;
        out = out * mZ;
    }
    return Matrix4(out);
}

Matrix4 Matrix4::Rotation(float degrees, const Vector3 &inaxis)	{
    Matrix4 m;
    Vector3 axis = inaxis;
    float c = cos((float) toRadians(degrees));
    float s = sin((float) toRadians(degrees));

    m.values[0]  = (axis.x * axis.x) * (1.0f - c) + c;
    m.values[1]  = (axis.y * axis.x) * (1.0f - c) + (axis.z * s);
    m.values[2]  = (axis.z * axis.x) * (1.0f - c) - (axis.y * s);

    m.values[4]  = (axis.x * axis.y) * (1.0f - c) - (axis.z * s);
    m.values[5]  = (axis.y * axis.y) * (1.0f - c) + c;
    m.values[6]  = (axis.z * axis.y) * (1.0f - c) + (axis.x * s);

    m.values[8]  = (axis.x * axis.z) * (1.0f - c) + (axis.y * s);
    m.values[9]  = (axis.y * axis.z) * (1.0f - c) - (axis.x * s);
    m.values[10] = (axis.z * axis.z) * (1.0f - c) + c;

    return m;
}

Matrix4 Matrix4::Scale(const Vector3 &scale) {
    Matrix4 m;

    m.values[0] = scale.x;
    m.values[5] = scale.y;
    m.values[10] = scale.z;	

    return m;
}

Matrix4 Matrix4::Translation(const Vector3 &translation) {
    Matrix4 m;

    m.values[12] = translation.x;
    m.values[13] = translation.y;
    m.values[14] = translation.z;	

    return m;
}

Matrix4 Matrix4::inverse(const Matrix4 &input) {
    // Inversion by Cramer's rule.  Code taken from an Intel publication
    double result[4][4];
    double tmp[12]; /* temp array for pairs */
    double src[16];
    double det; /* determinant */
    /* transpose matrix */
    for (int i = 0; i < 4; i++) {
        src[i + 0] = input.values[(i * 4) + 0];
        src[i + 4] = input.values[(i * 4) + 1];
        src[i + 8] = input.values[(i * 4) + 2];
        src[i + 12] = input.values[(i * 4) + 3];
    }
    /* calculate pairs for first 8 elements (cofactors) */
    tmp[0] = src[10] * src[15];
    tmp[1] = src[11] * src[14];
    tmp[2] = src[9] * src[15];
    tmp[3] = src[11] * src[13];
    tmp[4] = src[9] * src[14];
    tmp[5] = src[10] * src[13];
    tmp[6] = src[8] * src[15];
    tmp[7] = src[11] * src[12];
    tmp[8] = src[8] * src[14];
    tmp[9] = src[10] * src[12];
    tmp[10] = src[8] * src[13];
    tmp[11] = src[9] * src[12];
    /* calculate first 8 elements (cofactors) */
    result[0][0] = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7];
    result[0][0] -= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7];
    result[0][1] = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7];
    result[0][1] -= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7];
    result[0][2] = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7];
    result[0][2] -= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7];
    result[0][3] = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6];
    result[0][3] -= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6];
    result[1][0] = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3];
    result[1][0] -= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3];
    result[1][1] = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3];
    result[1][1] -= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3];
    result[1][2] = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3];
    result[1][2] -= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3];
    result[1][3] = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2];
    result[1][3] -= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2];
    /* calculate pairs for second 8 elements (cofactors) */
    tmp[0] = src[2]*src[7];
    tmp[1] = src[3]*src[6];
    tmp[2] = src[1]*src[7];
    tmp[3] = src[3]*src[5];
    tmp[4] = src[1]*src[6];
    tmp[5] = src[2]*src[5];

    tmp[6] = src[0]*src[7];
    tmp[7] = src[3]*src[4];
    tmp[8] = src[0]*src[6];
    tmp[9] = src[2]*src[4];
    tmp[10] = src[0]*src[5];
    tmp[11] = src[1]*src[4];
    /* calculate second 8 elements (cofactors) */
    result[2][0] = tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15];
    result[2][0] -= tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15];
    result[2][1] = tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15];
    result[2][1] -= tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15];
    result[2][2] = tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15];
    result[2][2] -= tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15];
    result[2][3] = tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14];
    result[2][3] -= tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14];
    result[3][0] = tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
    result[3][0] -= tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
    result[3][1] = tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
    result[3][1] -= tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
    result[3][2] = tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
    result[3][2] -= tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
    result[3][3] = tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9];
    result[3][3] -= tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8];
    /* calculate determinant */
    det = src[0] * result[0][0] + src[1] * result[0][1] + src[2] * result[0][2] + src[3] * result[0][3];
    /* calculate matrix inverse */
    det = 1.0f / det;

    Matrix4 matrixResult;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            matrixResult.values[(i * 4) + j] = float(result[i][j] * det);
        }
    }
    return matrixResult;
}