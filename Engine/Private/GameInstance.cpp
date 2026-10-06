#include "..\Public\GameInstance.h"

#include "Frustum.h"
#include "Shadow.h"
#include "Renderer.h"
#include "PipeLine.h"
#include "Input_Device.h"
#include"Font_Manager.h"
#include "Timer_Manager.h"
#include "Level_Manager.h"
#include "Graphic_Device.h"
#include "Object_Manager.h"
#include "Target_Manager.h"
#include "Prototype_Manager.h"
#include"Light_Manager.h"
#include"UI_Manager.h"
#include"Sound_Manager.h"
IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{
	
}

HRESULT CGameInstance::Initialize_Engine(const ENGINE_DESC& EngineDesc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext)
{

	m_ClientHandle = EngineDesc.hWnd;

	m_pGraphic_Device = CGraphic_Device::Create(EngineDesc.hWnd, EngineDesc.isWindowed, EngineDesc.iViewportWidth, EngineDesc.iViewportHeight, ppDevice, ppContext);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	/*m_pInput_Device = CInput_Device::Create(EngineDesc.hInstance, EngineDesc.hWnd);
	if (nullptr == m_pInput_Device)
		return E_FAIL;*/
	
	
	m_pLightManager = CLight_Manager::Create(*ppDevice, *ppContext,EngineDesc.iNumLevels);
	if (nullptr == m_pLightManager)
		return E_FAIL;

	m_pKeyManager = CKey_Manager::Create(EngineDesc.hInstance, EngineDesc.hWnd);
	if (nullptr == m_pKeyManager)
		return E_FAIL;

	m_pPipeLine = CPipeLine::Create();
	if (nullptr == m_pPipeLine)
		return E_FAIL;
	/*렌더러에서 타겟메니저를 이니셜라이즈에서 사용하기 때문에 렌더러보다 머저 생성한다.*/
	m_pTarget_Manager = CTarget_Manager::Create(*ppDevice, *ppContext);
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	m_pRenderer = CRenderer::Create(*ppDevice, *ppContext);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	m_pPrototype_Manager = CPrototype_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pPrototype_Manager)
		return E_FAIL;

	m_pObject_Manager = CObject_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	m_pUIManager = CUI_Manager::Create();
	if (nullptr == m_pUIManager)
		return E_FAIL;
	m_pColliderManager = CCollider_Manager::Create();
	if (nullptr == m_pColliderManager)
		return E_FAIL;

	m_pFont_Manager = CFont_Manager::Create(*ppDevice, *ppContext);
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	m_pShadow = CShadow::Create(*ppDevice, *ppContext);
	if (nullptr == m_pShadow)
		return E_FAIL;

	m_pFrustum = CFrustum::Create();
	if (nullptr == m_pFrustum)
		return E_FAIL;

	m_pSound_Manager = CSound_Manager::Create();
	if (nullptr == m_pSound_Manager)
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
	//m_pInput_Device->Update_InputDev();
	m_pUIManager->Update_UIs();
	m_pKeyManager->Update_KeyManager();

	m_pObject_Manager->Priority_Update(fTimeDelta);

	m_pPipeLine->Update();
	m_pFrustum->Update();

	m_pObject_Manager->Update(fTimeDelta);
	m_pObject_Manager->Late_Update(fTimeDelta);

	/* Transfom ->Priority 시점에 끝나고-> 확인은 update 혹은 late Updat 에서 하니까-> late update끝나고 지우자 Level Open 하기전에 지우니까 레벨 update보다는 앞에*/
	m_pColliderManager->Clear_Collision_Group();
	m_pLevel_Manager->Update(fTimeDelta);
}

HRESULT CGameInstance::Render_Begin(const _float4 & vClearColor)
{
	m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);
	m_pGraphic_Device->Clear_DepthStencil_View();

	return S_OK;
}

HRESULT CGameInstance::Draw()
{
	m_pRenderer->Draw_RenderObject();

	m_pLevel_Manager->Render();

	return S_OK;
}

HRESULT CGameInstance::Render_End()
{
	return m_pGraphic_Device->Present();
}

void CGameInstance::Clear(_int iLevelID)
{
	/* 특정 레벨용 객체들을 지운다. */

 	m_pObject_Manager->Clear(iLevelID);

	m_pPrototype_Manager->Clear(iLevelID);

}

_float CGameInstance::Compute_Random_Normal()
{
	
	return (_float)rand()/RAND_MAX;
}

