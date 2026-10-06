#include"stdafx.h"
#include "Monster.h"
#include "GameInstance.h"


CMonster::CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject{ pDevice, pContext }
{
}

CMonster::CMonster(const CMonster& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CMonster::Initialize_Prototype()
{


	return S_OK;
}


HRESULT CMonster::Initialize(void* pArg)
{
	MONSTER_DESC* pDesc = static_cast<MONSTER_DESC*>(pArg);

	if (nullptr != pDesc) 
	{

		m_Desc.iIndex = pDesc->iIndex;
		m_Desc.vScale = pDesc->vScale;
		m_Desc.vPosition = pDesc->vPosition;
		m_Desc.fAngle = pDesc->fAngle;
	}
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (nullptr==pDesc && nullptr != m_pTransformCom)
	{
		m_pTransformCom->Scaling(1, 1, 1);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
	}
	if (nullptr != pDesc)
	{
		_float4 Axis = { 0.f,1.f,0.f,0.f };
		m_pTransformCom->Scaling(m_Desc.vScale.x, m_Desc.vScale.y, m_Desc.vScale.z);
		m_pTransformCom->Rotation(XMLoadFloat4(&Axis), XMConvertToRadians(m_Desc.fAngle));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4( &m_Desc.vPosition));


	}
	

	return S_OK;
}

void CMonster::Priority_Update(_float fTimeDelta)
{
}

void CMonster::Update(_float fTimeDelta)
{
	if (false == m_isVisible)
		return;
	 m_bAnim_End = m_pModelCom->Play_Animation(fTimeDelta,m_isChangeing);
	
	 m_fTimeDelta = fTimeDelta;
	 if (/*m_facctimedelta > 0.5f &&*/ true == m_bStartAnim_Begin)
	 {
		/* m_bAnim_End = true;*/
		 m_fAccTimeDelta = 0.f;
	 }
	 if (true == m_bStartAnim_Begin && true == m_bAnim_End)
	 {
		 m_bStartAnim_Begin = false;
		 m_bStartAnim_End = true;
		
	 }
	 else if (true == m_bChangingAnim_Begin && true == m_bAnim_End)
	 {
		 m_bStartAnim_End = false;

		 m_bChangingAnim_End = true;
		 m_bChangingAnim_Begin = false;
	 }
	 else if (true == m_bEndAnim_Begin && false == m_bAnim_End)
	 {
		 m_bChangingAnim_End = false;

		 m_bEndAnim_End = true;
		 m_bEndAnim_Begin = false;
	 }

	
}

void CMonster::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_PRIORITY, this);
}

HRESULT CMonster::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
			return E_FAIL;

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}


	
	return S_OK;

}

void CMonster::StartAnimToAnim()
{
	m_bStartAnim_Begin = true;
	m_bStartAnim_End = { false };
	CallStartAnim = { 0 };
	m_bChangingAnim_Begin = { false };
	m_bChangingAnim_End = { false };
	 CallChangingAnim = { 0 };
	m_bEndAnim_Begin = { false };
	m_bEndAnim_End = { false };
	 CallEndAnim = { 0 };
	 m_fAccTimeDelta = 0.f;
	 m_fTimeDelta = 0.f;
}

_bool CMonster::Play_AnimToAnim(_uint iAnimIndex, _uint iNextAnimIndex, _float fDuration)
{
	
	

	if (m_bStartAnim_Begin) {
		m_fAccTimeDelta += m_fTimeDelta;
		if (CallStartAnim == 0) {
			
			m_pModelCom->SetUp_Animation(iAnimIndex, false);
			CallStartAnim++;
		}
		return false;
	}
	else if (true == m_bStartAnim_End) 
	{
		m_bChangingAnim_Begin = true;
		if (CallChangingAnim == 0) {
			m_pModelCom->Move_NextAnim(iNextAnimIndex, fDuration);
			CallChangingAnim++;
		}
		return false;
	}
	else if (true == m_bChangingAnim_End) 
	{
		m_bEndAnim_Begin = true;
		if (CallEndAnim == 0) {
			//m_pModelCom->SetUp_Animation(iNextAnimIndex, true);
			CallEndAnim++;
			m_fAccTimeDelta = 0;
			CallEndAnim = 0;
			CallChangingAnim = 0;
			CallStartAnim = 0;

			m_bChangingAnim_End = false;
			m_bChangingAnim_Begin = false;
			m_bEndAnim_Begin = false;
			m_bEndAnim_End = false;
			m_bEndAnim_Begin = false;
			m_bEndAnim_End = false;
		}
		return true;
	}
	
	

	
	return true;//¾Ö´Ô ÇÑ ¹ÙÄû ³¡³µ´Ù.
}

_bool CMonster::PlayNowAnim(_uint iAnimIndex, _bool isLoop)
{
	m_pModelCom->SetUp_Animation(iAnimIndex, true);
	return true;
}


HRESULT CMonster::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster1"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;


}

HRESULT CMonster::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);


	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

CMonster* CMonster::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster* pInstance = new CMonster(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created Monster_ Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CMonster::Clone(void* pArg)
{
	CMonster* pInstance = new CMonster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone Monster01 Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMonster::Free()
{
	__super::Free();

	
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}

