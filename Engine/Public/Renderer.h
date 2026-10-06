#pragma once

#include "Base.h"

BEGIN(Engine)

class CRenderer final : public CBase
{
public:
	enum RENDERGROUP { RG_PRIORITY, RG_SHADOW, RG_NONBLEND,RG_DISTORTION ,RG_GLOW,RG_NONLIGHT, RG_BLEND, RG_UI, RG_END };

private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderer() = default;

public:
	HRESULT Initialize();
	HRESULT Add_RenderObject(RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
	HRESULT Draw_RenderObject();

public:
	void Set_Event_Properties(class CTexture* _pTexture, _float fAccEventTimeDelta)
	{
		m_fAccEventTimeDelta = fAccEventTimeDelta;
		m_pEventTextureCom = _pTexture;
	};

#ifdef _DEBUG
public:
	HRESULT Add_DebugComponent(class CComponent* pDebugCom) {
		m_DebugComponents.push_back(pDebugCom);
		Safe_AddRef(pDebugCom);
		return S_OK;
	}
#endif



private:
	ID3D11Device*						m_pDevice = { nullptr };
	ID3D11DeviceContext*				m_pContext = { nullptr };
	list<class CGameObject*>			m_RenderObjects[RG_END];

	/*For Managing RenderTargets*/
	class CGameInstance* m_pGameInstance = { nullptr };
	/*렌타부를 렌더하기위한 정점 rect와 세이더*/
	class CShader* m_pShader = { nullptr };
	class CVIBuffer_Rect* m_pVIBuffer = { nullptr };
	_float4x4							m_WorldMatrix{}, m_ViewMatrix{}, m_ProjMatrix{};
	/*렌더 타겟들은 직교투영 대상 , 그리고 셰이드를 그리기 전에 빛과 노말로 명암연산 저장하기위한 texture2d에 저장하기위한 용도로*/
	_uint								m_iOriginalViewportWidth{}, m_iOriginalViewportHeight{};
	ID3D11DepthStencilView* m_pShadowDepthStencilView = { nullptr };
	ID3D11DepthStencilView* m_pSamplingDepthStencilView ={nullptr};

#ifdef _DEBUG
private:
	list<class CComponent*>				m_DebugComponents;
#endif
private:
	class CTexture* m_pEventTextureCom = { nullptr };
	_float	  m_fAccEventTimeDelta = { 0.f };

private:
	HRESULT Render_Priority();
	HRESULT Render_Shadow();
	/* 실제 객체들의 디퓨즈재질과 노멀 벡터를 각 렌더타겟에 렌더링했다. 빛 연산을 위한 재료 준비. */
	HRESULT Render_NonBlend();
	/* 빛연산을 수행한다. 결과를 Target_Shade에다가 렌더링 해놓는다. */
	HRESULT Render_Lights();
	HRESULT Render_Final();
	HRESULT Render_Distortion();
	HRESULT Render_Assemble_Distortion();

	
	HRESULT Render_Glow_Original();
	HRESULT Render_Glow_DownSamping();
	HRESULT Render_Glow_DownSampling_BlurX();
	HRESULT Render_Glow_DownSampling_BlurY();
	HRESULT Render_Glow_UpSampling();
	HRESULT Render_Glow_Deffered_UpSampling();

	HRESULT Render_Blur_X();
	HRESULT Render_Blur_Y();
	HRESULT Render_Glow();

	
	HRESULT Render_Assemble();
	HRESULT Render_NonLight();
	HRESULT Render_Blend();
	HRESULT Render_UI();
	
	HRESULT Ready_DepthStencilView(_uint iWidth, _uint iHeight, ID3D11DepthStencilView** DepthStencilView=nullptr);
	HRESULT SetUp_Viewport(_uint iWidth, _uint iHeight);


#ifdef _DEBUG
private:
	HRESULT Render_Debug();
#endif

public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END