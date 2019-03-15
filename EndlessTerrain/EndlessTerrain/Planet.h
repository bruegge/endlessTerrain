#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "Shader.h"
#include "Camera.h"

class CPlanet
{
public:
	CPlanet();
	~CPlanet();

	void InitializeGeometry(unsigned int nGridWidth);
	void RandomizeNoise();
	void Draw(CShader* pShader, CCamera* pCamera);
private:
	
	GLuint m_nVAO;
	GLuint m_nVBO;
	GLuint m_nIBO;
	GLint m_aPerm[512];
	unsigned int m_nCountIBO;
};

