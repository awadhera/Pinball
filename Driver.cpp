/******************************************************************************/
/*!
\file Driver.cpp
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1250
\par Project #01
\date 02/08/2012
\brief
Driver for the project
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#include <cstdlib>
#include <GL/glew.h>
#include <GL/wglew.h>
#include <sstream>		
#include <iostream>
#include "Logger.h"
#include "MatrixStack.h"
#include "Collision.h"
#include "ObjectWrapper.h"

/// stringstream typedef for use in logging while initializing openGL Render Context
typedef std::stringstream StringStream_t;

///Constants
const LPCTSTR LP_WINDOW_NAME = TEXT("CS1250_a.anshumanwadhera_1" ); /*! Window Name */
const LPCTSTR LP_CLASS_NAME = TEXT("Window_ClassName"); /*! Window Class */
const LPCTSTR LP_CREATE_WINDOW_ERROR = TEXT("Cannot create window"); /*! Window Error Message */

///Globals
HDC g_hDC = NULL; /*! Device context Handle */
HGLRC g_hGLRC = NULL; /*! openGL render context */
SIZE g_WinSize = { 0 }; /*! Viewport specific variable */
int flag = 1; /*! flag is used for handling WM_CLOSE message */
MatrixStack mv_s; /*! ModelView stack */
MatrixStack pr_s; /*! Projection stack */

