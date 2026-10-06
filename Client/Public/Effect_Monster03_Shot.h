#pragma once
#include"Client_Defines.h"
#include"GameObject.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END
BEGIN(Client)
class CEffect_Monster03_Shot final : public CGameObject
{
private:
	CEffect_Monster03_Shot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Monster03_Shot(const CEffect_Monster03_Shot& Prototype);
	virtual ~CEffect_Monster03_Shot() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	void Set_Matrix(_fmatrix fPosMatrix);
	void Set_Active(_bool isActive) { m_isActive = isActive; };
private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	_bool m_isActive = { true };

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Bind_ShaderResources_Glow();

public:
	static CEffect_Monster03_Shot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