_float CGameInstance::Compute_Random(_float fMin, _float fMax)
{
	return fMin+ (fMax-fMin)*Compute_Random_Normal();
}



KEY_STATE CGameInstance::Get_DIKeyState(KEY eKey)
{
	return m_pKeyManager->Get_DIKeySatate(eKey);
}

KEY_STATE CGameInstance::Get_DIMouseState(MOUSE eMouse)
{
	return m_pKeyManager->Get_DIMouseState(eMouse);
}

_long CGameInstance::Get_DIMouseMove(MOUSE_MOVE eMouseState)
{
	return m_pKeyManager->Get_DIMouseMove(eMouseState);
}


_float CGameInstance::Get_TimeDelta(const _wstring & strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return 0.f;

	return m_pTimer_Manager->Get_TimeDelta(strTimerTag);
}

HRESULT CGameInstance::Add_Timer(const _wstring & strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Add_Timer(strTimerTag);
}

void CGameInstance::Update_TimeDelta(const _wstring & strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return;

	return m_pTimer_Manager->Update_TimeDelta(strTimerTag);
}

HRESULT CGameInstance::Add_RenderObject(CRenderer::RENDERGROUP eRenderGroup, CGameObject * pRenderObject)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->Add_RenderObject(eRenderGroup, pRenderObject);
}

void CGameInstance::Set_Event_Properties(CTexture* _pTexture, _float fAccEventTimeDelta)
{
	m_pRenderer->Set_Event_Properties(_pTexture, fAccEventTimeDelta);
}

HRESULT CGameInstance::Add_DebugComponent(CComponent* pDebugCom)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->Add_DebugComponent(pDebugCom);
}

HRESULT CGameInstance::Open_Level(_int iLevelIndex, CLevel * pNewLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Open_Level(iLevelIndex, pNewLevel);
}

_int CGameInstance::Get_Current_Level()
{
	return m_pLevel_Manager->Get_Current_Level();
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _wstring & strPrototypeTag, CBase * pPrototype)
{
	if (nullptr == m_pPrototype_Manager)
		return E_FAIL;

	return m_pPrototype_Manager->Add_Prototype(iLevelIndex, strPrototypeTag, pPrototype);
}

CBase * CGameInstance::Clone_Prototype(Engine::PROTOTYPE eType, _uint iLevelIndex, const _wstring & strPrototypeTag, void * pArg)
{
	if (nullptr == m_pPrototype_Manager)
		return nullptr;

	return m_pPrototype_Manager->Clone_Prototype(eType, iLevelIndex, strPrototypeTag, pArg);
}

HRESULT CGameInstance::Add_GameObject_ToLayer(_uint iPrototypeLevelIndex, const _wstring & strPrototypeTag, _uint iLevelIndex, const _wstring & strLayerTag, void * pArg,CGameObject** ppout)
{
	
	if (nullptr == m_pObject_Manager)
		return E_FAIL;


	return m_pObject_Manager->Add_GameObject_ToLayer(iPrototypeLevelIndex, strPrototypeTag, iLevelIndex, strLayerTag, pArg, ppout);
}

HRESULT CGameInstance::Remove_Object(_uint iLevelID, const _wstring& strLayerTag, CGameObject* pRemoveObject)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;
	m_pObject_Manager->Remove_Object(iLevelID, strLayerTag, pRemoveObject);
}

CComponent* CGameInstance::Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iObjectIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;
	
	return m_pObject_Manager->Get_Component(iLevelIndex, strLayerTag, strComponentTag, iObjectIndex);
}

CComponent* CGameInstance::Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, _uint iPartObjectIndex, const _wstring& strPartComponentTag, _uint iObjectIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Component(iLevelIndex, strLayerTag,iPartObjectIndex, strPartComponentTag, iObjectIndex);

}

CGameObject* CGameInstance::Get_GameObject(_uint iLevelIndex, const _wstring& strLayerTag, _uint iObjectIndex)
{
	return m_pObject_Manager->Get_GameObject(iLevelIndex,  strLayerTag, iObjectIndex);
}

CGameObject* CGameInstance::Get_GameObject(_uint iLevelIndex, const _wstring& strLayerTag, _uint iPartObjectIndex, _uint iObjectIndex)
{
	return  m_pObject_Manager->Get_GameObject( iLevelIndex, strLayerTag, iPartObjectIndex, iObjectIndex);
}

