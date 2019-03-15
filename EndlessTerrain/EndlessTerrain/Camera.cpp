#include "Camera.h"
#include <glm\gtx\transform.hpp>

CCamera::CCamera(GLFWwindow* pWindow)
{
	m_pWindowGLFW = pWindow;
}

CCamera::~CCamera()
{
}

void CCamera::SetPerspective(float fFieldOfView, float fAspectRatio, float fNearClippingplane, float fFarClippingPlane)
{
	m_mProjectionMatrix = glm::perspective(fFieldOfView * 3.1415926f / 180.0f, fAspectRatio, fNearClippingplane, fFarClippingPlane);
}

void CCamera::SetOrientation(glm::vec3 EyePosition, glm::vec3 LookAtVector, glm::vec3 UpVector)
{
	m_mViewMatrix = glm::lookAt(EyePosition, LookAtVector, UpVector);
}

void CCamera::Translate(glm::vec3 vDirection)
{
	m_mViewMatrix = glm::translate(vDirection) * m_mViewMatrix;
}

void CCamera::Rotate(glm::vec3 vAxis, float fAngle)
{
	m_mViewMatrix = glm::rotate(fAngle, vAxis) * m_mViewMatrix;
}

glm::mat4 CCamera::GetViewMatrix()
{
	return m_mViewMatrix;
}

glm::mat4 CCamera::GetProjectionMatrix()
{
	return m_mProjectionMatrix;
}

glm::mat4 CCamera::GetViewProjectionMatrix()
{
	return m_mProjectionMatrix * m_mViewMatrix;
}

void CCamera::ManageKeyInputs()
{
	if (glfwGetKey(m_pWindowGLFW, GLFW_KEY_W) == GLFW_PRESS) { Translate(glm::vec3(0, 0, 0.001f)); }
	if (glfwGetKey(m_pWindowGLFW, GLFW_KEY_S) == GLFW_PRESS) { Translate(glm::vec3(0, 0, -0.001f)); }
	if (glfwGetKey(m_pWindowGLFW, GLFW_KEY_A) == GLFW_PRESS) { Translate(glm::vec3(0.001f, 0, 0)); }
	if (glfwGetKey(m_pWindowGLFW, GLFW_KEY_D) == GLFW_PRESS) { Translate(glm::vec3(-0.001f, 0, 0)); }
	if (glfwGetKey(m_pWindowGLFW, GLFW_KEY_Q) == GLFW_PRESS) { Translate(glm::vec3(0, 0.001f, 0)); }
	if (glfwGetKey(m_pWindowGLFW, GLFW_KEY_E) == GLFW_PRESS) { Translate(glm::vec3(0, -0.001f, 0)); }

	if (glfwGetKey(m_pWindowGLFW, GLFW_KEY_LEFT) == GLFW_PRESS) { Rotate(glm::vec3(0, 1, 0), -0.01f); }
	if (glfwGetKey(m_pWindowGLFW, GLFW_KEY_RIGHT) == GLFW_PRESS) { Rotate(glm::vec3(0, 1, 0), 0.01f); }
	if (glfwGetKey(m_pWindowGLFW, GLFW_KEY_UP) == GLFW_PRESS) { Rotate(glm::vec3(1, 0, 0), -0.01f); }
	if (glfwGetKey(m_pWindowGLFW, GLFW_KEY_DOWN) == GLFW_PRESS) { Rotate(glm::vec3(1, 0, 0), 0.01f); }


}