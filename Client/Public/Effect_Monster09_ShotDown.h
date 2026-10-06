#pragma once
#include"Client_Defines.h"
#include"AlphaObject.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END
BEGIN(Client)
class CEffect_Monster09_ShotDown final : public CAlphaObject
{
private:
	CEffect_Monster09_ShotDown(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Monster09_ShotDown(const CEffect_Monster09_ShotDown& Prototype);
	virtual ~CEffect_Monster09_ShotDown() = default;

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

public:
	void Set_Matrix(_fmatrix PosMatrix);



private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();


public:
	static CEffect_Monster09_ShotDown* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
