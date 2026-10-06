#pragma once
#include"GameObject.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CShader;
class CModel;
class CNavigation;
END


BEGIN(Client)
class CTerrain_Base abstract :public CGameObject
{
public:
	typedef struct : public CGameObject::GAMEOBJECT_DESC {

		_float3 vScale;
		_float4 vPosition;
		_float	fRotation;
		_float4  vAxis;

	}TERRAIN_BASE_DESC;

protected:
	explicit CTerrain_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CTerrain_Base(const CTerrain_Base& Prototype);
	virtual ~CTerrain_Base() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void Update_Transform_By_Desc(TERRAIN_BASE_DESC Desc);
	void Update_Transform_Go_Straight(const _float fTimeDelta);
	void Update_Transform_Go_Backward(const _float fTimeDelta);

protected:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CNavigation* m_pNavigationCom = { nullptr };
public:
	TERRAIN_BASE_DESC m_Desc;
private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:

	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END