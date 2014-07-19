/******************************************************************************/
/*!
\file ObjectGrid.h
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1050
\par Project #01
\date 21/12/2011
\brief
Interface for Grid mesh
Copyright (C) 2011 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#ifndef OBJECTGRID_H
#define OBJECTGRID_H

#include "ObjectBase.h"
#include<fstream>
#include<iostream>
#include "TGALoader.h"

//!  Grid class
/*!
Class for implementing Grid geometry Object
*/
class Grid : public BaseGeometry
{
	Grid(const Grid &);
	unsigned short subd_x,subd_y;	/*!< Number of subdivisions along x and y axis */
	unsigned short NUMBER_OF_INDICES;	/*!< Total number of indices */
	enum {	VBO,IBO,NUM_BUFFERS };	/*!< Generic enum. VBO = 0, IBO = 1, NUM_BUFFERS = 2 */
	unsigned int vbo[NUM_BUFFERS];	/*!< Vertex and Indices Buffer objects*/
	unsigned int g_vao[1];	/*!< Vertex array object */
	ModelSpace mSpace;	/*!< ModelSpace of the mesh */
	unsigned int texName[1];
	
public:
	TGA tgaObj;
/*	AffineTransform parentChild;	/*!< Transform for this composite object from its parent */
/*	AffineTransform local;	/*!< local transform for this object */
	Grid(const unsigned short &s_x = 3,const unsigned short &s_y = 3,const float &l = -0.5f,const float &r = 0.5f);
	~Grid();
	void createGeometry(const unsigned char &red,const unsigned char &green,const unsigned char &blue);
	void uploadGeometry();
	void drawGeometry();
	void destroyGeometry();
	void drawTransformedGeometry(MatrixStack &mv_s);
	bool addTexture(char *fileName);
};

#endif