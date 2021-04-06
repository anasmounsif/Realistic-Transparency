#pragma once
#include "vector3D.h"
#include "point3D.h"

class matrix4D {

public:

	float	m[4][4];								// elements

	matrix4D(void) {
		m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = 0.0;
		m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = 0.0;
		m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = 0.0;
		m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
	}

	matrix4D(const matrix4D& mat);						// copy constructor

	matrix4D::~matrix4D(void) {}

	float& operator ()(int i, int j) {
		return (m[i][j]);
	}
	
	const float& operator ()(int i, int j) const {
		return (m[i][j]);
	}
	
	matrix4D& operator= (const matrix4D& rhs);

	matrix4D operator* (const matrix4D& mat) const;

	matrix4D operator/ (const float d);
};

matrix4D::matrix4D(const matrix4D& mat) {
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			m[x][y] = mat.m[x][y];
}

matrix4D transponse(const matrix4D& mat) {
	matrix4D transp;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			transp.m[i][j] = mat.m[j][i];
			transp.m[j][i] = mat.m[i][j];
		}
	}
	return transp;
}

matrix4D& matrix4D::operator= (const matrix4D& rhs) {
	if (this == &rhs)
		return (*this);

	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			m[x][y] = rhs.m[x][y];

	return (*this);
}

matrix4D matrix4D::operator* (const matrix4D& mat) const {
	matrix4D 	product;

	for (int y = 0; y < 4; y++)
		for (int x = 0; x < 4; x++) {
			float sum = 0.0;

			for (int j = 0; j < 4; j++)
				sum += m[x][j] * mat.m[j][y];

			product.m[x][y] = sum;
		}

	return (product);
}

matrix4D matrix4D::operator/ (const float d) {
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			m[x][y] = m[x][y] / d;

	return (*this);
}

void set_identity(matrix4D& mat) {
	mat.m[0][0] = 1.0; mat.m[0][1] = 0.0; mat.m[0][2] = 0.0; mat.m[0][3] = 0.0;
	mat.m[1][0] = 0.0; mat.m[1][1] = 1.0; mat.m[1][2] = 0.0; mat.m[1][3] = 0.0;
	mat.m[2][0] = 0.0; mat.m[2][1] = 0.0; mat.m[2][2] = 1.0; mat.m[2][3] = 0.0;
	mat.m[3][0] = 0.0; mat.m[3][1] = 0.0; mat.m[3][2] = 0.0; mat.m[3][3] = 1.0;
}


vector3D operator* (const matrix4D& mat, const vector3D& v) {
	return (vector3D(mat.m[0][0] * v.x + mat.m[0][1] * v.y + mat.m[0][2] * v.z,
		mat.m[1][0] * v.x + mat.m[1][1] * v.y + mat.m[1][2] * v.z,
		mat.m[2][0] * v.x + mat.m[2][1] * v.y + mat.m[2][2] * v.z));
}

point3D operator* (const matrix4D& mat, const point3D& p) {
	return (point3D(mat.m[0][0] * p.x + mat.m[0][1] * p.y + mat.m[0][2] * p.z + mat.m[0][3],
		mat.m[1][0] * p.x + mat.m[1][1] * p.y + mat.m[1][2] * p.z + mat.m[1][3],
		mat.m[2][0] * p.x + mat.m[2][1] * p.y + mat.m[2][2] * p.z + mat.m[2][3]));
}












