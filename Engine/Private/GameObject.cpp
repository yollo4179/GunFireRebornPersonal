#include "..\Public\GameObject.h"
#include "GameInstance.h"
#include "Transform.h"

CGameObject::CGameObject(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
	, m_pGameInstance { CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CGameObject::CGameObject(const CGameObject & Prototype) 
	: m_pDevice{ Prototype.m_pDevice }
	, m_pContext{ Prototype.m_pContext }
	, m_pGameInstance{ Prototype.m_pGameInstance }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

pair<_vector, _vector> CGameObject::Get_PickingPosInWorld()
{
	POINT		pt{};
	GetCursorPos(&pt);
	ScreenToClient(m_pGameInstance->Get_ClientHandle(), &pt);



	_uint		iNumViewports = { 1 };
	D3D11_VIEWPORT		ViewportDesc{};
	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);
	_vector vMousePos;

	//마우스-> ndc 좌표
	vMousePos =
	{
			(_float)pt.x / (ViewportDesc.Width * 0.5f) - 1.f,
			(_float)pt.y / (ViewportDesc.Height * 0.5f) * -1 + 1.f,
			0.f,
			1.f
	};
	//투영 ->뷰스페이스
	_matrix matProj = {};
	matProj = m_pGameInstance->Get_TransformInversedMatrix(CPipeLine::D3DTS_PROJ);
	vMousePos = XMVector3TransformCoord(vMousePos, matProj);

	_vector vRayPos, vRayDir;

	vRayPos = { 0.f,0.f,0.f,1.f };
	vRayDir = vMousePos - vRayPos;

	//뷰->월드
	_matrix matView = {};
	matView = m_pGameInstance->Get_TransformInversedMatrix(CPipeLine::D3DTS_VIEW);
	vRayPos = XMVector3TransformCoord(vRayPos, matView);
	vRayDir = XMVector3TransformNormal(vRayDir, matView);
	vRayDir = XMVector3Normalize(vRayDir);//필수적으로 노말라이즈 시켜준다.

	return { vRayPos, vRayDir };
}

HRESULT CGameObject::Initialize_Prototype()
{
	return S_OK;
}

/* 게임내에서 직접 사용되기 위한 사본 객체가 생성되낟. */
HRESULT CGameObject::Initialize(void * pArg)
{
	if (nullptr != pArg)
	{
		GAMEOBJECT_DESC*		pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
		m_iData = pDesc->iData;
	}	

	m_pTransformCom = CTransform::Create(m_pDevice, m_pContext);
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CGameObject::Priority_Update(_float fTimeDelta)
{
}

void CGameObject::Update(_float fTimeDelta)
{
}

void CGameObject::Late_Update(_float fTimeDelta)
{
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

_matrix CGameObject::Get_WorldMatrix()
{
	if (m_pTransformCom)
	{
		return m_pTransformCom->Get_WorldMatrix();
	}

	return _matrix();
}

CComponent * CGameObject::Find_Component(const _wstring & strComponentTag)
{
	auto	iter = m_Components.find(strComponentTag);

	if(iter == m_Components.end())
		return nullptr;

	return iter->second;
}

HRESULT CGameObject::Add_Component(_uint iPrototypeLevelIndex, const _wstring & strPrototypeTag, const _wstring & strComponentTag, CComponent** ppOut, void * pArg)
{
	if (nullptr != Find_Component(strComponentTag))
		return E_FAIL;

	CComponent*		pComponent = static_cast<CComponent*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, iPrototypeLevelIndex, strPrototypeTag, pArg));
	if (nullptr == pComponent)
		return E_FAIL;

	m_Components.emplace(strComponentTag, pComponent);

	*ppOut = pComponent;//자식에서 ui 하나당 ref 1개씩 깎고,

	Safe_AddRef(pComponent);//부모에서 싹다 지워서 없애버림

	return S_OK;

}

void CGameObject::Free()
{
	__super::Free();

	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);
	m_Components.clear();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
