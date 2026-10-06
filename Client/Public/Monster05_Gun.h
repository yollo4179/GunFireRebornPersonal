#pragma once
#include"Client_Defines.h"
#include"PartObject.h"



BEGIN(Engine)
class CCollider;
END

BEGIN(Client)
class CMonster05_Gun final :public CPartObject
{
public:
	typedef struct :CPartObject::PARTOBJECT_DESC
	{
		_bool* pIsAnimEnd;
		_bool* pSniperShootSign;
		_uint* pState;
		const _float4x4* pSocketMatrix;
	}MONSTER05_GUN_DESC;


private:
	CMonster05_Gun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster05_Gun(const CMonster05_Gun& Prototype);
	virtual ~CMonster05_Gun() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	_uint* m_pState = { nullptr };
	_bool* m_pIsAnimEnd = { nullptr };
	_bool* m_pSniperShootSign = { nullptr };
	const _float4x4* m_pSocketMatrix = { nullptr };

private:
	vector< class CMonster05_Bullet*> m_pBullets;
	_uint m_iNowBulletIndex = { 0 };
	const _uint m_iNumBullets = { 20 };
	
	class CEffect_Monster05_Ready* m_pEffectMonster05_Ready = { nullptr };
	
	
private:
	HRESULT Ready_Components();
	HRESULT Ready_Bullets();
	HRESULT Bind_ShaderResources();
	HRESULT Ready_Effects();

public:
	static CMonster05_Gun* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END
