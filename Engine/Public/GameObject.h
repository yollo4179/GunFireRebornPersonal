#pragma once

#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	typedef struct : public CTransform::TRANSFORM_DESC
	{
		_uint			iData;
	}GAMEOBJECT_DESC;
protected:
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject(const CGameObject& Prototype);
	virtual ~CGameObject() = default;

public:
	pair<_vector, _vector> Get_PickingPosInWorld();

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_Shadow() { return S_OK; }

/*Render Distrubuter Shader*/
public:
	void Set_Glow(_bool _bGlow) { m_bGlow = _bGlow; };
	void Set_Distortion(_bool _bDistortion) { m_bDistortion = _bDistortion; };

public:
	/*For Transform*/
	_matrix Get_WorldMatrix();									//나의 트랜스폼의 월드 매트릭스를 상대방에게 넘겨준다.
	CTransform* Get_Transform() { return m_pTransformCom; };
	CNavigation* Get_Navigation() { return nullptr; };
public:
	class CComponent* Find_Component(const _wstring& strComponentTag);

protected:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	class CGameInstance*		m_pGameInstance = { nullptr };
	class CTransform*			m_pTransformCom = { nullptr };

	_bool						m_bGlow = { false };
	_bool						m_bDistortion = { false };

protected:
	map<const _wstring, class CComponent*>		m_Components;

protected:
	HRESULT Add_Component(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strComponentTag, CComponent** ppOut, void* pArg = nullptr);

protected:
	_uint						m_iData = {};

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

};

END