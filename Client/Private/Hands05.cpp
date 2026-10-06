#include"stdafx.h"
#include"Hands05.h"
#include"Hands05_Bullet.h"
#include"GameInstance.h"
#include"Player.h"
#include"UI_Arms.h"

CHands05::CHands05(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CHands{pDevice, pContext}
{
}

CHands05::CHands05(const CHands05& Prototype)
	:CHands(Prototype)
{
}

const _float4x4* CHands05::Get_BoneMatrix(const _char* pBoneName) const
{
	return m_pModelCom->Get_BoneMatrix(pBoneName);
}

HRESULT CHands05::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHands05::Initialize(void* pArg)
{
	HANDS05_DESC* pDesc = static_cast<HANDS05_DESC*>(pArg);
	m_pState = pDesc->pState;
	m_HandsInfo = g_HandsInfo[4];

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;


	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_isInfinity = true;
	m_pModelCom->SetUp_Animation(1, true);
	m_pMuzzle_Socket_Matrix=Get_BoneMatrix("muzzle");
	m_vecStates.resize((_uint)HANDS05_STATE::STATE_END);

	if (FAILED(Init_Mapping_Index_To_State()))
		return E_FAIL;

	if (FAILED(Init_AnimSpeed()))
		return E_FAIL;
	if (FAILED(Ready_Bullets()))
		return E_FAIL;
	m_iNumBullets_ICanFireNow = m_iNumBullets_YouHave = m_HandsInfo.fMaxLoadBullets;
	return S_OK;
	
}

HRESULT CHands05::Init_Mapping_Index_To_State()
{
	m_vecStates[(_uint)HANDS05_STATE::STATE_IDLE] = 0;
	m_vecStates[(_uint)HANDS05_STATE::STATE_ATTACK_PRESSING] = 1;
	m_vecStates[(_uint)HANDS05_STATE::STATE_LOAD] = 5;
	m_vecStates[(_uint)HANDS05_STATE::STATE_POP_OUT] = 7;
	return S_OK;
}

HRESULT CHands05::Init_AnimSpeed()
{
	m_pModelCom->Set_AnimSpeed(m_vecStates[(_uint)HANDS05_STATE::STATE_ATTACK_PRESSING], 1.0f);
	return S_OK;
}

void CHands05::Priority_Update(_float fTimeDelta)
{

	if (m_bShootSign && m_fAccShootingTiming >= m_fShootingTiming)
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
		m_pBullets[m_iNowBulletIndex]->Reset_Bullets(InitBulletMatrix);
		m_pBullets[m_iNowBulletIndex]->Set_Active(true);
		m_iNowBulletIndex = (m_iNowBulletIndex + 1) % g_iNumPlayer05_Bullets;

	}

	/*For Bullet*/
	for (size_t i = 0; i < g_iNumPlayer05_Bullets; ++i)
	{
		m_pBullets[i]->Priority_Update(fTimeDelta);
	}

	if (false == m_bActive)
		return;

	
	if (true == m_bShootSign)
		m_fAccShootingTiming += fTimeDelta;

}

