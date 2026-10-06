#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Terrain;
END


BEGIN(Client)

class CTerrain final : public CGameObject
{
private:
	CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTerrain(const CTerrain& Prototype);
	virtual ~CTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	const _float4* Get_Vertices() { return m_pPos; };
	_uint		  Get_VerticesX() { return m_VerticesX; };
	_uint		  Get_VerticesZ() { return m_VerticesZ; };
	_matrix       Get_MatWorldInverse() { return m_pTransformCom->Get_WorldMatrix_Inverse(); };
	_matrix       Get_MatWorld() { return m_pTransformCom->Get_WorldMatrix(); };


private:
	CShader*				m_pShaderCom = { nullptr };
	CTexture*				m_pTextureCom = { nullptr };
	CVIBuffer_Terrain*		m_pVIBufferCom = { nullptr };
	//for Picking
	const _float4*				m_pPos={ nullptr };
	_uint						m_VerticesX={ 0 };
	_uint						m_VerticesZ={ 0 };
	//WIREFRAME
	ID3D11RasterizerState* m_CullMode = {nullptr};
	ID3D11RasterizerState* m_NoneCullMode = { nullptr };


private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();


public:
	static CTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END