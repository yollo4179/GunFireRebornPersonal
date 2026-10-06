#include "Model.h"
#include"Mesh.h"
#include"Shader.h"
#include"Bone.h"
#include"Animation.h"
CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent {pDevice, pContext}
{
}

CModel::CModel(const CModel& Prototype)
:CComponent(Prototype)
, m_pAIScene{ Prototype.m_pAIScene }
, m_iNumMeshes{ Prototype.m_iNumMeshes }
, m_Meshes{ Prototype.m_Meshes }
, m_iNumMaterials{ Prototype.m_iNumMaterials }
, m_Materials{ Prototype.m_Materials }
, m_PreTransformMatrix{Prototype.m_PreTransformMatrix}
//, m_Bones{Prototype.m_Bones}// 이제 본은 얕은 복사 안할거다. 깊복으로 만들거임
,m_eModelType{Prototype.m_eModelType}
, m_iNumAnimations{ Prototype.m_iNumAnimations }
//, m_Animations{ Prototype.m_Animations }//애니메이션도 깊복 안함
, m_isBinary{Prototype.m_isBinary}
, m_vecTriangles{Prototype.m_vecTriangles }//navimesh 찍을때만
{

	/*for (auto& pBone : m_Bones)
		Safe_AddRef(pBone);*/

	for (auto& pPrototypeAnimation : Prototype.m_Animations)
	{
		m_Animations.push_back(pPrototypeAnimation->Clone()); //나(*this) 
	}
	for (auto& pPrototypeBone : Prototype.m_Bones)
	{
		m_Bones.push_back(pPrototypeBone->Clone());
	}
	// 복사 생성자 호출해서 다 깊은 복사로 넘겨준다 
	// -> 뼈도 자신의 뼈( 안에 포인터가 없고 남을 참조하지 않음)
	//애니메이션은 채널들을 공유하긴 하는데 -> 키프레임 인덱스는 애니메이션이 채널마다 따로 관리해서 
	//다른 객체와 모션이 겹치는 현상막을거임 
	// //(키프레임 공유안하니까 (한번 업ㄷ이트 도는 동안)같은동작인 얘들이 키프레임 계속 누적하는 현상막아서 
	//프레임 빨리 흐르는 현상도 막아줌 

	for (auto& Textures : m_Materials)
	{
		for (auto& SRVs : Textures)
		{
			for (auto& pSRV : SRVs)
				Safe_AddRef(pSRV);
		}
	}


	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);
}

_uint CModel::Get_BoneIndex(const _char* pBoneName) const 
{

	_uint iBoneIndex = { 0 };
	auto iter = find_if(m_Bones.begin(), m_Bones.end(),
		[&](CBone* pBone)->_bool
		{
			if (false == strcmp(pBone->Get_Name(), pBoneName))
				return true;// 멈취
			++iBoneIndex;//못멈췄으면 ++
			return false;//돌아 
		});
	if (iter == m_Bones.end())
		MSG_BOX("없어");
	return iBoneIndex;
}
const _float4x4* CModel::Get_BoneMatrix(const _char* pBoneName) const
{
	//벡터니까 find_if 로 이름으로 뼈를 찾자 파트 오브젝트는 뼈의 이름을 가지고 있겠지. 
	//아니면 하드코딩으로 박거나

	auto iter =find_if(m_Bones.begin(),m_Bones.end(), [&](CBone* pBone)->_bool
		{
			if (false == strcmp(pBone->Get_Name(), pBoneName))
				return true;

			return false;
		});

	if (iter == m_Bones.end())
		return nullptr;

	return  (*iter)->Get_CombinedTransformationFloat4x4();
}
void CModel::SetUp_Animation(_uint iAnimIndex, _bool isLoop)
{
	m_Animations[m_iCurrentAnimIndex]->Reset_CurrentKeyFrameIndices();
	m_isChangingMotion = false;
	m_iCurrentAnimIndex = iAnimIndex;  //애니메이션 바꾸자
	m_isLoop = isLoop;					//이 애니메이션 루프돌릴건지 물어봄
}
HRESULT CModel::Initialize_Prototype(TYPE eModelType, const _char* pModelFilePath,_fmatrix PreTransformMatrix, _tchar* biModelFilePath,bool _isBinary)
{
	m_isBinary = _isBinary;
	XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);
	
	if (nullptr == biModelFilePath)
	{
		_uint		iFlag = { aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast };

		if (eModelType == TYPE_NONANIM)
			iFlag |= aiProcess_PreTransformVertices;
	
		m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);
		
		if (0 == m_pAIScene)
			return E_FAIL;

		m_eModelType = eModelType;

		XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);// 애님이면 본에 , 논 애님이면 정점에 

		
		if (FAILED(Ready_Bones(m_pAIScene->mRootNode, -1)))
			return E_FAIL;
		
		if (FAILED(Ready_Meshes()))
			return E_FAIL;

 		if (FAILED(Ready_Materials(pModelFilePath)))
			return E_FAIL;

		if (FAILED(Ready_Animations()))
			return E_FAIL;
	
	}
	else
	{
		m_eModelType = eModelType;
		XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);// 애님이면 본에 , 논 애님이면 정점에 


		if (TYPE::TYPE_NONANIM == eModelType)
		{
			m_aiInfo.Initialze_NonAnim(biModelFilePath);
			if (FAILED(Ready_Meshes_Bin()))
				return E_FAIL;

			if (FAILED(Ready_Materials_Bin()))
				return E_FAIL;
		}
		else
		{
			m_aiInfo.Initialize_Anim(biModelFilePath);
			
			if (FAILED(Ready_Bones_Bin(m_aiInfo.m_RootNode, -1)))
				return E_FAIL;
			
			if (FAILED(Ready_Meshes_Bin()))
				return E_FAIL;

			if (FAILED(Ready_Materials_Bin()))
				return E_FAIL;
			 
			 
 
			 if (FAILED(Ready_Animations_Bin()))
				return E_FAIL;
			

		}
	}

	

	

	return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
{
	return S_OK;
}

