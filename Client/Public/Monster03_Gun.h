#pragma once
#include"Client_Defines.h"
#include"PartObject.h"

BEGIN(Engine)
END

BEGIN(Client)
class CMonster03_Gun final :public CPartObject
{
public:
	typedef struct :CPartObject::PARTOBJECT_DESC
	{
		_bool* pIsAnimEnd;
		_bool* pMultiShotSign = false;
		_uint* pState;
		
		const _float4x4* pSocketMatrix;
	}MONSTER03_GUN_DESC;


private:
	CMonster03_Gun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster03_Gun(const CMonster03_Gun& Prototype);
	virtual ~CMonster03_Gun() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	_uint* m_pState = { nullptr };
	_bool* m_pMultiShotSign = { nullptr };
	_bool* m_pIsAnimEnd = { nullptr };
	_bool* m_pSniperShootSign = { nullptr };
	const _float4x4* m_pSocketMatrix = { nullptr };

private:
	vector< class CMonster03_Bullet*> m_pBullets;
	_uint m_iNowBulletIndex = { 0 };
	const _uint m_iNumBullets = { 20 };
	vector<class CMonster03_Bullet_Multishot*> m_pMultiShotBullets;
	_uint m_iNowBullet_Multishot_Index = { 0 };
	const _uint m_iNum_Multishot_Bullets = { 20 };

private:
	HRESULT Ready_Components();
	HRESULT Ready_Bullets();
	HRESULT Bind_ShaderResources();

public:
	static CMonster03_Gun* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

class Monster05_Gun
{
};
END
