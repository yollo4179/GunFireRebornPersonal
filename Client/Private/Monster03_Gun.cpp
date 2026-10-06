#include"stdafx.h"
#include"Monster03_Gun.h"
#include"Monster03_Bullet.h"
#include"Monster03_Bullet_Multishot.h"
#include"GameInstance.h"
#include"Monster03.h"

CMonster03_Gun::CMonster03_Gun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject{ pDevice, pContext }
{
}

CMonster03_Gun::CMonster03_Gun(const CMonster03_Gun& Prototype)
	:CPartObject(Prototype)
{
}

HRESULT CMonster03_Gun::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster03_Gun::Initialize(void* pArg)
{
	MONSTER03_GUN_DESC* pDesc = static_cast<MONSTER03_GUN_DESC*>(pArg);
	m_pState = pDesc->pState;
	m_pSocketMatrix = pDesc->pSocketMatrix;
	m_pIsAnimEnd = pDesc->pIsAnimEnd;
	m_pMultiShotSign = pDesc->pMultiShotSign;
	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Bullets()))
		return E_FAIL;


	return S_OK;
}

void CMonster03_Gun::Priority_Update(_float fTimeDelta)
{

	for (size_t i = 0; i < m_iNumBullets; ++i)
	{
		if (m_pBullets[i]->Get_Active())
			m_pBullets[i]->Priority_Update(fTimeDelta);
	}
	for (size_t i = 0; i < m_iNumBullets; ++i)
	{
		if (m_pMultiShotBullets[i]->Get_Active())
			m_pMultiShotBullets[i]->Priority_Update(fTimeDelta);
	}

}

void CMonster03_Gun::Update(_float fTimeDelta)
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
	for (size_t i = 0; i < m_iNumBullets; ++i)
	{
		if (m_pMultiShotBullets[i]->Get_Active())
			m_pMultiShotBullets[i]->Update(fTimeDelta);
	}

}

void CMonster03_Gun::Late_Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumBullets; ++i)
	{
		if (m_pBullets[i]->Get_Active())
			m_pBullets[i]->Late_Update(fTimeDelta);
	}

	for (size_t i = 0; i < m_iNumBullets; ++i)
	{
		if (m_pMultiShotBullets[i]->Get_Active())
			m_pMultiShotBullets[i]->Late_Update(fTimeDelta);
	}

	if ((*m_pIsAnimEnd) && (*m_pState & M3_STATE::M3_STATE_FAR_SNIPE))
	{
		m_pBullets[m_iNowBulletIndex]->Set_Active(true);
		
		_vector vLook = XMLoadFloat4((_float4*)&m_WorldMatrix.m[2]);
		_vector vPos = XMLoadFloat4((_float4*)&m_WorldMatrix.m[3]);
		vPos = -XMVector3Normalize(vLook) * 0.5 + vPos;
		_matrix BulletMatrix = XMLoadFloat4x4(&m_WorldMatrix);
		BulletMatrix.r[3] = vPos;
		m_pBullets[m_iNowBulletIndex]->Reset_Bullets(BulletMatrix);


		m_iNowBulletIndex = (1 + m_iNowBulletIndex) % (m_iNumBullets);
		m_pGameInstance->PlayEffect(L"Monster03_Snipe.mp3", 0.1f);
	}

	if ((*m_pMultiShotSign&& (*m_pState & M3_STATE::M3_STATE_CLOSE_MULTISHOT)))
	{
		m_pMultiShotBullets[m_iNowBullet_Multishot_Index]->Set_Active(true);

		
		_vector vLook = XMLoadFloat4((_float4*)&m_WorldMatrix.m[2]);
		_vector vPos = XMLoadFloat4((_float4*)&m_WorldMatrix.m[3]);
		vPos = -XMVector3Normalize(vLook) * 0.5 + vPos;
		_matrix BulletMatrix = XMLoadFloat4x4(&m_WorldMatrix);
		BulletMatrix.r[3] = vPos;
		

			m_pMultiShotBullets[m_iNowBullet_Multishot_Index]->Reset_Bullets(BulletMatrix);
			m_iNowBullet_Multishot_Index = (1 + m_iNowBullet_Multishot_Index) % (m_iNum_Multishot_Bullets);
			m_pGameInstance->PlayEffect(L"Monster03_MultiShot.mp3", 0.1f);
	}




}

HRESULT CMonster03_Gun::Render()
{
	return S_OK;
}

HRESULT CMonster03_Gun::Ready_Components()
{



	return S_OK;
}

HRESULT CMonster03_Gun::Ready_Bullets()
{


	m_pBullets.resize(m_iNumBullets);
	m_pMultiShotBullets.resize(m_iNum_Multishot_Bullets);

	CMonster03_Bullet::MONSTER03_BULLET_DESC desc = {};
	desc.fRotationPerSec = XMConvertToRadians(90.f);
	desc.fSpeedPerSec = 10.f;
	desc.iData = 0;
	for (size_t i = 0; i < m_iNumBullets; ++i)
	{
		m_pBullets[i] = static_cast<CMonster03_Bullet*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster03_Bullet"), &desc));
	}

	CMonster03_Bullet_Multishot::MONSTER03_MULTISHOT_BULLET_DESC MultishotDesc = {};
	MultishotDesc.fRotationPerSec = XMConvertToRadians(90.f);
	MultishotDesc.fSpeedPerSec = 10.f;
	for (size_t i = 0; i < m_iNum_Multishot_Bullets; ++i)
	{
		m_pMultiShotBullets[i] = static_cast<CMonster03_Bullet_Multishot*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster03_Bullet_Multishot"), &MultishotDesc));
	}


	return S_OK;
}

HRESULT CMonster03_Gun::Bind_ShaderResources()
{
	return S_OK;
}

CMonster03_Gun* CMonster03_Gun::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster03_Gun* pInstance = new CMonster03_Gun(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMonster03_Gun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonster03_Gun::Clone(void* pArg)
{
	CMonster03_Gun* pInstance = new CMonster03_Gun(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMonster03_Gun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster03_Gun::Free()
{

	__super::Free();
	for (auto& pBullet : m_pBullets)
	{
		Safe_Release(pBullet);
	}

	for (auto& pBullet : m_pMultiShotBullets)
	{
		Safe_Release(pBullet);
	}



}
