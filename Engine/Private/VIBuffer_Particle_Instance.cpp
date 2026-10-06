#include "VIBuffer_Particle_Instance.h"
#include"GameInstance.h"
CVIBuffer_Particle_Instance::CVIBuffer_Particle_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CVIBuffer{pDevice, pContext}
{
}

CVIBuffer_Particle_Instance::CVIBuffer_Particle_Instance(const CVIBuffer_Particle_Instance& Prototype)
    :CVIBuffer(Prototype)
    , m_pVBInstance{Prototype.m_pVBInstance}
    , m_InstanceBufferDesc{ Prototype.m_InstanceBufferDesc }
    , m_InstanceInitialDesc{ Prototype.m_InstanceInitialDesc }
    , m_iNumInstances{ Prototype.m_iNumInstances }
    , m_iNumIndexPerInstance{ Prototype.m_iNumIndexPerInstance }
    , m_iInstanceStride{ Prototype.m_iInstanceStride }
    , m_pInstanceVertices{ Prototype.m_pInstanceVertices }
    ,m_pSpeeds{Prototype.m_pSpeeds}
    ,m_isLoop {Prototype.m_isLoop}
    ,m_vPivot{Prototype.m_vPivot}
    , m_vCenter {Prototype.m_vCenter}
    ,m_vRange{Prototype.m_vRange}

{
    Safe_AddRef(m_pVBInstance);//이거는 사실 복사 안해도됨 (null로 할거고 사본에서 따로 할당할것)

}

HRESULT CVIBuffer_Particle_Instance::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CVIBuffer_Particle_Instance::Initialize(void* pArg)
{
    m_pSpeeds_Gravity = new _float[m_iNumInstances];
    return m_pDevice->CreateBuffer(&m_InstanceBufferDesc, &m_InstanceInitialDesc, &m_pVBInstance);

}

HRESULT CVIBuffer_Particle_Instance::Render()
{
                                       //파티클당 인스턴스    //인스턴스 (파티클)개수   // 인덱스 시작위치 ->0(버텍스버퍼) 0(인덱스 버퍼) 0(2번째 버텍스 (인스턴스)버퍼)
    m_pContext->DrawIndexedInstanced(m_iNumIndexPerInstance, m_iNumInstances, 0, 0, 0);
    return S_OK;
}

HRESULT CVIBuffer_Particle_Instance::Bind_BufferDesc()
{
    ID3D11Buffer* pVertexBuffers[] = {
        m_pVB,
        m_pVBInstance,

    };

    _uint					iVertexStrides[] = {
        m_iVertexStride,
        m_iInstanceStride,

    };

    _uint					iOffsets[] = {
        0,
        0,
    };

    m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iVertexStrides, iOffsets);
    m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
    m_pContext->IASetPrimitiveTopology(m_ePrimitiveTopology);

    return S_OK;
}

