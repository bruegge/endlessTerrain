#pragma once
#include <vector>
#include "glm/glm.hpp"

class CQuadTree
{
public:
	struct STiles
	{
		glm::vec3 vEdgePositions[4];
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
	void RecursiveDivideTile(unsigned int nDepth, glm::vec3 vEdge0, glm::vec3 vEdge1, glm::vec3 vEdge2, glm::vec3 vEdge3, glm::vec3 vCameraPosition);


	bool m_bHasChanged = false;
	std::vector<STiles>* m_pvecTiles;
	unsigned int m_nCountTiles = 0;
	unsigned int m_nDepthMax = 3;
	glm::vec3 m_vEdges[4];
};