void CHands05::Update(_float fTimeDelta)
{

	
	/*For Bullet*/
	for (size_t i = 0; i < g_iNumPlayer05_Bullets; ++i)
	{
		m_pBullets[i]->Update(fTimeDelta);
	}


	if (false == m_bActive)
		return;

	
	m_isFired = false;
	/*For UI 지금 총알 수 UI에 보여주기*/
	m_pArms->Set_BulletsICanFire(m_iNumBullets_ICanFireNow);
	m_pArms->Set_BulletsIHaveNow(m_iNumBullets_YouHave);
	m_pArms->Set_IsInfinityBullets(m_isInfinity);

	/*POP OUT끝난 후 IDLE 상태*/
	if (true == m_bStartSetUp && m_isAnimEnd)
	{
		*m_pState ^= CPlayer::STATE_POP_OUT;
		*m_pState = CPlayer::STATE_IDLE;
		m_bStartSetUp = false;

		m_fAccShootingTiming = 0.f;
		m_bShootSign = false;
		
	}
	/*장전 끝났을때 ->POPOUT과 장전 해제*/
	if (CONDITION_LOAD && m_isAnimEnd)
	{
		if (CONDITION_POP_OUT)
			*m_pState ^= CPlayer::STATE_POP_OUT;
		*m_pState ^= CPlayer::STATE_LOAD;
		*m_pState = CPlayer::STATE_IDLE;
	}
	/*공격 _ MOUSE_UP 공격 그만둠*/
	if (CONDITION_ATTACK_UP)
	{
		
		/*애니메이션이끝났다.-> 공격 멈춘다*/
		
		{

			if (CONDITION_ATTACK_UP)
				*m_pState ^= CPlayer::STATE_ATTACK_UP;
			if (CONDITION_ATTACK_DOWN)
				*m_pState ^= CPlayer::STATE_ATTACK_DOWN;
			if (CONDITION_ATTACK_PRESSING)
				*m_pState ^= CPlayer::STATE_ATTACK_PRESSING;

			if (!CONDITION_IDLE)
				*m_pState |= CPlayer::STATE_IDLE;
		}
	}
	/* To Pop_Up*/
	if (CONDITION_POP_OUT && false == m_bStartSetUp)
	{
		m_pGameInstance->PlayEffect(L"Player05_Load.ogg",0.1f);
		m_pModelCom->SetUp_Animation(m_vecStates[(_uint)HANDS05_STATE::STATE_POP_OUT], false);
		m_bStartSetUp = true;

	}

	/*To Attack1 */
	else if (CONDITION_ATTACK)
	{
		m_fAccAttackDownTime += fTimeDelta;
		//루프 돌거나 장전중 캔슬 (다른 루프가 아닌 애니메이션이끝났을 때
		if (CONDITION_LOOP || CONDITION_LOAD || m_isAnimEnd)
		{
			


			if (m_isFired = __super::Fire_A_Bullet())
			{
				m_bShootSign = true;
				m_pArms->Set_BulletsICanFire(m_iNumBullets_ICanFireNow);
				m_pModelCom->Move_NextAnim(m_vecStates[(_uint)HANDS05_STATE::STATE_ATTACK_PRESSING], 0.2, false);

				m_pGameInstance->PlayEffect(L"Player05_Shot.wav", 0.2f);
				if (CONDITION_IDLE)
					*m_pState ^= CPlayer::STATE_IDLE;
				if (CONDITION_LOAD)
					*m_pState ^= CPlayer::STATE_LOAD;
			}

			
		}
	}
	/* 장전일때, 장전 하고 나서 -> 총알 재충전 */
	else if (CONDITION_LOAD)
	{
		if (CONDITION_LOOP || m_isAnimEnd)
		{

			if (__super::Load_Bullets())
			{
				m_pArms->Set_BulletsICanFire(m_HandsInfo.fMaxLoadBullets);
				m_pArms->Set_BulletsIHaveNow(m_iNumBullets_YouHave = m_iNumBullets_ICanFireNow = m_HandsInfo.fMaxLoadBullets);
			}
			m_pGameInstance->PlayEffect(L"Player05_Load.ogg", 0.1f);
			m_pModelCom->Move_NextAnim(m_vecStates[(_uint)HANDS05_STATE::STATE_LOAD],0.2f ,false);
			if (CONDITION_IDLE)
			{
				*m_pState ^= CPlayer::STATE_IDLE;
			}

		}

	}
	/* to Idle*/
	else if (CONDITION_IDLE && m_isAnimEnd)
	{
		m_pModelCom->Move_NextAnim(m_vecStates[(_uint)HANDS05_STATE::STATE_IDLE], 0.202F, true);
	}

	/*for debugging*/
	if (CONDITION_LOOP)
		int a = 0;
	if (CONDITION_LOAD)
		int a = 0;

	if (CONDITION_ATTACK)
	{

		if (CONDITION_ATTACK_PRESSING)
			int b = 0;
	}
	m_isAnimEnd = m_pModelCom->Play_Animation(fTimeDelta, m_isChangingMotion);
	DirectX::XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));





}

