#include"stdafx.h"
#include "SettingObject.h"

CSetting_Object::CSetting_Object(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject{pDevice, pContext}
{
}

CSetting_Object::CSetting_Object(const CSetting_Object& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CSetting_Object::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSetting_Object::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CSetting_Object::Priority_Update(_float fTimeDelta)
{
}

void CSetting_Object::Update(_float fTimeDelta)
{
}

void CSetting_Object::Late_Update(_float fTimeDelta)
{
}

HRESULT CSetting_Object::Render()
{
	return E_NOTIMPL;
}

void CSetting_Object::Update_Transform_By_Desc(SETTING_OBJECT_DESC Desc)
{
}

void CSetting_Object::Update_Transform_Go_Straight(const _float fTimeDelta)
{
}

void CSetting_Object::Update_Transform_Go_Backward(const _float fTimeDelta)
{
}

HRESULT CSetting_Object::Ready_Components()
{
	return E_NOTIMPL;
}

HRESULT CSetting_Object::Bind_ShaderResources()
{
	return E_NOTIMPL;
}

HRESULT CSetting_Object::Bind_ShaderResources_For_Collider()
{
	return E_NOTIMPL;
}

CSetting_Object* CSetting_Object::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

CGameObject* CSetting_Object::Clone(void* pArg)
{
	return nullptr;
}

void CSetting_Object::Free()
{
}
