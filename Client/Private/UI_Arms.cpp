#include"stdafx.h"
#include "UI_Arms.h"
#include"GameInstance.h"
#include"UI_Arm_Paint.h"
#include"UI_Arm1.h"
#include"UI_Arm2.h"
#include"UI_Arm3.h"
#include"UI_Numbers.h"



CUI_Arms::CUI_Arms(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI_Parent{ pDevice, pContext }
{
}

CUI_Arms::CUI_Arms(const CUI_Arms& Prototype)
:CUI_Parent(Prototype)
{
}

HRESULT CUI_Arms::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Arms::Initialize(void* pArg)
{
	UI_ARMS_DESC* pDesc = static_cast<UI_ARMS_DESC*>(pArg);

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	if (FAILED(Ready_ChildUIs()))
		return E_FAIL;

	for (_uint i = 0; i < (_uint)UI_STATE::STATE_END; ++i)
		m_EventFunction[i] = nullptr;


	return S_OK;
}

void CUI_Arms::Priority_Update(_float fTimeDelta)
{
	if (m_bActive)
	{
		for (auto& Child : m_ChildUIs)
			Child.second->Priority_Update(fTimeDelta);
	}
}

void CUI_Arms::Update(_float fTimeDelta)
{
	if (m_bActive)
	{
		for (auto& Child : m_ChildUIs)
			Child.second->Update(fTimeDelta);
	}
}

void CUI_Arms::Late_Update(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
	}
}

HRESULT CUI_Arms::Render()
{
	if (m_bActive)
	{
		for (auto& Child : m_ChildUIs)
			Child.second->Render();
	}
	return S_OK;
}

void CUI_Arms::Init_UI_MappingIndexWithUI(_uint iArmIndexInPlayer, _uint iArmIndexInUI)
{
	m_mapIndexMappingConverter.emplace(make_pair(iArmIndexInPlayer, iArmIndexInUI));
}

/*플레이어가 호출*/
void CUI_Arms::Set_CurArmIndex(_uint iArmIndexInPlayer, _uint iNowEquipedArmIndex)
{
	m_iNowArmNumber = m_mapIndexMappingConverter[iArmIndexInPlayer]; //For Paint
	
	m_iNowEquipedArmIndex = iNowEquipedArmIndex;  //For  123

}

void CUI_Arms::Set_BulletsICanFire(_uint iNumBullets)
{
	 m_iNumBulletsICanFire = iNumBullets; 
	 static_cast<CUI_Numbers*>(m_ChildUIs[L"Numbers"])->Set_Numerator(m_iNumBulletsICanFire);
	
}

void CUI_Arms::Set_BulletsIHaveNow(_uint iNumBullets_Left)
{
	 m_iNumBulletsIHaveNow = iNumBullets_Left; 
	 static_cast<CUI_Numbers*>(m_ChildUIs[L"Numbers"])->Set_MaxNumber(m_iNumBulletsIHaveNow);
}

void CUI_Arms::Set_IsInfinityBullets(_bool isInfinityBullets)
{
	 m_isInfinityBullets = isInfinityBullets; 
	 static_cast<CUI_Numbers*>(m_ChildUIs[L"Numbers"])->Set_Infinity(m_isInfinityBullets);
}


HRESULT CUI_Arms::Ready_ChildUIs()
{

	CUI_Arm_Paint::UI_ARM_PAINT_DESC PaintDesc = {};
	PaintDesc.fRotationPerSec = XMConvertToRadians(90.f);
	PaintDesc.fSpeedPerSec = 10.f;
	PaintDesc.fSizeX = 180;
	PaintDesc.fSizeY = 80;
	PaintDesc.fX = m_fX+ 120;
	PaintDesc.fY = m_fY + 30;
	PaintDesc.iData = 0;
	PaintDesc.pNowArmNumber = &m_iNowArmNumber;

	CUI_Arm1::UI_ARM1_DESC Arm1_Desc = {};
	Arm1_Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Arm1_Desc.fSpeedPerSec = 10.f;
	Arm1_Desc.fSizeX = 50;
	Arm1_Desc.fSizeY = 50;
	Arm1_Desc.fX = m_fX + 100;
	Arm1_Desc.fY = m_fY + 100;
	Arm1_Desc.iData = 0;
	Arm1_Desc.pNowEquipedArmIndex=&m_iNowEquipedArmIndex;

	CUI_Arm2::UI_ARM2_DESC Arm2_Desc = {};
	Arm2_Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Arm2_Desc.fSpeedPerSec = 10.f;
	Arm2_Desc.fSizeX = 50;
	Arm2_Desc.fSizeY = 50;
	Arm2_Desc.fX = m_fX + 150;
	Arm2_Desc.fY = m_fY + 100;
	Arm2_Desc.iData = 0;
	Arm2_Desc.pNowEquipedArmIndex = &m_iNowEquipedArmIndex;


	CUI_Arm3::UI_ARM3_DESC Arm3_Desc = {};
	Arm3_Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Arm3_Desc.fSpeedPerSec = 10.f;
	Arm3_Desc.fSizeX = 50;
	Arm3_Desc.fSizeY = 50;
	Arm3_Desc.fX = m_fX + 200;
	Arm3_Desc.fY = m_fY + 100;
	Arm3_Desc.iData = 0;
	Arm3_Desc.pNowEquipedArmIndex = &m_iNowEquipedArmIndex;

	CUI_Numbers::UI_NUMBERS_DESC NumberDesc = {};
	NumberDesc.fRadian = 0.f;
	NumberDesc.fRotationPerSec= XMConvertToRadians(90.f);
	NumberDesc.fSpeedPerSec = 10.f;
	NumberDesc.fSizeX = 20.f;
	NumberDesc.fSizeY = 20.f;
	NumberDesc.fX = m_fX + 120;
	NumberDesc.fY = m_fY;
	NumberDesc.iNumerator = 0;
	NumberDesc.iDenominator = 0;
	NumberDesc.isFraction = true;
	NumberDesc.vColor = { 0.f,1.f,0.f };
	NumberDesc.isDenominatorInfinity = false;

	CUIObject* pObject = { nullptr };
	pObject=static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Arm_Paint"), &PaintDesc));
	m_ChildUIs.emplace(L"Arm_Paint", pObject);
	pObject=static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Arm1"), &Arm1_Desc));
	m_ChildUIs.emplace(L"Arm_1", pObject);
	pObject=static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Arm2"), &Arm2_Desc));
	m_ChildUIs.emplace(L"Arm_2", pObject);
	pObject=static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Arm3"), &Arm3_Desc));
	m_ChildUIs.emplace(L"Arm_3", pObject);
	pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_PartUI_Numbers"), &NumberDesc));
	m_ChildUIs.emplace(L"Numbers", pObject);
	
	g_iWinSizeX;
	g_iWinSizeY;
	
	return S_OK;
}

HRESULT CUI_Arms::Bind_ShaderResources()
{
	return S_OK;
}

CUI_Arms* CUI_Arms::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Arms* pInstance = new  CUI_Arms(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created :  CUI_Arms");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Arms::Clone(void* pArg)
{
	CUI_Arms* pInstance = new  CUI_Arms(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned :  CUI_Arms");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Arms::Free()
{
	__super::Free();
}