//해당하는 메시를 클라(게임오브젝트에 메시의 개수를 던져주고, 메시를 정점 정보를 넘겨주고 그린다 )
//클라에서 반복문으로 처리-> 하나씩 그려줌 (그전에 메시게수를 클라에 주고 외부에서 반복문 돌린다.)
HRESULT CModel::Render(_uint iMeshIndex)
{
	if (iMeshIndex >= m_iNumMeshes)
		return E_FAIL;

	m_Meshes[iMeshIndex]->Bind_BufferDesc();
	m_Meshes[iMeshIndex]->Render();
	return S_OK; 
}


_bool CModel::Play_Animation(_float fTimeDelta, _bool& isChanging)
{
	
	isChanging = m_isChangingMotion;
	/*chaging motion*/
	if (m_isChangingMotion) {
		m_isFinished  = m_Animations[m_iCurrentAnimIndex]->Update_TransformationMatrices_ForMotionChange(m_Bones, m_isLoop, fTimeDelta);

		if (m_isFinished)
		{
			m_isChangingMotion = false;
			m_iCurrentAnimIndex = m_iNextAnimIndex;
			m_iNextAnimIndex = m_iNextAnimIndex;
			m_isLoop = m_isNextAnimationLoop;
		}
		isChanging = m_isChangingMotion;

		for (auto& pBone : m_Bones)
		{
			pBone->Update_CombinedTransformationMatrix(m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix));
		}
		return false;
	}

	/* 표현하고자 하는 애니메이션에 따른, 뼈의 상태(TransformationMatrix)를 갱신해준다.  */
	else if(false==m_isBinary)
		m_isFinished = m_Animations[m_iCurrentAnimIndex]->Update_TransformationMatrices(m_Bones, m_isLoop,fTimeDelta);
	/* 뼈의 바뀐 TransformationMatrix를 활용하여 렌덜이하기위한 최종 뼈의 CombinedTransformationMatrix를 만들어낸다. */
	
	for (auto& pBone : m_Bones)
	{
		pBone->Update_CombinedTransformationMatrix(m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix));
	}
	if (m_isFinished == true)
		int a = 0;

 	return m_isFinished;
}
HRESULT CModel::Bind_BoneMatrices(CShader* pShader, const _char* pConstantName, _uint iMeshIndex)
{
	return m_Meshes[iMeshIndex]->Bind_BoneMatrices(pShader,pConstantName,m_Bones);
}
HRESULT CModel::Bind_Material(CShader* pShader, const _char* pConstantName, _uint iMeshIndex, aiTextureType eType, _uint iTextureIndex)
{
		
	//외부로부터 메시인덱스를 던져주면 벡터에서 메시를 가져오고 메시내부의
	
	// 매핑된 머테리얼 인덱스 정보를 가져오는 함수 호출
	
	// ->[머테리얼의 개수 중 어떤 머테리얼][어떤 텍스처 타입][몇 번째 diffuse 텍스처 ]				
	if (m_Materials[m_Meshes[iMeshIndex]->Get_MaterialIndex()][eType].empty())
		return S_OK;

	return pShader->Bind_SRV(pConstantName, m_Materials[m_Meshes[iMeshIndex]->Get_MaterialIndex()][eType][iTextureIndex]);
	
}

/*For Next Anim*/
void CModel::Move_NextAnim(_uint NextAnimIndex,_float fDistanceAnimToAnim , _bool isNextAnimationLoop, _bool  IsOnTrackPosition )//애니메이션이 진행중인가? //트랙포지션에 있는가?
{
	
	m_Animations[m_iCurrentAnimIndex]->Store_CurrentAnim_KeyFrame_State(m_Bones, IsOnTrackPosition);//(뼈 객체 들을 저장한 컨테이너, 현재 애니메이션이 진행중인지 여부)
	m_Animations[m_iCurrentAnimIndex]->Store_AnimToChange_KeyFrame_State(m_Bones, m_Animations[NextAnimIndex]);//(뼈 객체 들을 저장한 컨테이너, 다음 애니메이션 객체)
	m_Animations[m_iCurrentAnimIndex]->SetUp_DistanceAnimToAnim(fDistanceAnimToAnim);	//현재 애니메이션 상태에서 다음 애니메이션으로 갈때 보간 Duration 간격
	m_iNextAnimIndex = NextAnimIndex;
	m_isNextAnimationLoop = isNextAnimationLoop;
	m_isChangingMotion = true;

}

void CModel::Set_AnimSpeed(int iIndex, _float fRatio)
{
	 m_Animations[iIndex]->Set_TickRatio(fRatio); 
}

void CModel::Add_Triangle(TRIANGLE_VERTICES Triangle)
{
	m_vecTriangles.push_back(Triangle);
}


HRESULT CModel::Ready_Meshes()
{

	m_iNumMeshes = m_pAIScene->mNumMeshes;  
	//aiScene에서메시의 개수를 가져오고 그 개수만큼 메시를 추출해서 메시를 저장한다( 메시클래스에 메시를 전달)
	//mNumMesh 도 있으면 mesh도 배열로 제공해준다 <-> 메시의 개수를 제공하면 그에 해당하는 메시도 같이 제공
	for (size_t i = 0; i < m_iNumMeshes; ++i)
	{
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eModelType, this,m_pAIScene->mMeshes[i],XMLoadFloat4x4(&m_PreTransformMatrix));//메시 클래스에 aiScene에 저장되어있는 메시(정보)를 전달(정점 인덱스 정보)[타입은 aimesh]
		if (nullptr == pMesh)
			return E_FAIL;
  		m_Meshes.push_back(pMesh);
	}
		//
	return S_OK;
}