///Function Prototypes
LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
void InitializeOpenGLRenderContext( HWND hWnd, HDC & hDC, HGLRC & hGLRC );
void InitializeOpenGLState( HWND hWnd );
void updateObjects( HWND , const float &frameStartTime, const float &ddt);
void drawObjects(HWND , HDC hDC);

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE /*hPrevInstance*/,
	LPSTR /*lpCmdLine*/,
	int nShowCmd )
{
	WNDCLASS wndClass = { 0 };
	MSG msg = { 0 };
	HWND hWnd;

	// Initialize window class
	wndClass.style			= CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc	= WndProc;
	wndClass.cbClsExtra		= 0;
	wndClass.cbWndExtra		= 0;
	wndClass.hInstance		= hInstance;
	wndClass.hIcon			= LoadIcon( NULL, IDI_APPLICATION );
	wndClass.hCursor		= LoadCursor( NULL, IDC_ARROW );
	wndClass.hbrBackground	= NULL;
	wndClass.lpszMenuName	= NULL;
	wndClass.lpszClassName	= LP_CLASS_NAME;

	// Register class name
	RegisterClass( &wndClass );

	///Create object of Singleton:Logger and Singleton::Wrapper
	Logger::create();
	Wrapper::create();
	Wrapper &wrapper = Wrapper::Instance();
	///Create the physics objects
	wrapper.createAndAddObjects();	

	/// Create the window 
	hWnd = CreateWindow(
		LP_CLASS_NAME,
		LP_WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL );

	/// If we were not able to create a window, display an error message and exit
	if( hWnd == NULL )
	{
		MessageBox( NULL, LP_CREATE_WINDOW_ERROR, LP_WINDOW_NAME, MB_OK );
		return FALSE;
	}
	/// Display the window
	/// Sets the specified window's show state
	ShowWindow( hWnd, nShowCmd );
	/// Evaluation of time taken by a frame.
	LARGE_INTEGER freq, oldCount, newCount;
	float startTime, dt;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&oldCount);
	startTime = 0.0f;

	/// Window's main loop
	while( IsWindow( hWnd ) )
	{
		while( PeekMessage( &msg, hWnd, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		if(flag)
		{
			QueryPerformanceCounter(&newCount);
			dt = ((newCount.QuadPart - oldCount.QuadPart)*1.0f/freq.QuadPart);
			oldCount = newCount;
			///Update the physics objects
			updateObjects( hWnd, startTime , dt );
			///Draw the geomtries represented by the physics objects
			drawObjects(hWnd,g_hDC);
		}
	}
	///Destroy the physics objects
	wrapper.destroyObjects();
	///Destroy the instances of Singleton:Wrapper and Singleton:Logger
	Wrapper::destroy();
	Logger::destroy();
	return static_cast<int>( msg.wParam );
}


/******************************************************************************/
/*!
The WndProc function is an application-defined function that processes
messages sent to a window

\param [in] hWnd
Handle to the window. 

\param [in] msg
Specifies the message. 

\param [in] wParam
Specifies additional message information. The contents of this parameter
depend on the value of the uMsg parameter. 

\param [in] lParam
Specifies additional message information. The contents of this parameter
depend on the value of the uMsg parameter. 

\return
The return value is the result of the message processing and depends on
the message sent.
*/
/******************************************************************************/
LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	static GLenum mode = GL_FILL;
	static bool isBackFaceEnabled = false;

	RECT rect;
	float aspectRatio;
	int diffX, diffY;
	float posX, posY, posZ, winZ;
	float centerX,centerY;
	Wrapper &wrapper = Wrapper::Instance();

	switch( msg )
	{
	case WM_CREATE:
		/// Initialize the physics objects
		wrapper.InitializePhysics();
		/// Set the initial camera position
		wrapper.camera.setCamera(0.0f,0.0f,50.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);
		/// Initialize openGL rendre context
		InitializeOpenGLRenderContext( hWnd, g_hDC, g_hGLRC );
		/// Initialize openGL state
		InitializeOpenGLState( hWnd );
		/// Create the geometries 
		wrapper.createGeometries();
		/// Upload the geometries to GPU
		wrapper.uploadGeometries();
		break;

	case WM_SIZE:
		// If the window is minimized, do not do anything
		if( wParam == SIZE_MINIMIZED )
			break;
		// Get the client rectangle
		GetClientRect( hWnd, &rect );
		// Compute the size of the window
		g_WinSize.cx = rect.right - rect.left;
		g_WinSize.cy = rect.bottom - rect.top;
		// Update viewport size within the client rectangle
		glViewport( 0, 0, g_WinSize.cx, g_WinSize.cy );
		(Logger::Instance()).checkAndReportGLError("Failed to set Viewport",'e',4,__LINE__,__FILE__);
		// If the height of the window is zero, we do not do anything
		if( g_WinSize.cy == 0 ) 
			break;
		/// Calculate aspect ratio
		aspectRatio = static_cast<float>( g_WinSize.cx ) / g_WinSize.cy;
		/// set the viewport width and height for camera
		wrapper.camera.vpWidth = g_WinSize.cx;
		wrapper.camera.vpHeight = g_WinSize.cy;
		// Set the projection matrix
		glMatrixMode( GL_PROJECTION );
		(Logger::Instance()).checkAndReportGLError("Failed to set Matrix mode to GL_PROJECTION",'e',4,__LINE__,__FILE__);
		// Set the projection matrix stack to IDENTITY and recalculate the projection matrix		
		pr_s.LoadIdentity();
		pr_s.MultMatrix(my_Ortho(-20.0f,20.0f,-20.0f * ( 1.0f / aspectRatio ),20.0f * ( 1.0f / aspectRatio ), 0.0f, 100.0f));
		/// Load the projection matrix on top of stack
		glLoadMatrixf(pr_s.last());
		(Logger::Instance()).checkAndReportGLError("Failed to load matrix on top of openGL Projection stack",'e',4,__LINE__,__FILE__);
		break;

	case WM_CHAR:
		switch( wParam ) 
		{
		case 0x1b: // ESC
			PostMessage( hWnd, WM_CLOSE, 0, 0 );
			break;
		case 'f':
		case 'F':
			/// Set object to be displayed as wired or solid
			mode = ( mode == GL_FILL ) ? GL_LINE : GL_FILL;
			glPolygonMode( GL_FRONT_AND_BACK, mode );
			(Logger::Instance()).checkAndReportGLError("Failed to change polygon mode",'e',4,__LINE__,__FILE__);
			break;
		case 'C':
		case 'c':
			/// Cull/Uncull the object
			isBackFaceEnabled = !isBackFaceEnabled;
			if( isBackFaceEnabled )
			{
				glEnable( GL_CULL_FACE );
				(Logger::Instance()).checkAndReportGLError("Failed to enable culling",'e',4,__LINE__,__FILE__);
			}
			else
			{
				glDisable( GL_CULL_FACE );
				(Logger::Instance()).checkAndReportGLError("Failed to disable culling",'e',4,__LINE__,__FILE__);
			}
			break;
		case 'w':
		case 'W':
			/// Move camera front
			wrapper.camera.moveFront();
			break;
		case 's':
		case 'S':
			/// Move camera back
			wrapper.camera.moveBack();
			break;
		case 'a':
		case 'A':
			/// Move camera left
			wrapper.camera.moveLeft();
			break;
		case 'd':
		case 'D':
			/// Move camera Right
			wrapper.camera.moveRight();
			break;
		case 'q':
		case 'Q':
			/// Move camera up
			wrapper.camera.moveUp();
			break;
		case 'e':
		case 'E':
			/// Move camera down
			wrapper.camera.moveDown();
			break;
		case 'r':
		case 'R':
			/// Reset the camera position to default
			wrapper.camera.setCamera(0.0f,0.0f,50.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);
			break;
		}
		break;
	case WM_MOUSEMOVE:
		/// Modify orientation of the camera			
		/// Back up the mouse current x and y positions
		wrapper.camera.mPrev.x = wrapper.camera.mCurr.x;
		wrapper.camera.mPrev.y = wrapper.camera.mCurr.y;
		/// Get the mouse current position
		wrapper.camera.mCurr.x = LOWORD( lParam );
		wrapper.camera.mCurr.y = HIWORD( lParam );
		/// Recalculate y as coordinates start at bottom left for openGL
		wrapper.camera.mCurr.y = wrapper.camera.vpHeight - wrapper.camera.mCurr.y;
		/// Evaluate diff of current and previous mouse positions
		diffX = wrapper.camera.mCurr.x - wrapper.camera.mPrev.x;
		diffY = wrapper.camera.mCurr.y - wrapper.camera.mPrev.y;
		if( wParam & MK_LBUTTON )
		{
			/// Get the center positions
			centerX = wrapper.camera.vpWidth/2.0f;
			centerY = wrapper.camera.vpHeight/2.0f;
			/// Calculate Depth values for window coordinates
			glReadPixels( wrapper.camera.mCurr.x, wrapper.camera.mCurr.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
			(Logger::Instance()).checkAndReportGLError("Failed to read depth component of window coordinate",'e',4,__LINE__,__FILE__);
			/// Unproject from window coordinates to world coorindates
			unProject( centerX+diffX, centerY+diffY, winZ, mv_s.top(), pr_s.top(),wrapper.camera.vpWidth,wrapper.camera.vpHeight, &posX, &posY, &posZ);
			/// Set the camera target positions
			wrapper.camera.target.x = (float) posX;
			wrapper.camera.target.y = (float) posY;
			wrapper.camera.target.z = (float) posZ;
		}
		// Right mouse button
		else if( wParam & MK_RBUTTON )
		{
		}
		// Middle mouse button
		else if( wParam & MK_MBUTTON )
		{			
		}

		break;
	case WM_CLOSE:
		flag = 0;
		/// Unbind buffers and destroy vertex and indices array for objects
		wrapper.destroyGeometries();
		/// Delete the OpenGL Render Context
		if( wglMakeCurrent( NULL, NULL ) == 0 ) /// Unselecting the render context
		{
			(Logger::Instance()).log("Failed to unselect the openGL render context",'e',4,__LINE__,__FILE__);
		}
		if( wglDeleteContext( g_hGLRC ) == 0 ) /// Deleting the render context
		{
			(Logger::Instance()).log("Failed to delete the openGL render context",'e',4,__LINE__,__FILE__);
		}
		/// Destroy the Device Context
		ReleaseDC( hWnd, g_hDC ); 
		/// Destroy the window
		DestroyWindow( hWnd );
		break;
	case WM_DESTROY:
		PostQuitMessage( EXIT_SUCCESS );
		break;
	default:
		return DefWindowProc( hWnd, msg, wParam, lParam );
	}
	return 0;
}

/******************************************************************************/
/*!
Creates a Window/OpenGL render context

\param [in] hWnd
Handle to this window

\param [out] hDC
Handle to the application device context

\param [out] hGLRC
Handle to the application render context
*/
/******************************************************************************/
void InitializeOpenGLRenderContext( HWND hWnd, HDC & hDC, HGLRC & hGLRC )
{
	const PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,								// version number
		PFD_DRAW_TO_WINDOW |			// support window
		PFD_SUPPORT_OPENGL |			// support OpenGL
		PFD_DOUBLEBUFFER,				// double buffered
		PFD_TYPE_RGBA,					// RGBA type
		32,								// 32-bit color depth
		0, 0, 0, 0, 0, 0,				// color bits ignored
		0,								// no alpha buffer
		0,								// shift bit ignored
		0,								// no accumulation buffer
		0, 0, 0, 0,						// accum bits ignored
		24,								// 24-bit z-buffer
		8,								// 8-bit stencil buffer
		0,								// no auxiliary buffer
		0,								// main layer
		0,								// reserved
		0, 0, 0							// layer masks ignored
	};

	BOOL bResult = FALSE;
	HGLRC tmpHGLRC = NULL;
	/// Get device context only once.
	hDC = GetDC( hWnd );
	/// Pixel format.
	int nPixelFormat = ChoosePixelFormat( hDC, &pfd );
	if( nPixelFormat == 0 )
	{
		(Logger::Instance()).log("Failed to specify the pixel format",'e',4,__LINE__,__FILE__);
	}
	bResult = SetPixelFormat( hDC, nPixelFormat, &pfd );
	if( bResult == FALSE )
	{
		(Logger::Instance()).log("Failed to set pixel format",'e',4,__LINE__,__FILE__);
	}
	/// Create the OpenGL Rendering Context, which is suitable for drawing on
	/// the device referenced by hDC. The rendering context has the same
	/// pixel format as the device context.
	tmpHGLRC = wglCreateContext( hDC );
	if( tmpHGLRC == NULL )
	{
		(Logger::Instance()).log("Failed to create the openGL render context",'e',4,__LINE__,__FILE__);
	}
	// Makes a specified OpenGL rendering context the calling thread's
	// current rendering context. All subsequent OpenGL calls made by the
	// thread are drawn on the device identified by hDC
	bResult = wglMakeCurrent( hDC, tmpHGLRC );
	if( bResult == FALSE )
	{
		(Logger::Instance()).log("Failed to make the specified openGL render context as current context",'e',4,__LINE__,__FILE__);
	}
	/// Initializing OpenGL extensions
	GLenum err = glewInit();
	if( err != GLEW_OK )
	{
		(Logger::Instance()).log("Failed to initialize GLEW",'e',4,__LINE__,__FILE__);
	}
	/// Stream used to display OpenGL version
	StringStream_t ss;
	/// Check if the create context for the new OpenGL is available
	if( wglewIsSupported( "WGL_ARB_create_context" ) == 1 )
	{
		const int attribs[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 3, 
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			/*WGL_CONTEXT_CORE_PROFILE_BIT_ARB*/
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
			0
		};
		hGLRC = wglCreateContextAttribsARB( hDC,0, attribs );
		if( !hGLRC )
		{
			(Logger::Instance()).log("Failed to create context Attrib ARB",'e',4,__LINE__,__FILE__);
		}
		wglMakeCurrent( NULL, NULL );
		wglDeleteContext( tmpHGLRC );
		wglMakeCurrent( hDC, hGLRC );
		/// Checking the OpenGL version number the OpenGL 3 way!
		int OpenGLVersion[2] = { 0 };
		glGetIntegerv( GL_MAJOR_VERSION, &OpenGLVersion[0] );
		(Logger::Instance()).checkAndReportGLError("Failed to get GL_MAJOR_VERSION",'e',4,__LINE__,__FILE__);
		glGetIntegerv( GL_MINOR_VERSION, &OpenGLVersion[1] );
		(Logger::Instance()).checkAndReportGLError("Failed to get GL_MINOR_VERSION",'e',4,__LINE__,__FILE__);
		/// Display the OpenGL version we are using
		ss << "OpenGL version:"
			<< OpenGLVersion[0]
		<< "."
			<< OpenGLVersion[1];
	}
	else
	{
		// It's not possible to make a GL 3.x context.
		// Use the old style context (GL 2.1 and before)
		hGLRC = tmpHGLRC;
		// Checking the OpenGL version number the OpenGL old way!
		const std::string strGLVersion = 
			reinterpret_cast<const char *>( glGetString( GL_VERSION ) );
		ss 	<< "WGL_ARB_create_context not supported!"
			<< "Cannot specify the OpenGL version to be used!"
			<< "OpenGL version:"
			<< strGLVersion;
	}
	(Logger::Instance()).log(ss.str().c_str(),'i',1,__LINE__,__FILE__);
	if( hGLRC == NULL )
	{
		(Logger::Instance()).log("openGL render context was not created",'e',4,__LINE__,__FILE__);
	}
}

