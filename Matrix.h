/******************************************************************************/
/*!
\file Matrix.h
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1250
\par Project #01
\date 02/08/2012
\brief
Interface for handling matrix operations
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#ifndef MATRIX_H
#define MATRIX_H

#include<GL/glew.h>
#include "Math.h"

//!  Matrix structure 
/*!
Class for simple camera management(UP,DOWN,LEFT,RIGHT,LOOKAT)
*/
struct Matrix
{
	GLfloat t[4][4];	/*!< 4x4 2-dimensional array of floats */
	Matrix();
	Matrix(const float *load);
	void MultMatrix(const Matrix &n);
	void MultMatrix(const float *right,const unsigned int &size);
	void MultMatrix(const Point &p);
	void MultMatrix(const Vector &v);
	void setIdentity();
	float Determinant() const;
};

void Transpose(const Matrix &in,Matrix &out);
bool Inverse(const Matrix &in,Matrix &out);

#endif