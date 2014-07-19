/******************************************************************************/
/*!
\file ObjectWrapper.cpp
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1250
\par Project #01
\date 02/08/2012
\brief
Implementation of Wrapper of objects for this project
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#include "ObjectWrapper.h"

///Initialize static instance of Wrapper
Wrapper * Wrapper::s_pInstance = 0;

/******************************************************************************/
/*!
Constructor for Wrapper class
*/
/******************************************************************************/
Wrapper::Wrapper()
{
}

/******************************************************************************/
/*!
Destructor for Wrapper class
*/
/******************************************************************************/
Wrapper::~Wrapper()
{
}

/******************************************************************************/
/*!
Create instance of Wrapper class
\return
void
*/
/******************************************************************************/
void Wrapper::create()
{
	s_pInstance = new Wrapper;
}

/******************************************************************************/
/*!
Destroy(deallocate) instance of Wrapper class
\return
void
*/
/******************************************************************************/
void Wrapper::destroy()
{
	delete s_pInstance;
}

/******************************************************************************/
/*!
Provide static instance of Wrapper class
\return
Reference to Wrapper instance
*/
/******************************************************************************/
Wrapper & Wrapper::Instance()
{
	return *s_pInstance;
}

/******************************************************************************/
/*!
Create the Vertices and Indices for all the geometric objects
\return
void
*/
/******************************************************************************/
void Wrapper::createGeometries()
{
	for(unsigned int i = 0;i < 15; ++i)
	{
		sphere[i].createGeometry(255,255,0);
	}
	for(unsigned int i = 15;i < 21; ++i)
	{
		sphere[i].createGeometry(129,239,239);
	}
	for(unsigned int i = 0;i < 10; ++i)
	{
		line[i].createGeometry(255,255,0);
	}
//	grid[0].addTexture("2.tga");
	grid[0].createGeometry(255,255,0);
}

/******************************************************************************/
/*!
Upload the geometries to the GPU
\return
void
*/
/******************************************************************************/
void Wrapper::uploadGeometries()
{
	for(unsigned int i = 0;i < 21; ++i)
	{
		sphere[i].uploadGeometry();
	}
	for(unsigned int i = 0;i < 10; ++i)
	{
		line[i].uploadGeometry();
	}
	grid[0].uploadGeometry();
}

/******************************************************************************/
/*!
Destroy the geometries - Unbind the buffers, delete the vertices and indices info
\return
void
*/
/******************************************************************************/
void Wrapper::destroyGeometries()
{
	for(unsigned int i = 0;i < 21; ++i)
	{
		sphere[i].destroyGeometry();
	}
	for(unsigned int i = 0;i < 10; ++i)
	{
		line[i].destroyGeometry();
	}	
	grid[0].destroyGeometry();
}

/******************************************************************************/
/*!
Create and Add the physics objects to the Vector
\return
void
*/
/******************************************************************************/
void Wrapper::createAndAddObjects()
{
	mSpace.limit_l = -0.5f;
	mSpace.limit_r = 0.5f;
	for(unsigned int i = 0;i < 15;++i)
	{
		p_sphere[i] = new GameObject(&sphere[i],circularwall);
	}
	for(unsigned int i = 15;i < 21;++i)
	{
		p_sphere[i] = new GameObject(&sphere[i],ball);
	}

	for(unsigned int i = 0; i < 10; ++i)
		p_line[i] = new GameObject(&line[i],straightwall);
		
	tile[0] = new GameObject(&grid[0],straightwall);
	for(unsigned int i = 0; i < 21; ++i)
		listOfObjects.push_back(p_sphere[i]);
	for(unsigned int i = 0; i < 10; ++i)
		listOfObjects.push_back(p_line[i]);
}

/******************************************************************************/
/*!
Destroy the physics objects
\return
void
*/
/******************************************************************************/
void Wrapper::destroyObjects()
{
	for(unsigned int i = 0;i < 21; ++i)
		delete p_sphere[i];
		
	for(unsigned int i = 0;i < 10; ++i)
		delete p_line[i];

	delete tile[0];
}

