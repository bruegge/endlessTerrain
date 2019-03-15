#include "Terrain.h"

CTerrain::CTerrain()
{
}

CTerrain::~CTerrain()
{
}

void CTerrain::InitializeGraphicsData()
{
	glGenVertexArrays(1, &m_nVAO);
	glBindVertexArray(m_nVAO);
	glGenBuffers(1, &m_nVBO);
	
}