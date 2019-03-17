#include "Settings.h"
#include <glad/glad.h>

static CSettings* pGlobalSettings = nullptr;

CSettings::CSettings()
{
}


CSettings::~CSettings()
{
}

void CSettings::CreateSettings(CPlanet* pPlanet)
{
	if (!pGlobalSettings)
	{
		pGlobalSettings = new CSettings();
		pGlobalSettings->m_pPlanet = pPlanet;
		pGlobalSettings->m_nGeometrySize = pPlanet->GetGeometrySize();
	}
}

CSettings* CSettings::GetSettings()
{
	return pGlobalSettings;
}

void CSettings::ApplySettings()
{
	if(m_bEnableWireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (m_pPlanet->GetGeometrySize() != m_nGeometrySize)
	{
		m_pPlanet->SetGeometrySize(m_nGeometrySize);
	}
	if (m_pPlanet->GetQuadTreeDepthMax() != m_nMaxQuadTreeDepth)
	{
		m_pPlanet->SetQuadTreeDepthMax(m_nMaxQuadTreeDepth);
	}
	m_pPlanet->SetPerlinNoiseCount(m_nPerlinNoiseCount);
}