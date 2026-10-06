#pragma once
#include"Base.h"
/* assimp에서는 뼈의 세가지 타입으로 제공한다.*/
/* aiNode aiBone, aiAniNode*/
#include"AI_Info.h"
BEGIN(Engine)
class CBone final :public CBase
{

private:
	CBone();
	virtual ~CBone() = default;


public:
	const _char* Get_Name() const {
		return m_szName;
	}
	_matrix Get_CombinedTransformationMatrix() const {
		return XMLoadFloat4x4(&m_CombinedTransformationMatrix);
	}
	/* (파트 오브젝트가 따라다닐 뼈의 최종 행렬을 가져다 준다 -> 이걸로 파트를 모델에 배치) * (컨테이너 월드 해서 -> 월드에 최종배치) */
	const _float4x4* Get_CombinedTransformationFloat4x4() const {
		return &m_CombinedTransformationMatrix;
	}

	//보간한 함수 // 채널(이 뼈와 연결된) 에서 플레이 시간에따라 보간해서 채울거임
	void Set_TransformationMatrix(_fmatrix TransformationMatrix)
	{
		XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
	}

public:
	HRESULT Initialize(const aiNode* pAINode, _int iParentBoneIndex);
	/*for Bin*/
	HRESULT Initialize( AI_Node* pAINode, _int iParentBoneIndex);
	void Update_CombinedTransformationMatrix(const vector<CBone*>& Bones, _fmatrix PreTransformMatrix);//매 시점마다 부모부터 나까지의 행렬을 곱해서 뼈의 행렬을 구해준다 . -> 부모의 인덱스로부터 탐색을 하겠지 .
private:
	_char		m_szName[MAX_PATH] = {};//본의 이름 이걸로 비교해서 검색할 것임
	
	/* 부모를 기준으로 했을때의 나만의 상태 행렬 */
	_float4x4	m_TransformationMatrix = {};

	/* m_Transformation* Parent's  m_CombinedTransformationMatrix */

	_float4x4 m_CombinedTransformationMatrix = {};

	_int	m_iParentBoneIndex = { -1 };
	 

public:
	static CBone* Create(const aiNode* pAINode, _int iParentBoneIndex);
	static CBone* Create( AI_Node* pAINode, _int iParentBoneIndex);
	virtual CBone* Clone();
	virtual void Free() override;
};
END

