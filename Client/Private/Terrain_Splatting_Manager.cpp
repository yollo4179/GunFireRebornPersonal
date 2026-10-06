#include"stdafx.h"
#include"Terrain_Splatting_Manager.h"
#include"GameInstance.h"
IMPLEMENT_SINGLETON(CTerrain_Splatting_Manager)

CTerrain_Splatting_Manager::CTerrain_Splatting_Manager()
{
}

void CTerrain_Splatting_Manager::Initialize()
{
}

_int CTerrain_Splatting_Manager::Sing_Up_Splatting_Info(TERRAIN_SPLATTING_INFO_DESC Desc)
{
	if (m_iNowAssignedIndex >= -1 + g_iNumMaxSplattingInfo)
		return -1;
	m_iNowAssignedIndex++;
	m_ArrBrushRange[m_iNowAssignedIndex] = Desc.g_BrushRange;
	m_ArrBrushLook[m_iNowAssignedIndex] = Desc.g_vBrushTextureLook;
	m_ArrBrushPos[m_iNowAssignedIndex] = Desc.g_vBrushPos;
	m_ArrMovingBrushTexture[m_iNowAssignedIndex] = Desc.g_MovingBrushTexture;
	m_ArrRangeBrushTexture[m_iNowAssignedIndex] = Desc.g_BrushTexture;
	m_ArrBrushSign[m_iNowAssignedIndex] = Desc.g_bBrushSign;
	m_ArrMovingLifeTime[m_iNowAssignedIndex] = Desc.fAccMovingLifeTime;


	return m_iNowAssignedIndex;
}


void CTerrain_Splatting_Manager::Update(float fTimeDelta)
{
	if (nullptr == m_pTerrainShaderCom)
		return;

	for (int i = 0; i <= m_iNowAssignedIndex; ++i)
	{
		if (FAILED(m_pTerrainShaderCom->Bind_RawValue("g_BrushRange", m_ArrBrushRange, sizeof(_float2))))
			return;
		
	
		if (FAILED(m_pTerrainShaderCom->Bind_RawValue("g_vBrushTextureLook", m_ArrBrushLook, sizeof(_float4))))
			return;
		if (FAILED(m_pTerrainShaderCom->Bind_RawValue("g_vBrushPos", m_ArrBrushPos, sizeof(_float4))))
			return;

		if (nullptr != m_ArrMovingBrushTexture[m_iNowAssignedIndex])
		{
			if (FAILED(m_ArrMovingBrushTexture[m_iNowAssignedIndex]->Bind_ShaderResource(m_pTerrainShaderCom, "g_MovingBrushTexture")))
				return;
		}
		if (nullptr != m_ArrRangeBrushTexture[m_iNowAssignedIndex])
		{
			if (FAILED(m_ArrRangeBrushTexture[m_iNowAssignedIndex]->Bind_ShaderResource(m_pTerrainShaderCom, "g_BrushTexture")))
				return;
		}
		

		
		
		if (FAILED(m_pTerrainShaderCom->Bind_RawValue("g_bBrushSign", m_ArrBrushSign[m_iNowAssignedIndex], sizeof(_bool))))
			return;

		if (FAILED(m_pTerrainShaderCom->Bind_RawValue("g_fAccMovingLifeTime", m_ArrMovingLifeTime[m_iNowAssignedIndex], sizeof(_float))))
			return;

	}



}

void CTerrain_Splatting_Manager::Clear_Splatting_And_TerrainInfo()
{
}

void CTerrain_Splatting_Manager::Clear_SplattingInfo()
{
}


void CTerrain_Splatting_Manager::Free()
{
	__super::Free();
	for (int i = 0; i < g_iNumMaxSplattingInfo; ++i)
	{
		Safe_Release(m_ArrMovingBrushTexture[i]);
		Safe_Release(m_ArrRangeBrushTexture[i]);
	}
	Safe_Release(m_pTerrainShaderCom);
}
