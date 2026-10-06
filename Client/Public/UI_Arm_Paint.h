#pragma once
#include"UIObject.h"
#include"Client_Defines.h"

BEGIN(Engine)
class CShader;
class CVIBuffer_Rect;
class CTexture;
END
BEGIN(Client)
class CUI_Arm_Paint final :public CUIObject
{
public:
	typedef struct : public CUIObject::UIOBJECT_DESC
	{
		_uint* pNowArmNumber;

	}UI_ARM_PAINT_DESC;
private:
	explicit CUI_Arm_Paint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUI_Arm_Paint(const CUI_Arm_Paint& Prototype);
	virtual ~CUI_Arm_Paint() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Render_BG();
	HRESULT Render_CurrentArm();

	//void Map_Convert(_uint iIndexArmInPlayer, _uint iIndexArmInUI);

private:
	CShader* m_pShaderCom = {nullptr};

	CTexture* m_pTextureCom[2] = {nullptr,nullptr};
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	_uint* m_pNowArmNumber = { nullptr };
	
	

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources(_uint iTextureIndex,_uint iNowArmTexture=0);


public:
	static   CUI_Arm_Paint* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END

