#include "Mesh.h"
#include"Shader.h"
#include"Bone.h"
CMesh::CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CVIBuffer{pDevice, pContext}
{
}
/*aimash _ For Anim *///정점도 애니메이션 정점을 위한 정점 구조체를 따로 의 해야함 
HRESULT CMesh::Initialize_Prototype(CModel::TYPE eModelType, class CModel* pModel, const aiMesh* pAIMesh, _fmatrix PreTransformMatrix)
{

	m_iMaterialIndex = pAIMesh->mMaterialIndex;//해당 메시에 대을되는 머테리얼 인덱스
//    //메시 정보를 임포터로 로드하면 메시 정보들을 뽑아올 수있다 (하나의 모델은 여러개의 매시로 분리되어 존재한다)

	strcpy_s(m_szName, pAIMesh->mName.data);//메시에도 각각 이름이 존재하며 이는 중요한 정보이다.

	m_iNumVertexBuffers = 1;//버퍼는 아직 1개만씀
	m_iNumVertices = pAIMesh->mNumVertices; //메시 버텍스의 정보를 가져온다 
	m_iIndexStride = sizeof(_uint);//정점 개수가 좀 많을 수도 있으니까int 형으로 인덱스의 개수를 넉넉하게 잡는다.
	m_iNumIndices = pAIMesh->mNumFaces * 3;  //메시의 인덱스 개수 대신에 폴리곤(삼각형)의 개수를 제공한다. 거기에 3을 곱하면 인덱스의 개수와 일치하므로 이를 인덱스 개수로 삼는다.
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER	
	

	HRESULT hr =CModel::TYPE_NONANIM == eModelType ? Ready_VertexBuffer_For_NonAnim(pAIMesh, PreTransformMatrix) : Ready_VertexBuffer_For_Anim(pAIMesh, pModel);
	
	if (FAILED(hr))
		return E_FAIL;
	
#pragma endregion

#pragma region INDEX_BUFFER
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	/* 정점버퍼를 몇 바이트 할당할까요? */
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.StructureByteStride = /*m_iIndexStride*/0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	_uint* pIndices = new _uint[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);

	_uint		iNumIndices = { 0 };

	for (size_t i = 0; i < pAIMesh->mNumFaces; i++)
	{
		pIndices[iNumIndices++] = pAIMesh->mFaces[i].mIndices[0];
		pIndices[iNumIndices++] = pAIMesh->mFaces[i].mIndices[1];
		pIndices[iNumIndices++] = pAIMesh->mFaces[i].mIndices[2];
	}

	ZeroMemory(&m_SubResourceDesc, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceDesc.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion



	return S_OK;
}
/*for binary*/
HRESULT CMesh::Initialize_Prototype(CModel::TYPE eModelType, class CModel* pModel, const AI_MeshInfo* pAIMesh, _fmatrix PreTransformMatrix)
{

	m_iMaterialIndex = pAIMesh->m_MaterialIndex;//해당 메시에 대을되는 머테리얼 인덱스
	//    //메시 정보를 임포터로 로드하면 메시 정보들을 뽑아올 수있다 (하나의 모델은 여러개의 매시로 분리되어 존재한다)

	strcpy_s(m_szName, pAIMesh->m_Name);//메시에도 각각 이름이 존재하며 이는 중요한 정보이다.

	m_iNumVertexBuffers = 1;//버퍼는 아직 1개만씀

	
	m_iNumVertices = pAIMesh->m_NumVertices; //메시 버텍스의 정보를 가져온다 

	m_iIndexStride = sizeof(_uint);//정점 개수가 좀 많을 수도 있으니까int 형으로 인덱스의 개수를 넉넉하게 잡는다.
	m_iNumIndices = pAIMesh->m_NumFaces* 3;  //메시의 인덱스 개수 대신에 폴리곤(삼각형)의 개수를 제공한다. 거기에 3을 곱하면 인덱스의 개수와 일치하므로 이를 인덱스 개수로 삼는다.
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER	

	HRESULT hr = CModel::TYPE_NONANIM == eModelType ? Ready_VertexBuffer_For_NonAnim(pAIMesh, PreTransformMatrix) : Ready_VertexBuffer_For_Anim(pAIMesh, pModel);

	if (FAILED(hr))
		return E_FAIL;
#pragma endregion

	


#pragma region INDEX_BUFFER
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	/* 정점버퍼를 몇 바이트 할당할까요? */
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.StructureByteStride = /*m_iIndexStride*/0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	_uint* pIndices = new _uint[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);

	_uint		iNumIndices = { 0 };
	//하나의 폴리곤 정보는 그 폴리곤(삼각형)을 이루는 정점들의 인덱스로 구성되어있다. -> 추출해서 저장하자.
	for (size_t i = 0; i < pAIMesh->m_NumFaces; i++)
	{
		pIndices[iNumIndices++] = pAIMesh->m_vecIndices[i].m_index[0];//폴리곤.인덱스1-> 얘도 포인터인데 동적 배열(FACE)에 구조체안의 동적배열로(크기FLOAT3)에 각각의 인덱스가 들어있다. 
		pIndices[iNumIndices++] = pAIMesh->m_vecIndices[i].m_index[1];
		pIndices[iNumIndices++] = pAIMesh->m_vecIndices[i].m_index[2];

		if (CModel::TYPE_NONANIM == eModelType) {
			/*For Navi Mesh*/
			TRIANGLE_VERTICES Triangle = {};
			Triangle.vPointA = m_vecVertices[pAIMesh->m_vecIndices[i].m_index[0]];
			Triangle.vPointB = m_vecVertices[pAIMesh->m_vecIndices[i].m_index[1]];
			Triangle.vPointC = m_vecVertices[pAIMesh->m_vecIndices[i].m_index[2]];
			pModel->Add_Triangle(Triangle);
		}

	}

	ZeroMemory(&m_SubResourceDesc, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceDesc.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion
	return S_OK;
}

HRESULT CMesh::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CMesh::Bind_BoneMatrices(CShader* pShader, const _char* pConstantName, const vector<CBone*>& Bones)
{
	ZeroMemory(m_BoneMatrices, sizeof(_float4x4) * 512); // 렌더어 셰이더에 뼈행렬을 던지기 위해 매번 업데이트를 수행한다.
	_uint iNumBones = { 0 };

	//이 메시에 영향을 주는 뼈에서부터 최종 트랜스폼 매트릭스를 가져오기위해서 모델의 뼈 벡터 기준의 인덱스를 하나씩 순회한다.
	//이 메시에 영향을 주는 뼈들의 매트릭스를 가져와 갱신하는 과정( 뼈벡터 셰이더에 던지려고) 
	for (auto& iBoneIndex/*모델 본벡터 인덱스*/ : m_BoneIndices)
	{
		XMStoreFloat4x4(&m_BoneMatrices[iNumBones],
			XMLoadFloat4x4(&m_BoneOffsetMatrices[iNumBones]) * Bones[iBoneIndex]/*뼈객체 가져옴*/
				->Get_CombinedTransformationMatrix()
		);
		++iNumBones;
	}
	/* 메시 단위로 렌더링을 수행 -> 매트릭스도 매시단위로 가져야 함*/
	/* 셰이더도 정점 들을 계산 하는 구조지만 메시 단위로 렌더링하므로 메시단위로 생각*/
	/* 셰이더는 매시단위로 메시에 영향을 주는 뼈들의 최종 변환 매트릭스들을 다 가지고 있어야됨*/
	/*한 정점에 영향을 줄수 있는 매시는 최대 4개이다 -> 모든 뼈가 셰이더에 있다면? -> 인덱스만 알변 된다.*/
	
	/* 결론: 메시안에 메시에 영향을 주는 뼈정보로부터 첫 번째부더 마지막까지 순회하면서 
	뼈의 이름으로 해당 뼈와 매핑되는 모델 뼈벡터의 인덱스를 찾았다면, 
	-> 메시벡터의 i번째 원소값는 i번째 뼈(메시기준)의 (모델 벡터에 저장된 실제 뼈객체)의 모델 인덱스이고*/
	/*ㄱi번째 원소로 i번째 뼈를 모델(벡터)에서 가져와 메트릭스를 얻어낸다. */
	
	//메시에 영향을 주는 본매트릭스를 전부 셰이더에 저장한다, 메시단위로 렌더링하기 전에 
	//일단 셰이더에 메시에 영향을 미치는 모든 뼈의 매트릭스를 전달하고: 뼈 매트릭스[메시 인덱스] 로 매핑해서 
	//해당 정점에 영행을 주는 뼈의 [메시 인덱스로 가져온 본매트릭스]를 정점에 곱한다. 
	return pShader->Bind_Matrices(pConstantName,m_BoneMatrices,512);  
}



HRESULT CMesh::Ready_VertexBuffer_For_NonAnim(const aiMesh* pAIMesh, _fmatrix PreTransformMatrix)
{

	m_iVertexStride = sizeof(VTXMESH);

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	/* 정점버퍼를 몇 바이트 할당할까요? */
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.StructureByteStride = m_iVertexStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;



	VTXMESH* pVertices = new VTXMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMESH) * m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PreTransformMatrix));

		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), PreTransformMatrix));

		memcpy(&pVertices[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vTangent, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vTangent), PreTransformMatrix));

	}

	ZeroMemory(&m_SubResourceDesc, sizeof m_SubResourceDesc);
	m_SubResourceDesc.pSysMem = pVertices;


	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_For_Anim(const aiMesh* pAIMesh, CModel* pModel)
{
	m_iVertexStride = sizeof(VTXANIMMESH);

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.StructureByteStride = m_iVertexStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXANIMMESH* pVertices = new VTXANIMMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMESH) * m_iNumVertices);
	//다일단 0으로 초기화 vBlendIndex도 기본적으로 default로
	//{0,0,0,0}을 가진다.->  셰이더 코드에서 
	// bone이 없을 때 가중치가 모두 0이므로
	// w에서 가중치 1로주고 본 1개로 채워주는데 쓰임(애니메이션 엇으면 본 1개 제공 -> 메시이름 =bone이름 -> 이걸로 본을찾고 행렬 찾기)
	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVertices[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
	}
	//여기까진 같다 .
	//근데 어떻게 한 정점에 영행을 미치는 4개의 본을 구할수 있지? 
	//우리가 채워줘야할 것은 나 정점에 영향을 미치는 4개의  본의 인덱스와 , 영향을 미치는 정도 값이다 
	// 
	/* 이 메시에 영향을 미치는 뼈의 개수 */
	m_iNumBones = pAIMesh->mNumBones;

	/*ai Bone : 이 메시에 영향을 주는 뼈가 어떤 정점에게 얼마나 열향을 주는지에 대한 정보를 가짐*/

