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
		pGlobalSettings->m_fTileDivisionAngle = 10.0f;
		pGlobalSettings->m_bEnableQuadTreeUpdate = true;
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
	if (m_bEnableCullFace)
	{
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDisable(GL_CULL_FACE);
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