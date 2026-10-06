#include "Bone.h"

CBone::CBone()
{
}

HRESULT CBone::Initialize(const aiNode* pAINode, _int iParentBoneIndex)
{
	strcpy_s(m_szName, pAINode->mName.data);// 뼈를 생성하면 나 (자신 )뼈의 이름을 가져온다 
	memcpy(&m_TransformationMatrix,&pAINode->mTransformation,sizeof(_float4x4));//aiNode로부터 자신의 행렬을 가져온다 -> 부모 기준에서의 자신만의 행렬
	
	
	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));
	//aiNode는 col MAJOR이므러 전치해서 ROW MAJOR로 바꿔야한다.
	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());//기본적으로 부모의 행렬을 곱할것이기때문에 일단 항등행렬로 채운다. 
	
	m_iParentBoneIndex = iParentBoneIndex;
	
	return S_OK;
}

HRESULT CBone::Initialize( AI_Node* pAINode, _int iParentBoneIndex)
{
	strcpy_s(m_szName, pAINode->m_szName);// 뼈를 생성하면 나 (자신 )뼈의 이름을 가져온다 
	memcpy(&m_TransformationMatrix, &pAINode->m_TransformationMatrix, sizeof(_float4x4));//aiNode로부터 자신의 행렬을 가져온다 -> 부모 기준에서의 자신만의 행렬
	//aiNode는 col MAJOR이므러 전치해서 ROW MAJOR로 바꿔야한다.

	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());//기본적으로 부모의 행렬을 곱할것이기때문에 일단 항등행렬로 채운다. 

	m_iParentBoneIndex = iParentBoneIndex;

	return S_OK;
}

void CBone::Update_CombinedTransformationMatrix(const vector<CBone*>& Bones, _fmatrix PreTransformMatrix)
{
	//내가 최상위 루트 노드라면 , 일단 본에 조정 행렬을 곱해 놓는다.
	if (-1 == m_iParentBoneIndex) {
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * PreTransformMatrix);
		return;
		// 부모 행렬일 때, (최상위 노드) -> 부모 행렬에 다른 행렬이 중복해서 곱해지며 안된다. 
		//한번 곱하고 끝내자, 심지어 0의 부모는 -1인덱스이므로 터진다. ( 아래 식에 의하면) 
	}
	//최종 변환 정보에 나만의 행렬에  부모의 뼈의 combined행렬을 곱한다 . -> 어차피 벡터 순서가 부모부터 업데이트 해주니까 부모부터 자식이  차례로 transformation 행렬이 곱해짐 -> 각각의 최종행렬완료 
	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) *
		XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_CombinedTransformationMatrix));
	
}



CBone* CBone::Create(const aiNode* pAINode, _int iParentBoneIndex)
{
	CBone* pInstance = new CBone();

	if (FAILED(pInstance->Initialize(pAINode, iParentBoneIndex)))
	{
		MSG_BOX("Failed to Created : CBone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CBone* CBone::Create( AI_Node* pAINode, _int iParentBoneIndex)
{
	CBone* pInstance = new CBone();

	if (FAILED(pInstance->Initialize(pAINode, iParentBoneIndex)))
	{
		MSG_BOX("Failed to Created : CBone bin");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CBone* CBone::Clone()
{
	return new CBone(*this);
}

void CBone::Free()
{
	
	__super::Free();
	
}
