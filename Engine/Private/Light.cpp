#include "Light.h"
#include"Shader.h"
#include"VIBuffer_Rect.h"

CLight::CLight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :m_pDevice(pDevice),
    m_pContext(pContext)
{
    Safe_AddRef(pDevice);
    Safe_AddRef(pContext);
}

HRESULT CLight::Initialize(const LIGHT_DESC& LightDesc)
{
    m_LightDesc = LightDesc;

    return S_OK;
}

HRESULT CLight::Render(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	//명암 연산 결과  vibUFFER에다가 그릴때
	_uint		iPassIndex = {};

	if (LIGHT_DESC::TYPE_DIRECTIONAL == m_LightDesc.eType)
	{
		//방향성광원은 한개만 연산하겠지만 
		if (FAILED(pShader->Bind_RawValue("g_vLightDir", &m_LightDesc.vDirection, sizeof(_float4))))
			return E_FAIL;
		iPassIndex = 1;
	}
	//점광원은 여러개 존재할수 있으므로  누적해서 기록하도록한다.
	else if (LIGHT_DESC::TYPE_POINT == m_LightDesc.eType)
	{
		if (FAILED(pShader->Bind_RawValue("g_vLightPos", &m_LightDesc.vPosition, sizeof(_float4))))
			return E_FAIL;
		if (FAILED(pShader->Bind_RawValue("g_fLightRange", &m_LightDesc.fRange, sizeof(_float))))
			return E_FAIL;
		iPassIndex = 2;
	}
	/* 빛 자체의 디퓨즈 앰비언트 스페큘러를 던진다 (물질의 것은 일정함)*/
	if (FAILED(pShader->Bind_RawValue("g_vLightDiffuse", &m_LightDesc.vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_vLightAmbient", &m_LightDesc.vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_vLightSpecular", &m_LightDesc.vSpecular, sizeof(_float4))))
		return E_FAIL;
	
	
	//빙연산을 위한 패스를 2개를 판다 ->0-> UI용 1과 2는 렌더타겟용 ShaderDeffered를 통해 버퍼에 기록 
	if (FAILED(pShader->Begin(iPassIndex)))
		return E_FAIL;

	return pVIBuffer->Render();
}

CLight* CLight::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHT_DESC& LightDesc)
{
    CLight* pInstance = new CLight(pDevice, pContext);
    if (FAILED(pInstance->Initialize(LightDesc)))
    {
        MSG_BOX("Created Light:Failed");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

void CLight::Free()
{
    __super::Free();
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
