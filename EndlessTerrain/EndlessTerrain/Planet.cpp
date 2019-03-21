#include "Planet.h"
#include <vector>
#include <chrono>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "Settings.h"

CPlanet::CPlanet()
{
	RandomizeNoise();
	m_aQuadTrees[0].SetEdges(glm::vec3(1, 1, -1), glm::vec3(1, 1, 1), glm::vec3(1, -1, 1), glm::vec3(1, -1, -1));
	m_aQuadTrees[1].SetEdges(glm::vec3(-1, 1, -1), glm::vec3(1, 1, -1), glm::vec3(1, -1, -1), glm::vec3(-1, -1, -1));
	m_aQuadTrees[2].SetEdges(glm::vec3(1, 1, 1), glm::vec3(-1, 1, 1), glm::vec3(-1, -1, 1), glm::vec3(1, -1, 1));
	m_aQuadTrees[3].SetEdges(glm::vec3(-1, 1, 1), glm::vec3(1, 1, 1), glm::vec3(1, 1, -1), glm::vec3(-1, 1, -1));
	m_aQuadTrees[4].SetEdges(glm::vec3(-1, 1, 1), glm::vec3(-1, 1, -1), glm::vec3(-1, -1, -1), glm::vec3(-1, -1, 1));
	m_aQuadTrees[5].SetEdges(glm::vec3(-1, -1, 1), glm::vec3(-1, -1, -1), glm::vec3(1, -1, -1), glm::vec3(1, -1, 1));
}

CPlanet::~CPlanet()
{
	glDeleteBuffers(1, &m_nIBO);
	glDeleteBuffers(1, &m_nVBO);
	glDeleteBuffers(1, &m_nVBOEdges);
	glDeleteVertexArrays(1, &m_nVAO);
}

unsigned int CPlanet::GetGeometrySize()
{
	return m_nGeometrySize;
}

void CPlanet::SetGeometrySize(unsigned int nGridWidth)
{
	m_nGeometrySize = nGridWidth;
	std::vector<glm::vec3> resultVBO(nGridWidth * nGridWidth);

	for (unsigned int x = 0; x < nGridWidth; ++x)
	{
		for (unsigned int y = 0; y < nGridWidth; ++y)
		{
			glm::vec3 sVertex;
			GLfloat fXPosition = static_cast<GLfloat>(x) / static_cast<GLfloat>(nGridWidth - 1);
			GLfloat fYPosition = static_cast<GLfloat>(y) / static_cast<GLfloat>(nGridWidth - 1);

			sVertex = glm::vec3((fXPosition), (fYPosition), 1);
			resultVBO[nGridWidth * x + y] = sVertex;
		}
	}

	std::vector<GLuint> resultIBO((nGridWidth - 1) * (nGridWidth - 1) * 6);
	unsigned int nVectorPosition = 0;
	for (unsigned int x = 0; x < nGridWidth - 1; ++x)
	{
		for (unsigned int y = 0; y < nGridWidth - 1; ++y)
		{
			/*patch
			resultIBO[nVectorPosition++] = (x + 1) * nGridWidth + y + 1;
			resultIBO[nVectorPosition++] = (x + 1) * nGridWidth + y;
			resultIBO[nVectorPosition++] = x * nGridWidth + y + 1;
			resultIBO[nVectorPosition++] = x * nGridWidth + y;
			*/

			//triangleList
			resultIBO[nVectorPosition++] = x * nGridWidth + y;
			resultIBO[nVectorPosition++] = (x + 1) * nGridWidth + y;
			resultIBO[nVectorPosition++] = (x + 1) * nGridWidth + y + 1;

			resultIBO[nVectorPosition++] = x * nGridWidth + y;
			resultIBO[nVectorPosition++] = (x + 1) * nGridWidth + y + 1;
			resultIBO[nVectorPosition++] = x * nGridWidth + y + 1;
			
		}
	}

	if (m_nVAO != 0)
	{
		glDeleteBuffers(1, &m_nIBO);
		glDeleteBuffers(1, &m_nVBO);
	}
	else
	{
		glGenVertexArrays(1, &m_nVAO);
		glGenBuffers(1, &m_nVBOEdges);
		glBindBuffer(GL_ARRAY_BUFFER, m_nVBOEdges);
		glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::vec3)+1) * 100000, nullptr, GL_DYNAMIC_DRAW);
	}
	glBindVertexArray(m_nVAO);
	glGenBuffers(1, &m_nVBO);
	glGenBuffers(1, &m_nIBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * resultVBO.size(), &resultVBO.front(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * resultIBO.size(), &resultIBO.front(), GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (GLvoid*)(0 * sizeof(float))); //position

	glBindBuffer(GL_ARRAY_BUFFER, m_nVBOEdges);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 13, (GLvoid*)(0 * sizeof(float))); //edge0
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 13, (GLvoid*)(3 * sizeof(float))); //edge1
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 13, (GLvoid*)(6 * sizeof(float))); //edge2
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 13, (GLvoid*)(9 * sizeof(float))); //edge3
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 13, (GLvoid*)(12 * sizeof(float))); //TJunctionFlag

	glBindVertexArray(0);
	m_nCountIBO = resultIBO.size();
}

