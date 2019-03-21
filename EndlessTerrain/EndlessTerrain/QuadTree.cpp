#include "QuadTree.h"
#include <queue>
#include "Settings.h"
#include "glm\gtx\vector_angle.hpp"

CQuadTree::CQuadTree()
{
	m_pvecTiles = new std::vector<CQuadTree::STiles>(100000);
	m_pCountTiles = new unsigned int();
	*m_pCountTiles = 0;
	
	for (unsigned int i = 0; i < 100000; ++i)
	{
		aNodesTree[i].SetZero();
	}
}

CQuadTree::CQuadTree(bool bEnableQuadTreeSphere)
{

}

CQuadTree::~CQuadTree()
{
	delete m_pvecTiles;
	delete m_pCountTiles;
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
	unsigned int countTilesPref = *m_pCountTiles;
	*m_pCountTiles = 0;
	m_bHasChanged = false;

	//build a new tree
	for (unsigned int i = 0; i < 4; ++i)
	{
		aNodesTree->sEdgePositions.vEdgePositions[i] = m_vEdges[i];
	}
	aNodesTree->nDepth = 0;

	std::queue<CQuadTree::SNodes*> qQueue;
	qQueue.push(&aNodesTree[0]);
	bool bHasChanges = ExpandTree(vCameraPosition, &qQueue);
	
	if (CSettings::GetSettings()->m_bEnableTJunctionElimination)
	{
		EliminateTJunctions();
	}
	
	if (countTilesPref != *m_pCountTiles || bHasChanges)
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
	v1 = glm::normalize(v1);
	v2 = glm::normalize(v2);
	float fAngle = glm::angle(v1, v2);

	//float fAngle2 = acos(glm::dot(v1, v2) / (GetLength(v1) * GetLength(v2)));
	

	return fAngle;
}

