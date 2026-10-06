#pragma once
#include"GameObject.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CVIBuffer_Trail;
class CShader;
class CTexture;
END
BEGIN(Client)
class CTrail_Monster05 final :public CGameObject
{
public:
	typedef struct
	{

	}TRAIL_MONSTER05_DESC;

private:
	CTrail_Monster05(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTrail_Monster05(const CTrail_Monster05& Prototype);
	virtual ~CTrail_Monster05() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void Set_BufferMatrix(_fmatrix matHigh, _fmatrix matLow);
	void Reset_VerticesCnt();
	void Resize_Vertices();

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Trail* m_pVIBufferCom = { nullptr };
	_bool m_bActive = { true };

	_float m_fAccTime = 0;
	_float m_fTime = { 360.f };
private:
	HRESULT Ready_Components();
	
	HRESULT Bind_ShaderResources();
	HRESULT Bind_ShaderResources_Glow();

public:
	static CTrail_Monster05* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
