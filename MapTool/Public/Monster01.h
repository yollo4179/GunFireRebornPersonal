#pragma once
#include"GameObject.h"
#include"Client_Defines.h"
#include"Monster.h"
BEGIN(Engine)
class CShader;
class CModel;
END


BEGIN(Client)
class CMonster01 final : public CMonster
{

private:
	explicit CMonster01(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CMonster01(const CMonster01& Prototype);
	virtual ~CMonster01() = default;


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
	static CMonster01* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free()override;

};

END