#pragma once


#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Flat final : public CVIBuffer
{
public:
	typedef struct
	{
		_int	iNumX_Vertices;
		_int	iNumZ_Vertices;
		_float	fSizeX;
		_float	fSizeZ;

	}VIBUFFER_FLAT_DESC;

private:
	CVIBuffer_Flat(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Flat(const CVIBuffer_Flat& Prototype);
	virtual ~CVIBuffer_Flat() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;

	vector<_float4>* Get_VerticesList() { return &m_vecVertices; };
	VIBUFFER_FLAT_DESC Get_Desc() { return m_Desc; };

private:

	vector<_float4> m_vecVertices = {};
	_bool			m_IsCopied = { false };
	VIBUFFER_FLAT_DESC m_Desc = {};
	_int m_iNumVerticesX = {10};
	_int m_iNumVerticesZ = { 10 };
	_float m_fSizeX = {1.f};
	_float m_fSizeZ = {1.f};
public:
	static CVIBuffer_Flat* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};
END