#include"stdafx.h"
#include"UI_MiniMapMain.h"
#include"GameInstance.h"
#include"Monster.h"
CUI_MiniMapMain::CUI_MiniMapMain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject(pDevice, pContext)
{
}

CUI_MiniMapMain::CUI_MiniMapMain(const CUI_MiniMapMain& Prototype)
	:CUIObject(Prototype)
{
}

HRESULT CUI_MiniMapMain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_MiniMapMain::Initialize(void* pArg)
{
	/*sizesms 200*200으로 하자 (반지름 100)*/
	UI_MAIN_MAP_DESC* pDesc = static_cast<UI_MAIN_MAP_DESC*>(pArg);
	m_eStageLevel = pDesc->eStageLevel;
	m_vCenter.x = pDesc->fX + (pDesc->fSizeX *0.5f);
	m_vCenter.y = pDesc->fY + (pDesc->fSizeY * 0.5f);
	m_fRadius = pDesc->fSizeX * 0.5f;
	m_fRange = pDesc->fRange;

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;
	if (FAILED(Ready_Components()))
		return E_FAIL;

		return S_OK;
}

void CUI_MiniMapMain::Priority_Update(_float fTimeDelta)
{
}

void CUI_MiniMapMain::Update(_float fTimeDelta)
{
	//플레이어 주위로 50 주위에 있는 얘들만 표기하자 

}

void CUI_MiniMapMain::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_MiniMapMain::Render()
{
	if (nullptr == m_ppMonsters)
		return S_OK;
	/*BG Render*/
	{
		BindAndRender_ShaderResources_BG();
	}
	/*MonsterRender*/

	/*Monster*/
	if (nullptr == m_pGameInstance->Get_Component(m_eStageLevel, L"Layer_APlayer", L"Com_Transform", 0))
		return S_OK;
	_vector PlayerPos =(static_cast<CTransform*>(m_pGameInstance->Get_Component(m_eStageLevel, L"Layer_APlayer", L"Com_Transform", 0)))->Get_WorldMatrix().r[3];
	
	_float fRatio = m_fRadius / (m_fRange+20);
	for (auto& Monster : *m_ppMonsters)
	{
		if (false == Monster->Get_Active()) continue;
		if (Monster->Get_Dead())continue;

		_vector MonsterPos =Monster->Get_WorldMatrix().r[3];
		
		_vector RelativeMonsterPos = MonsterPos - PlayerPos;
		if (m_fRange < XMVectorGetX(XMVector3Length(RelativeMonsterPos)))
			continue;
		else 
		{
			_vector MonsterPosInMap = fRatio * RelativeMonsterPos;
			MonsterPosInMap = XMVectorSetY(MonsterPosInMap,XMVectorGetZ(MonsterPosInMap));
			MonsterPosInMap = XMVectorSetZ(MonsterPosInMap, 0.f);
			_matrix MonMatrix = XMMatrixIdentity();
			MonMatrix.r[0] *= 15;
			MonMatrix.r[1] *= 15;
			MonMatrix.r[3] = MonsterPosInMap + m_pTransformCom->Get_WorldMatrix().r[3];
			BindAndRender_ShaderResources_Monster(MonMatrix);

		}

	}
	/*PlayerRender*/
	BindAndRender_ShaderResources_Player();
	/*Level*/
	BindAndRender_ShaderResources_LevelText();
	return S_OK;
}



void CUI_MiniMapMain::Set_Vector(vector< CMonster*>* ppMonsters)
{
	m_ppMonsters = ppMonsters;
}

HRESULT CUI_MiniMapMain::Ready_Components()
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
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MiniMap"),
		TEXT("Com_Texture1"), reinterpret_cast<CComponent**>(&m_pTextureCom_BG))))
		return E_FAIL;
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MiniMap_Player"),
		TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom_Player))))
		return E_FAIL;
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MiniMap_Monster"),
		TEXT("Com_Texture3"), reinterpret_cast<CComponent**>(&m_pTextureCom_Monster))))
		return E_FAIL;
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MiniMap_Portal"),
		TEXT("Com_Texture4"), reinterpret_cast<CComponent**>(&m_pTextureCom_Portal))))
		return E_FAIL;
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MiniMap_Level_Text"),
		TEXT("Com_Texture5"), reinterpret_cast<CComponent**>(&m_pTextureCom_Level))))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_MiniMapMain::BindAndRender_ShaderResources_BG()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
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

HRESULT CUI_MiniMapMain::BindAndRender_ShaderResources_Player()
{
	_matrix matPlayerPos = XMMatrixIdentity();
	matPlayerPos.r[0] *= 20;
	matPlayerPos.r[1] *= 20;
	matPlayerPos.r[3] = m_pTransformCom->Get_WorldMatrix().r[3];
	_float4x4 Float4x4World;
	XMStoreFloat4x4(&Float4x4World, matPlayerPos);

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &Float4x4World)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Player->Bind_ShaderResource(m_pShaderCom, "g_Texture")))
		return E_FAIL;
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();
	return S_OK;
	
}

HRESULT CUI_MiniMapMain::BindAndRender_ShaderResources_Monster(_fmatrix MonWorld)
{
	_float4x4 MatMonPosInWorld;
	XMStoreFloat4x4(&MatMonPosInWorld, MonWorld);
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &MatMonPosInWorld)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Monster->Bind_ShaderResource(m_pShaderCom, "g_Texture")))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();
	return S_OK;
}

HRESULT CUI_MiniMapMain::Bind_ShaderResources_Protal()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Portal->Bind_ShaderResource(m_pShaderCom, "g_Texture")))
		return E_FAIL;
	return S_OK;
}

HRESULT CUI_MiniMapMain::BindAndRender_ShaderResources_LevelText()
{
	_matrix matLevelPos = XMMatrixIdentity();
	matLevelPos.r[0] *= 200;
	matLevelPos.r[1] *= 50;
	_vector vPos = { 130,60,0 };
	matLevelPos.r[3] = vPos +m_pTransformCom->Get_WorldMatrix().r[3];
	_float4x4 Float4x4World;
	XMStoreFloat4x4(&Float4x4World, matLevelPos);

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &Float4x4World)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Level->Bind_ShaderResource(m_pShaderCom, "g_Texture")))
		return E_FAIL;
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();

	
	m_pGameInstance->Render_Font(TEXT("Font32"), m_szLevelName, _float2(205.f, 30.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));

	return S_OK;
}



CUI_MiniMapMain* CUI_MiniMapMain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_MiniMapMain* pInstance = new  CUI_MiniMapMain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created :  CUI_MiniMapMain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_MiniMapMain::Clone(void* pArg)
{
	CUI_MiniMapMain* pInstance = new  CUI_MiniMapMain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned :  CUI_MiniMapMain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_MiniMapMain::Free()
{

	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom_BG);
	Safe_Release(m_pTextureCom_Player);
	Safe_Release(m_pTextureCom_Monster);
	Safe_Release(m_pTextureCom_Portal);
	Safe_Release(m_pTextureCom_Level);
	Safe_Release(m_pVIBufferCom);
}