void CVIBuffer_Particle_Instance::Drop(_float fTimeDelta)
{

    D3D11_MAPPED_SUBRESOURCE SubResource{};
    //인스턴스의 정점 버퍼를 맵 언맵해서  CPU 단위에서 정보를 살짝 바꾸고 셰이더 단으로 던져서 PIXEL처리를 
    //서브리소스에서 pSysyem에 넣어준 정점 정보를 가져온다 
    m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
   
    VTXINSTANCE* pVertices = static_cast<VTXINSTANCE*>(SubResource.pData);//그래픽 카드에서 현재 점의 위치를 가져온디 ( 행렬)
    for (size_t i = 0; i < m_iNumInstances; ++i)
    {
        pVertices[i].vTranslation.y -= m_pSpeeds[i] * fTimeDelta; //(파티클이 내려온다 )

        pVertices[i].vLifeTime.y += fTimeDelta; //y는 트랙 포지션 x가 제한값 
        
        if (pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
        {
            if (true == m_isLoop) {
                pVertices[i].vTranslation = m_pInstanceVertices[i].vTranslation; //(처음에 원본에서 생성할때 생성한초기값)
                pVertices[i].vLifeTime.y = 0.f;
            }
        }


    }
    m_pContext->Unmap(m_pVBInstance, 0);

}

void CVIBuffer_Particle_Instance::Spread(_float fTimeDelta)
{
    D3D11_MAPPED_SUBRESOURCE SubResource{};
    m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
    VTXINSTANCE* pVertices = static_cast <VTXINSTANCE*> (SubResource.pData);

    for (size_t i = 0; i < m_iNumInstances; ++i)
    {
        _vector vDir = XMLoadFloat4(&pVertices[i].vTranslation) - XMVectorSetW(XMLoadFloat3(&m_vPivot), 1.f);
        XMStoreFloat4(&pVertices[i].vTranslation, XMLoadFloat4(&pVertices[i].vTranslation) + XMVector3Normalize(vDir) * m_pSpeeds[i] * fTimeDelta);
        
        pVertices[i].vLifeTime.y += fTimeDelta;

        if (pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
        {
            if (true == m_isLoop)
            {//다시우너래 상태로 되돌린다 (풀링처럼)
                pVertices[i].vTranslation = m_pInstanceVertices[i].vTranslation;
                pVertices[i].vLifeTime.y = 0.f;
            }
        }
    
    }
    m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Particle_Instance::Spread_Gravity(_float fTimeDelta)
{
    D3D11_MAPPED_SUBRESOURCE SubResource{};
    m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
    VTXINSTANCE* pVertices = static_cast <VTXINSTANCE*> (SubResource.pData);

    for (size_t i = 0; i < m_iNumInstances; ++i)
    {
        _vector vDir = XMLoadFloat4(&pVertices[i].vTranslation) - XMVectorSetW(XMLoadFloat3(&m_vPivot), 1.f);
        vDir = XMVector3Normalize(vDir);
        vDir = XMVectorSetY(vDir,m_pSpeeds_Gravity[i]);
        XMStoreFloat4(&pVertices[i].vTranslation, XMLoadFloat4(&pVertices[i].vTranslation) +  vDir* fTimeDelta);
        m_pSpeeds_Gravity[i] -= m_fGravity * fTimeDelta*2.f;


        pVertices[i].vLifeTime.y += fTimeDelta;

        if (pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
        {
            if (true == m_isLoop)
            {//다시우너래 상태로 되돌린다 (풀링처럼)
                pVertices[i].vTranslation = m_pInstanceVertices[i].vTranslation;
                pVertices[i].vLifeTime.y = 0.f;
                m_pSpeeds_Gravity[i] = m_pSpeeds[i];
            }
        }

    }
    m_pContext->Unmap(m_pVBInstance, 0);

}


void CVIBuffer_Particle_Instance::Hit_Random_Show(_float fTimeDelta)
{
    D3D11_MAPPED_SUBRESOURCE SubResource{};
    m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
    VTXINSTANCE* pVertices = static_cast <VTXINSTANCE*> (SubResource.pData);

    for (size_t i = 0; i < m_iNumInstances; ++i)
    {
       
        pVertices[i].vLifeTime.y += fTimeDelta;

        if (pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
        {
            if (true == m_isLoop)
            {//다시우너래 상태로 되돌린다 (풀링처럼)
                pVertices[i].vTranslation = m_pInstanceVertices[i].vTranslation;
                pVertices[i].vLifeTime.y = 0.f;
            }
        }

    }
    m_pContext->Unmap(m_pVBInstance, 0);

}
/*No Pivot*/
void CVIBuffer_Particle_Instance::Go_Up_Random(_float fTimeDelta)
{

    D3D11_MAPPED_SUBRESOURCE SubResource{};
    m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
    VTXINSTANCE* pVertices = static_cast <VTXINSTANCE*> (SubResource.pData);

    for (size_t i = 0; i < m_iNumInstances; ++i)
    {
        _vector vDir = XMVectorSet(0.f,1.f,0.f,0.f);
        XMStoreFloat4(&pVertices[i].vTranslation, XMLoadFloat4(&pVertices[i].vTranslation) + XMVector3Normalize(vDir) * m_pSpeeds[i] * fTimeDelta);

        pVertices[i].vLifeTime.y += fTimeDelta;

        if (pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
        {
            if(true ==m_isLoop)
            {
                pVertices[i].vTranslation = m_pInstanceVertices[i].vTranslation;
                pVertices[i].vLifeTime.y = 0.f;

            }
        }

    }
    m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Particle_Instance::Absorb(_float fTimeDelta)
{
    D3D11_MAPPED_SUBRESOURCE SubResource{};
    m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
    VTXINSTANCE* pVertices = static_cast <VTXINSTANCE*> (SubResource.pData);

    for (size_t i = 0; i < m_iNumInstances; ++i)
    {
        _vector vDir = XMVectorSetW(XMLoadFloat3(&m_vPivot), 1.f) - XMLoadFloat4(&pVertices[i].vTranslation);
        XMStoreFloat4(&pVertices[i].vTranslation, XMLoadFloat4(&pVertices[i].vTranslation) + XMVector3Normalize(vDir) * m_pSpeeds[i] * fTimeDelta);

        pVertices[i].vLifeTime.y += fTimeDelta;

        if (pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
        {
            if (true == m_isLoop)
            {//다시우너래 상태로 되돌린다 (풀링처럼)
                pVertices[i].vTranslation = m_pInstanceVertices[i].vTranslation;
                pVertices[i].vLifeTime.y = 0.f;
            }
        }

    }
    m_pContext->Unmap(m_pVBInstance, 0);

}

void CVIBuffer_Particle_Instance::Rotation_Horizon(_float fTimeDelta)
{


    D3D11_MAPPED_SUBRESOURCE SubResource{};
    m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
    VTXINSTANCE* pVertices = static_cast <VTXINSTANCE*> (SubResource.pData);

    
    for (size_t i = 0; i < m_iNumInstances; ++i)
    {
        _matrix MatRotationHorizon = XMMatrixRotationY(fTimeDelta * m_pSpeeds[i]);
        _vector vRotParticlePos=XMVector3TransformCoord(XMLoadFloat4(&pVertices[i].vTranslation), MatRotationHorizon);
        XMStoreFloat4(&pVertices[i].vTranslation, vRotParticlePos);

        pVertices[i].vLifeTime.y += fTimeDelta;

        if (pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
        {
            if (true == m_isLoop)
            {//다시우너래 상태로 되돌린다 (풀링처럼)
                pVertices[i].vTranslation = m_pInstanceVertices[i].vTranslation;
                pVertices[i].vLifeTime.y = 0.f;
            }
        }

    }
    m_pContext->Unmap(m_pVBInstance, 0);

}

void CVIBuffer_Particle_Instance::Reset_Vertices(_float fTimeDelta)
{
    D3D11_MAPPED_SUBRESOURCE SubResource{};
    m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
    VTXINSTANCE* pVertices = static_cast <VTXINSTANCE*> (SubResource.pData);

    for (size_t i = 0; i < m_iNumInstances; ++i)
    {
       pVertices[i].vTranslation = m_pInstanceVertices[i].vTranslation;
       pVertices[i].vLifeTime.y = 0.f;
       m_pSpeeds_Gravity[i] = m_pSpeeds[i];
    }
    m_pContext->Unmap(m_pVBInstance, 0);

}

void CVIBuffer_Particle_Instance::Reset_Vertices_Random(_float fTimeDelta)
{
    D3D11_MAPPED_SUBRESOURCE SubResource{};
    m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
    VTXINSTANCE* pVertices = static_cast <VTXINSTANCE*> (SubResource.pData);

    for (size_t i = 0; i < m_iNumInstances; ++i)
    {
      
            {// 가챠 다시돌림
                m_pInstanceVertices[i].vTranslation =
                    _float4(
                        m_pGameInstance->Compute_Random(m_vCenter.x - m_vRange.x * 0.5f, m_vCenter.x + m_vRange.x * 0.5f),
                        m_pGameInstance->Compute_Random(m_vCenter.y - m_vRange.y * 0.5f, m_vCenter.y + m_vRange.y * 0.5f),
                        m_pGameInstance->Compute_Random(m_vCenter.z - m_vRange.z * 0.5f, m_vCenter.z + m_vRange.z * 0.5f),
                        1.f);
                _vector vPos = XMLoadFloat4(&m_pInstanceVertices[i].vTranslation);
                if (XMVectorGetX(XMVector3Length(vPos)) >= abs(m_vRange.x - m_vCenter.x))
                {
                    vPos = XMVector3Normalize(vPos) * abs(m_vRange.x - m_vCenter.x);
                    XMStoreFloat4(&pVertices[i].vTranslation, vPos);
                }
                pVertices[i].vTranslation = m_pInstanceVertices[i].vTranslation;
                pVertices[i].vLifeTime.y = 0.f;
         

            }
        

    }
    m_pContext->Unmap(m_pVBInstance, 0);

}

void CVIBuffer_Particle_Instance::Reset_Particle_Transform(_fmatrix matParticle)
{
    D3D11_MAPPED_SUBRESOURCE SubResource{};
    m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
    VTXINSTANCE* pVertices = static_cast <VTXINSTANCE*> (SubResource.pData);


    /*z축으로 늘리는 파티클로 조정하고 ray는 트랜스폼으로 조정하자 */
    for (size_t i = 0; i < m_iNumInstances; ++i)
    {
        XMStoreFloat4(&m_pInstanceVertices[i].vRight, matParticle.r[0]);
        pVertices[i].vRight = m_pInstanceVertices[i].vRight; 

        XMStoreFloat4(&m_pInstanceVertices[i].vUp, matParticle.r[1]);
        pVertices[i].vUp = m_pInstanceVertices[i].vUp;
        
        XMStoreFloat4(&m_pInstanceVertices[i].vLook, matParticle.r[2]);
        pVertices[i].vLook = m_pInstanceVertices[i].vLook;

        XMStoreFloat4(&m_pInstanceVertices[i].vLook, matParticle.r[2]);
        pVertices[i].vLook = m_pInstanceVertices[i].vLook;
       
    }
    m_pContext->Unmap(m_pVBInstance, 0);

}



void CVIBuffer_Particle_Instance::Free()
{
    __super::Free();

    /* 버텍스초기 정보는 공유하니까 우너형 삭제시만 삭제하자 (얕복)*/
    if (false == m_isCloned) 
    {
        Safe_Delete_Array(m_pSpeeds);
        Safe_Delete_Array(m_pInstanceVertices);
    }

    if (true == m_isCloned)
    {
        Safe_Delete_Array(m_pSpeeds_Gravity);
    }
    Safe_Release(m_pVBInstance);

}
