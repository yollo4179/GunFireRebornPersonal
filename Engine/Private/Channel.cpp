#include "Channel.h"
#include"Model.h"
#include"Bone.h"
#include"Animation.h"
CChannel::CChannel()
{
}

HRESULT CChannel::Initialize(const aiNodeAnim* pAIChannel, const CModel* pModel)
{
	//채널의 이름 ( 뼈의 이름과 같다 -> 이걸로 인덱스 검색해서 실제 뼈를 찾자 ) 
	strcpy_s(m_szName, pAIChannel->mNodeName.data); //bin
    
	m_iBoneIndex = pModel->Get_BoneIndex(m_szName);//이름으로 인덱스 받아오는것은 왠만하면	1 const 로 하자

	//애니메이션의 duration 중에 지점을 설정하여 지점마다 상태를 세팅해놓음(이미 만든 사람이 해놓음)
	// 하나의 채널(뼈)에는
	// 각 뼈의 움직임의 상태를 지점으로 나누어 프레임 키로 표현해 제공한다.-> 
	// 우리는 여기서 이 지점 사이의 상태를 각각 선형보간하여(지점사이에 빈틈이없도록)
	// 각 지점에서의 상태 행렬을 만들어서 본에 시점에 맞는 상태행렬을 갱신해야된다
	//waypoint 사이의 상태를 이전 키프레임(지점) 과 다음 프레임(지점) 사이의 거리를 1로 
	//뒀을때 나는 어디 지점에 와있는지에 대한 비율을  현재 키프레임에 곱하여 
	//현재 상태를 나타낸다.

	//각 프레임 키별로 srt를 벡터로 제공하고 -> 이거 띄엄띄엄 있으므로 사이를 보간해서 자연스럽게 움직이도록 만들어야한다.
	
//srt를 조합해야되는데 srt 개수를 다르게 제공한다? 규칙이있음.

	m_iNumKeyFrames = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
	m_iNumKeyFrames = max(m_iNumKeyFrames, pAIChannel->mNumPositionKeys);
		
	
	_float3 vScale, vPosition;
	_float4 vRotation; //회전은 쿼터니언이여서 플롯4개로 준다 wxyz 순서로 저장하므로 통짜로 저장하지말고 나눠서 저장해야됨

	/* 채널은 키프레임마다의 상태를 가지고 있다-> 본메트릭스를 변환할 행렬을 가지고 있다 */
	/* 게임 오브젝트의 play 애니메에션으로 업데이트 할때마다 애니메이션의 Trackposition을 누적하고-> Trackposition위치에 따라  */
	/* 채널의 키프레임 위치에 맞는 상태를 행렬로 만들어 채널과 연결된 모델(사본의 클론 뼈들)의 뼈 행렬을 업데이트 한다 (부모뼈 기준 자신만의 뼈행렬)*/
	/* 채널의 기능은 애니메이션에서 키프레임 인덱스를 던지면 -> 애니메이 션의 다음 키프레임(뼈상태)와 선형보간하여 채널이 담당하는 본에 행렬을 던진다.*/
 	/* 결론: 채널은 트랙 포지션만 넘기면 뼈행렬을 만들어주는 뼈 행렬 팩토리이다*/
	/* 채널이 가지는 원소: 키프레임 개수(마지막 인덱스+1), 키프레임 마다의 크자이 행렬(백터로 분리for 선형보간) */
	for (size_t i = 0; i < m_iNumKeyFrames; ++i)
	{
		KEYFRAME KeyFrame = {};
		
		if (i < pAIChannel->mNumScalingKeys)
		{
			memcpy(&vScale, &pAIChannel->mScalingKeys[i].mValue, sizeof(_float3));//키프레임(mTime)이 0이면 이전 상태랑 같은값이 저장되어있음
			KeyFrame.fKeyFramePosition = pAIChannel->mScalingKeys[i].mTime;  //키프레임의 위치:0아니면 다른 프레임 키포지션이랑 같다(0이라면 이전상태 상속)
		}

		if (i < pAIChannel->mNumRotationKeys)//마찬가지
		{
			vRotation.x = pAIChannel->mRotationKeys[i].mValue.x;
			vRotation.y = pAIChannel->mRotationKeys[i].mValue.y;
			vRotation.z = pAIChannel->mRotationKeys[i].mValue.z;
			vRotation.w = pAIChannel->mRotationKeys[i].mValue.w;
			KeyFrame.fKeyFramePosition = pAIChannel->mRotationKeys[i].mTime;//현재 지점 (위치)(키 프레임의 위치)
			//현재 회전 지점
		}
		if (i < pAIChannel->mNumPositionKeys)//마찬가지
		{
			memcpy(&vPosition, &pAIChannel->mPositionKeys[i].mValue, sizeof(_float3));
			KeyFrame.fKeyFramePosition = pAIChannel->mPositionKeys[i].mTime;
		}//
		//vRotation, vPosition vScale 이 전부 for문 밖에 전역으로 선언했으니까 if 문 통과 안하면 전의 값을 유지한다.
		// 위의 규칙을 만족할수 있다 .
		//현재 위치 지점 
		KeyFrame.vScale = vScale;
		KeyFrame.vRotation = vRotation;
		KeyFrame.vPosition = vPosition;

		m_KeyFrames.push_back(KeyFrame);
	}
	//-> 모든 키프레임들을 의도적으로 벡터에 잘 담았다 
	
	return S_OK;
}
HRESULT CChannel::Initialize(const AI_Channel* pAIChannel, const CModel* pModel)
{

	strcpy_s(m_szName, pAIChannel->m_szName);

	 m_iBoneIndex = pModel->Get_BoneIndex(pAIChannel->m_szName);

	m_iNumKeyFrames = max(pAIChannel->m_NumScalingKeys, pAIChannel->m_NumRotationKeys);
	m_iNumKeyFrames = max(m_iNumKeyFrames, pAIChannel->m_NumPositionKeys);
	
	_float3 vScale, vPosition;
	_float4 vRotation;

	for (size_t i = 0; i < m_iNumKeyFrames; ++i)
	{
		KEYFRAME KeyFrame = {};

		if (i < pAIChannel->m_NumScalingKeys)//i가 내 스케일링 전체 개수보다 크면 다 같은 상태로 저장
		{
			memcpy(&vScale, &pAIChannel->m_ScalingKeys[i].m_Value, sizeof(_float3));
			KeyFrame.fKeyFramePosition = pAIChannel->m_ScalingKeys[i].KeyFramePos;
		}

		if (i < pAIChannel->m_NumRotationKeys)
		{
			vRotation.x = pAIChannel->m_RotationKeys[i].m_Value.x;
			vRotation.y = pAIChannel->m_RotationKeys[i].m_Value.y;
			vRotation.z = pAIChannel->m_RotationKeys[i].m_Value.z;
			vRotation.w = pAIChannel->m_RotationKeys[i].m_Value.w;
			KeyFrame.fKeyFramePosition = pAIChannel->m_RotationKeys[i].KeyFramePos;//현재 지점 (위치)(키 프레임의 위치)
			//현재 회전 지점
		}
 
		if (i < pAIChannel->m_NumPositionKeys)//마찬가지
		{
			memcpy(&vPosition, &pAIChannel->m_PositionKeys[i].m_Value, sizeof(_float3));
			KeyFrame.fKeyFramePosition = pAIChannel->m_PositionKeys[i].KeyFramePos;
		}
		KeyFrame.vScale = vScale;
		KeyFrame.vRotation = vRotation;
		KeyFrame.vPosition = vPosition;

		m_KeyFrames.push_back(KeyFrame);
	}

	/*if (false == strcmp(m_szName, "Bip001"))
	{
		_float3 InitialPosition = m_KeyFrames[0].vPosition;
		for (size_t i = 0; i < m_KeyFrames.size(); ++i)
		{
			m_KeyFrames[i].vPosition = InitialPosition;
		}
		
	}*/
 
 
  
 	return S_OK;
}
//선형보간 해서 본의 자신의 상태 행렬 업데이트
void CChannel::Update_TransformationMatrix(_float fCurrentTrackPosition, _uint* pKeyFrameIndex,const vector<class CBone*>& Bones)
{
	
	if (0.f == fCurrentTrackPosition) //이거 애니메이션에서 채널 돌릴때, loop 다돌았으면 0으로 만들거다. 인덱스도 0으로 만들자
		*pKeyFrameIndex = 0;

	//fCurerntTrackTime은  현재 내 위치->지점과 지점사이에서 내가 위치한 지점의 비율을 구하여 보간한다. 
	KEYFRAME		LastKeyFrame = m_KeyFrames.back();

	_float3			vScale;
	_float4			vRotation;
	_float3			vPosition;


	//마지막 키프레임을 지나가면 마지막 키프레임 값을 유지한다. 
	if (fCurrentTrackPosition >= LastKeyFrame.fKeyFramePosition)  //현재 위치 지점이 마지막 키프레임의 위치(지점)을지나갔다? 마지막 키프레임으로 세팅
	{
		vScale = LastKeyFrame.vScale;
		vRotation = LastKeyFrame.vRotation;
		vPosition = LastKeyFrame.vPosition;
	}

	else
	{
		//while로 프레임이 떨어지면 fTimeDelta가 너무 커져 다음키프레임이 다다다다다음 키프레임으로 이동-> ratio가 커져 
		//프레임 드랍이 일어나서 트래킹 포지션이 현재 목적 프레임 이전으로 이동하도록 loop돌려 현재 프레임으로 이동 
		while (fCurrentTrackPosition >= m_KeyFrames[*pKeyFrameIndex + 1].fKeyFramePosition)
			++*pKeyFrameIndex;

		_float			fRatio = (fCurrentTrackPosition - m_KeyFrames[*pKeyFrameIndex].fKeyFramePosition) /
			(m_KeyFrames[*pKeyFrameIndex + 1].fKeyFramePosition - m_KeyFrames[*pKeyFrameIndex].fKeyFramePosition);
		// ㅣ-----------ㅣ
		// ㅣ----ㅣ 
		//-> 이 비율을 구하는 과정이다 

		_float3			vLeftScale, vRightScale;
		_float4			vLeftRotation, vRightRotation;
		_float3			vLeftPosition, vRightPosition;

		vLeftScale = m_KeyFrames[*pKeyFrameIndex].vScale;
		vLeftRotation = m_KeyFrames[*pKeyFrameIndex].vRotation;
		vLeftPosition = m_KeyFrames[*pKeyFrameIndex].vPosition;

		vRightScale = m_KeyFrames[ *pKeyFrameIndex + 1].vScale;
		vRightRotation = m_KeyFrames[*pKeyFrameIndex + 1].vRotation;
		vRightPosition = m_KeyFrames[*pKeyFrameIndex + 1].vPosition;

		//비율이랑 두개의(지점의) 상태 벡터 넣으면 보간시켜주는   함수 
		XMStoreFloat3(&vScale, XMVectorLerp(XMLoadFloat3(&vLeftScale), XMLoadFloat3(&vRightScale), fRatio));
		XMStoreFloat4(&vRotation, XMQuaternionSlerp(XMLoadFloat4(&vLeftRotation), XMLoadFloat4(&vRightRotation), fRatio));
		XMStoreFloat3(&vPosition, XMVectorLerp(XMLoadFloat3(&vLeftPosition), XMLoadFloat3(&vRightPosition), fRatio));
	}
	//해당하는 본의 상태 행렬을 다시 세팅 (보간 값으로 ) //크자이 넣으면 알아서 곱해줌(쿼터니언기준의 행렬 넣야됨 회전은)
	Bones[m_iBoneIndex]->Set_TransformationMatrix(XMMatrixAffineTransformation(XMLoadFloat3(&vScale), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMLoadFloat4(&vRotation), XMVectorSetW(XMLoadFloat3(&vPosition), 1.f)));
}


