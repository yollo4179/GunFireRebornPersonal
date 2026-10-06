#pragma once
#include"GameObject.h"
#include"Client_Defines.h"
#include"PartObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END


BEGIN(Client)
class CHands : public CPartObject
{


protected:
	explicit CHands(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CHands(const CHands& Prototype);
	virtual ~CHands() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


	/*As PartObject*/
	void Set_Matrix_From_Parent(CGameObject* pParent);
	void Set_CurrentTransform_Matrix();

	virtual void SetActive(_bool bActiveState) { m_bActive = bActiveState; };

public:
	_bool Fire_A_Bullet();
	_bool Load_Bullets();
	void Set_HandsInfo(HandsInfo _NewInfo) { m_HandsInfo = _NewInfo; };
	HandsInfo Get_HandsInfo() { return m_HandsInfo; };
	void Set_NumChargingBullet(_uint iNumChargingBullets) { m_iNumChargingBullets = iNumChargingBullets; };
protected:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	
	/*for Hiding Armor*/
	_bool m_bActive = { false };

	/*For Bullets*/
	_uint m_iNumBullets_YouHave = 160;
	_uint m_iNumBullets_ICanFireNow = 40;
	_uint m_iNumChargingBullets = 40;
	_bool m_isInfinity = true;
	_bool m_isFired = false;
	/*For UI*/
	class CUI_Arms* m_pArms = { nullptr };

	/*ForAnim*/
	_bool m_isChangingMotion = { false };

	/*Status*/
	HandsInfo m_HandsInfo = {};

protected:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
protected:
	HRESULT Bind_ShaderResources_For_Collider();

public:
	static CHands* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free()override;

};

END
