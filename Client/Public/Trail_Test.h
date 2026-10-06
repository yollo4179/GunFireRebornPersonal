#pragma once
#include"GameObject.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CVIBuffer_Trail;
class CShader;
class CTexture;
END
BEGIN(Client)
class CTrail_Test final :public CGameObject
{

public:
	typedef struct
	{

	}TRAIL_TEST_DESC;

private:
	CTrail_Test(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTrail_Test(const CTrail_Test& Prototype);
	virtual ~CTrail_Test() = default;

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
	CVIBuffer_Trail* m_pVIBufferCom = { nullptr };
	_float4 m_fCenter = { 5.f,10.f,40.f,1.f };
	_float m_fAccAngle = {0.f};
	_float m_fAccBufferTiming = { 0.f };
	_float m_fAccBufferSubTiming = { 0.f };
	_float m_fSize = { 3.f };


private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();


public:
	static CTrail_Test* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END