HRESULT  CModel::Ready_Materials(const _char* pModelFilePath)
{
	m_iNumMaterials =m_pAIScene->mNumMaterials;
	//씬에서 머테리얼을가져온다
	m_Materials.resize(m_iNumMaterials);
	//머테리얼 개수만큼  벡터크기 재설정

	for (size_t i = 0; i < m_iNumMaterials; ++i)
	{
		m_Materials[i].resize(AI_TEXTURE_TYPE_MAX); 
		//머테리얼마다 텍스처의 종류는 18개씩 분간해서 들어있음 
		//크기를 18로 맞춘다.
		aiMaterial* pAIMaterial = m_pAIScene->mMaterials[i];
		//m_iNumMaterial중 i번째 머테리얼을 가져온다.
		for (size_t j = 1;  j < AI_TEXTURE_TYPE_MAX ; j++)//첫번째는 더미타입 1번째부터 diffuse 2번째가 specular ....18번째 타입까지 정보가 들어있다
		{
			_uint iNumTextures = pAIMaterial->GetTextureCount(aiTextureType(j));
			
			if (0 == iNumTextures&&1==j)
			{
				_char		szDrive[MAX_PATH] = "";
				_char		szDirectory[MAX_PATH] = "";
				_char		szFileName[MAX_PATH] = "hero_fpp_101_A_c";
				_char		szExt[MAX_PATH] = ".png";

				_char		szFullPath[MAX_PATH] = "";
				_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);


				strcpy_s(szFullPath, szDrive);
				strcat_s(szFullPath, szDirectory);
				strcat_s(szFullPath, szFileName);
				strcat_s(szFullPath, szExt);
				//DRIVE경로+ 디렉토리 경로 + 텍스처 이름 + 텍스처 확장자 (텍스처도 메시랑 같이 있어야됨)
				_tchar		szPerfectPath[MAX_PATH] = TEXT("");
				MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szPerfectPath, MAX_PATH);
				ID3D11ShaderResourceView* pSRV = { nullptr };
				if (false == strcmp(szExt, ".dds"))
				{
					if (SUCCEEDED(CreateDDSTextureFromFile(m_pDevice, szPerfectPath, nullptr, &pSRV)))
						m_Materials[i][j].push_back(pSRV);
				}


				else if (false == strcmp(szExt, ".tga"))
					int a = 10;
				else
				{
					if (SUCCEEDED(CreateWICTextureFromFile(m_pDevice, szPerfectPath, nullptr, &pSRV)))
						m_Materials[i][j].push_back(pSRV);
				}
				
			}
			for (size_t k = 0; k < iNumTextures; ++k)
			{
				aiString strTexturePath;//aiString 타입이 따로 존재
				 //i번째 머테리얼의 j타입의 텍스처iNumTextures 개중 k번째 텍스처의 주소 strTexturePath를 받아온다
				
				
				if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), k, &strTexturePath)))
					break;

				_char		szDrive[MAX_PATH] = "";
				_char		szDirectory[MAX_PATH] = "";
				_char		szFileName[MAX_PATH] = "";
				_char		szExt[MAX_PATH] = "";

				_char		szFullPath[MAX_PATH] = "";

				_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);
				//모델의 경로중 디렉토리 fbx 모델 파일이 존재하는 디렉토리(MODEL까지 가져온다.)
 				_splitpath_s(strTexturePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);
				//텍스처의 주소를 넘겨, 텍스처의 파일 이름 확장자를 가져온다. 

				strcpy_s(szFullPath, szDrive);
				strcat_s(szFullPath, szDirectory);
				strcat_s(szFullPath, szFileName);
				strcat_s(szFullPath, szExt);
				//DRIVE경로+ 디렉토리 경로 + 텍스처 이름 + 텍스처 확장자 (텍스처도 메시랑 같이 있어야됨)
				_tchar		szPerfectPath[MAX_PATH] = TEXT("");

				MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szPerfectPath, MAX_PATH);
				ID3D11ShaderResourceView* pSRV = { nullptr };

				if (false == strcmp(szExt, ".dds"))
				{
					if (SUCCEEDED(CreateDDSTextureFromFile(m_pDevice, szPerfectPath, nullptr, &pSRV)))
						m_Materials[i][j].push_back(pSRV);
				}


				else if (false == strcmp(szExt, ".tga"))
					int a = 10;
				else
				{
					if (SUCCEEDED(CreateWICTextureFromFile(m_pDevice, szPerfectPath, nullptr, &pSRV)))
						m_Materials[i][j].push_back(pSRV);
				}
			}
		}
	}
	return S_OK;
}



HRESULT CModel::Ready_Bones(const aiNode* pAINode, _int iParentBoneIndex)
{
	//(자기 자신의 노드 정보,부모의 인덱스)
	CBone* pBone = CBone::Create(pAINode, iParentBoneIndex);
	
	if (nullptr == pBone)
		return E_FAIL;

	m_Bones.push_back(pBone);


	iParentBoneIndex = m_Bones.size() - 1;
	for (size_t i = 0; i < pAINode->mNumChildren; ++i)//-> 
		//childnum이 있으면 child 노드도 존재한다. 
	{
		//child본에는 자기자신의 인덱스와 child bone aiNode를 전해줘 부모 자식 관계를 알게 한다. 
		Ready_Bones(pAINode->mChildren[i], iParentBoneIndex);
	}


	return S_OK;
}

HRESULT CModel::Ready_Animations()
{
	//애니메이션의 개수를 가져온다 
	// animation구조체는 채널 구조체를 가지고, 체널들 본(채널 이름) 웨이 포인트(키프레임) 구조체를 가진다 ...
	m_iNumAnimations = m_pAIScene->mNumAnimations;

	for (size_t i = 0; i < m_iNumAnimations; ++i)
	{
		CAnimation* pAnimation = CAnimation::Create(m_pAIScene->mAnimations[i], this);
		if (nullptr == pAnimation)
			return E_FAIL;
		m_Animations.push_back(pAnimation);
	
	}
	return S_OK;
}


