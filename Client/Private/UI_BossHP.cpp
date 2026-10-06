#include"stdafx.h"
#include"UI_BossHP.h"
#include"GameInstance.h"
#include"UI_Numbers.h"
CUI_BossHP::CUI_BossHP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI_Parent{pDevice, pContext}
{
}

CUI_BossHP::CUI_BossHP(const CUI_BossHP& Prototype)
	:CUI_Parent(Prototype)
{
}

HRESULT CUI_BossHP::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_BossHP::Initialize(void* pArg)
{
	UI_BOSS_HP_DESC* pDesc = static_cast<UI_BOSS_HP_DESC*>(pArg);

	m_pBossHPInfo = pDesc->pBossHPInfo;
	m_iBossNumber = pDesc->iBossNumber;

	m_fPrevRatio = (*m_pBossHPInfo).x / (*m_pBossHPInfo).y;


	if (FAILED(__super::Initialize(pArg)))
		return S_OK; 
	if (FAILED(Ready_Components()))
		return S_OK;

	if (FAILED(Ready_ChildUIs()))
		return S_OK;



	return S_OK;
}

void CUI_BossHP::Priority_Update(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	_float fBarRatio = (*m_pBossHPInfo).x / (*m_pBossHPInfo).y;
	if (m_bHitSign)
	{
		m_fPrevRatio -= fTimeDelta;
		if (m_fPrevRatio <= fBarRatio)
		{
			m_fPrevRatio = fBarRatio;
			m_bHitSign = false;
		}
	}
	if (m_fPrevRatio != fBarRatio)
	{
		m_bHitSign = true;
	}


	
	for (auto& Child : m_ChildUIs)
		Child.second->Priority_Update(fTimeDelta);
	static_cast<CUI_Numbers*>(m_ChildUIs[L"Numbers_Red"])->Set_Numerator((*m_pBossHPInfo).x);
	static_cast<CUI_Numbers*>(m_ChildUIs[L"Numbers_Red"])->Set_MaxNumber((*m_pBossHPInfo).y);
	


}

void CUI_BossHP::Update(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	
		for (auto& Child : m_ChildUIs)
			Child.second->Update(fTimeDelta);

}

void CUI_BossHP::Late_Update(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);

	
	for (auto& Child : m_ChildUIs)
		Child.second->Late_Update(fTimeDelta);
	

	return;
}

HRESULT CUI_BossHP::Render()
{

	BindAndRender_ShaderResources_BG();
	BindAndRender_ShaderResources_BossImg();
	BindAndRender_ShaderResources_HP_Gray_Back();
	BindAndRender_ShaderResources_HP_Gray();
	BindAndRender_ShaderResources_HP_Red();

	if (m_bActive)
	{
		for (auto& Child : m_ChildUIs)
			Child.second->Render();
	}
	m_pGameInstance->Render_Font(TEXT("Font32"), m_szBossName, _float2(500.f, 20.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));


	return S_OK;
}

HRESULT CUI_BossHP::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/*Texture*/


	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_BossHP_BG"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom_BG))))
		return E_FAIL;
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_BossHP_Img"),
		TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom_BossImg))))
		return E_FAIL;
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_BossHP_Red"),
		TEXT("Com_Texture3"), reinterpret_cast<CComponent**>(&m_pTextureCom_Red))))
		return E_FAIL;
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_BossHP_Gray"),
		TEXT("Com_Texture4"), reinterpret_cast<CComponent**>(&m_pTextureCom_Gray))))
		return E_FAIL;
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_BossHP_Back"),
		TEXT("Com_Texture5"), reinterpret_cast<CComponent**>(&m_pTextureCom_Back))))
		return E_FAIL;




}

HRESULT CUI_BossHP::BindAndRender_ShaderResources_BG()
{
	g_iWinSizeX; g_iWinSizeY;
	_matrix matBackFGroundPos = XMMatrixIdentity();
	matBackFGroundPos.r[0] *= 600.f;
	matBackFGroundPos.r[1] *= 80.f;

	matBackFGroundPos.r[3] = m_pTransformCom->Get_WorldMatrix().r[3];
	_float4x4 Float4x4World;
	XMStoreFloat4x4(&Float4x4World, matBackFGroundPos);

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &Float4x4World)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_BG->Bind_ShaderResource(m_pShaderCom, "g_Texture")))
		return E_FAIL;
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();
	return S_OK;
}

