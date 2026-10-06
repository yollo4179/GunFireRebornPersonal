#pragma once
#include"UIObject.h"
#include"Client_Defines.h"

BEGIN(Engine)
class CShader;
class CVIBuffer_Rect;
class CTexture;
END
BEGIN(Client)
class CUI_Arm1 final :public CUIObject
{
public:
	typedef struct : public CUIObject::UIOBJECT_DESC
	{
		_uint* pNowEquipedArmIndex;
	}UI_ARM1_DESC;
private:
	explicit CUI_Arm1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUI_Arm1(const CUI_Arm1& Prototype);
	virtual ~CUI_Arm1() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	_uint* m_pNowEquipedArmIndex = { nullptr };

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();


public:
	static   CUI_Arm1* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END


