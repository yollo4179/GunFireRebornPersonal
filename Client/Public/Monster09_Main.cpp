#include"stdafx.h"
#include"GameInstance.h"
#include"Monster09_Main.h"
#include"Monster09.h"
#include"Monster09_Mini.h"
#include"UI_BossHP.h"
#include"Camera_Player.h"
CMonster09_Main::CMonster09_Main(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster{pDevice, pContext}
{
}

CMonster09_Main::CMonster09_Main(const CMonster09_Main& Prototype)
	:CMonster(Prototype)
{
}

HRESULT CMonster09_Main::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster09_Main::Initialize(void* pArg)
{
	MONSTER09_MAIN_DESC* pDesc = static_cast<MONSTER09_MAIN_DESC*>(pArg);
	m_fDomainX = pDesc->fDomainX;
	m_fDomainZ = pDesc->fDomainZ;
	m_vBossHPInfo = pDesc->fHPInfo;
	pDesc->iNumPartObject = 0;
	m_pMonsters = pDesc->pMonsters;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Monsters()))
		return E_FAIL;
	if(FAILED(Ready_UI()))
		return E_FAIL;

	return S_OK;
}

void CMonster09_Main::Priority_Update(_float fTimeDelta)
{
	if (false ==m_bDead &&0.f >= m_vBossHPInfo.x)
	{
		m_bDead = true;
		(m_pUIBossHP)->Set_InActive();
		m_pGameInstance->StopBGM();
		m_pGameInstance->PlayEffect(L"Bgm_AfterDeadBoss.mp3", 0.1f);
		m_pGameInstance->PlayBGM(L"Monster09_Wave.mp3", 0.1f);
	}
	
	if (m_fAccMiniSummonTime < m_fMiniSummonTime)
	{
		m_fAccMiniSummonTime += fTimeDelta;
	}

	

	


	if ( (false == m_bDead)&&(true == m_isCutSceneDone))
	{
		if (m_fAccMiniSummonTime >= m_fMiniSummonTime)
		{
			m_fAccMiniSummonTime = 0.f;
			for (int i = 0; i < m_iNumMinis; ++i)
			{
				if (false == (m_pMinis[i]->Get_Active()))
				{
					m_pMinis[i]->Set_Active(true);
					_matrix matPos = XMMatrixIdentity();
					_vector vPos = { m_pGameInstance->Compute_Random(-15.f,15.f),0.f,m_pGameInstance->Compute_Random(140,160) ,1.f};
					matPos.r[3] = vPos;
					m_pMinis[i]->Reset_Position(matPos);
					break;
				}
			}
		
		}
	}
	


	for (auto Monster09 : m_pMonster09s)
	{
		if (Monster09->Get_Active())
			Monster09->Priority_Update(fTimeDelta);
	}
	for (auto& Monster09_Mini : m_pMinis)
	{
		if (Monster09_Mini->Get_Active())
			Monster09_Mini->Priority_Update(fTimeDelta);
	}

	_float m_fHpRatio = m_vBossHPInfo.x / m_vBossHPInfo.y;
	/*첫번째 몬스터 활성화 조건*/
	if (false == m_isCutSceneDone && m_bActiveCondition[0] && Compute_DistanceWithPlayer())
	{
		m_pGameInstance->StopBGM();
		m_pGameInstance->PlayBGM(L"Bgm_Boss.mp3", 0.1f);

		m_pUIBossHP->Set_Active();
		m_pMonster09s[0]->Set_Active();
		m_bActiveCondition[0] = false;
		m_pMonster09s[0]->CutSceneStart();
		m_isCutSceneDone = true;
		CCamera_Player* pCamera = static_cast<CCamera_Player*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY2, TEXT("Layer_Camara"), 0));
		pCamera->Set_Boss09_Camera(m_pMonster09s[0]);
	}

	if (m_bActiveCondition[1] && 0.8f >= m_fHpRatio) {
		m_pMonster09s[1]->Set_Active();
		m_bActiveCondition[1] = false;
		m_fMiniSummonTime = 15.f;
	}
	if (m_bActiveCondition[2] && 0.6f >= m_fHpRatio) {
		m_pMonster09s[2]->Set_Active();
		m_bActiveCondition[2] = false;
		m_fMiniSummonTime = 10.f;
	}
	if (m_bActiveCondition[3] && 0.4f >= m_fHpRatio) {
		m_pMonster09s[3]->Set_Active();
		m_bActiveCondition[3] = false;
		m_fMiniSummonTime = 8.f;
	}
	if (m_bActiveCondition[4] && 0.2f >= m_fHpRatio) {
		m_pMonster09s[4]->Set_Active();
		m_bActiveCondition[4] = false;
	}

	

	
}

void CMonster09_Main::Update(_float fTimeDelta)
{
	for (auto Monster09 : m_pMonster09s)
	{
		if (Monster09->Get_Active())
			Monster09->Update(fTimeDelta);
	}
	for (auto& Monster09_Mini : m_pMinis)
	{
		if (Monster09_Mini->Get_Active())
			Monster09_Mini->Update(fTimeDelta);
	}

	
}

void CMonster09_Main::Late_Update(_float fTimeDelta)
{
	for (auto Monster09 : m_pMonster09s)
	{
		if (Monster09->Get_Active())
			Monster09->Late_Update(fTimeDelta);
	}
	for(auto& Monster09_Mini :m_pMinis)
	{ 
		if (Monster09_Mini->Get_Active())
			Monster09_Mini->Late_Update(fTimeDelta);
	}
	
	

}

