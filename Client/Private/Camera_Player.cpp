#include "stdafx.h"
#include "..\Public\Camera_Player.h"

#include "GameInstance.h"
#include "Monster09.h"
#include"Monster09_Head.h"
CCamera_Player::CCamera_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera{ pDevice, pContext }
{
}

CCamera_Player::CCamera_Player(const CCamera_Player& Prototype)
	: CCamera(Prototype )
{

}

HRESULT CCamera_Player::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Player::Initialize(void* pArg)
{
	CAMERA_PLAYER_DESC* pDesc = static_cast<CAMERA_PLAYER_DESC*>(pArg);

	m_fMouseSensor = pDesc->fMouseSensor;
	m_pPlayerFloat4x4 = pDesc->pPlayerFloat4x4;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CCamera_Player::Priority_Update(_float fTimeDelta)
{
	if (true == m_bBoss09_CameraCutSceneNow)
	{
		Boss09_Camera(fTimeDelta);
		__super::Compute_PipeLineMatrices();
		return;
	}


	//_matrix matPlayer =XMLoadFloat4x4(*m_pPlayerFloat4x4);
	if (true == m_bCamera_Still)
		return;

 	_matrix PlayerMatrix = DirectX::XMLoadFloat4x4(m_pPlayerFloat4x4);
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, PlayerMatrix.r[0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, PlayerMatrix.r[1]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, PlayerMatrix.r[2]);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, PlayerMatrix.r[3]);

	

	__super::Compute_PipeLineMatrices();
	
	/*if (KEY_DOWN(KEY::TAB))
	{
		m_bFix = (m_bFix == false) ? true : false;
	}*/
	if (m_bFix)
	{
		Fix_Mouse();
		//Free_Mouse(fTimeDelta);
	}
}

void CCamera_Player::Update(_float fTimeDelta)
{
}

void CCamera_Player::Late_Update(_float fTimeDelta)
{
}

HRESULT CCamera_Player::Render()
{
	return S_OK;
}



void CCamera_Player::Fix_Mouse()
{
	POINT pt = { g_iWinSizeX >> 1, (g_iWinSizeY >> 1) };
	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

void CCamera_Player::Set_Camera_Still()
{
	m_bCamera_Still = true;
	g_Update_TimeDelta = false;
	m_bFix = false;
}

void CCamera_Player::Set_Camera_Player()
{
	m_bCamera_Still = false;
	g_Update_TimeDelta = true;
	m_bFix = true;
}

_bool CCamera_Player::Get_Camera_Still()
{
	return m_bCamera_Still;
}

void CCamera_Player::Boss09_Camera(_float fTimeDelta)
{
	m_fAccBoss09CameraTime += fTimeDelta;
	if (m_fAccBoss09CameraTime > m_fBoss09CameraTime)
		m_bBoss09_CameraCutSceneNow = false;
	if (nullptr == m_pMonster09) return;

	CMonster09_Head* pHead =static_cast<CMonster09_Head* >( m_pMonster09->Find_GameObject(CMonster09::PART_HEAD));
	if (nullptr == pHead)
		return;
	_matrix matHeadPos = pHead->Get_WorldMatrixOfPartObject();


	_vector vCamPosForBoss09 = XMLoadFloat3(&m_vCamPosForBoss09);
	vCamPosForBoss09 =XMVectorSetW(vCamPosForBoss09, 0.f);
	
	_vector vFinalPos = {};
	vFinalPos = matHeadPos.r[3] + vCamPosForBoss09;


	_vector vMoveDelta = {0.f,    -fTimeDelta*1.5f ,m_fAccBoss09CameraTime*0.1};
	_vector vFinalCamBasePos = {};
	
	vFinalCamBasePos = vCamPosForBoss09;
	vFinalCamBasePos += vMoveDelta;

	XMStoreFloat3(&m_vCamPosForBoss09, vFinalCamBasePos);


	_vector vMyLook =   XMVector3Normalize(matHeadPos.r[3]- vFinalPos);
	_vector vMyRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vMyLook));
	_vector vMyUp = XMVector3Cross(vMyLook, vMyRight);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vMyRight);
	m_pTransformCom->Set_State(CTransform::STATE_UP, vMyUp);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, vMyLook);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vFinalPos);

}

CCamera_Player* CCamera_Player::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Player* pInstance = new CCamera_Player(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Player::Clone(void* pArg)
{
	CCamera_Player* pInstance = new CCamera_Player(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CCamera_Player::Free()
{
	__super::Free();

}
