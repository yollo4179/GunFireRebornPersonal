#pragma once
#include"UI_Parent.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CShader;
class CTexture;
END

const int g_iNumGoblets = 6;
BEGIN(Client)
class CUI_Goblet final :public CUI_Parent
{
public:
	typedef struct : public CUIObject::UIOBJECT_DESC
	{
		//1280 720 -> 100, 100, -- 1180 620
	}UI_GOBLET_DESC;
	

private:
	explicit  CUI_Goblet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUI_Goblet(const CUI_Goblet& Prototype);
	virtual ~CUI_Goblet() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Reset_RandomGoblets();
	virtual void Set_Active()override;
	virtual void Set_InActive()override;

	void Set_NpcNo(_uint iNpcNo) { m_iNpcNo = iNpcNo; };
	_uint Get_NowNpcNo() { return m_iNpcNo; };
private:
	HRESULT  Ready_Components();
	HRESULT  Ready_ChildUIs();
	HRESULT  Bind_ShaderResources();

private:
	CShader* m_pShaderCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom_BackGround = { nullptr };

	_uint m_iNpcNo = {};
public:
	static   CUI_Goblet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

	_uint m_iGobletIndex[2] = {};
};
END