//-> 이 메시에 영향을 주는 (메시의)뼈를 다 순회하면서 -> 영향을 주는 4개의 뼈를 다 찾아야한다 . 

	for (size_t i = 0; i < m_iNumBones; ++i)
	{

		aiBone* pAIBone = pAIMesh->mBones[i]; //이 메시에 영향을 주는 뼈의 개수를 아니 그 뼈정보도 제공한다.
	
		_float4x4 OffsetMatrix = {};
		
		memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));
		//->ai에서 제공하는 보정함수를 곱하지않으면 ai_process->자동보정 효과가 나타나지않는다( 오체 분리됨)
		//-> 이 받아오는 정보는 COL MAJOR이므로 전치해서 받아와야 한다 .
		XMStoreFloat4x4(&OffsetMatrix, XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));
		
		
		m_BoneOffsetMatrices.push_back(OffsetMatrix);
		//->뼈의 개수만큼 offset 매트릭스들을 가진다. (각 인덱스 번째 뼈에 해당하는 조정 행렬을 저장한다.) 
		// i번 넣는다( i는 본 벡터의 인덱스)
		m_BoneIndices.push_back(pModel->Get_BoneIndex(pAIBone->mName.data));

		//이뼈가 영향을 주는 정점의 개수 (weights)
			for (size_t j = 0; j < pAIBone->mNumWeights; ++j)//해당하는 본의 가중치는 aiBone[나](정점에 영향을 주는)가 준다.
			{
				//pAIBone->mWeights[j]: 이뼈가 영향을 주는이 (메시의) j번째 정점의 정보*/
				//pAIBone->mWeights[j].mVertexId : 이뼈가 영향을 주는 j번쨰 정점의 인덱스  (인덱스 버퍼의 인덱스)

				if (0 == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.x)
				{
					pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndices.x = i;
					pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.x = pAIBone->mWeights[j].mWeight;
				}
				else if (0 == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.y)
				{
					pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndices.y = i;
					pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.y = pAIBone->mWeights[j].mWeight;
				}

				else if (0 == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.z)
				{
					pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndices.z = i;
					pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.z = pAIBone->mWeights[j].mWeight;
				}

				else if (0 == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.w)
				{
					pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndices.w = i;
					pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.w = pAIBone->mWeights[j].mWeight;
				}
					//모든 뼈를 다돌면서 정점에 영향을 미칠수 있는 뼈는 4개로 제한, 그리고 x,y,z,w 차례로 채워나간다.  다차면 더이상 안채움
					//뼈는 얘가 pBone은 메시가 가지고 있으니까,  영향을 주느뼈의 인덱스를 저장
			}
	}
	//어떤 매시는 뼈가 들어 있지 않다 -> 이것은 애니매이션이 없는 부위지만,  이 매시를 배치하기위한 뼈가 존재 하기 때문에 
	//내 메시의 원하는 뼈를 전체 모델의 뼈벡터에서 순회하면서 이름으로 일치하는 뼈를 찾아야 한다.
	// 그 뼈의 이름은 메시의 이름과 일치한다.( 배치를 위한 뼈 (애니메이션은 아니지만)) 
	if (0 == m_iNumBones)
	{
		m_iNumBones = 1;

		m_BoneIndices.push_back(pModel->Get_BoneIndex(m_szName));// 첫번째 칼메시는 뼈를 aimesh에서 제공하지 않지만 배치를 위한뼈가 model에 존재하므로,  그뼈를 이름으로찾아야한다
		_float4x4 OffsetMatrix;
		XMStoreFloat4x4(&OffsetMatrix, XMMatrixIdentity());// 메시가 제공하지 않는뼈인 배치뼈는 조정행렬로 항등행렬을 곱해주면 된다.
		m_BoneOffsetMatrices.push_back(OffsetMatrix);
	
	}
	ZeroMemory(&m_SubResourceDesc, sizeof m_SubResourceDesc);
	m_SubResourceDesc.pSysMem = pVertices;


	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}