void CPlanet::RandomizeNoise()
{
	//generate noise Array
	for (int i = 0; i < 256; i++) {
		m_aPerm[i] = i;
	}
	//shuffle
	for (int i = 0; i < 256; i++) {
		int swap_idx = rand() % 256;
		std::swap(m_aPerm[i], m_aPerm[swap_idx]);
	}

	for (int i = 256; i < 512; i++) {
		m_aPerm[i] = m_aPerm[i - 256];
	}
}

void CPlanet::Draw(CShader* pShader, CCamera* pCamera)
{
	glm::vec3 vCameraPosition = pCamera->GetCameraPosition();
	
	pShader->Bind();
	ApplyChangesToVBO(vCameraPosition);
	glBindVertexArray(m_nVAO);

	GLint nUniformLocationViewProjectionMatrix = glGetUniformLocation(pShader->GetID(), "viewProjectionMatrix");
	glUniformMatrix4fv(nUniformLocationViewProjectionMatrix, 1, GL_FALSE, &pCamera->GetViewProjectionMatrix()[0][0]);
	glUniform1iv(glGetUniformLocation(pShader->GetID(), "perm"), 512, m_aPerm);
		
	glUniform1i(glGetUniformLocation(pShader->GetID(), "perlinNoiseCount"), m_nPerlinNoiseCount);
	glUniform1i(glGetUniformLocation(pShader->GetID(), "gridSize"), m_nGeometrySize);

	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);

	glDrawElementsInstanced(GL_TRIANGLES, m_nCountIBO, GL_UNSIGNED_INT, 0, m_nCountTiles);
	
	glBindVertexArray(0);
	pShader->UnBind();
}

void CPlanet::ApplyChangesToVBO(glm::vec3 vCameraPosition)
{
	if (CSettings::GetSettings()->m_bEnableQuadTreeUpdate)
	{
		auto start = std::chrono::steady_clock::now();
		bool bChanged = false;
		for (unsigned int i = 0; i < 6; ++i)
		{
			m_aQuadTrees[i].CalculateTiles(vCameraPosition);
	
			if (m_aQuadTrees[i].IsChanged())
			{
				bChanged = true;
			}
		}
		auto end = std::chrono::steady_clock::now();
		std::chrono::duration<double> diff = end - start;
		start = std::chrono::steady_clock::now();
		CSettings::GetSettings()->m_dComputeTimeCPU = diff.count() * 1000.0;

		if (bChanged)
		{
			GLuint nOffset = 0;
			for (unsigned int i = 0; i < 6; ++i)
			{
				glBindBuffer(GL_ARRAY_BUFFER, m_nVBOEdges);
				glBufferSubData(GL_ARRAY_BUFFER, nOffset, (sizeof(glm::vec3) * 4 + 4) * m_aQuadTrees[i].GetTileCount(), &m_aQuadTrees[i].GetTiles()->at(0).vEdgePositions);
				nOffset += m_aQuadTrees[i].GetTileCount() * (4 * sizeof(glm::vec3) + 4);
			}
			m_nCountTiles = nOffset / (sizeof(glm::vec3) * 4 + 4);
			//info output number of instances
			CSettings::GetSettings()->m_nInfoInstanceCount = m_nCountTiles;
		}
	}
}

void CPlanet::SetPerlinNoiseCount(unsigned int nCount)
{
	m_nPerlinNoiseCount = nCount;
}

unsigned int CPlanet::GetQuadTreeDepthMax()
{
	return m_nQuadTreeDepthMax;
}

void CPlanet::SetQuadTreeDepthMax(unsigned int nDepthMax)
{
	m_nQuadTreeDepthMax = nDepthMax;
	for (unsigned int i = 0; i < 6; ++i)
	{
		m_aQuadTrees[i].SetDepthMax(nDepthMax);
	}
}