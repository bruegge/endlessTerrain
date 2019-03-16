#include "QuadTree.h"

CQuadTree::CQuadTree()
{
	m_pvecTiles = new std::vector<CQuadTree::STiles>(100000);
}

CQuadTree::~CQuadTree()
{
	delete m_pvecTiles;
}

void CQuadTree::SetEdges(glm::vec3 vEdge0, glm::vec3 vEdge1, glm::vec3 vEdge2, glm::vec3 vEdge3)
{
	m_vEdges[0] = vEdge0;
	m_vEdges[1] = vEdge1;
	m_vEdges[2] = vEdge2;
	m_vEdges[3] = vEdge3;
}

void CQuadTree::CalculateTiles(glm::vec3 vCameraPosition)
{
	unsigned int countTilesPref = m_nCountTiles;
	m_nCountTiles = 0;
	m_bHasChanged = false;
	RecursiveDivideTile(0, m_vEdges[0], m_vEdges[1], m_vEdges[2], m_vEdges[3], vCameraPosition);
	if (countTilesPref != m_nCountTiles)
	{
		m_bHasChanged = true;
	}
}

float GetLength(glm::vec3 v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

float GetAngle(glm::vec3 v1, glm::vec3 v2)
{
	return acos(glm::dot(v1, v2) / (GetLength(v1) * GetLength(v2)));
}

void CQuadTree::RecursiveDivideTile(unsigned int nDepth, glm::vec3 vEdge0, glm::vec3 vEdge1, glm::vec3 vEdge2, glm::vec3 vEdge3, glm::vec3 vCameraPosition)
{
	glm::vec3 vCenter = glm::normalize(vEdge0 + vEdge1 + vEdge2 + vEdge3 / 4.0f);
	float fMaxDistance = 2;

	float fDistance = std::fmin(std::fmax(0, glm::length(vCenter - vCameraPosition)), fMaxDistance);
	glm::vec3 vEdge0Normalized = glm::normalize(vEdge0);
	glm::vec3 vEdge1Normalized = glm::normalize(vEdge1);
	glm::vec3 vEdge2Normalized = glm::normalize(vEdge2);
	glm::vec3 vEdge3Normalized = glm::normalize(vEdge3);
	glm::vec3 vCameraPositionNormalized = glm::normalize(vCameraPosition);

	fDistance = std::fmin(std::fmin(std::fmax(0, glm::length(vEdge0Normalized - vCameraPosition)), fMaxDistance), fDistance);
	fDistance = std::fmin(std::fmin(std::fmax(0, glm::length(vEdge1Normalized - vCameraPosition)), fMaxDistance), fDistance);
	fDistance = std::fmin(std::fmin(std::fmax(0, glm::length(vEdge2Normalized - vCameraPosition)), fMaxDistance), fDistance);
	fDistance = std::fmin(std::fmin(std::fmax(0, glm::length(vEdge3Normalized - vCameraPosition)), fMaxDistance), fDistance);

	float fDivisionRate = (fMaxDistance - fDistance) / fMaxDistance * static_cast<float>(m_nDepthMax);

	float fAngle0 = GetAngle(vEdge0Normalized, glm::normalize(vCameraPosition - vEdge0Normalized)) * 180.0f / 3.1415926f;
	float fAngle1 = GetAngle(vEdge1Normalized, glm::normalize(vCameraPosition - vEdge1Normalized)) * 180.0f / 3.1415926f;
	float fAngle2 = GetAngle(vEdge2Normalized, glm::normalize(vCameraPosition - vEdge2Normalized)) * 180.0f / 3.1415926f;
	float fAngle3 = GetAngle(vEdge3Normalized, glm::normalize(vCameraPosition - vEdge3Normalized)) * 180.0f / 3.1415926f;
	float fAngleC = GetAngle(vCenter, vCameraPositionNormalized) * 180.0f / 3.1415926f;

	float fMinAngle = std::fmin(fAngleC, fAngle1);
	fMinAngle = std::fmin(fAngle0, fMinAngle);
	fMinAngle = std::fmin(fAngle1, fMinAngle);
	fMinAngle = std::fmin(fAngle2, fMinAngle);
	fMinAngle = std::fmin(fAngle3, fMinAngle);

	float fMaxAngle = 95;

	if (nDepth < fDivisionRate && fMinAngle < fMaxAngle)
	{
		//devide grid
		glm::vec3 vSplittedGrid[9];
		vSplittedGrid[0] = vEdge0;
		vSplittedGrid[2] = vEdge1;
		vSplittedGrid[6] = vEdge3;
		vSplittedGrid[8] = vEdge2;
		vSplittedGrid[4] = (vEdge0 + vEdge1 + vEdge2 + vEdge3) / 4.0f;
		vSplittedGrid[1] = (vSplittedGrid[2] + vSplittedGrid[0]) / 2.0f;
		vSplittedGrid[3] = (vSplittedGrid[0] + vSplittedGrid[6]) / 2.0f;
		vSplittedGrid[5] = (vSplittedGrid[2] + vSplittedGrid[8]) / 2.0f;
		vSplittedGrid[7] = (vSplittedGrid[6] + vSplittedGrid[8]) / 2.0f;

		RecursiveDivideTile(nDepth + 1, vSplittedGrid[0], vSplittedGrid[1], vSplittedGrid[4], vSplittedGrid[3], vCameraPosition);
		RecursiveDivideTile(nDepth + 1, vSplittedGrid[1], vSplittedGrid[2], vSplittedGrid[5], vSplittedGrid[4], vCameraPosition);
		RecursiveDivideTile(nDepth + 1, vSplittedGrid[4], vSplittedGrid[5], vSplittedGrid[8], vSplittedGrid[7], vCameraPosition);
		RecursiveDivideTile(nDepth + 1, vSplittedGrid[3], vSplittedGrid[4], vSplittedGrid[7], vSplittedGrid[6], vCameraPosition);
	}
	else
	{
		//save to tileList
		unsigned int nIndex = m_nCountTiles++;
		if (m_pvecTiles->at(nIndex).vEdgePositions[0] != vEdge0 || m_pvecTiles->at(nIndex).vEdgePositions[1] != vEdge1 || m_pvecTiles->at(nIndex).vEdgePositions[2] != vEdge2 || m_pvecTiles->at(nIndex).vEdgePositions[3] != vEdge3)
		{
			m_bHasChanged = true;
			m_pvecTiles->at(nIndex).vEdgePositions[0] = vEdge0;
			m_pvecTiles->at(nIndex).vEdgePositions[1] = vEdge1;
			m_pvecTiles->at(nIndex).vEdgePositions[2] = vEdge2;
			m_pvecTiles->at(nIndex).vEdgePositions[3] = vEdge3;
		}
	}
}

unsigned int CQuadTree::GetTileCount()
{
	return m_nCountTiles;
}

std::vector<CQuadTree::STiles>* CQuadTree::GetTiles()
{
	return m_pvecTiles;
}

bool CQuadTree::IsChanged()
{
	return m_bHasChanged;
}

void CQuadTree::SetDepthMax(unsigned int nDepthMax)
{
	m_nDepthMax = nDepthMax;
}