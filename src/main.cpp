// Tom Tsiliopoulos - 100616336

#include "GameObject.h"
#include "MorphGameObject.h"
#include "AnimationMath.h"

// Core Libraries (std::)
#include <iostream>
#include <string>
#include <math.h>
#include <vector>

// 3rd Party Libraries
#include <GLUT/freeglut.h>
#include <TTK/GraphicsUtils.h>
#include <TTK/Camera.h>
#include <TTK/OBJMesh.h>
#include <PointHandle.h>

// Defines and Core variables
#define FRAMES_PER_SECOND 60
const int FRAME_DELAY = 1000 / FRAMES_PER_SECOND; // Milliseconds per frame

// Window size
int windowWidth = 800;
int windowHeight = 600;

// Angle conversions conversions
const float degToRad = 3.14159f / 180.0f;
const float radToDeg = 180.0f / 3.14159f;

float deltaTime = 0.0f; // amount of time since last update (set every frame in timer callback)

float rotationSpeedModifier = 10.0f;

// Mouse position in pixels
glm::vec3 mousePosition; // x, y, 0
glm::vec3 mousePositionFlipped; // x, windowHeight - y, 0
TTK::Camera camera;

MorphGameObject gameObject;

void InitializeScene()
{
	std::string modelsPath = "../../assets/models/";

	// Load morph target meshes
	gameObject.addMorphTarget(modelsPath + "cube_000001.obj");
	gameObject.addMorphTarget(modelsPath + "cube_000010.obj");
	gameObject.addMorphTarget(modelsPath + "cube_000020.obj");
	gameObject.addMorphTarget(modelsPath + "cube_000030.obj");
	gameObject.addMorphTarget(modelsPath + "cube_000040.obj");
	gameObject.addMorphTarget(modelsPath + "cube_000050.obj");
}

// This is where we draw stuff
void DisplayCallbackFunction(void)
{
	// Set up scene
	TTK::Graphics::SetBackgroundColour(0.5f, 0.5f, 0.5f);
	TTK::Graphics::ClearScreen();
	TTK::Graphics::SetCameraMode3D(windowWidth, windowHeight);
	TTK::Graphics::DrawGrid();

	camera.update();

	gameObject.update(deltaTime);

	gameObject.draw();

	// Swap buffers
	// This is how we tell the program to put the things we just drew on the screen
	glutSwapBuffers();
}

/* function void KeyboardCallbackFunction(unsigned char, int,int)
* Description:
*   - this handles keyboard input when a button is pressed
*/
void KeyboardCallbackFunction(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: // the escape key
		glutExit();
		break;

	case 'q': // the 'q' key
	case 'Q':
		camera.moveUp();
		break;
	case 'e':
	case 'E':
		camera.moveDown();
	case 'W':
	case 'w':
		camera.moveForward();
		break;
	case 'S':
	case 's':
		camera.moveBackward();
		break;
	case 'A':
	case 'a':
		camera.moveRight();
		break;
	case 'D':
	case 'd':
		camera.moveLeft();
		break;
	}
}

/* function void KeyboardUpCallbackFunction(unsigned char, int,int)
* Description:
*   - this handles keyboard input when a button is lifted
*/
void KeyboardUpCallbackFunction(unsigned char key, int x, int y)
{
	switch (key)
	{

	}
}

/* function TimerCallbackFunction(int value)
* Description:
*  - this is called many times per second
*  - this enables you to animate things
*  - no drawing, just changing the state
*  - changes the frame number and calls for a redisplay
*  - FRAME_DELAY is the number of  to wait before calling the timer again
*/
void TimerCallbackFunction(int value)
{
	// Calculate the amount of time since the last frame
	static int elapsedTimeAtLastTick = 0;
	int totalElapsedTime = glutGet(GLUT_ELAPSED_TIME);

	deltaTime = (float)totalElapsedTime - elapsedTimeAtLastTick;
	deltaTime /= 1000.0f;
	elapsedTimeAtLastTick = totalElapsedTime;

	// Re-trigger the display event
	glutPostRedisplay();

	/* this call gives it a proper frame delay to hit our target FPS */
	glutTimerFunc(FRAME_DELAY, TimerCallbackFunction, 0);
}

/* function WindowReshapeCallbackFunction()
* Description:
*  - this is called whenever the window is resized
*  - and sets up the projection matrix properly
*/
void WindowReshapeCallbackFunction(int w, int h)
{
	/* Update our Window Properties */
	windowWidth = w;
	windowHeight = h;
}

// This is called when a mouse button is clicked
void MouseClickCallbackFunction(int button, int state, int x, int y)
{
	mousePosition.x = x;
	mousePosition.y = y;

	mousePositionFlipped.x = x;
	mousePositionFlipped.y = windowHeight - y;
}

void SpecialInputCallbackFunction(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		// ...
		break;
	case GLUT_KEY_DOWN:
		// ...
		break;
	case GLUT_KEY_LEFT:
		// ...
		break;
	case GLUT_KEY_RIGHT:
		// ...
		break;
	}
}

// Called when the mouse is clicked and moves
void MouseMotionCallbackFunction(int x, int y)
{
	// Update camera rotation
	if (mousePosition.length() > 0)
		camera.processMouseMotion(x, y, mousePosition.x, mousePosition.y, deltaTime);

	mousePosition.x = x;
	mousePosition.y = y;

	mousePositionFlipped.x = x;
	mousePositionFlipped.y = windowHeight - y;
}

// Called when the mouse is moved inside the window
void MousePassiveMotionCallbackFunction(int x, int y)
{
}

/* function main()
* Description:
*  - this is the main function
*  - does initialization and then calls glutMainLoop() to start the event handler
*/
int main(int argc, char **argv)
{
	// Memory Leak Detection
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	/* initialize the window and OpenGL properly */

	// Request an OpenGL 4.4 compatibility
	// A compatibility context is needed to use the provided rendering utilities
	// which are written in OpenGL 1.1
	glutInitContextVersion(4, 4);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutInit(&argc, argv);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Tutorial");

	auto s = glGetString(GL_VERSION);
	std::cout << "OpenGL Version: " << s << std::endl;

	/* set up our function callbacks */
	glutDisplayFunc(DisplayCallbackFunction);
	glutKeyboardFunc(KeyboardCallbackFunction);
	glutKeyboardUpFunc(KeyboardUpCallbackFunction);
	glutReshapeFunc(WindowReshapeCallbackFunction);
	glutMouseFunc(MouseClickCallbackFunction);
	glutMotionFunc(MouseMotionCallbackFunction);
	glutPassiveMotionFunc(MousePassiveMotionCallbackFunction);
	glutTimerFunc(1, TimerCallbackFunction, 0);
	glutSpecialFunc(SpecialInputCallbackFunction);

	// Init GL
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	/* Start Game Loop */
	deltaTime = (float)glutGet(GLUT_ELAPSED_TIME);
	deltaTime /= 1000.0f;

	InitializeScene();

	glutMainLoop();

	return 0;
}