HRESULT CMonster09_Main::Render()
{
	for (auto Monster09 : m_pMonster09s)
	{
		if (Monster09->Get_Active())
			Monster09->Render();
	}

	for (auto& Monster09_Mini : m_pMinis)
	{
		if (Monster09_Mini->Get_Active())
			Monster09_Mini->Render();
	}

	
	return S_OK;
}

_bool CMonster09_Main::Compute_DistanceWithPlayer()
{
	
	_int  m_iCurLevel = m_pGameInstance->Get_Current_Level();
	if (0 > m_iCurLevel || LEVELID::LEVEL_END <= m_iCurLevel)
		return false;
	CTransform* pTargetTransform = static_cast<CTransform*>(m_pGameInstance->Get_Component(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), TEXT("Com_Transform"), 0));
	if(nullptr == pTargetTransform)
		return false;

	_vector vTargetPos = pTargetTransform->Get_WorldMatrix().r[3];
	vTargetPos = XMVectorSetY(vTargetPos, 0.f);
	_vector vMyPos = m_pTransformCom->Get_WorldMatrix().r[3];
	vMyPos = XMVectorSetY(vMyPos, 0.f);

	_vector vTargetDir = vTargetPos - vMyPos;
	_float fDistance = XMVectorGetX(XMVector3Length(vTargetDir));

	if (fDistance <= 65)
		return true;

	return false;
}

HRESULT CMonster09_Main::Ready_Monsters()
{

	m_pMonster09s.resize(m_iMonster09s_Num);
	CMonster09::MONSTER09_DESC desc = {};
	desc.fRotationPerSec = 10;
	desc.vPosition = { 0.f,0.f,200.f,1.f };
	desc.iNaviIndex = 0.f;
	desc.vScale = { 1.f,1.f,1.f };
	desc.fSpeedPerSec = 10.f;
	desc.fAngle = XMConvertToRadians(180);
	desc.fRotationPerSec = XMConvertToRadians(90.f);
	desc.pMonster09s = &m_pMonster09s;
	desc.pBossHPInfo = &m_vBossHPInfo;
	desc.pDead = &m_bDead;
	m_pPlayerFloat4x4 = (static_cast<CTransform*>
		(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY2, L"Layer_APlayer", L"Com_Transform", 0)))
		->Get_WorldFloat4x4_Ptr();

	for (int i=0;i< m_iMonster09s_Num;++i)
	{
		//_float fInitialX = m_pGameInstance->Compute_Random(-m_fDomainX, m_fDomainX);
		desc.vPosition = { (_float)i, 0.f,200.f,1.f };
		desc.iSplattingIndex = i;
		m_pMonster09s[i] = static_cast<CMonster09*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Monster09"), &desc));
		m_pMonster09s[i]->Set_PlayerFloat4x4(m_pPlayerFloat4x4);
		m_pMonster09s[i]->Set_InActive();

		m_pMonsters->push_back(m_pMonster09s[i]);
	}
	

	/*Mini*/
	CMonster::MONSTER_DESC descMini = {};
	descMini.fRotationPerSec = 10;
	descMini.vPosition = { 0.f,0.f,200.f,1.f };
	descMini.iNaviIndex = 0.f;
	descMini.vScale = { 1.f,1.f,1.f };
	descMini.fSpeedPerSec = 10.f;
	//descMini.fAngle = XMConvertToRadians(180);
	descMini.fRotationPerSec = XMConvertToRadians(90.f);
	m_pMinis.resize(m_iNumMinis);
	for (int i = 0; i < m_iNumMinis; ++i)
	{

		_float fInitialX = m_pGameInstance->Compute_Random(-m_fDomainX, m_fDomainX);
		descMini.vPosition = { fInitialX, 0.f,200.f-m_fDomainZ*2.f,1.f };

		m_pMinis[i] = static_cast<CMonster09_Mini*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Monster09_Mini"), &descMini));
		m_pMinis[i]->Set_PlayerFloat4x4(m_pPlayerFloat4x4);
		m_pMinis[i]->Set_Active(false);

	}


	return S_OK;
}

HRESULT CMonster09_Main::Ready_UI()
{
	m_pUIBossHP;
	CUI_BossHP::UI_BOSS_HP_DESC  UI_HPDesc = {};
	UI_HPDesc.fRotationPerSec = XMConvertToRadians(90.F);
	UI_HPDesc.fSizeX = 0.f;
	UI_HPDesc.fSizeY = 0.f;
	UI_HPDesc.fX = 660;
	UI_HPDesc.fY = 100;
	UI_HPDesc.fSpeedPerSec=10.f;
	UI_HPDesc.iBossNumber = 0;
	UI_HPDesc.eType = UI_TYPE::UI_FIXED;
	UI_HPDesc.pBossHPInfo = &m_vBossHPInfo;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_BossHP"),
		LEVEL_GAMEPLAY2, L"Layer_UI", &UI_HPDesc, reinterpret_cast<CGameObject**>(&m_pUIBossHP))))
		return E_FAIL;

	m_pUIBossHP->Set_InActive();
	m_pUIBossHP->Set_SortDepth(8);
	m_pGameInstance->Add_UI_To_UIMap(TEXT("BossHP"), m_pUIBossHP);

	return S_OK;
}

CMonster09_Main* CMonster09_Main::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster09_Main* pInstance = new CMonster09_Main(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CMonster09_Main Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CMonster09_Main::Clone(void* pArg)
{
	CMonster09_Main* pInstance = new CMonster09_Main(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CMonster09 Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMonster09_Main::Free()
{
	for (auto & Monster09: m_pMonster09s)
	{
		Safe_Release(Monster09);
	}
	m_pMonster09s.clear();

	for (auto& Monster09_Mini : m_pMinis)
	{
		Safe_Release(Monster09_Mini);
	}
	m_pMinis.clear();
	__super::Free();
}
