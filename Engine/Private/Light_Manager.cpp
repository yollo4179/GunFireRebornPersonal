#include "Light_Manager.h"
#include"Light.h"
CLight_Manager::CLight_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
}

const LIGHT_DESC* CLight_Manager::Get_LightDesc(_uint iIndex) const
{
    auto iter = m_pLights[m_iCurLevel].begin();
    for (size_t i = 0; i < iIndex; ++i)
    {
        ++iter;
        
    }
   

    return (*iter)->Get_LightDesc();
}

HRESULT CLight_Manager::Initialize(_uint iNumLevels)
{
    m_iNumLevels = iNumLevels;
    m_pLights = new LIGHTS[m_iNumLevels];
    return S_OK;
}

HRESULT CLight_Manager::Add_Light(const LIGHT_DESC& LightDesc)
{
    CLight* pLight =  CLight::Create(m_pDevice, m_pContext,LightDesc);

    if (nullptr == pLight)
        return E_FAIL;

    m_pLights[m_iCurLevel].push_back(pLight);
    return S_OK;
}

HRESULT CLight_Manager::Render(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
    for (auto& pLight : m_pLights[m_iCurLevel])
    {
        pLight->Render(pShader, pVIBuffer);
    }

    return S_OK;
}

CLight_Manager* CLight_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iNumLevels)
{
    CLight_Manager* pInstance = new CLight_Manager(pDevice, pContext);
    if (pInstance->Initialize(iNumLevels))
    {
        MSG_BOX("Create Clight_manager: Failed");
        Safe_Release(pInstance);
        return nullptr;

    }
    return pInstance;
}

void CLight_Manager::Free()
{
    __super::Free();
    for (int i = 0; i < m_iNumLevels; ++i)
    {
        for (auto Light : m_pLights[i])
        {
            Safe_Release(Light);
        }
        m_pLights[i].clear();
    }
    Safe_Delete_Array(m_pLights);
   
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
