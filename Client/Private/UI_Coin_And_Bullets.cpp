#include"stdafx.h"
#include"UI_Coin_And_Bullets.h"
#include"GameInstance.h"
#include"UI_Numbers.h"
#include"Player.h"
CUI_Coin_And_Bullets::CUI_Coin_And_Bullets(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI_Parent(pDevice, pContext)
{
}

CUI_Coin_And_Bullets::CUI_Coin_And_Bullets(const CUI_Coin_And_Bullets& Prototype)
	:CUI_Parent(Prototype)
{
}

HRESULT CUI_Coin_And_Bullets::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CUI_Coin_And_Bullets::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	if (FAILED(Ready_Components()))
		return E_FAIL;
	if (FAILED(Ready_Numbers()))
		return E_FAIL;

	
	return S_OK;
}

void CUI_Coin_And_Bullets::Priority_Update(_float fTimeDelta)
{
	CPlayer* pPlayer = static_cast<CPlayer*>( m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), 0));
	PlayerItemInfo eItemInfo = pPlayer->Get_PlayerItemInfo();

	static_cast<CUI_Numbers*>(m_ChildUIs[L"Numbers_Coin"])->Set_Numerator(eItemInfo.iCoins);
	static_cast<CUI_Numbers*>(m_ChildUIs[L"Numbers_Bullets"])->Set_Numerator(eItemInfo.iNumBullets);




}

void CUI_Coin_And_Bullets::Update(_float fTimeDelta)
{
}

void CUI_Coin_And_Bullets::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Coin_And_Bullets::Render()
{
	if(FAILED(BindAndRender_ShaderResources_bullet()))
		return E_FAIL;

	if(FAILED(BindAndRender_ShaderResources_Coin()))
		return E_FAIL;


	for (auto& Child : m_ChildUIs)
		Child.second-> Render();

	return S_OK;
}

HRESULT CUI_Coin_And_Bullets::Ready_Components()
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
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Icon_Coin"),
		TEXT("Com_Texture1"), reinterpret_cast<CComponent**>(&m_pTextureCom_Coin))))
		return E_FAIL;
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Icon_bullets"),
		TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom_Bullet))))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Coin_And_Bullets::Ready_Numbers()
{
	CUIObject* pUIObject = { nullptr };
	CUI_Numbers::UI_NUMBERS_DESC NumberDesc_Coin = {};
	NumberDesc_Coin.fRadian = 0.f;
	NumberDesc_Coin.fRotationPerSec = XMConvertToRadians(90.f);
	NumberDesc_Coin.fSpeedPerSec = 10.f;
	NumberDesc_Coin.fSizeX = 35.f;
	NumberDesc_Coin.fSizeY = 35.f;
	NumberDesc_Coin.fX = m_fX - 0.5 * m_fSizeX + 30-130.f;
	NumberDesc_Coin.fY = m_fY-210 ;
	NumberDesc_Coin.iNumerator = 0;
	NumberDesc_Coin.iDenominator = 0;
	NumberDesc_Coin.isFraction = false;
	NumberDesc_Coin.vColor = { 1.f,1.f,1.f };
	NumberDesc_Coin.isDenominatorInfinity = false;

	pUIObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_PartUI_Numbers"), &NumberDesc_Coin));
	m_ChildUIs.emplace(L"Numbers_Coin", pUIObject);


	CUI_Numbers::UI_NUMBERS_DESC NumberDesc_Bullet = {};
	NumberDesc_Bullet.fRadian = 0.f;
	NumberDesc_Bullet.fRotationPerSec = XMConvertToRadians(90.f);
	NumberDesc_Bullet.fSpeedPerSec = 10.f;
	NumberDesc_Bullet.fSizeX = 35.f;
	NumberDesc_Bullet.fSizeY = 35.f;
	NumberDesc_Bullet.fX = m_fX - 0.5 * m_fSizeX + 250.f -100.f;
	NumberDesc_Bullet.fY = m_fY-210;
	NumberDesc_Bullet.iNumerator = 0;
	NumberDesc_Bullet.iDenominator = 0;
	NumberDesc_Bullet.isFraction = false;
	NumberDesc_Bullet.vColor = { 1.f,1.f,1.f };
	NumberDesc_Bullet.isDenominatorInfinity = false;

	pUIObject = static_cast <CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_PartUI_Numbers"), &NumberDesc_Bullet));
	m_ChildUIs.emplace(L"Numbers_Bullets", pUIObject);

	return S_OK;

	
}

HRESULT CUI_Coin_And_Bullets::BindAndRender_ShaderResources_Coin()
{
	_matrix  matWorldOfMoney = m_pTransformCom->Get_WorldMatrix();
	matWorldOfMoney.r[0] *= 0.2f;
	matWorldOfMoney.r[1] *= 0.2f;
	matWorldOfMoney.r[3] = XMVectorSetX(matWorldOfMoney.r[3], XMVectorGetX(matWorldOfMoney.r[3]) - m_fSizeX * 0.5f-100.f );
	matWorldOfMoney.r[3] = XMVectorSetY(matWorldOfMoney.r[3], XMVectorGetY(matWorldOfMoney.r[3]) +190.f);

	_float4x4 MoneyFloat4x4;
	XMStoreFloat4x4(&MoneyFloat4x4, matWorldOfMoney);
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &MoneyFloat4x4)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Coin->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();


	return S_OK;
}

HRESULT CUI_Coin_And_Bullets::BindAndRender_ShaderResources_bullet()
{

	_matrix  matWorldOfBullet = m_pTransformCom->Get_WorldMatrix();
	matWorldOfBullet.r[0] *= 0.2f;
	matWorldOfBullet.r[1] *= 0.2f;
	matWorldOfBullet.r[3] = XMVectorSetX(matWorldOfBullet.r[3], XMVectorGetX(matWorldOfBullet.r[3]) - m_fSizeX * 0.5f + 150.f);
	matWorldOfBullet.r[3] = XMVectorSetY(matWorldOfBullet.r[3], XMVectorGetY(matWorldOfBullet.r[3]) +190.f);

	_float4x4 BulletFloat4x4;
	XMStoreFloat4x4(&BulletFloat4x4, matWorldOfBullet);
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &BulletFloat4x4)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Bullet->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();
	return S_OK;
}

CUI_Coin_And_Bullets* CUI_Coin_And_Bullets::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Coin_And_Bullets* pInstance = new  CUI_Coin_And_Bullets(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created :  CUI_Coin_And_Bullets");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Coin_And_Bullets::Clone(void* pArg)
{
	CUI_Coin_And_Bullets* pInstance = new  CUI_Coin_And_Bullets(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned :  CUI_Coin_And_Bullets");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Coin_And_Bullets::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom_Coin);
	Safe_Release(m_pTextureCom_Bullet);
	Safe_Release(m_pVIBufferCom);
}