HRESULT CUI_BossHP::BindAndRender_ShaderResources_BossImg()
{
	_matrix matBossImgPos = XMMatrixIdentity();
	matBossImgPos.r[0] *= 120.f;
	matBossImgPos.r[1] *= 120.f;
	_vector vImgPos= { -310.f - 20.f,0.f,0.f };
	matBossImgPos.r[3] = m_pTransformCom->Get_WorldMatrix().r[3]+vImgPos;
	_float4x4 Float4x4World;
	XMStoreFloat4x4(&Float4x4World, matBossImgPos);

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &Float4x4World)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_BossImg->Bind_ShaderResource(m_pShaderCom, "g_Texture",0)))
		return E_FAIL;
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();
	return S_OK;
}

HRESULT CUI_BossHP::BindAndRender_ShaderResources_HP_Red()
{
	_float fBarRatio = (*m_pBossHPInfo).x / (*m_pBossHPInfo).y;
	_matrix matBarPos = XMMatrixIdentity();

	_float fSizeX = 500.f;
	_float fOffset = fSizeX * (1 - fBarRatio) * 0.5f;

	matBarPos.r[0] *= fSizeX * fBarRatio;
	matBarPos.r[1] *= 20.f;
	_vector vPos = { -1.f * fOffset ,0.f,0.f};
	matBarPos.r[3] = vPos + m_pTransformCom->Get_WorldMatrix().r[3];
	_float4x4 Float4x4World;
	XMStoreFloat4x4(&Float4x4World, matBarPos);

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &Float4x4World)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Red->Bind_ShaderResource(m_pShaderCom, "g_Texture")))
		return E_FAIL;
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();

}

HRESULT CUI_BossHP::BindAndRender_ShaderResources_HP_Gray()
{
	
	_matrix matBarPos = XMMatrixIdentity();

	_float fSizeX = 500.f;
	_float fOffset = fSizeX * (1 - m_fPrevRatio) * 0.5f;

	matBarPos.r[0] *= fSizeX * m_fPrevRatio;
	matBarPos.r[1] *= 20.f;
	_vector vPos = { -1.f * fOffset ,0.f,0.f };
	matBarPos.r[3] = vPos + m_pTransformCom->Get_WorldMatrix().r[3];
	_float4x4 Float4x4World;
	XMStoreFloat4x4(&Float4x4World, matBarPos);

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &Float4x4World)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Gray->Bind_ShaderResource(m_pShaderCom, "g_Texture")))
		return E_FAIL;
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();
}

HRESULT CUI_BossHP::BindAndRender_ShaderResources_HP_Gray_Back()
{

	_matrix matGrayBackPos = XMMatrixIdentity();
	matGrayBackPos.r[0] *= 500;
	matGrayBackPos.r[1] *= 20;

	matGrayBackPos.r[3] = m_pTransformCom->Get_WorldMatrix().r[3];
	_float4x4 Float4x4World;
	XMStoreFloat4x4(&Float4x4World, matGrayBackPos);

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &Float4x4World)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Back->Bind_ShaderResource(m_pShaderCom, "g_Texture")))
		return E_FAIL;
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();
}


HRESULT CUI_BossHP::Ready_ChildUIs()
{
	CUIObject* pUIObject = { nullptr };

	CUI_Numbers::UI_NUMBERS_DESC NumberDesc = {};
	NumberDesc.fRadian = 0.f;
	NumberDesc.fRotationPerSec = XMConvertToRadians(90.f);
	NumberDesc.fSpeedPerSec = 10.f;
	NumberDesc.fSizeX = 15.f;
	NumberDesc.fSizeY = 15.f;
	NumberDesc.fX = m_fX + 380.f;
	NumberDesc.fY = m_fY;
	NumberDesc.iNumerator = 0;
	NumberDesc.iDenominator = 0;
	NumberDesc.isFraction = true;
	NumberDesc.vColor = { 0.f,0.f,1.f };
	NumberDesc.isDenominatorInfinity = false;

	pUIObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_PartUI_Numbers"), &NumberDesc));
	m_ChildUIs.emplace(L"Numbers_Red", pUIObject);
	return S_OK;
}

CUI_BossHP* CUI_BossHP::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_BossHP* pInstance = new  CUI_BossHP(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created :  CUI_BossHP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_BossHP::Clone(void* pArg)
{
	CUI_BossHP* pInstance = new  CUI_BossHP(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned :  CUI_BossHP");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CUI_BossHP::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom_BG);
	Safe_Release(m_pTextureCom_BossImg);
	Safe_Release(m_pTextureCom_Red);
	Safe_Release(m_pTextureCom_Gray);
	Safe_Release(m_pTextureCom_Back);
	Safe_Release(m_pVIBufferCom);
}
