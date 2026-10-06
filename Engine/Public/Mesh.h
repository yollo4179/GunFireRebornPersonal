#pragma once
#include"VIBuffer.h"
#include"AI_Info.h"/* for binary*/
#include"Model.h"
BEGIN(Engine)
class CMesh final :public CVIBuffer
{
private: 
	explicit CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CMesh() = default;

public:
	_uint Get_MaterialIndex()const {
		return m_iMaterialIndex;
	}
public: 
	/*(for binary NonanimMesh)*/
	virtual HRESULT Initialize_Prototype(CModel::TYPE eModelType, class CModel* pModel, const AI_MeshInfo* pAIMesh, _fmatrix PreTransformMatrix);

	virtual HRESULT Initialize_Prototype(CModel::TYPE eModelType, class CModel* pModel, const aiMesh* pAIMesh,_fmatrix PreTransformMatrix);
	virtual HRESULT Initialize(void* pArg);

public:
	HRESULT Bind_BoneMatrices(class CShader* pShader, const _char* pConstantName, const vector<CBone*>& Bones);
private: 
	_char			m_szName[MAX_PATH] = "";//메시의 이름
	_uint			m_iMaterialIndex = {};
	_uint			m_iNumBones = {};// 하나의 메시는 여러개의 뼈를 가지고있다. 
	
	/*이 모델을 구성하는 전체 뼈의 몇 번째 뼈들이 이 메시에 영행을 주는 거야 라는 정보를 저장한다. */
	vector<_uint>	m_BoneIndices;//메시에 영향을 주는 본의 (모델에서 부여한)인덱스를 벡터로 저장 ->//인덱스0,1,2,3,4,...->리얼 본 인덱스 매핑 -> 부모뼈 0  자식뼈 2 3 4......(재귀순서로 인덱스가 먹여져 있음 )(순서대로 하면 ) 
	/*각 본에 매핑되는 조정행렬을 ai에서 얻어서 미리 곱해야 한다)*/
	vector<_float4x4> m_BoneOffsetMatrices; 
	//메시는 본의 매트릭스를 배열로 가지고 있다가 셰이더에 던져줄 예정 

	_float4x4		m_BoneMatrices[512]; 
	
private:
	/*For Making Triangle In Maptool-> to assemble Vertices to Triangle-Navi_mesh*/
	vector<_float3>m_vecVertices;

public:
	HRESULT Ready_VertexBuffer_For_NonAnim(const aiMesh* pAIMesh, _fmatrix PreTransformMatrix);//이제 메시는 애님이 있는거랑 없는거랑 구분한다.
	HRESULT Ready_VertexBuffer_For_Anim(const aiMesh* pAIMesh, class CModel* pModel);

	/*FOR BINARY */
	HRESULT Ready_VertexBuffer_For_NonAnim(const  AI_MeshInfo* pAIMesh, _fmatrix PreTransformMatrix);//이제 메시는 애님이 있는거랑 없는거랑 구분한다.
	HRESULT Ready_VertexBuffer_For_Anim(const  AI_MeshInfo* pAIMesh, class CModel* pModel);

public:
	/*뼈 벡터가 아니라 모델을 다가져와서 뼈를 참조 */
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eModelType,class CModel* pModel, const aiMesh* pAIMesh,_fmatrix PreTransformMatrix);
	/*for binary*/
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eModelType, class CModel* pModel, const AI_MeshInfo* pAIMesh, _fmatrix PreTransformMatrix);
	
	virtual CComponent* Clone(void* pArg)override;
	virtual void Free() override;
};
END