HRESULT CModel::Ready_Meshes_Bin()
{


	m_iNumMeshes = m_aiInfo.m_iNumMeshes;
	for (size_t i = 0; i < m_iNumMeshes; ++i)
	{
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eModelType, this, &m_aiInfo.m_vecMeshes[i], XMLoadFloat4x4(&m_PreTransformMatrix));//메시 클래스에 aiScene에 저장되어있는 메시(정보)를 전달(정점 인덱스 정보)[타입은 aimesh]
		if (nullptr == pMesh)
			return E_FAIL;
		m_Meshes.push_back(pMesh);
	}
	//
	return S_OK;
}
HRESULT CModel::Ready_Materials_Bin()
{

	m_iNumMaterials = m_aiInfo.m_iNumMaterials;  //m_pAIScene->mNumMaterials;
	//씬에서 머테리얼을가져온다
	m_Materials.resize(m_iNumMaterials);
	//머테리얼 개수만큼  벡터크기 재설정

	for (size_t i = 0; i < m_iNumMaterials; ++i)
	{
		m_Materials[i].resize(AI_TEXTURE_TYPE_MAX);
		//머테리얼마다 텍스처의 종류는 18개씩 분간해서 들어있음 
		//크기를 18로 맞춘다.
		//aiMaterial* pAIMaterial m_pAIScene->mMaterials[i];
		//m_iNumMaterial중 i번째 머테리얼을 가져온다.
		for (size_t j = 1; j < AI_TEXTURE_TYPE_MAX; j++)//첫번째는 더미타입 1번째부터 diffuse 2번째가 specular ....18번째 타입까지 정보가 들어있다
		{
			_uint iNumTextures = m_aiInfo.m_vecMaterials[i].m_vecNumTextures[j];//pAIMaterial->GetTextureCount(aiTextureType(j));
			//i번째 메터리얼에 [j번째 텍스처 타입]이 몇장인지 정보르 ㄹ가져온다. 
			for (size_t k = 0; k < iNumTextures; ++k)//diffuse는 1장 이상일 수 도 있다 
			{
				//aiString strTexturePath;//aiString 타입이 따로 존재
				////i번째 머테리얼의 j타입의 텍스처iNumTextures 개중 k번째 텍스처의 주소 strTexturePath를 받아온다
				//if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), k, &strTexturePath)))
				//	break;

				//_char		szDrive[MAX_PATH] = "";
				//_char		szDirectory[MAX_PATH] = "";
				//_char		szFileName[MAX_PATH] = "";
				

				//_char		szFullPath[MAX_PATH] = "";

				//_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);
				////모델의 경로중 디렉토리 fbx 모델 파일이 존재하는 디렉토리(MODEL까지 가져온다.)
				//_splitpath_s(strTexturePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);
				////텍스처의 주소를 넘겨, 텍스처의 파일 이름 확장자를 가져온다. 

				//strcpy_s(szFullPath, szDrive);
				//strcat_s(szFullPath, szDirectory);
				//strcat_s(szFullPath, szFileName);
				//strcat_s(szFullPath, szExt);
				////DRIVE경로+ 디렉토리 경로 + 텍스처 이름 + 텍스처 확장자 (텍스처도 메시랑 같이 있어야됨)
				//_tchar		szPerfectPath[MAX_PATH] = TEXT("");

				//MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szPerfectPath, MAX_PATH);
				_tchar szPath[MAX_PATH]; 
				lstrcpy(szPath,m_aiInfo.m_vecMaterials[i].m_vecTexturePaths[j][k].FilePath);
				_tchar		szExt[MAX_PATH] = L"";
				_wsplitpath_s(szPath, nullptr, 0, nullptr, 0, nullptr, 0, szExt,MAX_PATH);
				
				


				ID3D11ShaderResourceView* pSRV = { nullptr };

				if (false == lstrcmp(szExt, L".dds"))
				{
					if (SUCCEEDED(CreateDDSTextureFromFile(m_pDevice, szPath, nullptr, &pSRV)))
						m_Materials[i][j].push_back(pSRV);
				}


				else if (false == lstrcmp(szExt, L".tga"))
					int a = 10;
				else
				{
					if (SUCCEEDED(CreateWICTextureFromFile(m_pDevice, szPath, nullptr, &pSRV)))
						m_Materials[i][j].push_back(pSRV);
				}
			}
		}
	}
	return S_OK;


}
HRESULT CModel::Ready_Bones_Bin( AI_Node* pAINode, _int iParentBoneIndex)
{
	//(자기 자신의 노드 정보,부모의 인덱스)
	CBone* pBone = CBone::Create(pAINode, iParentBoneIndex);

	if (nullptr == pBone)
		return E_FAIL;

	m_Bones.push_back(pBone);


	iParentBoneIndex = m_Bones.size() - 1;
	for (size_t i = 0; i < pAINode->m_NumChildren; ++i)//-> 
		//childnum이 있으면 child 노드도 존재한다. 
	{
		//child본에는 자기자신의 인덱스와 child bone aiNode를 전해줘 부모 자식 관계를 알게 한다. 
		Ready_Bones_Bin(pAINode->m_vecChildren[i], iParentBoneIndex);
	}


	return S_OK;
}
HRESULT CModel::Ready_Animations_Bin()
{
	m_iNumAnimations = m_aiInfo.m_iNumAnimations;
	for (size_t i = 0; i < m_iNumAnimations; ++i)
	{
		CAnimation* pAnimation = CAnimation::Create(&m_aiInfo.m_vecAnims[i],this);

		if (nullptr == pAnimation)
			return E_FAIL;
		m_Animations.push_back(pAnimation);
	}
	return S_OK;
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eModelType, const _char* pModelFilePath, _fmatrix PreTransformMatrix, _tchar* biModelFilePath,bool _isBinary)
{
	CModel* pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eModelType, pModelFilePath, PreTransformMatrix, biModelFilePath,_isBinary)))
	{
		MSG_BOX("Failed to Created : CModel");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CModel::Clone(void* pArg)
{
	CModel* pInstance = new CModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel::Free()
{
	__super::Free();
	for (auto& pAnimation : m_Animations)
		Safe_Release(pAnimation);

	for (auto& pBone : m_Bones)
		Safe_Release(pBone);

	m_Bones.clear();

	for (auto& Textures : m_Materials)
	{
		for (auto& SRVs : Textures)
		{
			for (auto& pSRV : SRVs)
				Safe_Release(pSRV);

			SRVs.clear();
		}
		Textures.clear();
	}
	m_Materials.clear();

	for (auto& pMesh : m_Meshes)
		Safe_Release(pMesh);

	m_Meshes.clear();

	m_Importer.FreeScene();
}




HRESULT CModel::Make_Binary_NonAnim(TYPE eModelType, const _char* pModelFilePath, _tchar* OutputFilePath,_fmatrix PreTransformMatrix)
{
	//for extracting binary
	Assimp::Importer		Importer;
	const aiScene* pAIScene = { nullptr };

	_uint		iFlag = { aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast };

	if (eModelType == TYPE_NONANIM)
		iFlag |= aiProcess_PreTransformVertices;

	pAIScene = Importer.ReadFile(pModelFilePath, iFlag);

	if (0 == pAIScene)
		return E_FAIL;

	//pAIScene->mNumMeshes;
/*fileInput*/
	{

		HANDLE hFile = CreateFile(OutputFilePath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (INVALID_HANDLE_VALUE == hFile) {
			MSG_BOX("File Open :Failed");
			return E_FAIL;
		}
		_ulong	dwByte(0);
		_ulong   dwSize(0);

		//메시의 개수를 저장합니다.
		_uint iNumMeshes = pAIScene->mNumMeshes;
		WriteFile(hFile, &iNumMeshes, sizeof(_int), &dwByte, nullptr);
		//각 메시의 정보를 저장합니다. 

		for (int i = 0; i < iNumMeshes; ++i)
		{
			const aiMesh* meshInfo = pAIScene->mMeshes[i];
			WriteFile(hFile, &meshInfo->mMaterialIndex, sizeof(_int), &dwByte, nullptr);
			WriteFile(hFile, &meshInfo->mNumVertices, sizeof(_int), &dwByte, nullptr);
			WriteFile(hFile, &meshInfo->mNumFaces, sizeof(_int), &dwByte, nullptr);
			{
				_char			m_szName[MAX_PATH] = "";
				strcpy_s(m_szName, meshInfo->mName.data);
				_int LenName = strlen(m_szName) + 1;//널문자까지
				WriteFile(hFile, &LenName, sizeof(_int), &dwByte, nullptr);
				WriteFile(hFile, &meshInfo->mName, LenName, &dwByte, nullptr);
			}

			for (int j = 0; j < meshInfo->mNumVertices; ++j)
			{
				_float3 vPosition = {};
				_float3 vNormal = {};
				_float2 vTexcoord = {};
				_float3 vTangent = {};
				memcpy(&vPosition, &meshInfo->mVertices[j], sizeof(_float3));//정점 정보(위치)
				XMStoreFloat3(&vPosition, XMVector3TransformCoord(XMLoadFloat3(&vPosition), PreTransformMatrix));

				memcpy(&vNormal, &meshInfo->mNormals[j], sizeof(_float3));	//노말정보
				XMStoreFloat3(&vNormal, XMVector3TransformNormal(XMLoadFloat3(&vNormal), PreTransformMatrix));

				memcpy(&vTexcoord, &meshInfo->mTextureCoords[0][j], sizeof(_float2));//(포인터 배열(동적배열을 가리키는?))	//texcoord는 2차원 배열로 되어있다 (texcoord는 8개까지 사용할수 있으므로, diffuse용도, 법선 용도....)
				//TEXCOORD0,1,2,3,.....
				memcpy(&vTangent, &meshInfo->mTangents[j], sizeof(_float3));//TANGENT정보 -> 노말을 텍스처로 저장하면 모두다 같은 값이 나와버린다 -> 그래서 탄젠트 정보를 가지고 있음
				XMStoreFloat3(&vTangent, XMVector3TransformNormal(XMLoadFloat3(&vTangent), PreTransformMatrix));
				WriteFile(hFile, &vPosition, sizeof(_float3), &dwByte, nullptr);
				WriteFile(hFile, &vNormal, sizeof(_float3), &dwByte, nullptr);
				WriteFile(hFile, &vTexcoord, sizeof(_float2), &dwByte, nullptr);
				WriteFile(hFile, &vTangent, sizeof(_float3), &dwByte, nullptr);
			}
			//하나의 폴리곤 정보는 그 폴리곤(삼각형)을 이루는 정점들의 인덱스로 구성되어있다. -> 추출해서 저장하자.
			for (size_t j = 0; j < meshInfo->mNumFaces; j++)
			{
				int indices[3] = { 0,0,0 };
				indices[0] = meshInfo->mFaces[j].mIndices[0];//폴리곤.인덱스1-> 얘도 포인터인데 동적 배열(FACE)에 구조체안의 동적배열로(크기FLOAT3)에 각각의 인덱스가 들어있다. 
				indices[1] = meshInfo->mFaces[j].mIndices[1];
				indices[2] = meshInfo->mFaces[j].mIndices[2];

				WriteFile(hFile, indices, sizeof(indices), &dwByte, nullptr);
			}
		}
		//메터리얼 정보 넣을겁니다.

		_uint iNumMaterials = pAIScene->mNumMaterials;
		WriteFile(hFile, &iNumMaterials, sizeof(_uint), &dwByte, nullptr);

		for (int i = 0; i < iNumMaterials; ++i)
		{
			aiMaterial* pAIMaterial = pAIScene->mMaterials[i]; //메터리얼을 가져온다.
			for (size_t j = 1; j < AI_TEXTURE_TYPE_MAX; j++)//텍스처 타입마다 택스처의 개수가 몇개인지 저장한다.
			{
				_uint iNumTextures = pAIMaterial->GetTextureCount(aiTextureType(j));
				WriteFile(hFile, &iNumTextures, sizeof(_uint), &dwByte, nullptr);//18개의 텍스처 타입이 각각 몇개 있는지
				for (size_t k = 0; k < iNumTextures; ++k)//diffuse는 1장 이상일 수 도 있다 
				{
					aiString strTexturePath;//aiString 타입이 따로 존재
					//i번째 머테리얼의 j타입의 텍스처iNumTextures 개중 k번째 텍스처의 주소 strTexturePath를 받아온다
					if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), k, &strTexturePath)))
						break;

					_char		szDrive[MAX_PATH] = "";
					_char		szDirectory[MAX_PATH] = "";
					_char		szFileName[MAX_PATH] = "";
					_char		szExt[MAX_PATH] = "";

					_char		szFullPath[MAX_PATH] = "";

					_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);
					//모델의 경로중 디렉토리 fbx 모델 파일이 존재하는 디렉토리(MODEL까지 가져온다.)
					_splitpath_s(strTexturePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);
					//텍스처의 주소를 넘겨, 텍스처의 파일 이름 확장자를 가져온다. 

					strcpy_s(szFullPath, szDrive);
					strcat_s(szFullPath, szDirectory);
					strcat_s(szFullPath, szFileName);
					strcat_s(szFullPath, szExt);
					//DRIVE경로+ 디렉토리 경로 + 텍스처 이름 + 텍스처 확장자 (텍스처도 메시랑 같이 있어야됨)
					_tchar		szPerfectPath[MAX_PATH] = TEXT("");

					int size = (MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szPerfectPath, MAX_PATH) + 1) * sizeof(wchar_t);//널문자까지

					WriteFile(hFile, &size, sizeof(int), &dwByte, nullptr);
					WriteFile(hFile, szPerfectPath, size, &dwByte, nullptr);

				}

			}

		}






		CloseHandle(hFile);
	}


	return S_OK;
}

