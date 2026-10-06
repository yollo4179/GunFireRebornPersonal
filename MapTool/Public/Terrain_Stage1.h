#pragma once
#include"Terrain_Base.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CShader;
class CModel;
END


BEGIN(Client)
class CTerrain_Stage01 final :public CTerrain_Base
{
public :
	typedef struct : public CGameObject::GAMEOBJECT_DESC{


	}TERRAIN_STATGE01_DESC;

private:
	explicit CTerrain_Stage01(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CTerrain_Stage01(const CTerrain_Stage01& Prototype);
	virtual ~CTerrain_Stage01() =default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;



public:
	vector<TRIANGLE_VERTICES> m_vecTriangles;

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CTerrain_Stage01* Create(ID3D11Device* pDevice , ID3D11DeviceContext* pContext );
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END