void CChannel::Get_KeyFrameState_From_Channel(KEYFRAME& KeyFrame,_uint& BoneIndexOfThisChannel, _uint KeyFrameIndexForNextAnim )
{
	KeyFrame = m_KeyFrames[KeyFrameIndexForNextAnim];
	BoneIndexOfThisChannel = m_iBoneIndex;
}
//현재 애니메이션의 중간 지점을 구해서 애니메이션에서 시작지점으로 저장한다. 
void CChannel::Set_Start_Channel_KeyFrame(_float fCurrentTrackPosition, _uint* pKeyFrameIndex, const vector<class CBone*>& Bones, KEYFRAME& StartKeyFrame,_uint& BoneIndex)
{
	if (0.f == fCurrentTrackPosition)
		*pKeyFrameIndex = 0;

	KEYFRAME		LastKeyFrame = m_KeyFrames.back();

	
	//마지막 키프레임을 지나가면 마지막 키프레임 값을 유지한다. 
	if (fCurrentTrackPosition >= LastKeyFrame.fKeyFramePosition)  //현재 위치 지점이 마지막 키프레임의 위치(지점)을지나갔다? 마지막 키프레임으로 세팅
	{
		StartKeyFrame.vScale = LastKeyFrame.vScale;
		StartKeyFrame.vRotation = LastKeyFrame.vRotation;
		StartKeyFrame.vPosition = LastKeyFrame.vPosition;
		BoneIndex = m_iBoneIndex;
	}

	else
	{
		//while로 프레임이 떨어지면 fTimeDelta가 너무 커져 다음키프레임이 다다다다다음 키프레임으로 이동-> ratio가 커져 
		//프레임 드랍이 일어나서 트래킹 포지션이 현재 목적 프레임 이전으로 이동하도록 loop돌려 현재 프레임으로 이동 
		while (fCurrentTrackPosition >= m_KeyFrames[*pKeyFrameIndex + 1].fKeyFramePosition)
			++*pKeyFrameIndex;

		_float			fRatio = (fCurrentTrackPosition - m_KeyFrames[*pKeyFrameIndex].fKeyFramePosition) /
			(m_KeyFrames[*pKeyFrameIndex + 1].fKeyFramePosition - m_KeyFrames[*pKeyFrameIndex].fKeyFramePosition);
		// ㅣ-----------ㅣ
		// ㅣ----ㅣ 
		//-> 이 비율을 구하는 과정이다 

		_float3			vLeftScale, vRightScale;
		_float4			vLeftRotation, vRightRotation;
		_float3			vLeftPosition, vRightPosition;

		vLeftScale = m_KeyFrames[*pKeyFrameIndex].vScale;
		vLeftRotation = m_KeyFrames[*pKeyFrameIndex].vRotation;
		vLeftPosition = m_KeyFrames[*pKeyFrameIndex].vPosition;

		vRightScale = m_KeyFrames[*pKeyFrameIndex + 1].vScale;
		vRightRotation = m_KeyFrames[*pKeyFrameIndex + 1].vRotation;
		vRightPosition = m_KeyFrames[*pKeyFrameIndex + 1].vPosition;


		XMStoreFloat3(&StartKeyFrame.vScale, XMVectorLerp(XMLoadFloat3(&vLeftScale), XMLoadFloat3(&vRightScale), fRatio));
		XMStoreFloat4(&StartKeyFrame.vRotation, XMQuaternionSlerp(XMLoadFloat4(&vLeftRotation), XMLoadFloat4(&vRightRotation), fRatio));
		XMStoreFloat3(&StartKeyFrame.vPosition, XMVectorLerp(XMLoadFloat3(&vLeftPosition), XMLoadFloat3(&vRightPosition), fRatio));
		BoneIndex = m_iBoneIndex;
	}
}


CChannel* CChannel::Create(const aiNodeAnim* pAIChannel, const CModel* pModel)
{
	CChannel* pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(pAIChannel, pModel)))
	{
		MSG_BOX("Failed to Created : CChannel");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CChannel* CChannel::Create(const AI_Channel* pAIChannel, const CModel* pModel)
{
	CChannel* pInstance = new CChannel();
	if (FAILED(pInstance->Initialize(pAIChannel, pModel)))
	{
		MSG_BOX("Failed to Created: CChanel");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CChannel::Free()
{
	
	__super::Free();
}