HRESULT CModel::Make_Binary_Anim(TYPE eModelType, const _char* pModelFilePath, _tchar* OutputFilePath,_fmatrix PreTransformMatrix)
{
	//for extracting binary
	Assimp::Importer		Importer;
	const aiScene* pAIScene = { nullptr };

	_uint		iFlag = { aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast };

	/*if (eModelType == TYPE_NONANIM)
		iFlag |= aiProcess_PreTransformVertices;*/

	pAIScene = Importer.ReadFile(pModelFilePath, iFlag);

	if (0 == pAIScene)
		return E_FAIL;



	//pAIScene->mNumMeshes;
/*fileInput*/
	{

		HANDLE hFile = CreateFile(OutputFilePath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (INVALID_HANDLE_VALUE == hFile) {
			MSG_BOX("File Open :Failed");
			return E_FAIL;
		}
		_ulong	dwByte(0);
		_ulong   dwSize(0);

		//메시의 개수를 저장합니다.
		_uint iNumMeshes = pAIScene->mNumMeshes;
		WriteFile(hFile, &iNumMeshes, sizeof(_uint), &dwByte, nullptr);
		//각 메시의 정보를 저장합니다. 

		for (int i = 0; i < iNumMeshes; ++i)
		{
			const aiMesh* meshInfo = pAIScene->mMeshes[i];
			WriteFile(hFile, &meshInfo->mMaterialIndex, sizeof(_int), &dwByte, nullptr);
			WriteFile(hFile, &meshInfo->mNumVertices, sizeof(_int), &dwByte, nullptr);
			WriteFile(hFile, &meshInfo->mNumFaces, sizeof(_int), &dwByte, nullptr);
			{
				_char			m_szName[MAX_PATH] = "";
				strcpy_s(m_szName, meshInfo->mName.data);
				_int LenName = strlen(m_szName) + 1;//널문자까지
				WriteFile(hFile, &LenName, sizeof(_int), &dwByte, nullptr);
				WriteFile(hFile, &m_szName, LenName, &dwByte, nullptr);
			}

			for (int j = 0; j < meshInfo->mNumVertices; ++j)
			{
				_float3 vPosition = {};
				_float3 vNormal = {};
				_float2 vTexcoord = {};
				_float3 vTangent = {};
				memcpy(&vPosition, &meshInfo->mVertices[j], sizeof(_float3));//정점 정보(위치)
				//XMStoreFloat3(&vPosition, XMVector3TransformCoord(XMLoadFloat3(&vPosition), PreTransformMatrix));

				memcpy(&vNormal, &meshInfo->mNormals[j], sizeof(_float3));	//노말정보
				//XMStoreFloat3(&vNormal, XMVector3TransformNormal(XMLoadFloat3(&vNormal), PreTransformMatrix));

				memcpy(&vTexcoord, &meshInfo->mTextureCoords[0][j], sizeof(_float2));//(포인터 배열(동적배열을 가리키는?))	//texcoord는 2차원 배열로 되어있다 (texcoord는 8개까지 사용할수 있으므로, diffuse용도, 법선 용도....)
				//TEXCOORD0,1,2,3,.....
				memcpy(&vTangent, &meshInfo->mTangents[j], sizeof(_float3));//TANGENT정보 -> 노말을 텍스처로 저장하면 모두다 같은 값이 나와버린다 -> 그래서 탄젠트 정보를 가지고 있음
				//XMStoreFloat3(&vTangent, XMVector3TransformNormal(XMLoadFloat3(&vTangent), PreTransformMatrix));
				
				WriteFile(hFile, &vPosition, sizeof(_float3), &dwByte, nullptr);
				WriteFile(hFile, &vNormal, sizeof(_float3), &dwByte, nullptr);
				WriteFile(hFile, &vTexcoord, sizeof(_float2), &dwByte, nullptr);
				WriteFile(hFile, &vTangent, sizeof(_float3), &dwByte, nullptr);
			}

 
			_uint iNumBones = meshInfo->mNumBones;
			WriteFile(hFile, &iNumBones, sizeof(_uint), &dwByte, nullptr);

			for (size_t i = 0; i < iNumBones; ++i)
			{

				aiBone* pAIBone = meshInfo->mBones[i]; //이 메시에 영향을 주는 뼈의 개수를 아니 그 뼈정보도 제공한다.

				_float4x4 OffsetMatrix = {};

				memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));
				
				XMStoreFloat4x4(&OffsetMatrix, XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));

				//i번째 뼈의 조정 행렬 저장(i번째)
				WriteFile(hFile, &OffsetMatrix, sizeof(_float4x4), &dwByte, nullptr);
				//i번째 뼈의 이름 저장-> 모델 인덱스 가져오기 용도
				{
					_char			m_szName[MAX_PATH] = "";
					strcpy_s(m_szName, pAIBone->mName.data);
					_int LenName = strlen(m_szName) + 1;//널문자까지
					WriteFile(hFile, &LenName, sizeof(_int), &dwByte, nullptr);
					WriteFile(hFile, m_szName, LenName, &dwByte, nullptr);
				}
				
				_uint m_NumsWeights = pAIBone->mNumWeights;
				WriteFile(hFile, &m_NumsWeights, sizeof(_uint), &dwByte, nullptr);
				 

				//이뼈가 영향을 주는 정점의 개수 (weights)
				for (size_t j = 0; j < pAIBone->mNumWeights; ++j)//해당하는 본의 가중치는 aiBone[나](정점에 영향을 주는)가 준다.
				{
					_uint ID;
					_float Weight;
					memcpy(&ID, &pAIBone->mWeights[j].mVertexId,sizeof(_uint));
					memcpy(&Weight, &pAIBone->mWeights[j].mWeight, sizeof(_float));
					WriteFile(hFile, &ID, sizeof(_uint), &dwByte, nullptr);//j번째 정점의 인덱스
					WriteFile(hFile, &Weight, sizeof(_float), &dwByte, nullptr); //j번째 정점의 가중치
					
				}
			}
			






			//하나의 폴리곤 정보는 그 폴리곤(삼각형)을 이루는 정점들의 인덱스로 구성되어있다. -> 추출해서 저장하자.
			for (size_t j = 0; j < meshInfo->mNumFaces; j++)
			{
				int indices[3] = { 0,0,0 };
				indices[0] = meshInfo->mFaces[j].mIndices[0];//폴리곤.인덱스1-> 얘도 포인터인데 동적 배열(FACE)에 구조체안의 동적배열로(크기FLOAT3)에 각각의 인덱스가 들어있다. 
				indices[1] = meshInfo->mFaces[j].mIndices[1];
				indices[2] = meshInfo->mFaces[j].mIndices[2];

				WriteFile(hFile, indices, sizeof(indices), &dwByte, nullptr);
			}
		}
		//메터리얼 정보 넣을겁니다.

		_uint iNumMaterials = pAIScene->mNumMaterials;
		WriteFile(hFile, &iNumMaterials, sizeof(_uint), &dwByte, nullptr);

		for (int i = 0; i < iNumMaterials; ++i)
		{
			aiMaterial* pAIMaterial = pAIScene->mMaterials[i]; //메터리얼을 가져온다.
			for (size_t j = 1; j < AI_TEXTURE_TYPE_MAX; j++)//텍스처 타입마다 택스처의 개수가 몇개인지 저장한다.
			{
				_uint iNumTextures = pAIMaterial->GetTextureCount(aiTextureType(j));


				if (0 == iNumTextures && 1 == j)//Hero Arms
				{
					iNumTextures = 1;
					WriteFile(hFile, &iNumTextures, sizeof(_uint), &dwByte, nullptr);
					iNumTextures = 0;
					_char		szDrive[MAX_PATH] = "";
					_char		szDirectory[MAX_PATH] = "";
					_char		szFileName[MAX_PATH] = "prop";//hero_fpp_101_A_c
					_char		szExt[MAX_PATH] = ".png";

					_char		szFullPath[MAX_PATH] = "";
					_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);


					strcpy_s(szFullPath, szDrive);
					strcat_s(szFullPath, szDirectory);
					strcat_s(szFullPath, szFileName);
					strcat_s(szFullPath, szExt);
					//DRIVE경로+ 디렉토리 경로 + 텍스처 이름 + 텍스처 확장자 (텍스처도 메시랑 같이 있어야됨)
					_tchar		szPerfectPath[MAX_PATH] = TEXT("");
					int size = (MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szPerfectPath, MAX_PATH) + 1) * sizeof(wchar_t);//널문자까지
					WriteFile(hFile, &size, sizeof(int), &dwByte, nullptr);
					WriteFile(hFile, szPerfectPath, size, &dwByte, nullptr);

				}

				else 
					WriteFile(hFile, &iNumTextures, sizeof(_uint), &dwByte, nullptr);//18개의 텍스처 타입이 각각 몇개 있는지
				
				for (size_t k = 0; k < iNumTextures; ++k)//diffuse는 1장 이상일 수 도 있다 
				{
					aiString strTexturePath;//aiString 타입이 따로 존재
					//i번째 머테리얼의 j타입의 텍스처iNumTextures 개중 k번째 텍스처의 주소 strTexturePath를 받아온다
					if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), k, &strTexturePath)))
						break;

					_char		szDrive[MAX_PATH] = "";
					_char		szDirectory[MAX_PATH] = "";
					_char		szFileName[MAX_PATH] = "";
					_char		szExt[MAX_PATH] = "";

					_char		szFullPath[MAX_PATH] = "";

					_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);
					//모델의 경로중 디렉토리 fbx 모델 파일이 존재하는 디렉토리(MODEL까지 가져온다.)
					_splitpath_s(strTexturePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);
					//텍스처의 주소를 넘겨, 텍스처의 파일 이름 확장자를 가져온다. 

					strcpy_s(szFullPath, szDrive);
					strcat_s(szFullPath, szDirectory);
					strcat_s(szFullPath, szFileName);
					strcat_s(szFullPath, szExt);
					//DRIVE경로+ 디렉토리 경로 + 텍스처 이름 + 텍스처 확장자 (텍스처도 메시랑 같이 있어야됨)
					_tchar		szPerfectPath[MAX_PATH] = TEXT("");

					int size = (MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szPerfectPath, MAX_PATH) + 1) * sizeof(wchar_t);//널문자까지

					WriteFile(hFile, &size, sizeof(int), &dwByte, nullptr);
					WriteFile(hFile, szPerfectPath, size, &dwByte, nullptr);

				}

			}

		}
		/* 본정보 쓴다*/
		Write_Bones_Anim(hFile, pAIScene->mRootNode);

		/*애니메이션 정보를 채운다*/
		_uint iNumAnimations = pAIScene->mNumAnimations;
		WriteFile(hFile, &iNumAnimations, sizeof(_uint), &dwByte, nullptr);


		for(int i=0;i<iNumAnimations;++i)
		{ 
			aiAnimation* pAnim = pAIScene->mAnimations[i];
			{
				_char			m_szName[MAX_PATH] = "";
				strcpy_s(m_szName,pAnim->mName.data);
				_int LenName = strlen(m_szName) + 1;//널문자까지
				WriteFile(hFile, &LenName, sizeof(_int), &dwByte, nullptr);
				WriteFile(hFile, m_szName, LenName, &dwByte, nullptr);
			}
			_double Duration= pAnim->mDuration;
			_double TickPerSecond = pAnim->mTicksPerSecond;
			WriteFile(hFile, &Duration, sizeof(_double), &dwByte, nullptr);
			WriteFile(hFile, &TickPerSecond, sizeof(_double), &dwByte, nullptr);
			_uint iNumChannels = pAnim->mNumChannels;
			WriteFile(hFile, &iNumChannels, sizeof(_uint), &dwByte, nullptr);
			
			for (int j = 0; j < iNumChannels; ++j)
			{
				aiNodeAnim* pChannel =pAnim->mChannels[j];
				{
					_char			m_szName[MAX_PATH] = "";
					strcpy_s(m_szName, pChannel->mNodeName.data);
					_int LenName = strlen(m_szName) + 1;//널문자까지
					WriteFile(hFile, &LenName, sizeof(_int), &dwByte, nullptr);
					WriteFile(hFile, m_szName, LenName, &dwByte, nullptr);
				}

				_uint iNumPositionsKeys =pChannel->mNumPositionKeys ;
				WriteFile(hFile, &iNumPositionsKeys, sizeof(_uint), &dwSize, nullptr);
				for (int k = 0; k < iNumPositionsKeys; ++k)
				{	
					_float3 vPos = {};
					_double mTime = { 0 };
					memcpy(&vPos, &(pChannel->mPositionKeys[k].mValue), sizeof(_float3));
					memcpy(&mTime, &(pChannel->mPositionKeys[k].mTime), sizeof(_double));
					WriteFile(hFile, &(vPos), sizeof(_float3), &dwByte, nullptr);
					WriteFile(hFile, &mTime, sizeof(_double), &dwByte, nullptr);

				}

				_uint iNumScalingKeys = pChannel->mNumScalingKeys;
				WriteFile(hFile, &iNumScalingKeys, sizeof(_uint), &dwSize, nullptr);
				for (int k = 0; k < iNumScalingKeys; ++k)
				{
					_float3 vScale = {};
					_double mTime = { 0 };
					memcpy(&vScale,&(pChannel->mScalingKeys[k].mValue), sizeof(_float3));
					memcpy(&mTime, &(pChannel->mScalingKeys[k].mTime), sizeof(_double));
					WriteFile(hFile, &vScale, sizeof(_float3), &dwSize, nullptr);
					WriteFile(hFile, &mTime, sizeof(_double), &dwByte, nullptr);
				}
				
				_uint iNumRotationKeys = pChannel->mNumRotationKeys;
				WriteFile(hFile, &iNumRotationKeys, sizeof(_uint), &dwSize, nullptr);
				for (int k = 0; k < iNumRotationKeys; ++k)
				{
					_float4 vRot = {};
					_double mTime = { 0 };
					memcpy(&vRot.x, &(pChannel->mRotationKeys[k].mValue.x), sizeof(_float));
					memcpy(&vRot.y, &(pChannel->mRotationKeys[k].mValue.y), sizeof(_float));
					memcpy(&vRot.z, &(pChannel->mRotationKeys[k].mValue.z), sizeof(_float));
					memcpy(&vRot.w, &(pChannel->mRotationKeys[k].mValue.w), sizeof(_float));

					memcpy(&mTime, &(pChannel->mRotationKeys[k].mTime), sizeof(_double));
					WriteFile(hFile, &vRot, sizeof(_float4), &dwSize, nullptr);
					WriteFile(hFile, &mTime, sizeof(_double), &dwByte, nullptr);
				}
				int a = 0;


			}

		
		}



		CloseHandle(hFile);
	}


	return S_OK;
}

