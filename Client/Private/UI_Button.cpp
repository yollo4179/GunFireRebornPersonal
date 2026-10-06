#include"stdafx.h"
#include"UI_Button.h"

CUI_Button::CUI_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{pDevice, pContext}
{
}

CUI_Button::CUI_Button(const CUI_Button& Prototype)
	:CUIObject(Prototype)
{

}

bool CUI_Button::isClicked()
{
	/*if (m_bClicked)
	{
		if (nullptr == m_funcOnClicked)
			return false;
		m_funcOnClicked();

		m_bClicked = false;
	}*/
	return true;
}

HRESULT CUI_Button::Initialize_Prototype()
{
	
	
	return S_OK;
}

HRESULT CUI_Button::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}


void CUI_Button::Free()
{
	__super::Free();
}
