#include"stdafx.h"
#include"UI_Shop.h"
#include"UI_Shop_Button_Item.h"
#include"UI_Shop_ShowCard.h"
#include"GameInstance.h"
#include"Player.h"
#include"Hands.h"
#include"Camera_Player.h"
#include"UI_Numbers.h"

CUI_Shop::CUI_Shop(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI_Parent{ pDevice, pContext }
{
}

CUI_Shop::CUI_Shop(const CUI_Shop& Prototype)
	:CUI_Parent(Prototype)
{
	memcpy(m_eItemInfo, Prototype.m_eItemInfo, sizeof(m_eItemInfo));
}

HRESULT CUI_Shop::Initialize_Prototype()
{
	/*가격, 이름, 설명 , RARE인지 아닌지 강화 타입 (공격력 UP,치명타UP, )*/
	m_eItemInfo[UI_ITEM_LIST::ITEM_RICE] =
	{ {200},{UI_ITEM_LIST::ITEM_RICE} ,{L"Dumpling"} ,{L"It recovers 30% of your max HP\n right Away."} ,{0},{0} };
	m_eItemInfo[UI_ITEM_LIST::ITEM_BULLET] =
	{ {1500},{UI_ITEM_LIST::ITEM_BULLET}, { L"Bullets" },{L" You Can Use it to reinforce\n one of your Arms"},{0},{0} };
	m_eItemInfo[UI_ITEM_LIST::ITEM_GRENADE] = 
	{ {150},{UI_ITEM_LIST::ITEM_GRENADE}, { L"Grenade" } ,{L"It refills Grenades up to the max\n number of  Grenades you can have"} ,{0} ,{0} };

	m_eItemInfo[UI_ITEM_LIST::ITEN_SWORD] =
	{ {25},{UI_ITEM_LIST::ITEN_SWORD}, {L"Poisonous Ghost"} ,{L" Reinforce Random Option\n of Poisonous Ghost"} ,{1} ,{0}};;
	m_eItemInfo[UI_ITEM_LIST::ITEM_THUNDER_GAUNTLET] =
	{ {15},{UI_ITEM_LIST::ITEM_THUNDER_GAUNTLET}, { L"Thunder Gloves" } ,{L"Reinforce Random Option\n of Thunder Gloves"} ,{1} ,{0} };;
	m_eItemInfo[UI_ITEM_LIST::ITEM_DEFAULT_GUN] =
	{ {10},{UI_ITEM_LIST::ITEM_DEFAULT_GUN}, { L"Concealed Ammo" } ,{L"Reinforce Random Option\n of Concealed Ammo"} ,{1} ,{0} };;
	m_eItemInfo[UI_ITEM_LIST::ITEM_RASORCANON] =
	{ {20},{UI_ITEM_LIST::ITEM_RASORCANON}, { L"Deafening Mortar" } ,{L"Reinforce Random Option\n of Deafencing Mortar"} ,{1} ,{0} };;
	m_eItemInfo[UI_ITEM_LIST::ITEM_LIVING_GUN] =
	{ {25},{UI_ITEM_LIST::ITEM_LIVING_GUN}, { L"Hell Fish Shooter" } ,{L"Reinforce Random Option\n of Hell Fish Shooter"} ,{1} ,{0} };;
	/*rareItem Info*/

	
	return S_OK;
}

HRESULT CUI_Shop::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_ChildUIs()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	ResetItems();

	for (_uint i = 0; i < (_uint)UI_STATE::STATE_END; ++i)
		m_EventFunction[i] = nullptr;


	return S_OK;
}