HRESULT CModel::Write_Bones_Anim(HANDLE hFile, const aiNode* pNode)
{
	_ulong dwByte = 0;
	{
		_char szName[MAX_PATH] = "";
		strcpy_s(szName, pNode->mName.data);// 뼈를 생성하면 나 (자신 )뼈의 이름을 가져온다 
		_int LenName = strlen(szName) + 1;//널문자까지
		WriteFile(hFile, &LenName, sizeof(_int), &dwByte, nullptr);
		WriteFile(hFile, &szName, LenName, &dwByte, nullptr);
	}
	{
		_float4x4 TransformationMatrix;
		XMStoreFloat4x4(&TransformationMatrix, XMMatrixIdentity());
		memcpy(&TransformationMatrix, &pNode->mTransformation, sizeof(_float4x4));//aiNode로부터 자신의 행렬을 가져온다 -> 부모 기준에서의 자신만의 행렬
		XMStoreFloat4x4(&TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&TransformationMatrix)));
		WriteFile(hFile, &TransformationMatrix, sizeof(_float4x4), &dwByte, nullptr);
	}
	_int iNumChildren = pNode->mNumChildren;
	WriteFile(hFile, &iNumChildren, sizeof(_int), &dwByte, nullptr);


	for (int i = 0; i < iNumChildren; ++i)
	{
		Write_Bones_Anim(hFile, pNode->mChildren[i]);
	}


	return S_OK;
}
