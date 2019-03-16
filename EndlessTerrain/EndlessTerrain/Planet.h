#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "Shader.h"
#include "Camera.h"
#include <vector>
#include "QuadTree.h"

class CPlanet
{
public:
	CPlanet();
	~CPlanet();

	void SetGeometrySize(unsigned int nGridWidth);
	void RandomizeNoise();
	void Draw(CShader* pShader, CCamera* pCamera);
	unsigned int GetGeometrySize();
	unsigned int GetQuadTreeDepthMax();
	void SetQuadTreeDepthMax(unsigned int nDepthMax);
private:
	
	void ApplyChangesToVBO(glm::vec3 vCameraPosition);
	unsigned int m_nQuadTreeDepthMax = 3;
	unsigned int m_nCountTiles = 0;
	unsigned int m_nGeometrySize = 0;
	GLuint m_nVAO = 0;
	GLuint m_nVBO = 0;
	GLuint m_nVBOEdges = 0;
	GLuint m_nIBO = 0;
	GLint m_aPerm[512];
	CQuadTree m_aQuadTrees[6];
	unsigned int m_nCountIBO;
};

