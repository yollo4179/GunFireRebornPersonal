#pragma once
#include"UI_Parent.h"
#include"Client_Defines.h"

BEGIN(Client)
class CUI_Arms final:public  CUI_Parent
{
public:
	typedef struct : public CUIObject::UIOBJECT_DESC
	{

	}UI_ARMS_DESC;

private:
	explicit  CUI_Arms(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUI_Arms(const CUI_Arms& Prototype);
	virtual ~CUI_Arms() =default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	
public:
	void Init_UI_MappingIndexWithUI(_uint iArmIndexInPlayer, _uint iArmIndexInUI);
	void Set_CurArmIndex(_uint iArmIndex,_uint iNowEquipedArmIndex);

	/*For Numbers*//* Call In Hands*/
	void Set_BulletsICanFire(_uint iNumBullets);
	void Set_BulletsIHaveNow(_uint iNumBullets_Left);
	void Set_IsInfinityBullets(_bool isInfinityBullets);


private:
	HRESULT Ready_ChildUIs();
	HRESULT Bind_ShaderResources();


private:
	map<_uint, _uint> m_mapIndexMappingConverter;
	_uint m_iNowArmNumber = { 0 };//모든 무기 중 0,1,2,3,4,5,6

	_uint m_iNowEquipedArmIndex = { 0 };//0,1,2 중 (장착하고 있는것중


	_uint m_iNumBulletsICanFire = { 0 };
	_uint m_iNumBulletsIHaveNow = { 0 };
	_bool m_isInfinityBullets = { false };
public:
	static   CUI_Arms* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END