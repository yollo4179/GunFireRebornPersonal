#include "Animation.h"
#include"Channel.h"
#include"Bone.h"

CAnimation::CAnimation()
{
}

CAnimation::CAnimation(const CAnimation& Prototype)
	: m_iNumChannels{ Prototype.m_iNumChannels }
	, m_Channels{ Prototype.m_Channels }
	, m_fDuration{ Prototype.m_fDuration }
	, m_fTickPerSecond{ Prototype.m_fTickPerSecond }
	, m_fCurrentTrackPosition{ Prototype.m_fCurrentTrackPosition }
	, m_CurrentKeyFrameIndices{ Prototype.m_CurrentKeyFrameIndices }
	//채널의 키프레임 인덱스는 애님이 관리한다. 
{
	//채널은 주소 공유(뼈(채널)를 객체마다 다가진다고? 안돼 너무많아
	for (auto& pChannel : m_Channels)
		Safe_AddRef(pChannel);

	/*애니메이션은 채널정보를 공유한다.->근데 키프레임은 애니메이션으로 빼자(->채널 제어정보 Track Position, FrameIndices, TickPerSecond)*/
	for (auto Channel : m_Channels)
	{
		_uint iBoneIndex = { 0 };
		KEYFRAME eKeyFrame = {};
		Channel->Get_KeyFrameState_From_Channel(eKeyFrame, iBoneIndex);
		m_mapKeyFramesOfChannels.insert({ iBoneIndex,eKeyFrame });
	}

	strcpy_s(m_szName, Prototype.m_szName);
}
HRESULT CAnimation::Initialize(const aiAnimation* pAIAnimation, const CModel* pModel)
{
	strcpy_s(m_szName, pAIAnimation->mName.data);  //애니메이션의 이름을 저장한다 idle attack....

	//이 애니메이션의 전체 플레이 구간 ( 시간) 을 가져온다 
	m_fDuration = pAIAnimation->mDuration;
	/* 애니메이션 재생 속도를 가져온다*/
	m_fTickPerSecond = pAIAnimation->mTicksPerSecond;

	/* 이 애니메이션(대기, 공격 etc)이 컨트롤해야하는 뼈(Channel)들의 정보 */
	m_iNumChannels = pAIAnimation->mNumChannels;

	m_CurrentKeyFrameIndices.resize(m_iNumChannels);
	
	/* 뼈들의 개수를 가져왔으니 애님이 컨트롤하는 뼈들을 생성하자*/
	for (size_t i = 0; i < m_iNumChannels; i++)
	{
		/* ai의 뼈정보를 실객체 channel에 넘겨서 생성하자*/
		CChannel* pChannel = CChannel::Create(pAIAnimation->mChannels[i], pModel);
		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
	}
	 
	for (auto Channel : m_Channels)
	{
		_uint iBoneIndex = {0};
		KEYFRAME eKeyFrame = {};
		Channel->Get_KeyFrameState_From_Channel(eKeyFrame, iBoneIndex);
		m_mapKeyFramesOfChannels.insert({iBoneIndex,eKeyFrame});
	}

	return S_OK;
}

HRESULT CAnimation::Initialize(const AI_Animation* pAIAnimation, const CModel* pModel)
{
	strcpy_s(m_szName, pAIAnimation->m_szName);
	m_fDuration = pAIAnimation->m_fDuration;
	m_fTickPerSecond = pAIAnimation->m_fTickPerSecond;
	m_iNumChannels = pAIAnimation->m_iNumChannels;
	m_CurrentKeyFrameIndices.resize(m_iNumChannels);

	for (int i = 0; i < m_iNumChannels; ++i)
	{
		CChannel* pChannel = CChannel::Create(&pAIAnimation->m_vecChannels[i], pModel);
		if (nullptr == pChannel)
			return E_FAIL;
		m_Channels.push_back(pChannel);
	}

	return S_OK;
}

_bool CAnimation::Update_TransformationMatrices(const vector<class CBone*>& Bones,_bool isLoop, _float fTimeDelta)
{
	/* 애니매이션 전체 플레이구간중 현재 지점*///->플래이 속도랑 ftimedelta를 곱한 값을 누적헤서 더함
	//-> 현재 지점이 구해짐
	m_fCurrentTrackPosition += m_fTickPerSecond * fTimeDelta *m_fTickRatio;

	if (true == isLoop && m_fCurrentTrackPosition >= m_fDuration)
	{
		m_fCurrentTrackPosition = 0.f;
		//루프라면 내거리를 (트래킹 타임) 을 다시 0으로 되돌린다 
		//근데 그러면 그 본에 해당하는 채널의 키프레임 인덱스도 0으로 만들어줘야되는데....
	}
	else if (m_fCurrentTrackPosition >= m_fDuration) //루프가 아니네 ?!
		return true;  //애님 끝났다 



	for (size_t i = 0; i < m_iNumChannels; i++)
	{
		if (i == 50)
			int a = 0;
		m_Channels[i]->Update_TransformationMatrix(m_fCurrentTrackPosition, &m_CurrentKeyFrameIndices[i],Bones);
		//-> 키프레임 값이 바껴야 되니까 포인터로 키프레임 인덱스값을 바꿔준다. 
	}

	return false; //애님 안끝났다 ...
}

