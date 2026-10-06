#include"stdafx.h"
#include "UI_Numbers.h"
#include"GameInstance.h"

CUI_Numbers::CUI_Numbers(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject(pDevice, pContext)
{
}

CUI_Numbers::CUI_Numbers(const CUI_Numbers& Prototype)
	:CUIObject(Prototype)
{
}

HRESULT CUI_Numbers::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Numbers::Initialize(void* pArg)
{
	UI_NUMBERS_DESC* pDesc = static_cast<UI_NUMBERS_DESC*>(pArg);

	

	{
		m_vColor = pDesc->vColor;
		m_fRadian = pDesc->fRadian;
		m_isFraction = pDesc->isFraction;
		m_iNumerator = pDesc->iNumerator;

		if (m_isFraction) {
			m_isDenominatorInfinity = pDesc->isDenominatorInfinity;
			m_iDenominator = pDesc->iDenominator;

		}
	}
	


	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;


	_uint		iNumViewports = { 1 };
	D3D11_VIEWPORT		ViewportDesc{};
	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);
	m_fOffsetX = m_fX - ViewportDesc.Width * 0.5f;
	m_fOffsetY = -m_fY + ViewportDesc.Height * 0.5f;
	//m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), m_fRadian);


	if (FAILED(Ready_Components()))
		return E_FAIL;
	return S_OK;
}

void CUI_Numbers::Priority_Update(_float fTimeDelta)
{
	
}

void CUI_Numbers::Update(_float fTimeDelta)
{
}

void CUI_Numbers::Late_Update(_float fTimeDelta)
{
	if (m_bActive)
		m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Numbers::Render()
{


	






	
	string  strNumber = to_string(m_iNumerator);

	_float fPositionToRenderNextSymbol = m_fOffsetX;
	_vector vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	

	{

		m_pShaderCom->Bind_RawValue("g_ColorR", &m_vColor.x, sizeof(_float));
		m_pShaderCom->Bind_RawValue("g_ColorG", &m_vColor.y, sizeof(_float));
		m_pShaderCom->Bind_RawValue("g_ColorB", &m_vColor.z, sizeof(_float));
	}

	
	/* 일반 숫자였다면*/
	for (auto& Number_Of_Numerator : strNumber)
	{
		fPositionToRenderNextSymbol += m_fSizeX;
		vCurPos = XMVectorSet(fPositionToRenderNextSymbol, m_fOffsetY, 0.f, 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurPos);
		
		BunchToRender(Number_Of_Numerator - '0');
		
	}
	/* 뒤에 percentage가 있다면*/
	if (m_isFraction)
	{
		/* 슬래시를 그린다*/

		fPositionToRenderNextSymbol += m_fSizeX;
		vCurPos = XMVectorSet(fPositionToRenderNextSymbol, m_fOffsetY, 0.f, 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurPos);
		
		BunchToRender(11);
		/* 부모가 무한대라면 부모를 무한대로 그린다 */
		if (m_isDenominatorInfinity)
		{
			fPositionToRenderNextSymbol += m_fSizeX;
			vCurPos = XMVectorSet(fPositionToRenderNextSymbol, m_fOffsetY, 0.f, 1.f);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurPos);
			
			BunchToRender(10);
		}
		/*무한대가 아니라면 분모를 숫자로서 제대로 채워준다 */
		else
		{
			strNumber = to_string(m_iDenominator);
			for (auto Number_Of_Denominator : strNumber)
			{
				fPositionToRenderNextSymbol += m_fSizeX;
				vCurPos = XMVectorSet(fPositionToRenderNextSymbol, m_fOffsetY, 0.f, 1.f);
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurPos);

				BunchToRender(Number_Of_Denominator -'0');
				
			}
		}

	
	
	}

	{

		int iClearColor = 1.f;
		m_pShaderCom->Bind_RawValue("g_ColorR", &iClearColor, sizeof(_float));
		m_pShaderCom->Bind_RawValue("g_ColorG", &iClearColor, sizeof(_float));
		m_pShaderCom->Bind_RawValue("g_ColorB", &iClearColor, sizeof(_float));
	}

	vCurPos = XMVectorSet(m_fOffsetX, m_fOffsetY, 0.f, 1.f);
	 m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurPos);
	return S_OK;
}

HRESULT CUI_Numbers::BunchToRender(_uint iSymbol)
{
	if (FAILED(Bind_ShaderResources(iSymbol)))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();

	return S_OK;

}

HRESULT CUI_Numbers::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Numbers"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

HRESULT CUI_Numbers::Bind_ShaderResources(_uint iSymbol)
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", iSymbol)))
		return E_FAIL;

	return S_OK;
}

CUI_Numbers* CUI_Numbers::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Numbers* pInstance = new  CUI_Numbers(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created :  CUI_Numbers");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Numbers::Clone(void* pArg)
{
	CUI_Numbers* pInstance = new  CUI_Numbers(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned :  CUI_Numbers");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Numbers::Free()
{
	__super::Free();


	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