void CUI_Shop::Priority_Update(_float fTimeDelta)
{
	if (KEY_UP(KEY::ESC))
	{
		m_bActive = false;

		CCamera_Player* pTargetCamera = static_cast<CCamera_Player*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_Camara"), 0));
		if (nullptr == pTargetCamera)
			return;
		pTargetCamera->Set_Camera_Player();
	}


	if (m_bActive)
	{
		for (auto& Child : m_ChildUIs)
			Child.second->Priority_Update(fTimeDelta);
	}

	CPlayer* pPlayer = static_cast<CPlayer*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), 0));
	PlayerItemInfo eItemInfo = pPlayer->Get_PlayerItemInfo();

	static_cast<CUI_Numbers*>(m_ChildUIs[L"Numbers_Coins_Shop"])->Set_Numerator(eItemInfo.iCoins);
	static_cast<CUI_Numbers*>(m_ChildUIs[L"Numbers_Bullets_Shop"])->Set_Numerator(eItemInfo.iNumBullets);




}

void CUI_Shop::Update(_float fTimeDelta)
{
	if (m_bActive)
	{
		for (auto& Child : m_ChildUIs)
			Child.second->Update(fTimeDelta);
	}
}

void CUI_Shop::Late_Update(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
	}

	/*for (auto& Child : m_ChildUIs)
		Child.second->Late_Update(fTimeDelta);*/


	
	_tchar sz_UITag[MAX_PATH] = L"Button_Item_Picture_0%d";
	for (int i = 1; i <= 8; ++i)
	{
		_tchar szMyTag[MAX_PATH]=L"";
		wsprintf(szMyTag, sz_UITag, i);
		
		if (m_ChildUIs[szMyTag]->IsMouseOn())
		{
			m_ShowCardNo = i;

			static_cast<CUI_Shop_ShowCard*>
				(m_ChildUIs[L"ShowCard"])->
				Set_ItemInfo(static_cast<CUI_Shop_Button_Item*>
					(m_ChildUIs[szMyTag])->Get_ItemInfo());

		}
		if (m_ChildUIs[szMyTag]->Get_Button())
		{
			CPlayer* pPlayer =static_cast<CPlayer*>( m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), 0));
			CHands* pHands01 = static_cast<CHands*> (m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), CPlayer::PART_HANDS01, 0));
			CHands* pHands02 = static_cast<CHands*> (m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), CPlayer::PART_HANDS02, 0));
			CHands* pHands03 = static_cast<CHands*> (m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), CPlayer::PART_HANDS03, 0));
			CHands* pHands04 = static_cast<CHands*> (m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), CPlayer::PART_HANDS04, 0));
			CHands* pHands05 = static_cast<CHands*> (m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), CPlayer::PART_HANDS05, 0));

			CUI_Shop_Button_Item* pTarget = nullptr;
			pTarget = static_cast<CUI_Shop_Button_Item*>(m_ChildUIs[szMyTag]);
			if(!(pTarget->Get_SoldOut()))
			{

				ItemInfo eItemInfo = pTarget->Get_ItemInfo();
				_float4 ePlayerHP = pPlayer->Get_HPInfo();
				PlayerItemInfo ePlayerItemInfo = pPlayer->Get_PlayerItemInfo();		
				
				if (eItemInfo.iItemNo <= ITEM_GRENADE &&eItemInfo.iValue <= ePlayerItemInfo.iCoins)
				{
					switch (eItemInfo.iItemNo)
					{
					case ITEM_RICE:
					{
						ePlayerHP.z *= 1.3f;
						if (ePlayerHP.z > ePlayerHP.w)
							ePlayerHP.z = ePlayerHP.w;
						pPlayer->Set_HPInfo(ePlayerHP);
						ePlayerItemInfo.iCoins -= eItemInfo.iValue;
						pPlayer->Set_PlayerItemInfo(ePlayerItemInfo);
						pTarget->Set_SoldOut();
						break;
					}
					case ITEM_BULLET:
					{
						_uint iExtraBullet = (_uint)m_pGameInstance->Compute_Random(1000.f, 2000.f);
						ePlayerItemInfo.iNumBullets += iExtraBullet;
						ePlayerItemInfo.iCoins -= eItemInfo.iValue;
						pPlayer->Set_PlayerItemInfo(ePlayerItemInfo);
						pTarget->Set_SoldOut();
						break;
					}
					case ITEM_GRENADE:
					{
						ePlayerItemInfo.iNumGrenades = ePlayerItemInfo.iNumMaxGrenades;
						ePlayerItemInfo.iCoins -= eItemInfo.iValue;
						pPlayer->Set_PlayerItemInfo(ePlayerItemInfo);
						pTarget->Set_SoldOut();
						break;
					}
					default:
					{
						break;
					}
					}
					
				}
				else if (eItemInfo.iItemNo > ITEM_GRENADE && eItemInfo.iValue <= ePlayerItemInfo.iNumBullets)
				{
					_uint RandomReinforceNo = (_uint)m_pGameInstance->Compute_Random(0.f, 4.f);
					HandsInfo eHandsInfo;
					switch (eItemInfo.iItemNo)
					{
					case ITEN_SWORD:
						eHandsInfo = pHands01->Get_HandsInfo();
						break;
					case ITEM_THUNDER_GAUNTLET:
						eHandsInfo = pHands02->Get_HandsInfo();
						break;
					case ITEM_DEFAULT_GUN:
						eHandsInfo = pHands03->Get_HandsInfo();
						break;
					case ITEM_RASORCANON:
						eHandsInfo = pHands04->Get_HandsInfo();
						break;
					case ITEM_LIVING_GUN:
						eHandsInfo = pHands05->Get_HandsInfo();
						break;

					default:
						break;
					}
					_float fExtraRate = m_pGameInstance->Compute_Random(0.1f, 0.3f);
					switch (RandomReinforceNo)
					{
					case 0:
						eHandsInfo.fCriticalDemage =(_uint)eHandsInfo.fCriticalDemage* (1.f+ fExtraRate);
						break;
					case 1:
						eHandsInfo.fCriticalRate = (_uint)eHandsInfo.fCriticalRate * (1.f + fExtraRate);
						break;
					case 2:
						 eHandsInfo.fDemage = (_uint)eHandsInfo.fDemage * (1.f + fExtraRate);
						break;
					case 3:
						eHandsInfo.fMaxLoadBullets = (_uint)eHandsInfo.fMaxLoadBullets*(1.f + 3.f * fExtraRate);
						
						break;
					default :
						break;
					}
					switch (eItemInfo.iItemNo)
					{
					case ITEN_SWORD:
						g_HandsInfo[0] = eHandsInfo;
						pHands01->Set_HandsInfo(eHandsInfo);
						pHands01->Set_NumChargingBullet(eHandsInfo.fMaxLoadBullets);
						break;
					case ITEM_THUNDER_GAUNTLET:
						g_HandsInfo[1] = eHandsInfo;
						pHands02->Set_HandsInfo(eHandsInfo);
						pHands02->Set_NumChargingBullet(eHandsInfo.fMaxLoadBullets);
						break;
					case ITEM_DEFAULT_GUN:
						g_HandsInfo[2] = eHandsInfo;
						pHands03->Set_HandsInfo(eHandsInfo);
						pHands03->Set_NumChargingBullet(eHandsInfo.fMaxLoadBullets);
						break;
					case ITEM_RASORCANON:
						g_HandsInfo[3] = eHandsInfo;
						pHands04->Set_HandsInfo(eHandsInfo);
						pHands04->Set_NumChargingBullet(eHandsInfo.fMaxLoadBullets);
						break;
					case ITEM_LIVING_GUN:
						g_HandsInfo[4] = eHandsInfo;
						pHands05->Set_HandsInfo(eHandsInfo);
						pHands05->Set_NumChargingBullet(eHandsInfo.fMaxLoadBullets);
						break;

					default:
						break;
					}
					ePlayerItemInfo.iNumBullets -= eItemInfo.iValue;
					m_eItemInfo[eItemInfo.iItemNo].iValue = (_uint)m_eItemInfo[eItemInfo.iItemNo].iValue * 1.3f;
					static_cast<CUI_Shop_Button_Item*>(m_ChildUIs[szMyTag])->Set_ItemInfo(m_eItemInfo[eItemInfo.iItemNo]);
					pPlayer->Set_PlayerItemInfo(ePlayerItemInfo);

				}

			}
			pTarget->Set_Button(false);
			/* Player Logic DEPENDING On*/
		}
	
	}


}

