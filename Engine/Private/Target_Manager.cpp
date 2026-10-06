#include "..\Public\Target_Manager.h"
#include "RenderTarget.h"

CTarget_Manager::CTarget_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CTarget_Manager::Initialize()
{
	return S_OK;
}

HRESULT CTarget_Manager::Add_RenderTarget(const _wstring& strTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
	if (nullptr != Find_RenderTarget(strTargetTag))
		return E_FAIL;

	/*렌더 타겟 없으면 추가*/ /*-> diffuse든 normal이든 개별 렌더 타겟을 저장한다*/
	CRenderTarget* pRenderTarget = CRenderTarget::Create(m_pDevice, m_pContext, iWidth, iHeight, ePixelFormat, vClearColor);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	m_RenderTargets.emplace(strTargetTag, pRenderTarget);

	return S_OK;
}

HRESULT CTarget_Manager::Add_MRT(const _wstring& strMRTTag, const _wstring& strTargetTag)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(strTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	/*개별 타겟을 일단 등록한후  개별 타겟을 묶어주는 작업-> 등록한 개별 타겟을 찾고 멀티렌더타겟 키로 리스트를 찾아 리스트에 푸시백한다.*/
	
	list<CRenderTarget*>* pMRTList = Find_MRT(strMRTTag);

	/*멀타뷰 리스트가 없으면 하나 리스트 하나 만들고 리스트에 (찾은 렌더타겟)를 넣은후 멀타뷰에 추가한다 */
	if (nullptr == pMRTList)
	{
		list<CRenderTarget*>		MRTList = { pRenderTarget };

		m_MRTs.emplace(strMRTTag, MRTList);
	}
	else
		pMRTList->push_back(pRenderTarget);

	Safe_AddRef(pRenderTarget);

	return S_OK;
}
/*장치에 바인딩*/
HRESULT CTarget_Manager::Begin_MRT(const _wstring& strMRTTag, ID3D11DepthStencilView* pDSV, _bool isClear)
{
	/* 모아놓은 렌더타겟들을 장치에 바인딩한다. */
	list<CRenderTarget*>* pMRTList = Find_MRT(strMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;


	ID3D11ShaderResourceView* pSRV[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = {
		nullptr
	};

	m_pContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pSRV);


	/*연산용 벡버퍼 를위해 백버퍼 값 가져온다- depth도 같이(주니까) 요구하니까 넣자*/
	m_pContext->OMGetRenderTargets(1, &m_pBackRTV, &m_pOriginalDSV);

	ID3D11RenderTargetView* pRenderTargets[8] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	/*걍 zero memory 할걸...*/
	_uint			iNumRenderTargets = {};

	for (auto& pRenderTarget : *pMRTList)
	{
		if (true == isClear)
			pRenderTarget->Clear();
		
		pRenderTargets[iNumRenderTargets++] = pRenderTarget->Get_RTV();
	}
	/*렌더 시작 전에 일단 클리어하고 시작-> 렌더 타겟 장치에 바인딩 */
	/*Clear depth stenci;*/
	if (nullptr != pDSV)
		m_pContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	m_pContext->OMSetRenderTargets(iNumRenderTargets, pRenderTargets, nullptr == pDSV ? m_pOriginalDSV : pDSV);
	return S_OK;
}

HRESULT CTarget_Manager::End_MRT()
{
	m_pContext->OMSetRenderTargets(1, &m_pBackRTV, m_pOriginalDSV);//끝날때 백버퍼 장치에 바인딩- 연산용이니까 기존 백버퍼는 그대로-> 후처리 연산 시작
	//OMSetRenderTargets하면 addref 자동으로 증가-> 하나씩 내여주자  
	Safe_Release(m_pOriginalDSV);
	Safe_Release(m_pBackRTV);

	return S_OK;
}
/*셰이더에 바인딩*/
HRESULT CTarget_Manager::Bind_ShaderResource(CShader* pShader, const _char* pConstantName, const _wstring& strTargetTag)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(strTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	return pRenderTarget->Bind_ShaderResource(pShader, pConstantName);
}

#ifdef _DEBUG

HRESULT CTarget_Manager::Ready_Debug(const _wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(strTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	return pRenderTarget->Ready_Debug(fX, fY, fSizeX, fSizeY);
}

HRESULT CTarget_Manager::Render_Debug(const _wstring& strMRTTag, CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	list<CRenderTarget*>* pMRTList = Find_MRT(strMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	for (auto& pRenderTarget : *pMRTList)
		pRenderTarget->Render_Debug(pShader, pVIBuffer);

	return S_OK;
}

#endif

CRenderTarget* CTarget_Manager::Find_RenderTarget(const _wstring& strTargetTag)
{
	auto	iter = m_RenderTargets.find(strTargetTag);
	if (iter == m_RenderTargets.end())
		return nullptr;

	return iter->second;

}

list<class CRenderTarget*>* CTarget_Manager::Find_MRT(const _wstring& strMRTTag)
{
	auto	iter = m_MRTs.find(strMRTTag);

	if (iter == m_MRTs.end())
		return nullptr;

	return &iter->second;
}

CTarget_Manager* CTarget_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTarget_Manager* pInstance = new CTarget_Manager(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CTarget_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CTarget_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	for (auto& Pair : m_MRTs)
	{
		for (auto& pRenderTarget : Pair.second)
			Safe_Release(pRenderTarget);

		Pair.second.clear();
	}
	m_MRTs.clear();

	for (auto& Pair : m_RenderTargets)
		Safe_Release(Pair.second);

	m_RenderTargets.clear();
}