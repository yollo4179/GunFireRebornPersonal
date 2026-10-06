#pragma once
#include"UI_Parent.h"
#include"Client_Defines.h"

BEGIN(Client)
class CUI_MonsterHP final:public CUI_Parent
{
public:
	typedef struct :public CUIObject::UIOBJECT_DESC
	{
		_float4x4* UIWorlFloat4x4;
		_float* pViewZDistance;
		_float4* pHPInfo;
	}UI_MONSTER_HP_DESC;


private:
	CUI_MonsterHP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_MonsterHP(const CUI_MonsterHP& Prototype);
	virtual ~CUI_MonsterHP() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	//void GetUIMatrixFromWorld(_fmatrix matWorld) { XMStoreFloat4x4(m_UIWorlFloat4x4, matWorld); };

private:
	HRESULT Ready_ChildUIs();
	HRESULT Bind_ShaderResources();

private:
	_float4x4* m_UIWorlFloat4x4 = {};
	_float* m_pViewZDistance = { 0 };
	_float4* m_pHPInfo = { nullptr };
public:
	static   CUI_MonsterHP* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};
END

