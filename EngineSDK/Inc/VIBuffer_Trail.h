#pragma once
#include"VIBuffer.h"
BEGIN(Engine)
class ENGINE_DLL CVIBuffer_Trail final : public CVIBuffer
{

public :
	typedef struct
	{
		_float fTrailLength;
		_uint iNumMaxVertices;
	}TRAIL_DESC;

protected:
	CVIBuffer_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Trail(const CVIBuffer_Trail& Prototype);
	virtual ~CVIBuffer_Trail() = default;
public:
	void Add_Points(_fmatrix vtxTrail_High_Matrix, _fmatrix vtxTrail_Low_Matrix);
	void Sub_Points();
	void Add_Points_CatMullRom(_fmatrix vtxTrail_High_Matrix, _fmatrix vtxTrail_Low_Matrix);
	void Clear_Points();
	void Resize_Vertices();
	void Reset_VerticesCnt();

	virtual HRESULT Render()override;

public:
	virtual HRESULT Bind_BufferDesc()override;
private:
	_float3 m_vLocalTrailLow = { 0.0f,-0.5f,0.f };
	_float3 m_vLocalTrailHigh = { 0.f,0.5f,0.f };


	_uint m_iNumMaxVertices = {};
	_uint m_fTrailLength = {};
	deque <VTXPOSTEX> m_TrailVertices;

	_uint m_iEndIndex = { 0 };
	_uint m_iCatmullRomIndex[4] = {};
	_uint m_iCatmullRomCnt = {10};
	_uint m_iVerticesCnt = {};
	_bool m_bStartSpline = true;;


protected:
	ID3D11Buffer* m_pVB = { nullptr };
	ID3D11Buffer* m_pIB = { nullptr };

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

	
public:
	static CVIBuffer_Trail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};
END
