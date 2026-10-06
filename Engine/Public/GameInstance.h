#pragma once

#include"Shadow.h"
#include "Renderer.h"
#include"Collider_Manager.h"
#include "PipeLine.h"
#include "Prototype_Manager.h"
#include"Key_Manager.h"
/* 1. 클라이언트 개발자가 사용할 만한 엔진에서 제공해주는 기능들을 보여준다 .*/
/* 2. 엔진의 기능을 원활하게 사용할 수 있도록 엔진을 초기화한다.Initialize_Engine */
/* 3. 사용 종료시에 엔진 기능 구현을 위해서 할당했던 모든 데이터를 삭제한다. Release_Engine*/
/* 4. 엔진에서 갱신이 필요한 모든 객체를 한곳에 모아서 갱신해 준다. */

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public: /* For.GameInstance */
	HRESULT Initialize_Engine(const ENGINE_DESC& EngineDesc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext);
	void Update_Engine(_float fTimeDelta);
	HRESULT Render_Begin(const _float4& vClearColor = _float4(0.f, 0.f, 1.f, 1.f));
	HRESULT Draw();
	HRESULT Render_End();
	void Clear(_int iLevelID);

	_float Compute_Random_Normal();
	_float Compute_Random(_float fMin, _float fMax);

public: /* for.Input_Device */
	KEY_STATE	Get_DIKeyState(KEY eKeyID);
	KEY_STATE	Get_DIMouseState(MOUSE eMouse);
	_long	Get_DIMouseMove(MOUSE_MOVE eMouseState);

public: /* For.timer_Manager */
	_float Get_TimeDelta(const _wstring& strTimerTag);
	HRESULT	Add_Timer(const _wstring& strTimerTag);
	void Update_TimeDelta(const _wstring& strTimerTag);

public: /* For.Renderer */
	HRESULT Add_RenderObject(CRenderer::RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
	void Set_Event_Properties(CTexture* _pTexture, _float fAccEventTimeDelta);
#ifdef _DEBUG
public:
	HRESULT Add_DebugComponent(class CComponent* pDebugCom);
#endif
public: /* for.Level_Manager */
	HRESULT Open_Level(_int iLevelIndex, class CLevel* pNewLevel);
	_int    Get_Current_Level();

public: /* For.Prototype_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag, class CBase* pPrototype);
	class CBase* Clone_Prototype(Engine::PROTOTYPE eType, _uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);

public: /* For.Object_Manager */
	HRESULT Add_GameObject_ToLayer(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr,CGameObject** ppout = nullptr);
	HRESULT	Remove_Object(_uint iLevelID, const _wstring& strLayerTag, CGameObject* pRemoveObject);
	class CComponent* Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iObjectIndex = 0);
	class CComponent* Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, _uint iPartObjectIndex, const _wstring& strPartComponentTag, _uint iObjectIndex = 0);
	class CGameObject* Get_GameObject(_uint iLevelIndex, const _wstring& strLayerTag, _uint iObjectIndex);
	class CGameObject* Get_GameObject(_uint iLevelIndex, const _wstring& strLayerTag, _uint iPartObjectIndex, _uint iObjectIndex);

public: /* For.PipeLine */
	void Set_Transform(CPipeLine::D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix);
	_matrix Get_TransformMatrix(CPipeLine::D3DTRANSFORMSTATE eState);
	_float4x4 Get_TransformFloat4x4(CPipeLine::D3DTRANSFORMSTATE eState);
	_matrix Get_TransformInversedMatrix(CPipeLine::D3DTRANSFORMSTATE eState);
	_float4x4 Get_TransformInversedFloat4x4(CPipeLine::D3DTRANSFORMSTATE eState);
	const _float4* Get_CamPosition() const;

