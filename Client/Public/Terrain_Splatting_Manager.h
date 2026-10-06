#pragma once
#include"Base.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CShader;
class CTexture;
END
//디바이스도 필요 없을거같다 .

const int g_iNumMaxSplattingInfo = 20;
BEGIN(Client)
/*다음 레벨로 이동한다면 오브젝트 먼저 지우기 전에 클리어를 하자.*/
class CTerrain_Splatting_Manager final:public CBase
{

	DECLARE_SINGLETON(CTerrain_Splatting_Manager);

public:
	typedef struct
	{
		_float2* g_BrushRange;
		_float4* g_vBrushTextureLook;
		_float4* g_vBrushPos;
		CTexture* g_MovingBrushTexture;
		CTexture* g_BrushTexture;
		_bool*	g_bBrushSign;
		_float* fAccMovingLifeTime;


	}TERRAIN_SPLATTING_INFO_DESC;
private:
	CTerrain_Splatting_Manager();
	virtual ~CTerrain_Splatting_Manager() =default;

public:
	void Initialize();
	void Set_TerrainShader(CShader* pShader) { m_pTerrainShaderCom = pShader; Safe_AddRef(m_pTerrainShaderCom); };
	
	_int Sing_Up_Splatting_Info(TERRAIN_SPLATTING_INFO_DESC);
	void Update(float fTimeDelta);

	void Clear_Splatting_And_TerrainInfo();
	void Clear_SplattingInfo();
	_float2*  m_ArrBrushRange[g_iNumMaxSplattingInfo] = {};
	_float4*  m_ArrBrushLook[g_iNumMaxSplattingInfo] = {};
	_float4*  m_ArrBrushPos[g_iNumMaxSplattingInfo] = {};
	CTexture* m_ArrMovingBrushTexture[g_iNumMaxSplattingInfo] = {};
	CTexture* m_ArrRangeBrushTexture[g_iNumMaxSplattingInfo] = {};
	_bool*    m_ArrBrushSign[g_iNumMaxSplattingInfo] = {};
	_float*   m_ArrMovingLifeTime[g_iNumMaxSplattingInfo] = {};

	int m_iNowAssignedIndex = -1;
private:
	CShader* m_pTerrainShaderCom = { nullptr };

public:
	virtual void Free() override;
};
END