/******************************************************************************/
/*!
Initialize OpenGL states
*/
/******************************************************************************/
void InitializeOpenGLState( HWND /*hWnd*/ )
{
	/// Set the background color
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	(Logger::Instance()).checkAndReportGLError("Failed to clear background color",'e',4,__LINE__,__FILE__);
	/// Specify the vertex point size on screen
	glEnable(GL_PROGRAM_POINT_SIZE);
	(Logger::Instance()).checkAndReportGLError("Failed to enable GL_PROGRAM_POINT_SIZE",'e',4,__LINE__,__FILE__);
	GLfloat *currPointSize = new GLfloat;
	glGetFloatv(GL_POINT_SIZE_MAX,currPointSize);
	(Logger::Instance()).checkAndReportGLError("Failed to get GL_POINT_SIZE_MAX",'e',4,__LINE__,__FILE__);
	glPointSize(3.0);
	(Logger::Instance()).checkAndReportGLError("Failed to specify point size",'e',4,__LINE__,__FILE__);
	delete currPointSize;
	/// Enabling depth test
	glEnable( GL_DEPTH_TEST );	/// Enable Z-buffering
	(Logger::Instance()).checkAndReportGLError("Failed to enable GL_DEPTH_TEST",'e',4,__LINE__,__FILE__);
	glClearDepth( 1.0f );		/// Depth Buffer Setup
	(Logger::Instance()).checkAndReportGLError("Failed to setup depth buffer",'e',4,__LINE__,__FILE__);
	glDepthFunc( GL_LEQUAL );
	/// Set the value used to clear the Stencil Buffer
	glClearStencil( 0 );
	(Logger::Instance()).checkAndReportGLError("Failed to clear stencil buffer",'e',4,__LINE__,__FILE__);
	/// Enabling primitive restart
	glPrimitiveRestartIndex( PRIMITIVE_RESTART_INDEX );
	glEnable( GL_PRIMITIVE_RESTART );
	(Logger::Instance()).checkAndReportGLError("Failed to enable PRIMITIVE_RESTART_INDEX",'e',4,__LINE__,__FILE__);
	/// Set back face culling
	glFrontFace( GL_CCW );
	glCullFace( GL_BACK );
	(Logger::Instance()).checkAndReportGLError("Failed to setup culling",'e',4,__LINE__,__FILE__);
}

