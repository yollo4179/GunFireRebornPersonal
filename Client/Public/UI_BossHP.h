#pragma once
#include"UI_Parent.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUI_BossHP final : public CUI_Parent 
{
public:
	typedef struct :CUIObject::UIOBJECT_DESC
	{
		_float2* pBossHPInfo;
		_uint iBossNumber;
		/*보스가 여러개라면 보스 텍스처를 위한 인덱스도 추가하자*/
	} UI_BOSS_HP_DESC;

private:
	explicit CUI_BossHP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUI_BossHP(const CUI_BossHP& Prototype);
	virtual ~CUI_BossHP() =default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	HRESULT Ready_Components();
	HRESULT BindAndRender_ShaderResources_BG();
	HRESULT BindAndRender_ShaderResources_BossImg();
	HRESULT BindAndRender_ShaderResources_HP_Red();
	HRESULT BindAndRender_ShaderResources_HP_Gray();
	HRESULT BindAndRender_ShaderResources_HP_Gray_Back();

	HRESULT Ready_ChildUIs();
public:
	void Init_HPInfo(_float2* pPlayerHPInfo) { m_pBossHPInfo = pPlayerHPInfo; };
	void Set_szBossName(_tchar* p_szBossName) { memcpy(m_szBossName, p_szBossName, sizeof(m_szBossName)); };


private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom_BG = { nullptr };
	CTexture* m_pTextureCom_BossImg = { nullptr };
	CTexture* m_pTextureCom_Red = { nullptr };
	CTexture* m_pTextureCom_Gray = { nullptr };
	CTexture* m_pTextureCom_Back = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	
	_float2* m_pBossHPInfo = { nullptr };
	_int m_iBossNumber = {};

	/*for After HpReduction*/
	_float m_fPrevRatio = {};
	_bool m_bHitSign = {};
	
	_tchar m_szBossName[MAX_PATH] = L"Abyssal Serpent";

public:
	static CUI_BossHP* Create(ID3D11Device* pDeivce, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END