HRESULT CUI_Shop::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();


	_tchar sz_UITag[MAX_PATH] = L"Button_Item_Picture_0%d";
	for (int i = 1; i <= 8; ++i)
	{
		_tchar szMyTag[MAX_PATH]=L"";
		wsprintf(szMyTag, sz_UITag, i);

		m_ChildUIs[szMyTag]->Render();
	}
	if (-1 != m_ShowCardNo)
	{
		m_ChildUIs[L"ShowCard"]->Render();
		m_ShowCardNo = -1;
	}


	m_pGameInstance->Render_Font(TEXT("Font64"), TEXT("Shop"), _float2(m_fX - 0.5f * m_fSizeX + 50.f, m_fY - 0.5 * m_fSizeY + 10.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));


	if (FAILED(BindAndRender_ShaderResources_bullet()))
		return E_FAIL;

	if (FAILED(BindAndRender_ShaderResources_Coin()))
		return E_FAIL;

	m_ChildUIs[L"Numbers_Coins_Shop"]->Render();
	m_ChildUIs[L"Numbers_Bullets_Shop"]->Render();

	return S_OK;
}

void CUI_Shop::ResetItems()
{
	
	CUI_Shop_Button_Item* pTarget = nullptr;

	pTarget = static_cast<CUI_Shop_Button_Item*>(m_ChildUIs[L"Button_Item_Picture_01"]);
	pTarget->Reset_SoldOut();
	_uint ItemNo = {};
	pTarget->Set_Item(0);
	pTarget->Set_ItemInfo(m_eItemInfo[0]);

	pTarget = static_cast<CUI_Shop_Button_Item*>(m_ChildUIs[L"Button_Item_Picture_02"]);
	pTarget->Reset_SoldOut();
	
	pTarget->Set_Item(1);
	pTarget->Set_ItemInfo(m_eItemInfo[1]);

	pTarget = static_cast<CUI_Shop_Button_Item*>(m_ChildUIs[L"Button_Item_Picture_03"]);
	pTarget->Reset_SoldOut();
	
	pTarget->Set_Item(2);
	pTarget->Set_ItemInfo(m_eItemInfo[2]);

	pTarget = static_cast<CUI_Shop_Button_Item*>(m_ChildUIs[L"Button_Item_Picture_04"]);
	pTarget->Reset_SoldOut();

	pTarget->Set_Item(3);
	pTarget->Set_ItemInfo(m_eItemInfo[3]);

	pTarget = static_cast<CUI_Shop_Button_Item*>(m_ChildUIs[L"Button_Item_Picture_05"]);
	pTarget->Reset_SoldOut();

	pTarget->Set_Item(4);
	pTarget->Set_ItemInfo(m_eItemInfo[4]);

	pTarget = static_cast<CUI_Shop_Button_Item*>(m_ChildUIs[L"Button_Item_Picture_06"]);
	pTarget->Reset_SoldOut();

	pTarget->Set_Item(5);
	pTarget->Set_ItemInfo(m_eItemInfo[5]);

	pTarget = static_cast<CUI_Shop_Button_Item*>(m_ChildUIs[L"Button_Item_Picture_07"]);
	pTarget->Reset_SoldOut();

	pTarget->Set_Item(6);
	pTarget->Set_ItemInfo(m_eItemInfo[6]);

	pTarget = static_cast<CUI_Shop_Button_Item*>(m_ChildUIs[L"Button_Item_Picture_08"]);
	pTarget->Reset_SoldOut();

	pTarget->Set_Item(7);
	pTarget->Set_ItemInfo(m_eItemInfo[7]);
}

