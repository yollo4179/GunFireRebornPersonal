#pragma once
#include"Client_Defines.h"
#include"Npc.h"
BEGIN(Engine)
class CShader;class CTexture;
END
BEGIN(Client)
class CNpc_ItemBox  final :public CNpc
{
public:
	typedef struct :public CNpc::NPC_DESC
	{

	}NPC_ITEMBOX_DESC;

private:
	explicit CNpc_ItemBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CNpc_ItemBox(const CNpc_ItemBox& Prototype);
	virtual ~CNpc_ItemBox() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Activate_Npc()override;

private:

	_bool m_isChangingMotion = { false };

	_bool m_isAnimDone = { false };
	
	CTexture* m_pTextureCom_Glow = { nullptr };
private:
	HRESULT Ready_Components();
	HRESULT Ready_Particle();
	HRESULT Bind_ShaderResources_Glow();

public:
	static   CNpc_ItemBox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};END

