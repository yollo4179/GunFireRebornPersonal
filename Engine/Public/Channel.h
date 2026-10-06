#pragma once
#include"Base.h"
#include"AI_Info.h"
BEGIN(Engine)
class CChannel final : public CBase
{


private:
	CChannel();
	virtual ~CChannel() = default;

public: 
	HRESULT Initialize(const aiNodeAnim* pAIChannel, const class CModel* pModel);
	/*FOR BINARY*/
	HRESULT Initialize(const AI_Channel* pAIChannel, const class CModel* pModel);
	void Update_TransformationMatrix(_float fCurrentTrackPosition,_uint* pKeyFrameIndex, const vector<class CBone*>& Bones);
	
	/* For Joining With NextAnimation*/
	
	//애니메이션 바뀔때 이어질 키프레임을 애니메이션에서 채널별로 관리한다 .map( 채널의 본인덱스, 키프레임)-> 이어질 키프레임을 애니메이션에 던지자(초기화시)  
	void Get_KeyFrameState_From_Channel(KEYFRAME& KeyFrame, _uint& BoneIndexOfThisChannel, _uint KeyFrameIndexForNextAnim = 0); 
	void Set_Start_Channel_KeyFrame(_float fCurrentTrackPosition, _uint* pKeyFrameIndex, const vector<class CBone*>& Bones,KEYFRAME& StartKEyFrame,_uint& BoneIndex);
	_uint Get_BoneIndex() { return m_iBoneIndex; };

private:
	_char					m_szName[MAX_PATH] = {};// 뼈의 이름과 동기화되어있다. 
	_uint					m_iNumKeyFrames = {};
	vector<KEYFRAME>		m_KeyFrames;
	_uint					m_iBoneIndex = {}; //내가 뼈의 이름은 아니까 인덱스를 이름으로 가져와서 뼈에게 접근 해 현재 이뼈의 상태 행렬을( 보간하여 전달해준다.)
	


public:
	static CChannel* Create(const aiNodeAnim* pAIChannel, const class CModel* pModel);
	static CChannel* Create(const AI_Channel* pAIChannel, const class CModel* pModel);
	virtual void Free() override;
};
END
