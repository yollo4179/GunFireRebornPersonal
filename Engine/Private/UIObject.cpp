#include "..\Public\UIObject.h"
#include"GameInstance.h"
CUIObject::CUIObject(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject { pDevice, pContext }
{
}

CUIObject::CUIObject(const CUIObject & Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CUIObject::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CUIObject::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;
	
	UIOBJECT_DESC*	pDesc = static_cast<UIOBJECT_DESC*>(pArg);

	m_fSizeX = pDesc->fSizeX;
	m_fSizeY = pDesc->fSizeY;
	/*m_fX = pDesc->fX;
	m_fY = pDesc->fY;*/
	m_fX = pDesc->fX + (pDesc->fSizeX * 0.5);
	m_fY = pDesc->fY + (pDesc->fSizeY * 0.5);
	m_eType = pDesc->eType;

	_uint		iNumViewports = { 1 };
	D3D11_VIEWPORT		ViewportDesc{};

	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);


	/* 직교튀영을 위한 뷰ㅡ, 투영행르을 만들었다. */
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.f, 1.f));

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	/* 던져준 fX, fY,  fSizeX, fSizeY로 그려질 수 있도록 월드행렬의 상태를 제어해둔다. */
	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - ViewportDesc.Width * 0.5f, -m_fY + ViewportDesc.Height * 0.5f, 0.f, 1.f));


	return S_OK;
}

void CUIObject::Priority_Update(_float fTimeDelta)
{
	POINT		pt{};
	GetCursorPos(&pt);
	ScreenToClient(m_pGameInstance->Get_ClientHandle(), &pt);
	_float2 fMousePos = { (_float)pt.x, (_float)pt.y };

	m_PrevMouseOn = m_NowMouseOn;
	m_NowMouseOn = Check_Is_Mouse_On(fMousePos);

	if (true == m_PrevMouseOn && false == m_NowMouseOn)
		m_eUIState = UI_STATE::AWAY;

	else if (true == m_NowMouseOn && MOUSE_DOWN(MOUSE::DIM_LB))
		m_eUIState = UI_STATE::DOWN;

	else if (true == m_NowMouseOn && MOUSE_UP(MOUSE::DIM_LB))
		m_eUIState = UI_STATE::UP;
	/*  마우스 동작 먼저 받고 조건이 겹치니까 먼저 확인 */

	else if (true == m_NowMouseOn && MOUSE_PRESSING(MOUSE::DIM_LB))
		m_eUIState == UI_STATE::PRESSING;
	else if (true == m_NowMouseOn)
		m_eUIState = UI_STATE::ON;
	else
		m_eUIState = UI_STATE::NONE;
}

_bool CUIObject::Check_Is_Mouse_On(_float2 MousePos)
{
	_float2 LeftTop = { m_fX - m_fSizeX*0.5f , m_fY -m_fSizeY*0.5f};
	_float2 RightBottom = {m_fX+ m_fSizeX*0.5f , m_fY +m_fSizeY*0.5f };


	_bool bCondition = (LeftTop.x < MousePos.x && MousePos.x < RightBottom.x) && (LeftTop.y < MousePos.y && MousePos.y < RightBottom.y);
	return bCondition;
}

void CUIObject::Update_Event()
{
	for (_uint i = 0; i < (_uint)UI_STATE::STATE_END; ++i)
	{
		if (i == (_uint)m_eUIState)
		{
			if (nullptr!=m_EventFunction[i])
			{
				m_EventFunction[(_uint)m_eUIState]();
			}
		}
	}

}



void CUIObject::Free()
{
	__super::Free();
}
