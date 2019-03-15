#include "Planet.h"
#include <vector>
#include <glm\gtx\transform.hpp>


CPlanet::CPlanet()
{
	RandomizeNoise();
}


CPlanet::~CPlanet()
{
}

void CPlanet::InitializeGeometry(unsigned int nGridWidth)
{
	glGenVertexArrays(1, &m_nVAO);
	glBindVertexArray(m_nVAO);
	glGenBuffers(1, &m_nVBO);
	glGenBuffers(1, &m_nIBO);

	std::vector<glm::vec3> resultVBO(nGridWidth * nGridWidth);

	for (unsigned int x = 0; x < nGridWidth; ++x)
	{
		for (unsigned int y = 0; y < nGridWidth; ++y)
		{
			glm::vec3 sVertex;
			GLfloat fXPosition = static_cast<GLfloat>(x) / static_cast<GLfloat>(nGridWidth - 1);
			GLfloat fYPosition = static_cast<GLfloat>(y) / static_cast<GLfloat>(nGridWidth - 1);

			sVertex = glm::vec3((fXPosition - 0.5f)*2.0f, (fYPosition - 0.5f)*2.0f, 1);
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

			resultIBO[nVectorPosition++] = x * nGridWidth + y;
			resultIBO[nVectorPosition++] = (x + 1) * nGridWidth + y;
			resultIBO[nVectorPosition++] = (x + 1) * nGridWidth + y + 1;

			resultIBO[nVectorPosition++] = x * nGridWidth + y;
			resultIBO[nVectorPosition++] = (x + 1) * nGridWidth + y + 1;
			resultIBO[nVectorPosition++] = x * nGridWidth + y + 1;
			
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * resultVBO.size(), &resultVBO.front(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * resultIBO.size(), &resultIBO.front(), GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, NULL); //position
	

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
	pShader->Bind();
	glBindVertexArray(m_nVAO);

	GLint nUniformLocationViewProjectionMatrix = glGetUniformLocation(pShader->GetID(), "viewProjectionMatrix");
	glUniformMatrix4fv(nUniformLocationViewProjectionMatrix, 1, GL_FALSE, &pCamera->GetViewProjectionMatrix()[0][0]);

	glUniform1iv(glGetUniformLocation(pShader->GetID(), "perm"), 512, m_aPerm);

	GLint nUniformLocationModelMatrix = glGetUniformLocation(pShader->GetID(), "modelMatrix");
	glm::mat4 mModelMatrix;
	for (int i = 0; i < 6; ++i)
	{
		mModelMatrix[0] = glm::vec4(1, 0, 0, 0);
		mModelMatrix[1] = glm::vec4(0, 1, 0, 0);
		mModelMatrix[2] = glm::vec4(0, 0, 1, 0);
		mModelMatrix[3] = glm::vec4(0, 0, 0, 1);

		switch (i)
		{
		case 0:
			break;
		case 1:
			mModelMatrix = glm::rotate(90.0f*3.1415926f / 180.0f, glm::vec3(0, 1, 0));
			break;
		case 2:
			mModelMatrix = glm::rotate(90.0f*3.1415926f / 180.0f, glm::vec3(0, -1, 0));
			break;
		case 3:
			mModelMatrix = glm::rotate(90.0f*3.1415926f / 180.0f, glm::vec3(1, 0, 0));
			break;
		case 4:
			mModelMatrix = glm::rotate(90.0f*3.1415926f / 180.0f, glm::vec3(-1, 0, 0));
			break;
		case 5:
			mModelMatrix = glm::rotate(180.0f*3.1415926f / 180.0f, glm::vec3(0, 1, 0));
			break;

		default:
			break;
		}
		glUniformMatrix4fv(nUniformLocationModelMatrix, 1, GL_FALSE, &mModelMatrix[0][0]);

		glDrawElements(GL_TRIANGLES, m_nCountIBO, GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);
	pShader->UnBind();
}