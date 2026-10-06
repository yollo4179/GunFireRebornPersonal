#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include <Effects.h>
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Collider;
END


BEGIN(Client)

class CCubeCollider final : public CGameObject
{
public:
	typedef struct:public CGameObject::GAMEOBJECT_DESC
	{
		_float4 vPos;
		_float vPosY_From_Base;
		_float3	vScale;
		_float4 vRotAxis;
		_float fAngle;
		_int index;

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

	
	
	void Update_Transform_By_Desc(CUBE_DESC Desc)
	{
		m_Desc = Desc;
		
		_float4 FinalPos =m_Desc.vPos;
		FinalPos.y+= m_Desc.vPosY_From_Base =Desc.vScale.y*0.5;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&FinalPos));
		m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_Desc.fAngle);
		m_pTransformCom->Scaling(m_Desc.vScale.x, m_Desc.vScale.y, m_Desc.vScale.z);
		BoundingBox Box;
		Box.Center = { 0.f,0.f,0.f };
		Box.Extents = { 0.5f,0.5f,0.5f };
		Box.Transform(Box, m_pTransformCom->Get_WorldMatrix());
		m_Box = Box;

		return;
	}
	_float3 Get_Transform_Scale();
	_float4 Get_Transform_Move_Position();
	_matrix Get_WorldMatrix_Inverse();
	_matrix Get_WorldMatrix();
	//int Get_Index() { return m_iMyIndex; }


private:
	//CShader* m_pShaderCom = { nullptr };
	//CTexture* m_pTextureCom = { nullptr };
	//class CVIBuffer_Cube* m_pVIBufferCom = { nullptr };

	CShader* m_pShader_For_ColliderCom = { nullptr };
	CVIBuffer_Collider* m_pVIBuffer_ColliderCom = { nullptr };
		
	//WIREFRAME for Collider for picking
	ID3D11RasterizerState* m_CullMode = { nullptr };
	ID3D11RasterizerState* m_NoneCullMode = { nullptr };

public:
	BoundingBox m_Box;
	CUBE_DESC m_Desc = {};
	//std::unique_ptr<PrimitiveBatch<VertexPositionColor>>  m_batch;
	float m_arrColiderColor[3] = { 1.f,0.f,0.f };
	
private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();


public:
	static CCubeCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END