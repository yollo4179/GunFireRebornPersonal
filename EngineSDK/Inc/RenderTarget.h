#pragma once
#include"Base.h"
BEGIN(Engine)
class CRenderTarget final : public CBase
{
private:
	CRenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderTarget() = default;

public:
	/*렌더타겟 클래스가 보관하는 진짜 렌더 타겟 뷰를 가져온다*/
	ID3D11RenderTargetView* Get_RTV() const {
		return m_pRTV;
	}

public:
	HRESULT Initialize(_uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);
	HRESULT Clear();

	HRESULT Bind_ShaderResource(class CShader* pShader, const _char* pConstantName);


#ifdef _DEBUG
public:
	HRESULT Ready_Debug(_float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
#endif

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	/*렌더 타겟 뷰를 생성하기위한 텍스처*/
	ID3D11Texture2D* m_pTexture2D = { nullptr };
	/*우리 ㅣ목적인 렌더 타겟 뷰*/
	ID3D11RenderTargetView* m_pRTV = { nullptr };
	/* 셰이더리소스 뷰용 으로도 쓸것이기 때문에 같이 생성한다(둘다 사용)*/
	ID3D11ShaderResourceView* m_pSRV = { nullptr };
	/* 두개의 렌더 타겟뷰는 공용체처럼 공간을 고융한다 rtv에 그리면 srv에도 똑같이 그려짐*/
	_float4								m_vClearColor = {};
	/*초기화 색상을 정해줘서 논블렌드 오브젝트를 기록하고 나머지는 쓰레기 값으로 채우지 않고 특정색상으로 기록해서 스카이박스를 채우는등 다양한 활용도가 존재*/

#ifdef _DEBUG
private:
	_float4x4							m_WorldMatrix{};
	/* 
	직교 투영용 월드 매트락스다 ->화면 어디에
	렌더타겟뷰를 디버그용으로 그릴지 행렬로 보관
	*/
#endif

public:
	static CRenderTarget* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);
	virtual void Free() override;





};
END