/******************************************************************************/
/*!
Update function for physics objects - Runs every frame and updates the states
\param [in] frameStartTime
Reference to a constant float representing frame start time
\param [in] ddt
Reference to a constant float representing time taken by a frame
*/
/******************************************************************************/
void updateObjects( HWND ,const float &frameStartTime, const float &ddt)
{
	Wrapper &wrapper = Wrapper::Instance();
	std::vector<GameObject *>::iterator i,j;
	/// Factor for multipass collision tests
	float dt = ddt/20.0f;
	if(wrapper.listOfObjects.size()!=0)
	{
		for(double k = 0.0;k <= ddt;k+=dt)
		{
			/// Make collisionFLag as false for all objects
			/// Indicates that no object has collided yet.
			for(i = wrapper.listOfObjects.begin();i!=wrapper.listOfObjects.end(); ++i)
			{
				if((*i)->dynamic)
					(*i)->collisionFlag = false;
			}
			/// Check for collision detection here between every pair of physics objects
			/// Mark collisionFlag as True if collision is detected
			for(i = wrapper.listOfObjects.begin();i!=wrapper.listOfObjects.end(); ++i)
			{
				for(j = i+1 ;j!= wrapper.listOfObjects.end(); ++j)
				{
					if(((*i)->dynamic)&&((*j)->dynamic))
					{
						if(collisionBallBall(*(*i),*(*j),frameStartTime,dt))
						{
							(*i)->collisionFlag = true;
							(*j)->collisionFlag = true;
						}
					}
					else if(((*i)->dynamic)||((*j)->dynamic))
					{
						if(((*i)->shape == ball && (*j)->shape == straightwall))
						{
							if(collisionBallStraightWall(*(*i),*(*j),frameStartTime,dt))
							{
								(*i)->collisionFlag = true;
							}
						}
						else if(((*i)->shape == straightwall && (*j)->shape == ball))
						{
							if(collisionBallStraightWall(*(*j),*(*i),frameStartTime,dt))
							{
								(*j)->collisionFlag = true;
							}
						}

						else if(((*i)->shape == ball && (*j)->shape == circularwall))
						{
							if(collisionBallCircularWall(*(*i),*(*j),frameStartTime,dt))
							{
								(*i)->collisionFlag = true;
							}
						}
						else if(((*i)->shape == circularwall && (*j)->shape == ball))
						{
							if(collisionBallCircularWall(*(*j),*(*i),frameStartTime,dt)) 
							{
								(*j)->collisionFlag = true;
							}
						}
					}
				}
			}
			/// If collisionFlag is false, object does not collide with anything in this frame
			/// Hence update the object with its usual trajectory
			for(i = wrapper.listOfObjects.begin();i!=wrapper.listOfObjects.end(); ++i)
			{
				if((*i)->dynamic && (!(*i)->collisionFlag))
					updatePositionwrtVelocity(*(*i),dt);
			}
		}
	}
}