unsigned int CQuadTree::GetTileCount()
{
	return *m_pCountTiles;
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

bool CQuadTree::ExpandTree(glm::vec3 vCameraPosition, std::queue<CQuadTree::SNodes*>* pQueue)
{
	unsigned int nNodePosition = 0;
	bool bHasChanges = false;
	while (!pQueue->empty())
	{
		SNodes* pTempNode = pQueue->front();
		pQueue->pop();

		//check if we are in camera view
		glm::vec3 vCenterPosition = (pTempNode->sEdgePositions.vEdgePositions[0] + pTempNode->sEdgePositions.vEdgePositions[1] + pTempNode->sEdgePositions.vEdgePositions[2] + pTempNode->sEdgePositions.vEdgePositions[3])*0.25f;
		glm::vec3 vEdge0ToCamera = vCameraPosition - glm::normalize(pTempNode->sEdgePositions.vEdgePositions[0]);
		glm::vec3 vEdge1ToCamera = vCameraPosition - glm::normalize(pTempNode->sEdgePositions.vEdgePositions[1]);
		glm::vec3 vEdge2ToCamera = vCameraPosition - glm::normalize(pTempNode->sEdgePositions.vEdgePositions[2]);
		glm::vec3 vEdge3ToCamera = vCameraPosition - glm::normalize(pTempNode->sEdgePositions.vEdgePositions[3]);
		glm::vec3 vCenterToCamera = vCameraPosition - glm::normalize(vCenterPosition);

		float fAngle = GetAngle(vEdge0ToCamera, pTempNode->sEdgePositions.vEdgePositions[0]);
		fAngle = std::fmin(GetAngle(vEdge1ToCamera, pTempNode->sEdgePositions.vEdgePositions[1]), fAngle);
		fAngle = std::fmin(GetAngle(vEdge2ToCamera, pTempNode->sEdgePositions.vEdgePositions[2]), fAngle);
		fAngle = std::fmin(GetAngle(vEdge3ToCamera, pTempNode->sEdgePositions.vEdgePositions[3]), fAngle);
		fAngle = std::fmin(GetAngle(vCenterToCamera, vCenterPosition), fAngle);

		if (fAngle * 180.0f / 3.1415926f < 100) //is in view
		{
			bool bSaveTile = true;
			//check if we need to subdivide
			if (pTempNode->nDepth < m_nDepthMax)
			{
				//calculate edges to camera angle
				fAngle = GetAngle(vEdge0ToCamera, vEdge1ToCamera);
				fAngle = std::fmax(GetAngle(vEdge0ToCamera, vEdge2ToCamera), fAngle);
				fAngle = std::fmax(GetAngle(vEdge0ToCamera, vEdge3ToCamera), fAngle);
				fAngle = std::fmax(GetAngle(vEdge1ToCamera, vEdge2ToCamera), fAngle);
				fAngle = std::fmax(GetAngle(vEdge1ToCamera, vEdge3ToCamera), fAngle);
				fAngle = std::fmax(GetAngle(vEdge2ToCamera, vEdge3ToCamera), fAngle);

				if (fAngle * 180.0f / 3.1415926f > CSettings::GetSettings()->m_fTileDivisionAngle)
				{
					bSaveTile = false;
					glm::vec3 vEdge0 = pTempNode->sEdgePositions.vEdgePositions[0];
					glm::vec3 vEdge2 = pTempNode->sEdgePositions.vEdgePositions[1];
					glm::vec3 vEdge6 = pTempNode->sEdgePositions.vEdgePositions[3];
					glm::vec3 vEdge8 = pTempNode->sEdgePositions.vEdgePositions[2];
					glm::vec3 vEdge1 = (vEdge0 + vEdge2) * 0.5f;
					glm::vec3 vEdge3 = (vEdge0 + vEdge6) * 0.5f;
					glm::vec3 vEdge5 = (vEdge2 + vEdge8) * 0.5f;
					glm::vec3 vEdge7 = (vEdge6 + vEdge8) * 0.5f;
					glm::vec3 vEdge4 = (vEdge1 + vEdge7) * 0.5f;

					//subdivide
					nNodePosition++;
					aNodesTree[nNodePosition].SetPropertys(pTempNode->nDepth + 1, vEdge0, vEdge1, vEdge4, vEdge3);
					pQueue->push(&aNodesTree[nNodePosition]);
					pTempNode->pChildren[0] = &aNodesTree[nNodePosition];
					nNodePosition++;
					aNodesTree[nNodePosition].SetPropertys(pTempNode->nDepth + 1, vEdge1, vEdge2, vEdge5, vEdge4);
					pQueue->push(&aNodesTree[nNodePosition]);
					pTempNode->pChildren[1] = &aNodesTree[nNodePosition];
					nNodePosition++;
					aNodesTree[nNodePosition].SetPropertys(pTempNode->nDepth + 1, vEdge3, vEdge4, vEdge7, vEdge6);
					pQueue->push(&aNodesTree[nNodePosition]);
					pTempNode->pChildren[2] = &aNodesTree[nNodePosition];
					nNodePosition++;
					aNodesTree[nNodePosition].SetPropertys(pTempNode->nDepth + 1, vEdge4, vEdge5, vEdge8, vEdge7);
					pQueue->push(&aNodesTree[nNodePosition]);
					pTempNode->pChildren[3] = &aNodesTree[nNodePosition];
				}
			}
			if (bSaveTile)
			{
				//add node to tileList
				
				if (!m_pvecTiles->at(*m_pCountTiles).IsSame(&pTempNode->sEdgePositions))
				{
					m_pvecTiles->at(*m_pCountTiles).vEdgePositions[0] = pTempNode->sEdgePositions.vEdgePositions[0];
					m_pvecTiles->at(*m_pCountTiles).vEdgePositions[1] = pTempNode->sEdgePositions.vEdgePositions[1];
					m_pvecTiles->at(*m_pCountTiles).vEdgePositions[2] = pTempNode->sEdgePositions.vEdgePositions[2];
					m_pvecTiles->at(*m_pCountTiles).vEdgePositions[3] = pTempNode->sEdgePositions.vEdgePositions[3];
					m_pvecTiles->at(*m_pCountTiles).fFlagTJunctions = pTempNode->nDepth;
					
					bHasChanges = true;
				}
				*m_pCountTiles = (*m_pCountTiles) + 1;
			}
		}
	}
	return bHasChanges;
}

unsigned int CQuadTree::GetDepth(glm::vec3 vPosition)
{
	SNodes* pTempNode = &aNodesTree[0];
	float fDepth = 0;
	while (pTempNode)
	{
		bool bHasChildren = false;
		for (unsigned int i = 0; i < 4; i++)
		{
			SNodes* pTempNodeCild = pTempNode->pChildren[i];
			if (pTempNodeCild)
			{
				if (pTempNodeCild->sEdgePositions.IsPointInside(vPosition))
				{
					bHasChildren = true;
					pTempNode = pTempNodeCild;
					fDepth++;
					break;
				}
			}
		}
		if (!bHasChildren)
		{
			break;
		}
	}
	return fDepth;
}

void CQuadTree::EliminateTJunctions()
{
	for (unsigned int i = 0; i < *m_pCountTiles; ++i)
	{
		STiles* pTemp = &m_pvecTiles->at(i);
		glm::vec3 vEdge0 = pTemp->vEdgePositions[0];
		glm::vec3 vEdge1 = pTemp->vEdgePositions[1];
		glm::vec3 vEdge2 = pTemp->vEdgePositions[3];

		glm::vec3 vXDirection = vEdge1 - vEdge0;
		glm::vec3 vYDirection = vEdge2 - vEdge0;

		unsigned int nTJunctionFlag = 0;
		glm::vec3 vCenter = (vEdge1 + vEdge2) * 0.5f;
		glm::vec3 v0 = vCenter - vYDirection;
		glm::vec3 v1 = vCenter - vXDirection;
		glm::vec3 v2 = vCenter + vYDirection;
		glm::vec3 v3 = vCenter + vXDirection;

		unsigned int nDepth = GetDepth(vCenter);

		if (GetDepth(v0) < nDepth) 
		{
			nTJunctionFlag |= 0x00000001;
		}
		if (GetDepth(v1) < nDepth) 
		{
			nTJunctionFlag |= 0x00000002;
		}
		if (GetDepth(v2) < nDepth) 
		{
			nTJunctionFlag |= 0x00000004;
		}
		if (GetDepth(v3) < nDepth) 
		{
			nTJunctionFlag |= 0x00000008;
		}

		float fTJunction = *reinterpret_cast<float*>(&nTJunctionFlag);
		pTemp->fFlagTJunctions = fTJunction;
	}
}

//***************************CQuadTreeSphere***************************//


CQuadTreeSphere::CQuadTreeSphere()
{
	m_pvecTiles = new std::vector<CQuadTree::STiles>(100000);

	for (unsigned int i = 0; i < 6; ++i)
	{
		m_vecQuadTrees.push_back(new CQuadTree(false));
		m_vecQuadTrees[i]->m_pvecTiles = m_pvecTiles;
		m_vecQuadTrees[i]->m_pCountTiles = &m_nCountTiles;
	}
}

CQuadTreeSphere::~CQuadTreeSphere()
{
	for (unsigned int i = 5; i >= 0; --i)
	{
		delete m_vecQuadTrees[i];
	}
	m_vecQuadTrees.clear();

	delete m_pvecTiles;
}

void CQuadTreeSphere::CalculateTiles()
{

}
