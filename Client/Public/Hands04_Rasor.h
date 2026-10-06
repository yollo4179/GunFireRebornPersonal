#pragma once
#include"Client_Defines.h"
#include"GameObject.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END
BEGIN(Client)
class CHands04_Rasor final : public CGameObject
{
public :
	typedef struct :CGameObject::GAMEOBJECT_DESC
	{

	}HANDS04_RASOR_DESC;

private:
	explicit CHands04_Rasor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CHands04_Rasor(const CHands04_Rasor& Prototype);
	virtual ~CHands04_Rasor() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	HRESULT Render_Glow();

public:
	void SetPos(_fvector SrcPos, _fvector DescPos);
	void Set_Active(_bool isActive) { m_isActive = isActive; };
private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Bind_ShaderResources_Glow();

private:
	_float3 m_vSrcPos = {};
	_float3 m_vDestPos = {};
	CTexture* m_pTextureCom = {};
	CTexture* m_pTextureCom_Glow = {};

	CShader* m_pShaderCom = {};
	CVIBuffer_Rect* m_pVIBufferCom = {};
	_bool m_isActive = {false};

	_float m_fAccTime = { 0.f };
	_float m_fTimeLimit = { 10.f };
	_float m_fUVLength = { 0.f };
public:
	static   CHands04_Rasor* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

