#pragma once
#include"Client_Defines.h"
#include"AlphaObject.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END
BEGIN(Client)
class CMonster09_WarningSign final : public CAlphaObject
{
private:
	CMonster09_WarningSign(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster09_WarningSign(const CMonster09_WarningSign& Prototype);
	virtual ~CMonster09_WarningSign() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	void Set_Vector(_fvector vecPos);

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	_float					m_fFrame = { 5.f }; //이펙트의 프레임은 90프레임이었음



private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Bind_ShaderResources_Glow();


public:
	static CMonster09_WarningSign* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END