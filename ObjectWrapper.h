/******************************************************************************/
/*!
\file ObjectWrapper.h
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1250
\par Project #01
\date 02/08/2012
\brief
Interface for Wrapper of geometric objects and their transforms for this project
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#ifndef OBJECTWRAPPER_H
#define OBJECTWRAPPER_H

#include "ObjectLine.h"
#include "ObjectSphere.h"
#include "Camera.h"
#include "GameObject.h"
#include "ObjectGrid.h"

//! Wrapper class
/*!
Class for implmenting wrapper of physics objects and geometric objects
*/
class Wrapper
{
	static Wrapper *s_pInstance;	/*! Pointer to a Wrapper object which is static*/
	Wrapper();
	~Wrapper();
	Wrapper(Wrapper &);
	Wrapper & operator = (Wrapper &);
public:
	Line line[10];		/*!< Geometric objects for line mesh */
	Sphere sphere[21];  /*!< Geometric objects for sphere mesh*/
	Grid grid[1];
	GameObject *p_sphere[21];	/*!< Physics object representing a geometric sphere*/
	GameObject *p_line[10];		/*!< Physics object representing a geometric line*/
	GameObject *tile[1];
	std::vector<GameObject *> listOfObjects;	/*!< Vector of physics objects */
	ModelSpace mSpace;		/*!< ModelSpace of the geometries */
	Camera camera;	/*!< Camera object of the project */
	static void create();
	static void destroy();
	static Wrapper &Instance();
	void InitializePhysics();
	void createAndAddObjects();
	void destroyObjects();
	void createGeometries();
	void uploadGeometries();
	void destroyGeometries();
};

#endif