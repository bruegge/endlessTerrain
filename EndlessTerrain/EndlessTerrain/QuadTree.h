#pragma once
#include <vector>
#include "glm/glm.hpp"
#include <queue>

class CQuadTreeSphere;

class CQuadTree
{
	friend class CQuadTreeSphere;
public:
	struct STiles
	{
		glm::vec3 vEdgePositions[4];
		float fFlagTJunctions = 0;
		
		void SetZero()
		{
			for (unsigned int i = 0; i < 4; ++i)
			{
				vEdgePositions[i] = glm::vec3(0, 0, 0);
			}
		}

		bool IsPointInside(glm::vec3 vPoint)
		{
			glm::vec3 vMin = glm::vec3(100, 100, 100);
			glm::vec3 vMax = glm::vec3(-100, -100, -100);
			for (unsigned int i = 0; i < 4; ++i)
			{
				vMin.x = std::fmin(vMin.x, vEdgePositions[i].x);
				vMin.y = std::fmin(vMin.y, vEdgePositions[i].y);
				vMin.z = std::fmin(vMin.z, vEdgePositions[i].z);

				vMax.x = std::fmax(vMax.x, vEdgePositions[i].x);
				vMax.y = std::fmax(vMax.y, vEdgePositions[i].y);
				vMax.z = std::fmax(vMax.z, vEdgePositions[i].z);
			}
			glm::vec3 vSize = vMax - vMin;

			return vPoint.x >= vMin.x && vPoint.x <= vMax.x && vPoint.y >= vMin.y && vPoint.y <= vMax.y && vPoint.z >= vMin.z && vPoint.z <= vMax.z;
		}

		bool IsSame(STiles* pTile)
		{
			for (unsigned int i = 0; i < 4; ++i)
			{
				if (vEdgePositions[i] != pTile->vEdgePositions[i])
				{
					return false;
				}
			}
			return true;
		}
	};

	CQuadTree();
	~CQuadTree();

	void SetEdges(glm::vec3 vEdge0, glm::vec3 vEdge1, glm::vec3 vEdge2, glm::vec3 vEdge3);
	void CalculateTiles(glm::vec3 vCameraPosition);
	unsigned int GetTileCount();
	std::vector<STiles>* GetTiles();
	bool IsChanged();
	void SetDepthMax(unsigned int nDepthMax);

private:
	struct SNodes
	{
		STiles sEdgePositions;
		SNodes* pChildren[4];
		unsigned int nDepth;

		void SetZero()
		{
			sEdgePositions.SetZero();
			for (unsigned int i = 0; i < 4; ++i)
			{
				pChildren[i] = nullptr;
			}
		}

		void SetPropertys(unsigned int nDepth, glm::vec3 vEdge0, glm::vec3 vEdge1, glm::vec3 vEdge2, glm::vec3 vEdge3)
		{
			this->nDepth = nDepth;
			sEdgePositions.vEdgePositions[0] = vEdge0;
			sEdgePositions.vEdgePositions[1] = vEdge1;
			sEdgePositions.vEdgePositions[2] = vEdge2;
			sEdgePositions.vEdgePositions[3] = vEdge3;
			for (unsigned int i = 0; i < 4; ++i)
			{
				pChildren[i] = nullptr;
			}
		}
	};
	bool ExpandTree(glm::vec3 vCameraPosition, std::queue<CQuadTree::SNodes*>* pQueue);
	
	CQuadTree(bool bEnableQuadTreeSphere);
	unsigned int GetDepth(glm::vec3 vPosition);
	void EliminateTJunctions();

	bool m_bHasChanged = false;
	std::vector<STiles>* m_pvecTiles;
	unsigned int* m_pCountTiles = 0;
	unsigned int m_nDepthMax = 3;
	glm::vec3 m_vEdges[4];
	SNodes aNodesTree[100000];
};

class CQuadTreeSphere
{
public:
	CQuadTreeSphere();
	~CQuadTreeSphere();

	void CalculateTiles();
private:
	
	std::vector<CQuadTree::STiles>* m_pvecTiles;
	std::vector<CQuadTree*> m_vecQuadTrees;
	unsigned int m_nCountTiles = 0;


};