#pragma once
#include "Planet.h"

class CSettings
{
public:
	static void CreateSettings(CPlanet* pPlanet);
	static CSettings* GetSettings();
	void ApplySettings();

	bool m_bEnableWireframe = false;
	unsigned int m_nGeometrySize = 10;
	unsigned int m_nMaxQuadTreeDepth = 3;
	unsigned int m_nInfoInstanceCount = 0;
	unsigned int m_nPerlinNoiseCount = 0;

private:

	CPlanet* m_pPlanet = nullptr;
	CSettings();
	~CSettings();
};