/******************************************************************************/
/*!
Initialize the properties of the physics objects
\return
void
*/
/******************************************************************************/
void Wrapper::InitializePhysics()
{
	tile[0]->translate.upload(5.0f,0.0f,0.0f);
	tile[0]->geoType->local.add(&tile[0]->translate);

	float scaleFactor;
	p_sphere[0]->dynamic = false;
	p_sphere[0]->p.x = 0.0f;
	p_sphere[0]->p.y = -8.0f;
	p_sphere[0]->p.z = 0.0f;
	p_sphere[0]->radius = 0.75f;

	p_sphere[1]->dynamic = false;
	p_sphere[1]->p.x = 2.0f;
	p_sphere[1]->p.y = -8.0f;
	p_sphere[1]->p.z = 0.0f;
	p_sphere[1]->radius = 0.5f;

	p_sphere[2]->dynamic = false;
	p_sphere[2]->p.x = 6.0f;
	p_sphere[2]->p.y = -5.0f;
	p_sphere[2]->p.z = 0.0f;
	p_sphere[2]->radius = 0.5f;

	p_sphere[3]->dynamic = false;
	p_sphere[3]->p.x = -6.0f;
	p_sphere[3]->p.y = -5.0f;
	p_sphere[3]->p.z = 0.0f;
	p_sphere[3]->radius = 0.5f;

	p_sphere[4]->dynamic = false;
	p_sphere[4]->p.x = -2.0f;
	p_sphere[4]->p.y = -8.0f;
	p_sphere[4]->p.z = 0.0f;
	p_sphere[4]->radius = 0.5f;

	p_sphere[5]->dynamic = false;
	p_sphere[5]->p.x = -2.0f;
	p_sphere[5]->p.y = 8.0f;
	p_sphere[5]->p.z = 0.0f;
	p_sphere[5]->radius = 0.5f;
	
	p_sphere[6]->dynamic = false;
	p_sphere[6]->p.x = -5.0f;
	p_sphere[6]->p.y = 3.0f;
	p_sphere[6]->p.z = 0.0f;
	p_sphere[6]->radius = 0.5f;

	p_sphere[7]->dynamic = false;
	p_sphere[7]->p.x = -4.5f;
	p_sphere[7]->p.y = -2.0f;
	p_sphere[7]->p.z = 0.0f;
	p_sphere[7]->radius = 0.5f;

	p_sphere[8]->dynamic = false;
	p_sphere[8]->p.x = 4.5f;
	p_sphere[8]->p.y = -2.0f;
	p_sphere[8]->p.z = 0.0f;
	p_sphere[8]->radius = 0.5f;

	p_sphere[9]->dynamic = false;
	p_sphere[9]->p.x = 5.0f;
	p_sphere[9]->p.y = 3.0f;
	p_sphere[9]->p.z = 0.0f;
	p_sphere[9]->radius = 0.5f;

	p_sphere[10]->dynamic = false;
	p_sphere[10]->p.x = 2.0f;
	p_sphere[10]->p.y = 8.0f;
	p_sphere[10]->p.z = 0.0f;
	p_sphere[10]->radius = 0.5f;

	p_sphere[11]->dynamic = false;
	p_sphere[11]->p.x = 6.0f;
	p_sphere[11]->p.y = 5.0f;
	p_sphere[11]->p.z = 0.0f;
	p_sphere[11]->radius = 0.5f;

	p_sphere[12]->dynamic = false;
	p_sphere[12]->p.x = -6.0f;
	p_sphere[12]->p.y = 5.0f;
	p_sphere[12]->p.z = 0.0f;
	p_sphere[12]->radius = 0.5f;

	p_sphere[13]->dynamic = false;
	p_sphere[13]->p.x = 0.0f;
	p_sphere[13]->p.y = 0.0f;
	p_sphere[13]->p.z = 0.0f;
	p_sphere[13]->radius = 0.5f;

	p_sphere[14]->dynamic = false;
	p_sphere[14]->p.x = 0.0f;
	p_sphere[14]->p.y = 4.0f;
	p_sphere[14]->p.z = 0.0f;
	p_sphere[14]->radius = 0.25f;
	
	p_sphere[15]->mass = 8.0f;
	p_sphere[15]->speed = 4.0f;
	p_sphere[15]->dynamic = true;
	p_sphere[15]->p.x = -2.0f;
	p_sphere[15]->p.y = 2.0f;
	p_sphere[15]->p.z = 0.0f;
	p_sphere[15]->v.x = -0.5f;
	p_sphere[15]->v.y = 1.0f;
	p_sphere[15]->v.z = 0.0f;
	p_sphere[15]->v.Normalize();
	p_sphere[15]->radius = 0.5f;

	p_sphere[16]->mass = 8.0f;
	p_sphere[16]->speed = 4.0f;
	p_sphere[16]->dynamic = true;
	p_sphere[16]->p.x = 2.0f;
	p_sphere[16]->p.y = 2.0f;
	p_sphere[16]->p.z = 0.0f;
	p_sphere[16]->v.x = 1.0f;
	p_sphere[16]->v.y = 0.5f;
	p_sphere[16]->v.z = 0.0f;
	p_sphere[16]->v.Normalize();
	p_sphere[16]->radius = 0.5f;
	
	p_sphere[17]->mass = 4.0f;
	p_sphere[17]->speed = 4.0f;
	p_sphere[17]->dynamic = true;
	p_sphere[17]->p.x = -2.0f;
	p_sphere[17]->p.y = -2.0f;
	p_sphere[17]->p.z = 0.0f;
	p_sphere[17]->v.x = -0.7f;
	p_sphere[17]->v.y = -0.3f;
	p_sphere[17]->v.z = 0.0f;
	p_sphere[17]->v.Normalize();
	p_sphere[17]->radius = 0.25f;

	p_sphere[18]->mass = 4.0f;
	p_sphere[18]->speed = 4.0f;
	p_sphere[18]->dynamic = true;
	p_sphere[18]->p.x = 2.0f;
	p_sphere[18]->p.y = -2.0f;
	p_sphere[18]->p.z = 0.0f;
	p_sphere[18]->v.x = 0.4f;
	p_sphere[18]->v.y = -0.2f;
	p_sphere[18]->v.z = 0.0f;
	p_sphere[18]->v.Normalize();
	p_sphere[18]->radius = 0.25f;

	p_sphere[19]->mass = 2.0f;
	p_sphere[19]->speed = 4.0f;
	p_sphere[19]->dynamic = true;
	p_sphere[19]->p.x = -2.0f;
	p_sphere[19]->p.y = 0.0f;
	p_sphere[19]->p.z = 0.0f;
	p_sphere[19]->v.x = -1.0f;
	p_sphere[19]->v.y = 1.0f;
	p_sphere[19]->v.z = 0.0f;
	p_sphere[19]->v.Normalize();
	p_sphere[19]->radius = 0.160f;

	p_sphere[20]->mass = 2.0f;
	p_sphere[20]->speed = 4.0f;
	p_sphere[20]->dynamic = true;
	p_sphere[20]->p.x = 2.0f;
	p_sphere[20]->p.y = 0.0f;
	p_sphere[20]->p.z = 0.0f;
	p_sphere[20]->v.x = 0.0f;
	p_sphere[20]->v.y = 1.0f;
	p_sphere[20]->v.z = 0.0f;
	p_sphere[20]->v.Normalize();
	p_sphere[20]->radius = 0.160f;

	for(unsigned int i = 0; i < 21; ++i)
	{
		/// Find the scaling factor for objects based on object radius and geometric modelspace
		scaleFactor = (p_sphere[i]->radius*2.0f)/(mSpace.limit_r - mSpace.limit_l);
		/// Upload the scale and translate transforms for the objects
		p_sphere[i]->scale.upload(scaleFactor,scaleFactor,scaleFactor);
		p_sphere[i]->translate.upload(p_sphere[i]->p.x,p_sphere[i]->p.y,p_sphere[i]->p.z);
		p_sphere[i]->geoType->local.add(&p_sphere[i]->translate);
		p_sphere[i]->geoType->local.add(&p_sphere[i]->scale);
	}
	
	p_line[0]->V0.x = 10.0f;
	p_line[0]->V0.y = -10.0f;
	p_line[0]->V0.z = 0.0f;
	p_line[0]->V1.x = 7.0f;
	p_line[0]->V1.y = 10.0f;
	p_line[0]->V1.z = 0.0f;
	
	p_line[1]->V0.x = 7.0f;
	p_line[1]->V0.y = 10.0f;
	p_line[1]->V0.z = 0.0f;
	p_line[1]->V1.x = -7.0f;
	p_line[1]->V1.y = 10.0f;
	p_line[1]->V1.z = 0.0f;

	p_line[2]->V0.x = -7.0f;
	p_line[2]->V0.y = 10.0f;
	p_line[2]->V0.z = 0.0f;
	p_line[2]->V1.x = -10.0f;
	p_line[2]->V1.y = -10.0f;
	p_line[2]->V1.z = 0.0f;

	p_line[3]->V0.x = -10.0f;
	p_line[3]->V0.y = -10.0f;
	p_line[3]->V0.z = 0.0f;
	p_line[3]->V1.x = 10.0f;
	p_line[3]->V1.y = -10.0f;
	p_line[3]->V1.z = 0.0f;

	p_line[4]->V0.x = 2.0f;
	p_line[4]->V0.y = -8.0f;
	p_line[4]->V0.z = 0.0f;
	p_line[4]->V1.x = 6.0f;
	p_line[4]->V1.y = -5.0f;
	p_line[4]->V1.z = 0.0f;

	p_line[5]->V0.x = -6.0f;
	p_line[5]->V0.y = -5.0f;
	p_line[5]->V0.z = 0.0f;
	p_line[5]->V1.x = -2.0f;
	p_line[5]->V1.y = -8.0f;
	p_line[5]->V1.z = 0.0f;

	p_line[6]->V0.x = -5.0f;
	p_line[6]->V0.y = 3.0f;
	p_line[6]->V0.z = 0.0f;
	p_line[6]->V1.x = -4.5f;
	p_line[6]->V1.y = -2.0f;
	p_line[6]->V1.z = 0.0f;

	p_line[7]->V0.x = 4.5f;
	p_line[7]->V0.y = -2.0f;
	p_line[7]->V0.z = 0.0f;
	p_line[7]->V1.x = 5.0f;
	p_line[7]->V1.y = 3.0f;
	p_line[7]->V1.z = 0.0f;

	p_line[8]->V0.x = 2.0f;
	p_line[8]->V0.y = 8.0f;
	p_line[8]->V0.z = 0.0f;
	p_line[8]->V1.x = 6.0f;
	p_line[8]->V1.y = 5.0f;
	p_line[8]->V1.z = 0.0f;

	p_line[9]->V0.x = -6.0f;
	p_line[9]->V0.y = 5.0f;
	p_line[9]->V0.z = 0.0f;
	p_line[9]->V1.x = -2.0f;
	p_line[9]->V1.y = 8.0f;
	p_line[9]->V1.z = 0.0f;

	for(unsigned int i = 0;i < 10; ++i)
	{
		p_line[i]->mass = 10.0f;
		p_line[i]->speed = 0.0f;
		p_line[i]->dynamic = false;
		p_line[i]->p.x = (p_line[i]->V0.x+p_line[i]->V1.x)/2;
		p_line[i]->p.y = (p_line[i]->V0.y+p_line[i]->V1.y)/2;
		p_line[i]->p.z = (p_line[i]->V0.z+p_line[i]->V1.z)/2;
		float length = sqrt(((p_line[i]->V1.x-p_line[i]->V0.x)*(p_line[i]->V1.x-p_line[i]->V0.x))
				   +((p_line[i]->V1.y-p_line[i]->V0.y)*(p_line[i]->V1.y-p_line[i]->V0.y))
				   +((p_line[i]->V1.y-p_line[i]->V0.y)*(p_line[i]->V1.z-p_line[i]->V0.z)));
		/// Find the scaling factor for objects based on object radius and geometric modelspace
		scaleFactor = length/(mSpace.limit_r - mSpace.limit_l);
		/// Upload the scale,translate and rotate transforms for the objects
		p_line[i]->scale.upload(scaleFactor,scaleFactor,scaleFactor);
		p_line[i]->translate.upload(p_line[i]->p.x,p_line[i]->p.y,p_line[i]->p.z);
		p_line[i]->rotate.upload(atan((p_line[i]->V1.y - p_line[i]->V0.y)/(p_line[i]->V1.x - p_line[i]->V0.x))*180.0f/M_PI,0.0f,0.0f,1.0f);
		p_line[i]->geoType->local.add(&p_line[i]->translate);
		p_line[i]->geoType->local.add(&p_line[i]->rotate);
		p_line[i]->geoType->local.add(&p_line[i]->scale);
	}
}