public:/*for Light_Manager*/
	HRESULT Add_Light(const LIGHT_DESC& LightDesc);
	const LIGHT_DESC* Get_LightDesc(_uint iIndex)const;
	HRESULT Render_Lights(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
	void Set_LightsLevel(_uint iLevel);

public:/*For UI Manager*/
	HRESULT Add_UI_To_UIMap(const _wstring UI_Tag, class CUIObject* pUIObject, UI_TYPE eType = UI_TYPE::UI_FIXED);
	HRESULT Set_Active_A_UI(const _wstring UI_Tag);
	HRESULT Set_InActive_A_UI(const _wstring UI_Tag);
	HRESULT Clear_UIObjects();
	class CUIObject* Get_UI_By_Tag(const _wstring UI_Tag);
	HWND	Get_ClientHandle() { return m_ClientHandle; };
	/*For UI Managing DemageFont*/
	class CUIObject* Get_Now_UI_DemageFont();
	HRESULT Add_DemageFonts(CUIObject* pDemageFont);
	void Reset_NumDemageFonts();
	
public:/* For SoundManager*/
	void PlayEffect(const TCHAR* pSoundKey, float fVolume);
	void PlayBGM(const TCHAR* pSoundKey, float fVolume);
	void PlayBGM(const TCHAR* pSoundKey, float fVolume, int iUpdate);
	
	void Resume();
	void Pause();
	void StopBGM();
	void StopAll();
	
	void SetBGMVolume(float fVolume);
	void SetBGMPitch(float fPitch);
	void UpdateBGM(const _float& fTimeDelta, const _float& fSpeed);
	void SetBGM(int iUpdate);

	void StopSound(_uint eID);
	_uint Get_NowEffectID();


public:/*For Collider Manager*/
	HRESULT		Add_Collision_Group(CCollider_Manager::COLLISION_GROUP eMyGroup, CGameObject* pGameObject, CCollider* pCollider);
	void		Clear_Collision_Group();
	_bool		Check_Collision(CCollider* pCollider, CCollider_Manager::COLLISION_GROUP eTargetGroup, CGameObject** ppOutTargetObject);
public: /* For Font_Manager*/
	HRESULT Add_Font(const _wstring& strFontTag, const _tchar* pFontFilePath);
	HRESULT Render_Font(const _wstring& strFontTag, const _tchar* pText, const _float2& vPosition, _fvector vColor, _float fRotation = 0.f, const _float2& vOrigin = _float2(0.f, 0.f));

public: /* Target_Manager */
	HRESULT Add_RenderTarget(const _wstring& strTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);
	HRESULT Add_MRT(const _wstring& strMRTTag, const _wstring& strTargetTag);
	HRESULT Begin_MRT(const _wstring& strMRTTag, ID3D11DepthStencilView* pDSV = nullptr, _bool isClear = true);
	HRESULT End_MRT();
	HRESULT Bind_RT_ShaderResource(class CShader* pShader, const _char* pConstantName, const _wstring& strTargetTag);

public: /* Shadow */
	const _float4x4* Get_Shadow_Transform(CShadow::D3DTRANSFORMSTATE eState);
	void SetUp_Shadow_TransformMatrix(CShadow::D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix);

public: /* Frustum */
	_bool isIn_Frustum_InWorldSpace(_fvector vWorldPos, _float fRange = 0.f);


#ifdef _DEBUG
public:
	HRESULT Ready_RT_Debug(const _wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_RT_Debug(const _wstring& strMRTTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);


#endif


private:
	class CGraphic_Device*				m_pGraphic_Device = { nullptr };
	class CKey_Manager*					m_pKeyManager = { nullptr };
	//class CInput_Device*				m_pInput_Device = { nullptr };
	class CTimer_Manager*				m_pTimer_Manager = { nullptr };	
	class CLevel_Manager*				m_pLevel_Manager = { nullptr };
	class CPrototype_Manager*			m_pPrototype_Manager = { nullptr };
	class CObject_Manager*				m_pObject_Manager = { nullptr };
	class CPipeLine*					m_pPipeLine = { nullptr };
	class CRenderer*					m_pRenderer = { nullptr };
	class CLight_Manager*				m_pLightManager = { nullptr }; 
	class CUI_Manager*					m_pUIManager = { nullptr };
	class CCollider_Manager*			m_pColliderManager = { nullptr };
	class CFont_Manager*				m_pFont_Manager = { nullptr };
	class CTarget_Manager*				m_pTarget_Manager = { nullptr };
	class CShadow*						m_pShadow =	{ nullptr };
	class CFrustum*					    m_pFrustum	= { nullptr };
	class CSound_Manager*				m_pSound_Manager = { nullptr };
	HWND								m_ClientHandle = {};
	
public:
	static void Release_Engine();

public:
	virtual void Free() override;
};

END