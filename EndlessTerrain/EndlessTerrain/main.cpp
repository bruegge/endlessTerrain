#include <glad\glad.h>
#include "WindowGLFW.h"

CWindowGLFW* pWindow;

void Init()
{
	pWindow = new CWindowGLFW();
}

void LoadContent()
{

}

void GameLoop()
{
	glClearColor(0.1f, 0.1f, 0.02f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	bool bClose = false;
	do //render loop
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the color and the depth buffer


		pWindow->ManageInputs();

		pWindow->SwapBuffers();
	} while (bClose == false);
}

void FreeContent()
{

}

void main()
{
	Init();
	LoadContent();
	GameLoop();
	FreeContent();
}