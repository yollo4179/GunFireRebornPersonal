#pragma once
#include"Monster.h"
#include"Client_Defines.h"

BEGIN(Client)
class CTestMonster final :public CMonster
{
private:
	explicit CTestMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CTestMonster(const CTestMonster& Prototype);
	virtual ~CTestMonster() = default;

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

	_float m_fAccTime = {0.f};
public:
	static CTestMonster* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free()override;

};
END