void CGameInstance::Set_Transform(CPipeLine::D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix)
{
	if (nullptr == m_pPipeLine)
		return;

	return m_pPipeLine->Set_Transform(eState, TransformMatrix);
}

_matrix CGameInstance::Get_TransformMatrix(CPipeLine::D3DTRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_TransformMatrix(eState);
}

_float4x4 CGameInstance::Get_TransformFloat4x4(CPipeLine::D3DTRANSFORMSTATE eState)
{

	if (nullptr == m_pPipeLine)
		return _float4x4();
	
	return m_pPipeLine->Get_TransformFloat4x4(eState);
}

_matrix CGameInstance::Get_TransformInversedMatrix(CPipeLine::D3DTRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_TransformInversedMatrix(eState);
}

_float4x4 CGameInstance::Get_TransformInversedFloat4x4(CPipeLine::D3DTRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return _float4x4();

	return m_pPipeLine->Get_TransformInversedFloat4x4(eState);
}

const _float4* CGameInstance::Get_CamPosition() const
{
	if(nullptr == m_pPipeLine)
		return nullptr;
	m_pPipeLine->Get_CamPosition();
}

HRESULT CGameInstance::Add_Light(const LIGHT_DESC& LightDesc)
{
	if (nullptr == m_pLightManager)
		return E_FAIL;

	return m_pLightManager->Add_Light(LightDesc);
	
}

const LIGHT_DESC* CGameInstance::Get_LightDesc(_uint iIndex) const
{
	if (nullptr == m_pLightManager)
		return nullptr;

	return m_pLightManager->Get_LightDesc(iIndex);
	
}

HRESULT CGameInstance::Render_Lights(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	return m_pLightManager->Render(pShader, pVIBuffer);
}

void CGameInstance::Set_LightsLevel(_uint iLevel)
{
	m_pLightManager->Set_CurLevel(iLevel);
}



HRESULT CGameInstance::Add_UI_To_UIMap(const _wstring UI_Tag, CUIObject* pUIObject, UI_TYPE eType)
{
	m_pUIManager->Add_UI_To_UIMap(UI_Tag, pUIObject, eType);
	return S_OK;
}

HRESULT CGameInstance::Set_Active_A_UI(const _wstring UI_Tag)
{
	m_pUIManager->Set_Active_A_UI(UI_Tag);
	return S_OK;
}

HRESULT CGameInstance::Set_InActive_A_UI(const _wstring UI_Tag)
{
	m_pUIManager->Set_InActive_A_UI(UI_Tag);
	return S_OK;
}

HRESULT CGameInstance::Clear_UIObjects()
{
	m_pUIManager->Clear_UIObjects();
	return S_OK;
}

CUIObject* CGameInstance::Get_UI_By_Tag(const _wstring UI_Tag)
{
	return m_pUIManager->Find_UIObject(UI_Tag);
	
}

CUIObject* CGameInstance::Get_Now_UI_DemageFont()
{
	return m_pUIManager->Get_Now_UI_DemageFont();
	
}

HRESULT CGameInstance::Add_DemageFonts(CUIObject* pDemageFont)
{
	return m_pUIManager->Add_DemageFonts(pDemageFont);
}

void CGameInstance::Reset_NumDemageFonts()
{
	m_pUIManager->Reset_NumDemageFonts();
}

void CGameInstance::PlayEffect(const TCHAR* pSoundKey, float fVolume)
{
	m_pSound_Manager->PlayEffect(pSoundKey, fVolume);
}

void CGameInstance::PlayBGM(const TCHAR* pSoundKey, float fVolume)
{
	m_pSound_Manager->PlayBGM(pSoundKey, fVolume);
}

void CGameInstance::PlayBGM(const TCHAR* pSoundKey, float fVolume, int iUpdate)
{
	m_pSound_Manager->PlayBGM(pSoundKey, fVolume, iUpdate);
}

void CGameInstance::Resume()
{
	m_pSound_Manager->Resume();
}

void CGameInstance::Pause()
{
	m_pSound_Manager->Pause();
}

void CGameInstance::StopBGM()
{
	m_pSound_Manager->StopBGM();
}

void CGameInstance::StopAll()
{
	m_pSound_Manager->StopAll();
}

void CGameInstance::SetBGMVolume(float fVolume)
{
	m_pSound_Manager->SetBGMVolume(fVolume);
}

void CGameInstance::SetBGMPitch(float fPitch)
{
	m_pSound_Manager->SetBGMPitch(fPitch);
}

