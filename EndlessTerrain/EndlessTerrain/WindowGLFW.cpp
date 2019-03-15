#include "WindowGLFW.h"
#include <iostream>


CWindowGLFW::CWindowGLFW()
{
	m_nScreenWidth = 800;
	m_nScreenHeight = 600;
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		return;
	}
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

																   // Open a window and create its OpenGL context
	m_pWindow = glfwCreateWindow(m_nScreenWidth, m_nScreenHeight, "OpenGL Window", NULL, NULL);
	if (m_pWindow == NULL)
	{
		std::cout << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(m_pWindow); // Initialize GLEW
	if (!gladLoadGL())
	{
		std::cout << "Failed to initialize GLAD." << std::endl;
		return;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(m_pWindow, GLFW_STICKY_KEYS, GL_TRUE);

	std::cout << "Window creation successful" << std::endl;

}

CWindowGLFW::~CWindowGLFW()
{
}

void CWindowGLFW::SetWindowSize(unsigned int nWidth, unsigned int nHeight)
{
	m_nScreenWidth = nWidth;
	m_nScreenHeight = nHeight;
	glfwSetWindowSize(m_pWindow, m_nScreenWidth, m_nScreenHeight);
}

glm::uvec2 CWindowGLFW::GetWindowSize() const
{
	return glm::uvec2(m_nScreenWidth, m_nScreenHeight);
}

bool CWindowGLFW::ManageInputs() const
{
	glfwPollEvents();

	if (glfwGetKey(m_pWindow, GLFW_KEY_1) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (glfwGetKey(m_pWindow, GLFW_KEY_2) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	return glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(m_pWindow); //stop program when closing the window or pressing "escape"
}


void CWindowGLFW::SwapBuffers() const
{
	glfwSwapBuffers(m_pWindow);
}

GLFWwindow* CWindowGLFW::GetWindowID()
{
	return m_pWindow;
}