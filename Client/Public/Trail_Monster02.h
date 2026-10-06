#pragma once
#include"GameObject.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CVIBuffer_Trail;
class CShader;
class CTexture;
END
BEGIN(Client)
class CTrail_Monster02 final :public CGameObject
{

public:
	typedef struct
	{

	}TRAIL_MONSTER04_DESC;

private:
	CTrail_Monster02(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTrail_Monster02(const CTrail_Monster02& Prototype);
	virtual ~CTrail_Monster02() = default;

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

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Bind_ShaderResources_Glow();

public:
	static CTrail_Monster02* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
