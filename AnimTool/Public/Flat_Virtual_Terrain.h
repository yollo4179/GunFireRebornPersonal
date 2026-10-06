#pragma once
#include"Terrain_Base.h"
BEGIN(Engine)
class CVIBuffer_Flat;
END

BEGIN(Client)
class CFlat_Virtual_Terrain final: public CTerrain_Base
{

public:
	typedef struct: public CTerrain_Base::TERRAIN_BASE_DESC
	{
		_float fSizeX;
		_float fSizeZ;
		_int	iNumX;
		_int	iNumZ;

	}FLAT_VIRTUAL_TERRAIN_DESC;
private:
	explicit CFlat_Virtual_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CFlat_Virtual_Terrain(const CFlat_Virtual_Terrain& Prototype);
	virtual ~CFlat_Virtual_Terrain()=default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void Update_Transform_By_Desc(TERRAIN_BASE_DESC Desc);
	void Update_Transform_Go_Straight(const _float fTimeDelta);

	/*For Collision*/
	const vector<_float4>* Get_VecVertices_World() { return &m_vecVertices_World; };
protected:

	CVIBuffer_Flat* m_pVIBufferCom = { nullptr };

private:
	//WIREFRAME
	ID3D11RasterizerState* m_CullMode = { nullptr };
	ID3D11RasterizerState* m_NoneCullMode = { nullptr };

public:
	FLAT_VIRTUAL_TERRAIN_DESC m_Flat_Desc;
	vector<_float4> m_vecVertices_Origin;
	vector<_float4> m_vecVertices_World;


private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:

	static CFlat_Virtual_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END