void CUI_Shop::Set_Active()
{
	m_bActive = true;
	
	CCamera_Player* pTargetCamera = static_cast<CCamera_Player*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_Camara"), 0));
	if (nullptr == pTargetCamera)
	return;
	pTargetCamera->Set_Camera_Still();
}

HRESULT CUI_Shop::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Inventory_BackGround"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom_BackGround))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Icon_Coin"),
		TEXT("Com_Texture1"), reinterpret_cast<CComponent**>(&m_pTextureCom_Coins))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Icon_bullets"),
		TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom_Bullets))))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Shop::Ready_ChildUIs()
{
	CUI_Shop_Button_Item::UI_BUTTTON_SHOP_ITEM_DESC ItemDesc1 = {};
	ItemDesc1.fRotationPerSec = XMConvertToRadians(90.f);
	ItemDesc1.fSpeedPerSec = 10.f;
	ItemDesc1.fSizeX =200;
	ItemDesc1.fSizeY =250;
	ItemDesc1.fX =(m_fX-m_fSizeX*0.5f)+ 30.f ;
	ItemDesc1.fY = (m_fY - m_fSizeY * 0.5f) + 150.f;
	
	CUI_Shop_Button_Item::UI_BUTTTON_SHOP_ITEM_DESC ItemDesc2 = {};
	ItemDesc2.fRotationPerSec = XMConvertToRadians(90.f);
	ItemDesc2.fSpeedPerSec = 10.f;
	ItemDesc2.fSizeX = 200;
	ItemDesc2.fSizeY = 250;
	ItemDesc2.fX = (m_fX - m_fSizeX * 0.5f) + 30.f+200.f;
	ItemDesc2.fY = (m_fY - m_fSizeY * 0.5f) + 150.f;

	CUI_Shop_Button_Item::UI_BUTTTON_SHOP_ITEM_DESC ItemDesc3 = {};
	ItemDesc3.fRotationPerSec = XMConvertToRadians(90.f);
	ItemDesc3.fSpeedPerSec = 10.f;
	ItemDesc3.fSizeX = 200;
	ItemDesc3.fSizeY = 250;
	ItemDesc3.fX = (m_fX - m_fSizeX * 0.5f) + 30.f+400.f;
	ItemDesc3.fY = (m_fY - m_fSizeY * 0.5f) + 150.f;

	CUI_Shop_Button_Item::UI_BUTTTON_SHOP_ITEM_DESC ItemDesc4 = {};
	ItemDesc4.fRotationPerSec = XMConvertToRadians(90.f);
	ItemDesc4.fSpeedPerSec = 10.f;
	ItemDesc4.fSizeX = 200;
	ItemDesc4.fSizeY = 250;
	ItemDesc4.fX = (m_fX - m_fSizeX * 0.5f) + 30.f;
	ItemDesc4.fY = (m_fY - m_fSizeY * 0.5f) + 150.f+ 250.f;

	CUI_Shop_Button_Item::UI_BUTTTON_SHOP_ITEM_DESC ItemDesc5 = {};
	ItemDesc5.fRotationPerSec = XMConvertToRadians(90.f);
	ItemDesc5.fSpeedPerSec = 10.f;
	ItemDesc5.fSizeX = 200;
	ItemDesc5.fSizeY = 250;
	ItemDesc5.fX = (m_fX - m_fSizeX * 0.5f) + 30.f+200.f;
	ItemDesc5.fY = (m_fY - m_fSizeY * 0.5f) + 150.f + 250.f;

	CUI_Shop_Button_Item::UI_BUTTTON_SHOP_ITEM_DESC ItemDesc6 = {};
	ItemDesc6.fRotationPerSec = XMConvertToRadians(90.f);
	ItemDesc6.fSpeedPerSec = 10.f;
	ItemDesc6.fSizeX = 200;
	ItemDesc6.fSizeY = 250;
	ItemDesc6.fX = (m_fX - m_fSizeX * 0.5f) + 30.f+ 400.f;
	ItemDesc6.fY = (m_fY - m_fSizeY * 0.5f) + 150.f + 250.f;

	CUI_Shop_Button_Item::UI_BUTTTON_SHOP_ITEM_DESC ItemDesc7 = {};
	ItemDesc7.fRotationPerSec = XMConvertToRadians(90.f);
	ItemDesc7.fSpeedPerSec = 10.f;
	ItemDesc7.fSizeX = 200;
	ItemDesc7.fSizeY = 250;
	ItemDesc7.fX = (m_fX - m_fSizeX * 0.5f) + 30.f+600.f;
	ItemDesc7.fY = (m_fY - m_fSizeY * 0.5f) + 150.f + 250.f;

	CUI_Shop_Button_Item::UI_BUTTTON_SHOP_ITEM_DESC ItemDesc8 = {};
	ItemDesc8.fRotationPerSec = XMConvertToRadians(90.f);
	ItemDesc8.fSpeedPerSec = 10.f;
	ItemDesc8.fSizeX = 200;
	ItemDesc8.fSizeY = 250;
	ItemDesc8.fX = (m_fX - m_fSizeX * 0.5f) + 30.f+800.f;
	ItemDesc8.fY = (m_fY - m_fSizeY * 0.5f) + 150.f + 250.f;

	CUI_Shop_ShowCard::UI_SHOP_SHOWCARD_DESC ShowCarDesc = {};
	ShowCarDesc.fRotationPerSec = XMConvertToRadians(90.f);
	ShowCarDesc.fSpeedPerSec = 10.f;
	ShowCarDesc.fSizeX = 600;
	ShowCarDesc.fSizeY = 250;
	ShowCarDesc.fX = (m_fX - m_fSizeX * 0.5f) + 30.f + 600.f;
	ShowCarDesc.fY = (m_fY - m_fSizeY * 0.5f) + 150.f;

	CUIObject* pObject = { nullptr };
	/* For Item Picture*/
	pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Shop_Item_Button"), &ItemDesc1));
	m_ChildUIs.emplace(L"Button_Item_Picture_01", pObject);
	pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Shop_Item_Button"), &ItemDesc2));
	m_ChildUIs.emplace(L"Button_Item_Picture_02", pObject);
	pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Shop_Item_Button"), &ItemDesc3));
	m_ChildUIs.emplace(L"Button_Item_Picture_03", pObject);
	pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Shop_Item_Button"), &ItemDesc4));
	m_ChildUIs.emplace(L"Button_Item_Picture_04", pObject);

	pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Shop_Item_Button"), &ItemDesc5));
	m_ChildUIs.emplace(L"Button_Item_Picture_05", pObject);
	pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Shop_Item_Button"), &ItemDesc6));
	m_ChildUIs.emplace(L"Button_Item_Picture_06", pObject);
	pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Shop_Item_Button"), &ItemDesc7));
	m_ChildUIs.emplace(L"Button_Item_Picture_07", pObject);
	pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Shop_Item_Button"), &ItemDesc8));
	m_ChildUIs.emplace(L"Button_Item_Picture_08", pObject);


	pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Shop_ShowCard"), &ShowCarDesc));
	m_ChildUIs.emplace(L"ShowCard", pObject);


	CUI_Numbers::UI_NUMBERS_DESC NumberDesc_Coin = {};
	NumberDesc_Coin.fRadian = 0.f;
	NumberDesc_Coin.fRotationPerSec = XMConvertToRadians(90.f);
	NumberDesc_Coin.fSpeedPerSec = 10.f;
	NumberDesc_Coin.fSizeX = 40.f;
	NumberDesc_Coin.fSizeY = 40.f;
	NumberDesc_Coin.fX = m_fX + m_fSizeX * 0.5f - 450.f+10.f;
	NumberDesc_Coin.fY = m_fY - m_fSizeY * 0.5f + 40.f;
	NumberDesc_Coin.iNumerator = 0;
	NumberDesc_Coin.iDenominator = 0;
	NumberDesc_Coin.isFraction = false;
	NumberDesc_Coin.vColor = { 1.f,1.f,1.f };
	NumberDesc_Coin.isDenominatorInfinity = false;

	pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_PartUI_Numbers"), &NumberDesc_Coin));
	m_ChildUIs.emplace(L"Numbers_Coins_Shop", pObject);


	CUI_Numbers::UI_NUMBERS_DESC NumberDesc_Bullet = {};
	NumberDesc_Bullet.fRadian = 0.f;
	NumberDesc_Bullet.fRotationPerSec = XMConvertToRadians(90.f);
	NumberDesc_Bullet.fSpeedPerSec = 10.f;
	NumberDesc_Bullet.fSizeX = 40.f;
	NumberDesc_Bullet.fSizeY = 40.f;
	NumberDesc_Bullet.fX = m_fX + m_fSizeX * 0.5f - 200.f+10.f;
	NumberDesc_Bullet.fY = m_fY - m_fSizeY * 0.5f + 40.f;
	NumberDesc_Bullet.iNumerator = 0;
	NumberDesc_Bullet.iDenominator = 0;
	NumberDesc_Bullet.isFraction = false;
	NumberDesc_Bullet.vColor = { 1.f,1.f,1.f };
	NumberDesc_Bullet.isDenominatorInfinity = false;

	pObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_PartUI_Numbers"), &NumberDesc_Bullet));
	m_ChildUIs.emplace(L"Numbers_Bullets_Shop", pObject);


	return S_OK;
}

