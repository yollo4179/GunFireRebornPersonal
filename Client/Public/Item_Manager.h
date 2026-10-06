#pragma once
#include"Base.h"
#include"Client_Defines.h"

const int g_iNum_DropItem_Bullets_Standard= 20;
const int g_iNum_DropItem_Bullets_Special= 20;
const int g_iNum_DropItem_Coins= 50;
const int g_iNum_DropItem_Grenade = 20;
BEGIN(Engine)
class CNavigation;
END
BEGIN(Client)
class CItem_Manager final :public CBase
{

	DECLARE_SINGLETON(CItem_Manager);

public:
	enum DROPITEM_TYPE
	{
		DROPITEM_BULLET_SPECIAL=0,
		DROPITEM_BULLET_STANDARD=1,
		DROPITEM_COIN=2,
		DROPITEM_GRENADE=3,
		DROPITEM_END,
	};
private:
	CItem_Manager();
	virtual ~CItem_Manager() = default;
public:
	void Initialize();


public :
	void Set_Item(DROPITEM_TYPE eItemType ,class CDropItem* pItemToSet);
	void Active_Item(DROPITEM_TYPE eItemType, _fmatrix fMonsterMatrix, CNavigation* pNavigation);
	void Clear_All_Items();
	/*gameObject에 일단 넣고  여기서는 active관리만 하자*/

	class CDropItem* m_pCoins[g_iNum_DropItem_Coins] = {};
	int m_iNowCoinIndex = 0;

	class CDropItem* m_pBullets_Standard[g_iNum_DropItem_Bullets_Standard] = {};
	int m_iNowBullet_StandardIndex = 0;

	class CDropItem* m_pBullets_Special[g_iNum_DropItem_Bullets_Special] = {};
	int m_iNowBullet_SpecialIndex = 0;

	class CDropItem* m_pGrenades[g_iNum_DropItem_Grenade] = {};
	int m_iNowGrenadeIndex = 0;

public:
	virtual void Free() override;
};
END
