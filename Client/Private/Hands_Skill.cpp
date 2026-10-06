#include"stdafx.h"
#include"Hands_Skill.h"
#include"GameInstance.h"
#include"Hands_Skill_Granade.h"
#include"Player.h"

CHands_Skill::CHands_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CHands{ pDevice, pContext }
{
}
CHands_Skill::CHands_Skill(const CHands_Skill& Prototype)
	:CHands(Prototype)
{
}
const _float4x4* CHands_Skill::Get_BoneMatrix(const _char* pBoneName) const
{
	return m_pModelCom->Get_BoneMatrix(pBoneName);
}
HRESULT CHands_Skill::Initialize_Prototype()
{
	return S_OK;
}
HRESULT CHands_Skill::Initialize(void* pArg)
{
	HANDS_SKILL_DESC* pDesc = static_cast<HANDS_SKILL_DESC*>(pArg);
	m_pState = pDesc->pState;
	m_iNowMapStage = pDesc->iNowMapStage;
	
	m_HandsInfo = g_HandsInfo[5];


	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;


	if (FAILED(Ready_Components()))
		return E_FAIL;


	m_pModelCom->SetUp_Animation(2, false);
	m_pMuzzle_Socket_Matrix = Get_BoneMatrix("Bip001 L Finger0Nub");
	
	if (FAILED(Init_AnimSpeed()))
		return E_FAIL;
	if (FAILED(Ready_Bullets()))
		return E_FAIL;
	return S_OK;
}
HRESULT CHands_Skill::Init_AnimSpeed()
{
	return S_OK;
}
void CHands_Skill::Priority_Update(_float fTimeDelta)
{
	for (int i = 0; i < 3; ++i)
	{
		m_pGranades[i]->Priority_Update(fTimeDelta);
	}
	

	if (m_isAnimEnd) 
	{
		*m_pState = CPlayer::STATE_GRANADE_END;
		m_bActive = false;
		m_fAccShootingTiming = 0.f;
	}


	if (false == m_bActive)
		return;

	if (true == m_bShootSign)
		m_fAccShootingTiming += fTimeDelta;


}
void CHands_Skill::Update(_float fTimeDelta)
{

	
	if (false == m_bActive)
		return;

	for (int i = 0; i < 3; ++i)
	{
		m_pGranades[i]->Update(fTimeDelta);
	}

	DirectX::XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));
	
	
	if (m_fAccShootingTiming >= m_fShootingTiming)
	{
		m_bShootSign = false;
		m_fAccShootingTiming = 0.f;
		_matrix InitBulletMatrix;
		if (m_pMuzzle_Socket_Matrix)
		{
			_matrix SocketMatrix = XMLoadFloat4x4(m_pMuzzle_Socket_Matrix);
			for (size_t i = 0; i < 3; ++i)
			{
				SocketMatrix.r[i] = XMVector3Normalize(SocketMatrix.r[i]);
			}
			InitBulletMatrix = SocketMatrix * XMLoadFloat4x4(&m_WorldMatrix);
		}
		pair<_vector, _vector> pairRay = Get_PickingPosInWorld();
		_float3 vRaySrc, vRayDir;
		XMStoreFloat3(&vRaySrc, pairRay.first);
		XMStoreFloat3(&vRayDir, pairRay.second);

		_vector vPickingPos = XMVectorSetW(XMLoadFloat3(&vRaySrc), 1.f) + XMLoadFloat3(&vRayDir) * 1000;
		_vector vMuzzlePos = InitBulletMatrix.r[3];
		_vector vLook = XMVector3Normalize(vPickingPos - vMuzzlePos);
		_vector vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook));
		_vector vUp = XMVector3Cross(vLook, vRight);
		InitBulletMatrix.r[0] = vRight;
		InitBulletMatrix.r[1] = vUp;
		InitBulletMatrix.r[2] = vLook;
		InitBulletMatrix.r[3] = vMuzzlePos;
		m_pGranades[m_iNowGranadeIndex]->Reset_Bullets(InitBulletMatrix);
		m_pGranades[m_iNowGranadeIndex]->Set_Active(true);
		m_iNowGranadeIndex = (m_iNowGranadeIndex + 1) % 3;

	}
	m_isAnimEnd = m_pModelCom->Play_Animation(fTimeDelta, m_isChangingMotion);




}
void CHands_Skill::Late_Update(_float fTimeDelta)
{
	for (int i = 0; i < 3; ++i)
	{
		m_pGranades[i]->Late_Update(fTimeDelta);
	}

	if (false == m_bActive)
		return;
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}
HRESULT CHands_Skill::Render()
{
	if (false == m_bActive)
		return S_OK;

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
void CHands_Skill::SetActive(_bool bActiveState)
{
	m_bActive = bActiveState;
	m_pModelCom->SetUp_Animation(2, false);
	
	m_bShootSign = true;
	m_fAccShootingTiming = 0.f;
	m_isAnimEnd = false;
}
void CHands_Skill::Set_BombRange(_float fBombRange)
{	
	//핸즈 인포 바꾼 후  이거 호출하자 
	fBombRange = m_HandsInfo.iRange / 10.f;

	for (int i = 0; i < 3; ++i)
	{
		m_pGranades[i]->Set_BombRange(fBombRange);
	}

}
HRESULT CHands_Skill::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model  for Anim*/
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Player_Skill"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;
	return S_OK;
}
HRESULT CHands_Skill::Bind_ShaderResources()
{
	if(FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);

	return S_OK;
}
HRESULT CHands_Skill::Ready_Bullets()
{
	CHands_Skill_Granade::HANDS_SKILL_GRANADE_DESC desc = {};
	desc.iNowMapStage = m_iNowMapStage;
	desc.fRotationPerSec = XMConvertToRadians(90.f);
	desc.fSpeedPerSec = 10.f;
	desc.iData = 0;
	for (int i = 0; i < 3; ++i)
	{
		m_pGranades[i] = static_cast<CHands_Skill_Granade*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_Hands_Skill_Granade"), &desc));

	}
	return S_OK;;
}
CHands_Skill* CHands_Skill::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHands_Skill* pInstance = new CHands_Skill(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed Create CHands_Skill");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}
CGameObject* CHands_Skill::Clone(void* pArg)
{
	CGameObject* pInstance = new CHands_Skill(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed CHands_Skill :Clone");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}
void CHands_Skill::Free()
{

	g_HandsInfo[5] = m_HandsInfo;
	__super::Free();
	for (int i = 0; i < 3; ++i)
	{
		Safe_Release(m_pGranades[i]);
	}
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}

