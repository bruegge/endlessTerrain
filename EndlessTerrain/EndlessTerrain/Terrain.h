#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>


class CTerrain
{
public:
	CTerrain();
	~CTerrain();

private:
	void InitializeGraphicsData();
	GLuint m_nVBO;
	GLuint m_nVAO;
	GLuint m_nIBO;
};

