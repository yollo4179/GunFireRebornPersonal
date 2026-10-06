#pragma once
#include"Terrain_Base.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CShader;
class CModel;
END


BEGIN(Client)
class CTerrain_Stage02 final :public CTerrain_Base
{
public:
	typedef struct : public CGameObject::GAMEOBJECT_DESC {


	}TERRAIN_STATGE01_DESC;

private:
	explicit CTerrain_Stage02(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CTerrain_Stage02(const CTerrain_Stage02& Prototype);
	virtual ~CTerrain_Stage02() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;




private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CTerrain_Stage02* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END
