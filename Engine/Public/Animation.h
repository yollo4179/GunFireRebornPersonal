#pragma once
#include"Base.h"
#include"AI_Info.h"
/* 애니메이션 클래스는 하나의 애니메이션을 가진다 */
/* 애니멩션을 하나 돌리는데 필요한 모든뼈들의 정보도 애니메이션이 가진다*/
/* 이뼈는 채널이라고 불린다. (채널은 뼈이다) */
/* 각 뼈에대한 변환은 채널에서 해결한다*/
/* 각 채널(뼈는)은 시점에따른 상태 행렬을 구햐여 본 객체에 행렬을 세팅 (현재 상태의 행렬을 )*/
/* 시점에 맞게 교체한다*/ 
/* 이 갱신은 채널에서 (자신에 이름에 매핑된 뼈를 (인덱스로)찾아와서  시점에 맞게 새로 세팅한다)*/
BEGIN(Engine)
class CAnimation final : public CBase
{
private:
	CAnimation();
	CAnimation(const CAnimation& Prototype);
	virtual ~CAnimation() = default;


public:
	HRESULT Initialize(const aiAnimation* pAIAnimation, const class CModel* pModel);
	/* for binary*/
	HRESULT Initialize(const AI_Animation* pAIAnimation, const class CModel* pModel);
	_bool Update_TransformationMatrices(const vector<class CBone*>& Bones, _bool isLoop, _float fTimeDelta);


public:
	/*For Anim Change*/
	bool Get_FrameInfo(KEYFRAME& eFrame, _uint ChanennelIndexYouWant); //찾았으면 정보와 함께 true, 아니면 false 반환 

	void Store_CurrentAnim_KeyFrame_State(const vector<class CBone*>& Bones ,_bool SlerpWhileSlerp);
	void Store_AnimToChange_KeyFrame_State(const vector<class CBone*>& Bones, CAnimation* NextAnim);
	_bool Update_TransformationMatrices_ForMotionChange(const vector<class CBone*>& Bones, _bool isLoop = false, _float fTimeDelta = 0.f);
	_bool Update_TransformMatrix_ForMotionChange(const vector<class CBone*>& Bones, _uint iBoneIndex, KEYFRAME Start, KEYFRAME Last);
	void SetUp_DistanceAnimToAnim(_float fDistance = 0.2) { m_fDistance = fDistance; }
	void Reset_CurrentKeyFrameIndices();
	/*while Slerping*/
	_bool Set_KeyFrames_OnTrackPos(const vector<class CBone*>& Bones, KEYFRAME Start, KEYFRAME Last, KEYFRAME& Result);
	void Set_TickRatio(_float fSpeed) { m_fTickRatio = fSpeed; }
private:
	_char		m_szName[MAX_PATH] = {};
	_uint		m_iNumChannels = {};
	vector<class CChannel*> m_Channels;//애니메이션은 채널을 가진다 ( 채널은 애니메이션에 영향을 미치는 뼈이다 - 그 뼈의 행렬만 변화시킨다.

	_float		m_fTickRatio = { 1.f };
	_float		m_fDuration = {};
	_float m_fTickPerSecond = {};
	_float m_fCurrentTrackPosition = {};
	vector<_uint> m_CurrentKeyFrameIndices;//d이제 채널의 키프레임은 이 애니메이션이 관리한다.
	

	/* for Motion Change*/
	/* 1. Other animation will use this info  first Key frame of Channels this Anim has */
	map< _uint, KEYFRAME>m_mapKeyFramesOfChannels;
	/* 2. we need these for changing to other Anim*/
	map<_uint,KEYFRAME>  m_mapStartKeyFrames ; // 현재 애니메이션의 상태를 <채널 인덱스,키프레임>으로 저장  
	map<_uint,KEYFRAME>  m_mapLastKeyFrames ;
	_float m_KeyFramePosOfStartAnim = { 0 };
	_float m_KeyFramePosOfNextAnim = { 0 };
	_float m_fDistance = { 0.4f };

public:
	static CAnimation* Create( const  aiAnimation* pAIAnimation,const  class CModel* pModel);
	/*for binary*/
	static CAnimation* Create(const AI_Animation* pAIAnimation, const class CModel* pModel);
	virtual CAnimation* Clone();
	virtual void Free() override;
};
END

