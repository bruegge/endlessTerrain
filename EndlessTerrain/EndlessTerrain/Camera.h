#pragma once
#include "glm\glm.hpp"
#include "glfw3.h"

class CCamera
{
public:
	CCamera(GLFWwindow* pWindow);
	~CCamera();

	void SetOrientation(glm::vec3 EyePosition, glm::vec3 LookAtVector, glm::vec3 UpVector);
	void SetPerspective(float fFieldOfView, float fAspectRatio, float fNearClippingplane, float fFarClippingPlane);

	void Translate(glm::vec3 vDirection);
	void Rotate(glm::vec3 vaxis, float fAngle);

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewProjectionMatrix();

	void ManageKeyInputs();

private:
	glm::mat4 m_mViewMatrix;
	glm::mat4 m_mProjectionMatrix;
	GLFWwindow* m_pWindowGLFW;
};

