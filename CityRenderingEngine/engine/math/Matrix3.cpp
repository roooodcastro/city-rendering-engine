#include "Matrix3.h"

Matrix3::Matrix3(void) {
	toIdentity();
}

Matrix3::Matrix3(const Matrix4 &m4) {
	toZero();
	values[0] = m4.values[0];
	values[1] = m4.values[1];
	values[2] = m4.values[2];
	values[3] = m4.values[4];
	values[4] = m4.values[5];
	values[5] = m4.values[6];
	values[6] = m4.values[8];
	values[7] = m4.values[9];
	values[8] = m4.values[10];
}

Matrix3::~Matrix3(void) {
	toIdentity();
}

void Matrix3::toIdentity() {
	toZero();
	values[0] = 1.0f;
	values[4] = 1.0f;
	values[8] = 1.0f;
}

void Matrix3::toZero() {
	for(int i = 0; i < 9; i++) {
		values[i] = 0.0f;
	}
}