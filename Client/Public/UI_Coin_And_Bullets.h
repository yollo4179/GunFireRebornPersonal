#pragma once
#include"UI_Parent.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END
BEGIN(Client)
class CUI_Coin_And_Bullets final :public CUI_Parent
{
public:
	/*percent로 나타낼것인지 그냥 숫자로 나타낼것인지  precentage 가 true라면 어떤 숫자가 들어갈 것인지 */
	typedef struct : public CUIObject::UIOBJECT_DESC
	{
	

	}UI_COIN_AND_BULLET_DESC;



private:
	explicit CUI_Coin_And_Bullets(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUI_Coin_And_Bullets(const CUI_Coin_And_Bullets& Prototype);
	virtual ~CUI_Coin_And_Bullets() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();
	HRESULT Ready_Numbers();
	HRESULT BindAndRender_ShaderResources_Coin();
	HRESULT BindAndRender_ShaderResources_bullet();
private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom_Coin = { nullptr };
	CTexture* m_pTextureCom_Bullet = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

public:
	static  CUI_Coin_And_Bullets* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