/******************************************************************************/
/*!
Draw Geometry function - Runs every frame and draws the geometries
\param [in] hDC
Handle to the application device context
*/
/******************************************************************************/
void drawObjects( HWND , HDC hDC)
{
	glClear(GL_COLOR_BUFFER_BIT|  GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
	(Logger::Instance()).checkAndReportGLError("Failed to clear buffers",'e',4,__LINE__,__FILE__);
	/// Set the matrix mode to modelview
	glMatrixMode( GL_MODELVIEW );
	(Logger::Instance()).checkAndReportGLError("Failed to setup matrix mode to GL_MODELVIEW",'e',4,__LINE__,__FILE__);
	/// Update the camera related specifics
	Wrapper &wrapper = Wrapper::Instance();
	wrapper.camera.lookAt();
	/// Load Identity on model view matrix stack
	mv_s.LoadIdentity();
	/// Multiply the camera view matrix with top of model view matrix stack
	mv_s.MultMatrix(wrapper.camera.viewMtx);
	
	for(unsigned int i = 0;i < 21; ++i)
	{
		wrapper.sphere[i].drawTransformedGeometry(mv_s);
	}
	for(unsigned int i = 0;i < 10; ++i)
	{
		wrapper.line[i].drawTransformedGeometry(mv_s);
	}
	
	//wrapper.grid[0].drawTransformedGeometry(mv_s);
	/// Swap the buffers
	if( SwapBuffers( hDC ) == 0 )
	{
		(Logger::Instance()).log("Failed to swap buffers",'e',4,__LINE__,__FILE__);
	}
}