#include <glad\glad.h>
#include "WindowGLFW.h"
#include "Planet.h"
#include "Shader.h"
#include "Camera.h"

CWindowGLFW* pWindow;
CPlanet* pPlanet;
CShader* pShader;
CCamera* pCamera;

void Init()
{
	pWindow = new CWindowGLFW();
	pPlanet = new CPlanet();
	pShader = new CShader();
	pCamera = new CCamera(pWindow->GetWindowID());
}

void LoadContent()
{
	pPlanet->InitializeGeometry(33);
	pShader->CreateShaderProgram("../shaders/VS_DrawTerrain.glsl", nullptr, nullptr, nullptr, "../shaders/FS_DrawTerrain.glsl");
	pCamera->SetOrientation(glm::vec3(0, 0, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	pCamera->SetPerspective(90, static_cast<float>(pWindow->GetWindowSize().x) / static_cast<float>(pWindow->GetWindowSize().y), 0.001f, 100.0f);
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


		bClose = pWindow->ManageInputs();
		pCamera->ManageKeyInputs();
		pPlanet->Draw(pShader, pCamera);

		pWindow->SwapBuffers();
	} while (bClose == false);
}

void FreeContent()
{
	delete pWindow;
	delete pPlanet;
}

void main()
{
	Init();
	LoadContent();
	GameLoop();
	FreeContent();
}