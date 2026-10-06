#pragma once
#include"UIObject.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CShader;
class CVIBuffer_Rect;
class CTexture;
END
BEGIN(Client)
class CUI_MonsterHP_Blue final : public CUIObject
{
public:
	typedef struct : public CUIObject::UIOBJECT_DESC
	{
		_float4x4* pMonsterHpFloat4x4;
		_float* pViewZDistance;
		_float4* pMonsterHPInfo;
		_float2   vSize;
	}UI_MONSTER_HP_BLUE_DESC;

private:
	explicit CUI_MonsterHP_Blue(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUI_MonsterHP_Blue(const CUI_MonsterHP_Blue& Prototype);
	virtual ~CUI_MonsterHP_Blue() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


private:
	void Update_Set_Transform();

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CTexture* m_pTextureCom_BG = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	_float4x4* m_pMonsterHpFloat4x4 = { nullptr };
	_float* m_pViewZDistance = { nullptr };
	_float4* m_pMonsterHPInfo = { nullptr };
	_float2 m_vSize = {};
	_float m_fTimeDelta = 0;

	/*ForBG*/
	_float m_fPrevRatio = {};
	_bool m_bHitSign = {};
	

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Bind_ShaderResources_ForBG();

public:
	static   CUI_MonsterHP_Blue* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END


