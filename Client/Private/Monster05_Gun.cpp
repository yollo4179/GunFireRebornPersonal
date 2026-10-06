#include"stdafx.h"
#include"Monster05_Gun.h"
#include"Monster05_Bullet.h"
#include"GameInstance.h"
#include"Monster05.h"
#include"Effect_Monster05_Ready.h"
CMonster05_Gun::CMonster05_Gun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject{pDevice, pContext}
{
}

CMonster05_Gun::CMonster05_Gun(const CMonster05_Gun& Prototype)
	:CPartObject(Prototype)
{
}

HRESULT CMonster05_Gun::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster05_Gun::Initialize(void* pArg)
{
	MONSTER05_GUN_DESC* pDesc = static_cast<MONSTER05_GUN_DESC*>(pArg);
	m_pState = pDesc->pState;
	m_pSocketMatrix = pDesc->pSocketMatrix;
	m_pIsAnimEnd = pDesc->pIsAnimEnd;
	m_pSniperShootSign = pDesc->pSniperShootSign;
	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Bullets()))
		return E_FAIL;

	if (FAILED(Ready_Effects()))
		return E_FAIL;
	

	return S_OK;
}

void CMonster05_Gun::Priority_Update(_float fTimeDelta)
{

	for (size_t i = 0; i < m_iNumBullets; ++i)
	{
		if (m_pBullets[i]->Get_Active())
			m_pBullets[i]->Priority_Update(fTimeDelta);
	}

}

void CMonster05_Gun::Update(_float fTimeDelta)
{

	
		if (m_pSocketMatrix) {
			_matrix SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);
			for (size_t j = 0; j < 3; ++j)
			{
				SocketMatrix.r[j] = XMVector3Normalize(SocketMatrix.r[j]);
			}
			XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_pParentMatrix));
		}
		

		for (size_t i = 0; i < m_iNumBullets; ++i)
		{
			if (m_pBullets[i]->Get_Active())
				m_pBullets[i]->Update(fTimeDelta);
		}
	
}

void CMonster05_Gun::Late_Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumBullets; ++i)
	{
		if(m_pBullets[i]->Get_Active())
		m_pBullets[i]->Late_Update(fTimeDelta);
	}

	if ((*m_pIsAnimEnd)&&(*m_pState & M5_STATE::M5_STATE_CLOSE_FIRE)) 
	{
		m_pBullets[m_iNowBulletIndex]->Set_Active(true);
		m_pBullets[m_iNowBulletIndex]->Reset_Bullets(XMLoadFloat4x4(&m_WorldMatrix));
		m_pBullets[m_iNowBulletIndex]->Set_TracingMode(false);
		m_iNowBulletIndex =(1 + m_iNowBulletIndex)%(m_iNumBullets);
		m_pGameInstance->PlayEffect(L"Monster05_Shot.mp3", 0.1f);
	}

 	if ((*m_pSniperShootSign) && (*m_pState & M5_STATE::M5_STATE_FAR_SNIPE))
	{
		m_pBullets[m_iNowBulletIndex]->Set_Active(true);
		m_pBullets[m_iNowBulletIndex]->Reset_Bullets(XMLoadFloat4x4(&m_WorldMatrix));
		m_pBullets[m_iNowBulletIndex]->Set_TracingMode(true);
		m_iNowBulletIndex = (1 + m_iNowBulletIndex) % (m_iNumBullets);
		m_pGameInstance->PlayEffect(L"Monster05_Snipe.mp3", 0.1f);
	}
	if ((*m_pState & M5_STATE::M5_STATE_FAR_SNIPE))
	{
		m_pEffectMonster05_Ready->Set_Active(true);
		m_pEffectMonster05_Ready->Set_Matrix(XMLoadFloat4x4(&m_WorldMatrix));
		m_pEffectMonster05_Ready->Late_Update(fTimeDelta);
	}
	

	
}

HRESULT CMonster05_Gun::Render()
{
	return S_OK;
}

HRESULT CMonster05_Gun::Ready_Components()
{



	return S_OK;
}

HRESULT CMonster05_Gun::Ready_Bullets()
{

	
	m_pBullets.resize(m_iNumBullets);
	
	CMonster05_Bullet::MONSTER05_BULLET_DESC desc = {};
	desc.fRotationPerSec = XMConvertToRadians(90.f);
	desc.fSpeedPerSec = 10.f;
	desc.iData = 0;
	for (size_t i = 0; i < m_iNumBullets; ++i)
	{
		m_pBullets[i] = static_cast<CMonster05_Bullet*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster05_Bullet"), &desc));
	}


	return S_OK;
}

HRESULT CMonster05_Gun::Bind_ShaderResources()
{
	return S_OK;
}

HRESULT CMonster05_Gun::Ready_Effects()
{
	m_pEffectMonster05_Ready = static_cast<CEffect_Monster05_Ready*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_Monster05_Ready"), nullptr));

	return S_OK;
}

CMonster05_Gun* CMonster05_Gun::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster05_Gun* pInstance = new CMonster05_Gun(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMonster05_Gun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonster05_Gun::Clone(void* pArg)
{
	CMonster05_Gun* pInstance = new CMonster05_Gun(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMonster05_Gun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster05_Gun::Free()
{

	__super::Free();
	for (auto& pBullet : m_pBullets)
	{
		Safe_Release(pBullet);
	}
	Safe_Release(m_pEffectMonster05_Ready);

}
