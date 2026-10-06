#pragma once
#include"VIBuffer.h"
#include"VIBuffer.h"
BEGIN(Engine)
class ENGINE_DLL CVIBuffer_Collider final :public CVIBuffer
{
private:
	explicit CVIBuffer_Collider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CVIBuffer_Collider(const CVIBuffer_Collider& Prototype);
	virtual ~CVIBuffer_Collider() = default;


public:
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Prototype();


public:
	static CVIBuffer_Collider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg)override;
	virtual void Free()override;
};
END

