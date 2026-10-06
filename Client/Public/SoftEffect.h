#pragma once
#include"Client_Defines.h"
#include"AlphaObject.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END
BEGIN(Client)
class CSoftEffect final : public CAlphaObject
{
private:
	CSoftEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSoftEffect(const CSoftEffect& Prototype);
	virtual ~CSoftEffect() = default;

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
	_float					m_fFrame = { 0.f }; //이펙트의 프레임은 90프레임이었음



private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();


public:
	static CSoftEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END
