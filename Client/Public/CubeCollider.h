#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Cube;
END


BEGIN(Client)

class CCubeCollider final : public CGameObject
{
public:
	typedef struct:public CGameObject::GAMEOBJECT_DESC
	{
		_float3 vPos;
		_float3	vScale;
		_float4 vRotAxis;

	}CUBE_DESC;
private:
	CCubeCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCubeCollider(const CCubeCollider& Prototype);
	virtual ~CCubeCollider() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void Update_Transform_Scale(_float3 vDeltaScale);
	void Update_Transform_Rotaion(_float4 vAxis, _float fRadian);
	void Update_Transform__Move_Position(_float4 _vDeltaMove);
	_float3 Get_Transform_Scale();
	_float4 Get_Transform_Move_Position();


private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	class CVIBuffer_Cube* m_pVIBufferCom = { nullptr };

	_float3 m_vScale;
	_float3 m_vPos;
	_float4 m_vRotAxis;

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();


public:
	static CCubeCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END