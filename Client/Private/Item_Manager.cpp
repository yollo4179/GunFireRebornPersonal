#include"stdafx.h"
#include"Item_Manager.h"
#include"GameInstance.h"
#include"DropItem.h"


IMPLEMENT_SINGLETON(CItem_Manager)

CItem_Manager::CItem_Manager()
{

}

void CItem_Manager::Initialize()
{
}

void CItem_Manager::Set_Item(DROPITEM_TYPE eItemType, CDropItem* pItemToSet)
{
	switch (eItemType)
	{
	case DROPITEM_BULLET_SPECIAL:
		if (m_iNowBullet_SpecialIndex >= g_iNum_DropItem_Bullets_Special)
		{
			m_iNowBullet_SpecialIndex = 0;
			Safe_Release(pItemToSet);
			return;
		}
			
		m_pBullets_Special[m_iNowBullet_SpecialIndex] = pItemToSet;
		++m_iNowBullet_SpecialIndex;
		break;
	case DROPITEM_BULLET_STANDARD:
		if (m_iNowBullet_StandardIndex >= g_iNum_DropItem_Bullets_Standard)
		{
			m_iNowBullet_StandardIndex = 0;
			Safe_Release(pItemToSet);
			return;
		}
		m_pBullets_Standard[m_iNowBullet_StandardIndex] = pItemToSet;
		++m_iNowBullet_StandardIndex;
		break;
	case DROPITEM_COIN:
		if (m_iNowCoinIndex >= g_iNum_DropItem_Coins)
		{
			m_iNowCoinIndex = 0;
			Safe_Release(pItemToSet);
			return;
		}
		
		m_pCoins[m_iNowCoinIndex] = pItemToSet;
		++m_iNowCoinIndex;
		break;

	case DROPITEM_GRENADE:
		if (m_iNowGrenadeIndex >= g_iNum_DropItem_Grenade)
		{
			m_iNowGrenadeIndex = 0;
			Safe_Release(pItemToSet);
			return;
		}

		m_pGrenades[m_iNowGrenadeIndex] = pItemToSet;
		++m_iNowGrenadeIndex;
		break;
	default:
		break;
	}


}

void CItem_Manager::Active_Item(DROPITEM_TYPE eItemType, _fmatrix matMonsterMatrix, CNavigation* pNavigation)
{

	switch (eItemType)
	{
	case DROPITEM_BULLET_SPECIAL:
		if (m_iNowBullet_SpecialIndex >= g_iNum_DropItem_Bullets_Special)
		{
			m_iNowBullet_SpecialIndex = 0;
		}

		m_pBullets_Special[m_iNowBullet_SpecialIndex]->Reset_Item(matMonsterMatrix, pNavigation) ;
		++m_iNowBullet_SpecialIndex;
		break;
	case DROPITEM_BULLET_STANDARD:
		if (m_iNowBullet_StandardIndex >= g_iNum_DropItem_Bullets_Standard)
		{
			m_iNowBullet_StandardIndex = 0;
		}
		m_pBullets_Standard[m_iNowBullet_StandardIndex]->Reset_Item(matMonsterMatrix, pNavigation);
		++m_iNowBullet_StandardIndex;
		break;
	case DROPITEM_COIN:
		if (m_iNowCoinIndex >= g_iNum_DropItem_Coins)
		{
			m_iNowCoinIndex = 0;
		}

		m_pCoins[m_iNowCoinIndex]->Reset_Item(matMonsterMatrix, pNavigation);
		++m_iNowCoinIndex;
		break;

	case DROPITEM_GRENADE:
		if (m_iNowGrenadeIndex >= g_iNum_DropItem_Grenade)
		{
			m_iNowGrenadeIndex = 0;
		}

		m_pGrenades[m_iNowGrenadeIndex]->Reset_Item(matMonsterMatrix, pNavigation);
		++m_iNowGrenadeIndex;
		break;
	default:
		break;
	}
	


}

void CItem_Manager::Clear_All_Items()
{
	for (size_t i = 0; i < g_iNum_DropItem_Bullets_Standard; ++i)
	{
		Safe_Release(m_pBullets_Standard[i]);
		m_pBullets_Standard[i] = nullptr;
	};
	for (size_t i = 0; i < g_iNum_DropItem_Bullets_Special; ++i)
	{
		Safe_Release(m_pBullets_Special[i]);
		m_pBullets_Special[i] = nullptr;
	};

	for (size_t i = 0; i < g_iNum_DropItem_Coins; ++i)
	{
		Safe_Release(m_pCoins[i]);
		m_pCoins[i] = nullptr;
	};
	for (size_t i = 0; i < g_iNum_DropItem_Grenade; ++i)
	{
		Safe_Release(m_pGrenades[i]);
		m_pGrenades[i] = nullptr;
	};
}

void CItem_Manager::Free()
{
	__super::Free();

	for (size_t i = 0; i < g_iNum_DropItem_Bullets_Standard; ++i)
	{
		Safe_Release(m_pBullets_Standard[i]);
	};
	for (size_t i = 0; i < g_iNum_DropItem_Bullets_Special; ++i)
	{
		Safe_Release(m_pBullets_Special[i]);
	};
	
	for (size_t i = 0; i < g_iNum_DropItem_Coins; ++i)
	{
		Safe_Release(m_pCoins[i]);
	};
	for (size_t i = 0; i < g_iNum_DropItem_Grenade; ++i)
	{
		Safe_Release(m_pGrenades[i]);
	};


}
