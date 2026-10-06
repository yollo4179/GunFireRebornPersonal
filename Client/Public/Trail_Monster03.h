#pragma once
#include"GameObject.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CVIBuffer_Trail;
class CShader;
class CTexture;
END
BEGIN(Client)
class CTrail_Monster03 final :public CGameObject
{
public:
	typedef struct
	{

	}TRAIL_MONSTER05_DESC;

private:
	CTrail_Monster03(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTrail_Monster03(const CTrail_Monster03& Prototype);
	virtual ~CTrail_Monster03() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void Set_BufferMatrix(_fmatrix matHigh, _fmatrix matLow);
	void Set_Matrix(_fmatrix matWorld, _float3 Size);
	void ResetMyMatrixByPos();
	void Reset_VerticesCnt();
	void Resize_Vertices();

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Trail* m_pVIBufferCom = { nullptr };
	CTransform* m_pLocalTransform = { nullptr };

	_bool m_bActive = { true };

	_float m_fAccTime = 0;
	_float m_fTime = { 360.f };
	_float m_fRange = { 0.5f };
	_float3 m_vMyPosInLocal = {};

	_float m_fAccAddVeticesTime = { 0.f };
	_float m_fAddVerticesTime = { 0.02f };

private:

	HRESULT Ready_Components();
	HRESULT Ready_MyTransform();
	HRESULT Bind_ShaderResources();
	HRESULT Bind_ShaderResources_Glow();

public:
	static CTrail_Monster03* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
