#include <glad\glad.h>
#include "WindowGLFW.h"
#include "Planet.h"
#include "Shader.h"
#include "Camera.h"
#include "Settings.h"
#include "../ImGUI/imgui.h"
#include "../ImGUI/imgui_impl_glfw.h"
#include "../ImGUI/imgui_impl_opengl3.h"

CWindowGLFW* pWindow;
CPlanet* pPlanet;
CShader* pShader;
CCamera* pCamera;
CSettings* pSettings;

void Init()
{
	pWindow = new CWindowGLFW(1024,900);
	pPlanet = new CPlanet();
	pShader = new CShader();
	pCamera = new CCamera(pWindow->GetWindowID());
	pPlanet->SetGeometrySize(9);
	pPlanet->SetQuadTreeDepthMax(4);
	CSettings::CreateSettings(pPlanet);
	pSettings = CSettings::GetSettings();

	{ //GUI
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(pWindow->GetWindowID(), true);
		ImGui_ImplOpenGL3_Init("#version 430");
	}

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);
}

void LoadContent()
{
	pShader->CreateShaderProgram("../shaders/VS_DrawTerrain.glsl", nullptr, nullptr, nullptr, "../shaders/FS_DrawTerrain.glsl");
	pCamera->SetOrientation(glm::vec3(0, 0, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	pCamera->SetPerspective(90, static_cast<float>(pWindow->GetWindowSize().x) / static_cast<float>(pWindow->GetWindowSize().y), 0.00001f, 10.0f);
}

void GameLoop()
{
	glClearColor(0.1f, 0.1f, 0.02f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	bool bClose = false;
	do //render loop
	{
		pSettings->ApplySettings();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the color and the depth buffer

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		bClose = pWindow->ManageInputs();
		pCamera->ManageKeyInputs();
		pPlanet->Draw(pShader, pCamera);

		{
			ImGui::Begin("Settings");                          // Create a window called "Hello, world!" and append into it.
						
			ImGui::Text("Number instances: %i", pSettings->m_nInfoInstanceCount);               // Display some text (you can use a format strings too)
			ImGui::Text("ComputeTime CPU: %f ms", pSettings->m_dComputeTimeCPU);               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Enable WireFrames", &(pSettings->m_bEnableWireframe));      // Edit bools storing our window open/close state
			ImGui::Checkbox("Enable face culling", &(pSettings->m_bEnableCullFace));      // Edit bools storing our window open/close state
			ImGui::Checkbox("Enable QuadTree update", &(pSettings->m_bEnableQuadTreeUpdate));      // Edit bools storing our window open/close state
			ImGui::Checkbox("Enable TJunction elimination", &(pSettings->m_bEnableTJunctionElimination));      // Edit bools storing our window open/close state

			int nRadioButtonValue;
			ImGui::RadioButton("Number 1", &nRadioButtonValue, 1);
			ImGui::RadioButton("Number 2", &nRadioButtonValue, 2);

			float fGridSize = static_cast<float>(pSettings->m_nGeometrySize);
			ImGui::SliderFloat("single grid size", &fGridSize, 2.0f, 33.0f);   
			pSettings->m_nGeometrySize = static_cast<unsigned int>(fGridSize);
			pSettings->m_nGeometrySize += 1 - pSettings->m_nGeometrySize % 2;


			float fMaxQuadTreeDepth = static_cast<float>(pSettings->m_nMaxQuadTreeDepth);
			ImGui::SliderFloat("QuadTree Max Depth", &fMaxQuadTreeDepth, 0.0f, 20.0f);
			pSettings->m_nMaxQuadTreeDepth = static_cast<unsigned int>(fMaxQuadTreeDepth);

			ImGui::SliderFloat("QuadTree Division Angle", &pSettings->m_fTileDivisionAngle, 90.0f, 6.0f);

			float fPerlinNoiseCount = static_cast<float>(pSettings->m_nPerlinNoiseCount);
			ImGui::SliderFloat("Number iterations perlin noise", &fPerlinNoiseCount, 0.0f, 30.0f);
			pSettings->m_nPerlinNoiseCount = static_cast<unsigned int>(fPerlinNoiseCount);

		
			ImGui::Text("(%.1f FPS)", ImGui::GetIO().Framerate);
			ImGui::End();
		}

		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		pWindow->SwapBuffers();
	} while (bClose == false);
}

void FreeContent()
{
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
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