/*for binary*/
HRESULT CMesh::Ready_VertexBuffer_For_NonAnim(const AI_MeshInfo* pAIMesh, _fmatrix PreTransformMatrix)
{
	m_iVertexStride = sizeof(VTXMESH);
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	/* 정점버퍼를 몇 바이트 할당할까요? */
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.StructureByteStride = m_iVertexStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXMESH* pVertices = new VTXMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMESH) * m_iNumVertices);

	/*_vector vecAxis = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_matrix marRot= DirectX::XMMatrixRotationAxis(vecAxis,XMConvertToRadians(180.f));*/


	{
		/*For NaviMesh*/
		m_vecVertices.resize(m_iNumVertices);
		/*For NaviMesh*/
	}
	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		//왜memcpy로 받아오는가? aiSenen에서 가져오는 타입은 dx11에서쓰는 타입과 일치하지 않는다. 다른 구조체 (다른 타입) 이므로 memcpy로 1바이트씩 읽어서 데이터만 읽어서 가져온다.


		memcpy(&pVertices[i].vPosition, &pAIMesh->m_vecPositions[i], sizeof(_float3));//정점 정보(위치)
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PreTransformMatrix));
		{
			/*For NaviMesh*/
			m_vecVertices[i] = pVertices[i].vPosition;
			/*For NaviMesh*/
		}
		memcpy(&pVertices[i].vNormal, &pAIMesh->m_vecNormals[i], sizeof(_float3));	//노말정보
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), PreTransformMatrix));

		memcpy(&pVertices[i].vTexcoord, &pAIMesh->m_vecTexcoords[i], sizeof(_float2));//(포인터 배열(동적배열을 가리키는?))	//texcoord는 2차원 배열로 되어있다 (texcoord는 8개까지 사용할수 있으므로, diffuse용도, 법선 용도....)
		//TEXCOORD0,1,2,3,.....
		memcpy(&pVertices[i].vTangent, &pAIMesh->m_vecTangents[i], sizeof(_float3));//TANGENT정보 -> 노말을 텍스처로 저장하면 모두다 같은 값이 나와버린다 -> 그래서 탄젠트 정보를 가지고 있음
		XMStoreFloat3(&pVertices[i].vTangent, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vTangent), PreTransformMatrix));
	}

	ZeroMemory(&m_SubResourceDesc, sizeof m_SubResourceDesc);
	m_SubResourceDesc.pSysMem = pVertices;


	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
	return S_OK;
}
HRESULT CMesh::Ready_VertexBuffer_For_Anim(const AI_MeshInfo* pAIMesh, CModel* pModel)
{
	


	m_iVertexStride = sizeof(VTXANIMMESH);
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	/* 정점버퍼를 몇 바이트 할당할까요? */
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.StructureByteStride = m_iVertexStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXANIMMESH* pVertices = new VTXANIMMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMESH) * m_iNumVertices);

	/*_vector vecAxis = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_matrix marRot= DirectX::XMMatrixRotationAxis(vecAxis,XMConvertToRadians(180.f));*/

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		
		memcpy(&pVertices[i].vPosition, &pAIMesh->m_vecPositions[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &pAIMesh->m_vecNormals[i], sizeof(_float3));	
		memcpy(&pVertices[i].vTexcoord, &pAIMesh->m_vecTexcoords[i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &pAIMesh->m_vecTangents[i], sizeof(_float3));
		
	}
	/*여기서 새로운 정보를 채움*/



	m_iNumBones = pAIMesh->m_iNumBones;

	for (size_t i = 0; i < m_iNumBones; ++i)
	{

		//aiBone* pAIBone = pAIMesh->mBones[i]; //이 메시에 영향을 주는 뼈의 개수를 아니 그 뼈정보도 제공한다.

		//_float4x4 OffsetMatrix = {};

		//memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));
		////->ai에서 제공하는 보정함수를 곱하지않으면 ai_process->자동보정 효과가 나타나지않는다( 오체 분리됨)
		////-> 이 받아오는 정보는 COL MAJOR이므로 전치해서 받아와야 한다 .
		//XMStoreFloat4x4(&OffsetMatrix, XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));


		m_BoneOffsetMatrices.push_back(pAIMesh->m_vecOffsetMatrices[i]);
		//->뼈의 개수만큼 offset 매트릭스들을 가진다. (각 인덱스 번째 뼈에 해당하는 조정 행렬을 저장한다.) 
		// i번 넣는다( i는 본 벡터의 인덱스)
		m_BoneIndices.push_back(pModel->Get_BoneIndex(pAIMesh->m_vecBoneNames[i].Name));

		//이뼈가 영향을 주는 정점의 개수 (weights)
		for (size_t j = 0; j < pAIMesh->m_vecNumWeights[i] ; ++j)//해당하는 본의 가중치는 aiBone[나](정점에 영향을 주는)가 준다.
		{
			//pAIBone->mWeights[j]: 이뼈가 영향을 주는이 (메시의) j번째 정점의 정보*/
			//pAIBone->mWeights[j].mVertexId : 이뼈가 영향을 주는 j번쨰 정점의 인덱스  (인덱스 버퍼의 인덱스)

			if (0 == pVertices[pAIMesh->m_vecWeightsID[i][j]].vBlendWeights.x)
			{
				pVertices[pAIMesh->m_vecWeightsID[i][j]].vBlendIndices.x = i;
				pVertices[pAIMesh->m_vecWeightsID[i][j]].vBlendWeights.x =pAIMesh->m_vecWeights[i][j];
			}
			else if (0 == pVertices[pAIMesh->m_vecWeightsID[i][j]].vBlendWeights.y)
			{
				pVertices[pAIMesh->m_vecWeightsID[i][j]].vBlendIndices.y = i;
				pVertices[pAIMesh->m_vecWeightsID[i][j]].vBlendWeights.y = pAIMesh->m_vecWeights[i][j];
			}

			else if (0 == pVertices[pAIMesh->m_vecWeightsID[i][j]].vBlendWeights.z)
			{
				pVertices[pAIMesh->m_vecWeightsID[i][j]].vBlendIndices.z = i;
				pVertices[pAIMesh->m_vecWeightsID[i][j]].vBlendWeights.z = pAIMesh->m_vecWeights[i][j];
			}
			else if (0 == pVertices[pAIMesh->m_vecWeightsID[i][j]].vBlendWeights.w)
			{
				pVertices[pAIMesh->m_vecWeightsID[i][j]].vBlendIndices.w = i;
				pVertices[pAIMesh->m_vecWeightsID[i][j]].vBlendWeights.w = pAIMesh->m_vecWeights[i][j];
			}
			//모든 뼈를 다돌면서 정점에 영향을 미칠수 있는 뼈는 4개로 제한, 그리고 x,y,z,w 차례로 채워나간다.  다차면 더이상 안채움
			//뼈는 얘가 pBone은 메시가 가지고 있으니까,  영향을 주느뼈의 인덱스를 저장
		}
	}
	if (0 == m_iNumBones)
	{
		m_iNumBones = 1;

		m_BoneIndices.push_back(pModel->Get_BoneIndex(m_szName));// 첫번째 칼메시는 뼈를 aimesh에서 제공하지 않지만 배치를 위한뼈가 model에 존재하므로,  그뼈를 이름으로찾아야한다
		_float4x4 OffsetMatrix;
		XMStoreFloat4x4(&OffsetMatrix, XMMatrixIdentity());// 메시가 제공하지 않는뼈인 배치뼈는 조정행렬로 항등행렬을 곱해주면 된다.
		m_BoneOffsetMatrices.push_back(OffsetMatrix);

	}



	ZeroMemory(&m_SubResourceDesc, sizeof m_SubResourceDesc);
	m_SubResourceDesc.pSysMem = pVertices;


	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
	return S_OK;




}
/*ㄹfor binary non anim mesh */
CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eModelType, class CModel* pModel,const AI_MeshInfo* pAIMesh, _fmatrix PreTransformMatrix)
{
	CMesh* pInstance = new CMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eModelType, pModel,pAIMesh, PreTransformMatrix)))
	{
		MSG_BOX("Failed to Created : CMesh_bin");
		Safe_Release(pInstance);
	}
	return pInstance;
}
/*for Ai*/
CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eModelType, class CModel* pModel, const aiMesh* pAIMesh, _fmatrix PreTransformMatrix)
{

	CMesh* pInstance = new CMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eModelType, pModel, pAIMesh, PreTransformMatrix)))
	{
		MSG_BOX("Failed to Created : CMesh");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CMesh::Clone(void* pArg)
{
    CMesh* pInstance = new CMesh(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Clone : CMesh");
        Safe_Release(pInstance);
        return nullptr;
    }
    return pInstance;
	/*안쓸것- 등록을 안하고 모델이 가지니까.*/
}

void CMesh::Free()
{
    __super::Free();
}
