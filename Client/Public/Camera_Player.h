#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_Player final : public CCamera
{
public:
	typedef struct : public CCamera::CAMERA_DESC
	{
		const _float4x4* pPlayerFloat4x4;
		_float			fMouseSensor = {};
	}CAMERA_PLAYER_DESC;
private:
	CCamera_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Player(const CCamera_Player& Prototype);
	virtual ~CCamera_Player() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	void		 Fix_Mouse();
	void		Set_Camera_Still();
	void		Set_Camera_Player();
	_bool		Get_Camera_Still();

	void Boss09_Camera(_float fTimeDelta);
	void Set_Boss09_Camera(class CMonster09* pTarget) { m_pMonster09 = pTarget;  m_bBoss09_CameraCutSceneNow = true; };
private:

private:
	_float			m_fMouseSensor = {};
	const _float4x4* m_pPlayerFloat4x4 = { nullptr };
	_bool m_bFix = { true };
	_bool m_bCamera_Still = { false };

	/*Boss CutScene*/
	_bool m_bBoss09_CameraCutSceneNow = { false };
	
	_float m_fAccBoss09CameraTime = { 0.f };
	_float m_fBoss09CameraTime = { 6.f };
	class CMonster09* m_pMonster09 = { nullptr };
	_float3 m_vCamPosForBoss09 = {0.f,30.f,-40.f};

public:
	static CCamera_Player* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};


END