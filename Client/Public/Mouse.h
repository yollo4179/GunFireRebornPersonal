#pragma once
#include"UIObject.h"
#include"Client_Defines.h"

enum class MOUSE_STATE
{
	MOUSE_NONE,
	MOUSE_DRAGGING,
	MOUSE_CLICKED,
	MOUSE_END,
};
BEGIN(Engine)
class CTexture;
class CShader;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CMouse final :public CUIObject
{
private:
	explicit CMouse(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CMouse(const CMouse& Prototype);
	virtual ~CMouse()=default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


private:
	void Update_MousePos();

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CTexture* m_pTextureCom_Target = {nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	_float2 m_vPos;

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CMouse* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END
