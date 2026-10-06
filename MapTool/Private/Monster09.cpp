#include"stdafx.h"
#include "Monster09.h"
#include "GameInstance.h"


CMonster09::CMonster09(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster{ pDevice, pContext }
{
}

CMonster09::CMonster09(const CMonster09& Prototype)
	:CMonster(Prototype)
{
}

HRESULT CMonster09::Initialize_Prototype()
{


	return S_OK;
}


HRESULT CMonster09::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pModelCom->SetUp_Animation(rand() % 4, true);
	return S_OK;
}

void CMonster09::Priority_Update(_float fTimeDelta)
{
	m_fAccMotionChangeTime += fTimeDelta;
	if (m_fAccMotionChangeTime > 2.f)
	{

		m_iCurAnimIndex = rand() % 7;
		m_pModelCom->Move_NextAnim(m_iCurAnimIndex);



		m_fAccMotionChangeTime = 0.f;
	}
}

void CMonster09::Update(_float fTimeDelta)
{
	m_pModelCom->Play_Animation(fTimeDelta, m_isChangeing);
}

void CMonster09::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_PRIORITY, this);
}

HRESULT CMonster09::Render()
{
	__super::Render();

	return S_OK;

}

HRESULT CMonster09::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Player09"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;
	/*Com_Shader_For_Collider*/
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxCubeCol"),
		TEXT("Com_Shader_For_Collider"), reinterpret_cast<CComponent**>(&m_pShader_For_ColliderCom))))
		return E_FAIL;
	/*Com_VIBuffer_Collider*/
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Collider"),
		TEXT("Com_VIBuffer_Collider"), reinterpret_cast<CComponent**>(&m_pVIBuffer_ColliderCom))))
		return E_FAIL;
	return S_OK;


}

HRESULT CMonster09::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();

	return S_OK;
}

CMonster09* CMonster09::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster09* pInstance = new CMonster09(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created Monster06_ Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CMonster09::Clone(void* pArg)
{
	CMonster09* pInstance = new CMonster09(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone Monster06 Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMonster09::Free()
{
	__super::Free();

}