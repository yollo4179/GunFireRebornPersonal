#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Flat;
END


BEGIN(Client)

class CWater_Stage02 final : public CGameObject
{
private:
	CWater_Stage02(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWater_Stage02(const CWater_Stage02& Prototype);
	virtual ~CWater_Stage02() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom[2] = {nullptr};
	CVIBuffer_Flat* m_pVIBufferCom = { nullptr };
private:
	_float m_fTime = {};


private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();


public:
	static CWater_Stage02* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END