HRESULT CUI_Shop::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_BackGround->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Shop::BindAndRender_ShaderResources_Coin()
{

	_matrix  matWorldOfMoney = m_pTransformCom->Get_WorldMatrix();
	matWorldOfMoney.r[0] = {100.f,0.f,0.f,0.f};
	matWorldOfMoney.r[1] = {0.f,100.f,0.f,0.f};
	matWorldOfMoney.r[3] = XMVectorSetX(matWorldOfMoney.r[3], XMVectorGetX(matWorldOfMoney.r[3]) + m_fSizeX * 0.5f - 450.f);
	matWorldOfMoney.r[3] = XMVectorSetY(matWorldOfMoney.r[3], XMVectorGetY(matWorldOfMoney.r[3]) + m_fSizeY * 0.5f - 50.f);

	_float4x4 MoneyFloat4x4;
	XMStoreFloat4x4(&MoneyFloat4x4, matWorldOfMoney);
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &MoneyFloat4x4)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Coins->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();


	return S_OK;
	
}

HRESULT CUI_Shop::BindAndRender_ShaderResources_bullet()
{
	_matrix  matWorldOfBullet = m_pTransformCom->Get_WorldMatrix();
	matWorldOfBullet.r[0] = {100.f,0.f,0.f,0.f};
	matWorldOfBullet.r[1] = {0.f,100.f,0.f,0.f};
	matWorldOfBullet.r[3] = XMVectorSetX(matWorldOfBullet.r[3], XMVectorGetX(matWorldOfBullet.r[3]) + m_fSizeX * 0.5f - 200.f);
	matWorldOfBullet.r[3] = XMVectorSetY(matWorldOfBullet.r[3], XMVectorGetY(matWorldOfBullet.r[3]) + m_fSizeY * 0.5f-50.f);

	_float4x4 BulletFloat4x4;
	XMStoreFloat4x4(&BulletFloat4x4, matWorldOfBullet);
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &BulletFloat4x4)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Bullets->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();
	return S_OK;
}

CUI_Shop* CUI_Shop::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Shop* pInstance = new  CUI_Shop(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created :  CUI_Shop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Shop::Clone(void* pArg)
{
	CUI_Shop* pInstance = new  CUI_Shop(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned :  CUI_Inventory");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Shop::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom_BackGround);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom_Bullets);
	Safe_Release(m_pTextureCom_Coins);
}