void CGameInstance::UpdateBGM(const _float& fTimeDelta, const _float& fSpeed)
{
	m_pSound_Manager->UpdateBGM(fTimeDelta, fSpeed);
}

void CGameInstance::SetBGM(int iUpdate)
{
	m_pSound_Manager->SetBGM(iUpdate);
}

void CGameInstance::StopSound(_uint eID)
{
	m_pSound_Manager->StopSound(eID);
}

_uint CGameInstance::Get_NowEffectID()
{
	return m_pSound_Manager->Get_NowEffectID();
}

HRESULT CGameInstance::Add_Collision_Group(CCollider_Manager::COLLISION_GROUP eMyGroup, CGameObject* pGameObject, CCollider* pCollider)
{
	m_pColliderManager->Add_Collision_Group(eMyGroup,pGameObject, pCollider);
	return S_OK;
}

void CGameInstance::Clear_Collision_Group()
{
	m_pColliderManager->Clear_Collision_Group();
}

_bool CGameInstance::Check_Collision(CCollider* pCollider, CCollider_Manager::COLLISION_GROUP eTargetGroup, CGameObject** ppOutTargetObject)
{
	return m_pColliderManager->Check_Collision(pCollider,eTargetGroup, ppOutTargetObject);
}

HRESULT CGameInstance::Add_Font(const _wstring& strFontTag, const _tchar* pFontFilePath)
{
	return m_pFont_Manager->Add_Font(strFontTag, pFontFilePath);
}

HRESULT CGameInstance::Render_Font(const _wstring& strFontTag, const _tchar* pText, const _float2& vPosition, _fvector vColor, _float fRotation, const _float2& vOrigin)
{
	return m_pFont_Manager->Render_Font(strFontTag, pText, vPosition, vColor, fRotation, vOrigin);
}

HRESULT CGameInstance::Add_RenderTarget(const _wstring& strTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
	return m_pTarget_Manager->Add_RenderTarget(strTargetTag, iWidth, iHeight, ePixelFormat, vClearColor);
}

HRESULT CGameInstance::Add_MRT(const _wstring& strMRTTag, const _wstring& strTargetTag)
{
	return m_pTarget_Manager->Add_MRT(strMRTTag, strTargetTag);
}

HRESULT CGameInstance::Begin_MRT(const _wstring& strMRTTag, ID3D11DepthStencilView* pDSV, _bool isClear)
{
	return m_pTarget_Manager->Begin_MRT(strMRTTag,pDSV, isClear);
}

HRESULT CGameInstance::End_MRT()
{
	return m_pTarget_Manager->End_MRT();
}

HRESULT CGameInstance::Bind_RT_ShaderResource(CShader* pShader, const _char* pConstantName, const _wstring& strTargetTag)
{
	return m_pTarget_Manager->Bind_ShaderResource(pShader, pConstantName, strTargetTag);
}

const _float4x4* CGameInstance::Get_Shadow_Transform(CShadow::D3DTRANSFORMSTATE eState)
{
	return m_pShadow->Get_Shadow_Transform(eState);
}

void CGameInstance::SetUp_Shadow_TransformMatrix(CShadow::D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix)
{
	m_pShadow->SetUp_TransformMatrix(eState, TransformMatrix);
}

_bool CGameInstance::isIn_Frustum_InWorldSpace(_fvector vWorldPos, _float fRange)
{
	return m_pFrustum->isIn_InWorldSpace(vWorldPos, fRange);
}

HRESULT CGameInstance::Ready_RT_Debug(const _wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	return m_pTarget_Manager->Ready_Debug(strTargetTag, fX, fY, fSizeX, fSizeY);
}

HRESULT CGameInstance::Render_RT_Debug(const _wstring& strMRTTag, CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	return m_pTarget_Manager->Render_Debug(strMRTTag, pShader, pVIBuffer);
}


void CGameInstance::Release_Engine()
{
	CGameInstance::GetInstance()->Free();

	CGameInstance::GetInstance()->DestroyInstance();
}

void CGameInstance::Free()
{
	__super::Free();

	//Safe_Release(m_pInput_Device);
	Safe_Release(m_pFrustum);
	Safe_Release(m_pShadow);
	Safe_Release(m_pTarget_Manager);
	Safe_Release(m_pFont_Manager);
	Safe_Release(m_pLightManager);
	Safe_Release(m_pKeyManager);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pColliderManager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pUIManager);
 	Safe_Release(m_pPrototype_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pSound_Manager);
}