bool CAnimation::Get_FrameInfo(KEYFRAME& eFrame, _uint ChanennelIndexYouWant)  
{
	//겹치는 키프레임만 바꿀꺼야 ㅇㅋ?
	auto iter = m_mapKeyFramesOfChannels.find(ChanennelIndexYouWant);  

	if (m_mapKeyFramesOfChannels.end() == iter)
		return false;
	
	/* 이 채널의 next keyframe 찾았으니까 반환 -> transform 할때 선형 보간의 nextkeyframe으로 쓰겠다 */
	eFrame = (*iter).second;
	return true;            
}

void CAnimation::Store_CurrentAnim_KeyFrame_State(const vector<class CBone*>& Bones ,_bool IsOnTrackPosition)
{
	if (IsOnTrackPosition)
	{
		if (m_mapStartKeyFrames.empty())
		{
			MSG_BOX("Store CurrentAnimState Before Changing Motion");
			return;
		}
		for (auto& FramePair : m_mapStartKeyFrames)
		{
			KEYFRAME keyFrame = {};
			if (m_mapLastKeyFrames.end() != m_mapLastKeyFrames.find(FramePair.first)) {

				_uint iBoneIndex = FramePair.first;
				Set_KeyFrames_OnTrackPos(Bones, m_mapStartKeyFrames[iBoneIndex], m_mapLastKeyFrames[iBoneIndex], keyFrame);
				m_mapStartKeyFrames[FramePair.first] = keyFrame;
			}
		}	
	}
	else
	{
		//오염방지 
		m_mapStartKeyFrames.clear();

		m_KeyFramePosOfStartAnim = m_fCurrentTrackPosition;

		for (size_t i = 0; i < m_iNumChannels; i++)
		{
			KEYFRAME keyFrame = {};
			_uint BoneIndex = { 0 };
			m_Channels[i]->Set_Start_Channel_KeyFrame(m_fCurrentTrackPosition, &m_CurrentKeyFrameIndices[i], Bones, keyFrame, BoneIndex);
			m_mapStartKeyFrames.emplace(BoneIndex, keyFrame);
		}
	}
}

void CAnimation::Store_AnimToChange_KeyFrame_State(const vector<class CBone*>& Bones, CAnimation* NextAnim)
{

	if (m_mapStartKeyFrames.empty())
	{
		MSG_BOX("Store CurrentAnimState Before Changing Motion");
		return;
	}
		
	m_mapLastKeyFrames.clear();
	m_KeyFramePosOfNextAnim = m_fDistance+m_KeyFramePosOfStartAnim;

	for (auto &CurKeyFramePair: m_mapStartKeyFrames)
	{
		KEYFRAME KeyFrame = {};
		_uint BoneIndex = { 0 };
		if(NextAnim->Get_FrameInfo(KeyFrame,CurKeyFramePair.first)) //  다음 애니메이션과 겹치는 부위일때만 추가하자.
			m_mapLastKeyFrames.emplace(CurKeyFramePair.first,KeyFrame);
		
	}
	int a = 0;
}

_bool CAnimation::Update_TransformationMatrices_ForMotionChange(const vector<class CBone*>& Bones, _bool isLoop, _float fTimeDelta)
{
	m_szName;

	m_fCurrentTrackPosition += m_fTickPerSecond * fTimeDelta* m_fTickRatio;
	
	_bool IsFinished = true;
	_bool bReturn = true;
	for (auto&LastKeyFramePair : m_mapLastKeyFrames)
	{
		_uint iBoneKey = LastKeyFramePair.first;
		IsFinished = Update_TransformMatrix_ForMotionChange(Bones, iBoneKey, m_mapStartKeyFrames[iBoneKey], LastKeyFramePair.second);
		if (false == IsFinished) 
			bReturn = false;
	}
	if (bReturn == true)//애니메이션끝
	{
		m_KeyFramePosOfStartAnim = 0;
		m_KeyFramePosOfNextAnim = 0;
		m_mapStartKeyFrames.clear();
		m_mapLastKeyFrames.clear();
		m_fCurrentTrackPosition = 0.f;
		Reset_CurrentKeyFrameIndices();
	}


	return bReturn;
}