void CHands05::Late_Update(_float fTimeDelta)
{
	/*For Bullet*/
	for (size_t i = 0; i < g_iNumPlayer05_Bullets; ++i)
	{
		
			m_pBullets[i]->Late_Update(fTimeDelta);
	}

	if (false == m_bActive)
		return;

	
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_GLOW, this);
}

HRESULT CHands05::Render()
{
	if (false == m_bActive)
		return S_OK;

	if (true == m_bGlow)
	{
		if (FAILED(Render_Glow()))
			return E_FAIL;
	}

	else {

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
	}

	return S_OK;
}

HRESULT CHands05::Render_Glow()
{
	if (FAILED(Bind_ShaderResources_Glow()))
		return E_FAIL;

	if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom_Glow, "g_DiffuseTexture", 2, aiTextureType_DIFFUSE, 0)))
		return E_FAIL;
	if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom_Glow, "g_BoneMatrices", 2)))
		return E_FAIL;

	m_pShaderCom_Glow->Begin(2);

	m_pModelCom->Render(2);
	return S_OK;
}

HRESULT CHands05::Ready_Components()
{

	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model  for Anim*/
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Player05"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;
	/*ForGlow*/
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh_Player01_Glow"),
		TEXT("Com_Shader_Glow"), reinterpret_cast<CComponent**>(&m_pShaderCom_Glow))))
		return E_FAIL;
	/*ComTexture*/
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player01_Glow"),
		TEXT("Com_Texture_Glow"), reinterpret_cast<CComponent**>(&m_pTextureCom_Glow))))
		return E_FAIL;
	return S_OK;
}

HRESULT CHands05::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);

	return S_OK;
}

HRESULT CHands05::Bind_ShaderResources_Glow()
{

	if (FAILED(m_pShaderCom_Glow->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom_Glow->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom_Glow->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	_float3 vThreshold = { 0.f,0.5f,0.0f };
	if (FAILED(m_pShaderCom_Glow->Bind_RawValue("g_vThreshold", &vThreshold, sizeof(_float3))))
		return E_FAIL;
	_float vIntensity = 0.2f;
	if (FAILED(m_pShaderCom_Glow->Bind_RawValue("g_fIntensity", &vIntensity, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pTextureCom_Glow->Bind_ShaderResource(m_pShaderCom_Glow, "g_EmissiveTexture", 0)))
		return E_FAIL;


	return S_OK;
}

HRESULT CHands05::Ready_Bullets()
{
	CHands05_Bullet::HANDS05_BULLET_DESC desc = {};
	desc.fRotationPerSec = XMConvertToRadians(90.f);
	desc.fSpeedPerSec = 10.f;
	desc.iData = 0;
	for (size_t i = 0; i < g_iNumPlayer05_Bullets; ++i)
	{
		m_pBullets[i] = static_cast<CHands05_Bullet*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_Hands05_Bullet"), &desc));
	}

	return S_OK;
}

CHands05* CHands05::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHands05* pInstance = new CHands05(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed Create CHands05");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CHands05::Clone(void* pArg)
{
	CGameObject* pInstance = new CHands05(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed CHands05 :Clone");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CHands05::Free()
{
	
	__super::Free();
	for (int i = 0; i < g_iNumPlayer05_Bullets; ++i)
	{
		Safe_Release(m_pBullets[i]);
	}
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	/*ForGlow*/
	Safe_Release(m_pShaderCom_Glow);
	Safe_Release(m_pTextureCom_Glow);
	g_HandsInfo[4] = m_HandsInfo;
}
