#pragma once

#include "Base.h"

/* 우리가 생성한 렌더타겟들을 모아놓는다. */
/* 장치에 동시(최대 8개)에 바인딩할 타겟들을 한번더 묶어놓는다. */
/* 장치에 바인딩해라! (묶어놓은 렌더타겟들을 동시에 장치에 올린다.) */
/* ㄴ 멀티렌더타겟안의 렌더묶음을 바인딩--*/

BEGIN(Engine)
class CTarget_Manager final : public CBase
{

private:
	CTarget_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTarget_Manager() = default;

public:
	HRESULT Initialize();
	/*렌더 타겟을 추가한다- 개별 렌더타겟 추가-키로 관리, 태그 가로 세로크기 픽셀의 데이터 포맷 계산 안된값 채워줄 색상-*/
	HRESULT Add_RenderTarget(const _wstring& strTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);
	/*멀티 렌더 타겟을 추가한다 -> 렌더 타겟 여러개를 만들어놓고 하나의 키로 매핑된 리스트에 만든 렌더타겟들을 묶어서관리-*/
	HRESULT Add_MRT(const _wstring& strMRTTag, const _wstring& strTargetTag);
	/*선택한 멀티 렌더타겟내의(렌더 타겟 묶음 들)을 다 바인딩한다)*/
	HRESULT Begin_MRT(const _wstring& strMRTTag, ID3D11DepthStencilView* pDSV = nullptr, _bool isClear = true);
	/*바인딩 끝나고 렌더후 정리*/
	HRESULT End_MRT();
	/*렌더러(렌더함수)에서 렌더할때 , 렌더러가 가진(멤버변수로) 렌더타겟뷰를 여기서 셰이더로 보낸다 묶는다*/
	HRESULT Bind_ShaderResource(class CShader* pShader, const _char* pConstantName, const _wstring& strTargetTag);

#ifdef _DEBUG
public:
	HRESULT Ready_Debug(const _wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug(const _wstring& strMRTTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);

#endif


private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	/*렌더 타겟 뷰와 뎊스 스텐실 ->백버퍼 렌타뷰 만들때 뎊스 스텐실 뷰를 같이 요구해서 일단 같이 놓자-, */
	/* 나중에 스박용으로 사용 ,디바이스(system)에서 처음 생성한백버퍼는 놔두고
	(원래 가지고 있던 백버퍼는 그대로 가지고 있고,) diffuse normal shade 를 위한 연산용 백버퍼를 하나 더만들자-> */
	ID3D11RenderTargetView* m_pBackRTV = { nullptr };
	ID3D11DepthStencilView* m_pOriginalDSV = { nullptr };
	map<const _wstring, class CRenderTarget*>			m_RenderTargets;//하나의 렌더타겟 별도로 키로 관리
	map<const _wstring, list<class CRenderTarget*>>		m_MRTs;         //만든 렌더타겟들을 묶어서 키로 관리
	/* 각각의 렌더 타겟들은 백버퍼랑 사이즈가 똑같아야함*/

	/*백버퍼에는 스카이박스만 그리고 , render priority함수가 끝나면, nonblend(지형, 플레이어) 그리기 전에 diffuse normal 바인딩하고, 거기에 값들을 그린다*/
public:
	class CRenderTarget* Find_RenderTarget(const _wstring& strTargetTag);
	class list<class CRenderTarget*>* Find_MRT(const _wstring& strMRTTag);

public:
	static CTarget_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;


};
END