_bool CAnimation::Update_TransformMatrix_ForMotionChange(const vector<class CBone*>& Bones, _uint iBoneIndex,KEYFRAME Start, KEYFRAME Last)
{
	//if (0.f == fCurrentTrackPosition) //이거 애니메이션에서 채널 돌릴때, loop 다돌았으면 0으로 만들거다. 인덱스도 0으로 만들자
	//	*pKeyFrameIndex = 0;

	
		//시간이 지나거나, 프레임드랍으로 렉이 심하면 바로 다음 애니메이션으로 넘어감
		if ((m_fCurrentTrackPosition - m_KeyFramePosOfStartAnim) > m_fDistance)
			return true; //끝나면 true 
		
		_float fRatio = (m_fCurrentTrackPosition - m_KeyFramePosOfStartAnim) / m_fDistance;
		
		// ㅣ-----------ㅣ
		// ㅣ----ㅣ 
		//-> 이 비율을 구하는 과정이다 

		_float3 vScale;
		_float3 vPosition;
		_float4 vRotation;

		_float3			vLeftScale, vRightScale;
		_float4			vLeftRotation, vRightRotation;
		_float3			vLeftPosition, vRightPosition;

		vLeftScale = Start.vScale;
		vLeftRotation = Start.vRotation;
		vLeftPosition = Start.vPosition;

		vRightScale = Last.vScale;
		vRightRotation = Last.vRotation;
		vRightPosition = Last.vPosition;

		//비율이랑 두개의(지점의) 상태 벡터 넣으면 보간시켜주는   함수 
		XMStoreFloat3(&vScale, XMVectorLerp(XMLoadFloat3(&vLeftScale), XMLoadFloat3(&vRightScale), fRatio));
		XMStoreFloat4(&vRotation, XMQuaternionSlerp(XMLoadFloat4(&vLeftRotation), XMLoadFloat4(&vRightRotation), fRatio));
		XMStoreFloat3(&vPosition, XMVectorLerp(XMLoadFloat3(&vLeftPosition), XMLoadFloat3(&vRightPosition), fRatio));

		Bones[iBoneIndex]->Set_TransformationMatrix(XMMatrixAffineTransformation(XMLoadFloat3(&vScale), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMLoadFloat4(&vRotation), XMVectorSetW(XMLoadFloat3(&vPosition), 1.f)));
		return false;
}

void CAnimation::Reset_CurrentKeyFrameIndices()
{
	m_fCurrentTrackPosition = 0.f;
	for(auto& index: m_CurrentKeyFrameIndices)
		index = 0;
}

_bool CAnimation::Set_KeyFrames_OnTrackPos(const vector<class CBone*>& Bones, KEYFRAME Start, KEYFRAME Last, KEYFRAME& Result)
{
	//마지막 키프레임을 지나가면 마지막 키프레임 값을 유지한다. 
	if ((m_fCurrentTrackPosition - m_KeyFramePosOfStartAnim) > m_fDistance) //현재 위치 지점이 마지막 키프레임의 위치(지점)을지나갔다? 마지막 키프레임으로 세팅
	{
		Result.vScale = Last.vScale;
		Result.vRotation = Last.vRotation;
		Result.vPosition = Last.vPosition;
	}

	else
	{


		_float			fRatio = (m_fCurrentTrackPosition - m_KeyFramePosOfStartAnim) / m_fDistance;
		// ㅣ-----------ㅣ
		// ㅣ----ㅣ 
		//-> 이 비율을 구하는 과정이다 

		_float3			vLeftScale, vRightScale;
		_float4			vLeftRotation, vRightRotation;
		_float3			vLeftPosition, vRightPosition;

		vLeftScale = Start.vScale;
		vLeftRotation = Start.vRotation;
		vLeftPosition = Start.vPosition;

		vRightScale = Last.vScale;
		vRightRotation = Last.vRotation;
		vRightPosition = Last.vPosition;


		XMStoreFloat3(&Result.vScale, XMVectorLerp(XMLoadFloat3(&vLeftScale), XMLoadFloat3(&vRightScale), fRatio));
		XMStoreFloat4(&Result.vRotation, XMQuaternionSlerp(XMLoadFloat4(&vLeftRotation), XMLoadFloat4(&vRightRotation), fRatio));
		XMStoreFloat3(&Result.vPosition, XMVectorLerp(XMLoadFloat3(&vLeftPosition), XMLoadFloat3(&vRightPosition), fRatio));
	}

	return true;
}

//모델에 이름으로 인덱스 가져오는 함수가있음 -> 이걸로 뼈를 변환할 찾자
CAnimation* CAnimation::Create(const aiAnimation* pAIAnimation,  const  CModel* pModel)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(pAIAnimation, pModel)))
	{
		MSG_BOX("Failed to Created : CAnimation");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CAnimation* CAnimation::Create(const AI_Animation* pAIAnimation, const CModel* pModel)
{
	CAnimation* pInstance = new CAnimation();
	if (FAILED(pInstance->Initialize(pAIAnimation, pModel)))
	{
		MSG_BOX("Failed ToCreate: CAnimation bin");
		Safe_Release(pInstance);
	}
	
	return pInstance;
}

CAnimation* CAnimation::Clone()
{
	return new CAnimation(*this);
}

void CAnimation::Free()
{
	__super::Free();

	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

	m_Channels.clear();
}
