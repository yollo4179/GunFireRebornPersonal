#include "AI_Info.h"

AI_Info::~AI_Info()
{
	if (m_RootNode)
	{
		ClearNodes(m_RootNode);
	}
}

HRESULT  AI_Info::Initialze_NonAnim(wchar_t* inputFilePath)
{

		HANDLE hFile = CreateFile(inputFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (INVALID_HANDLE_VALUE == hFile) {
			MSG_BOX("File Read :Failed");
			return E_FAIL;
		}
		_ulong	dwByte(0);
		_ulong   dwSize(0);
		ReadFile(hFile, &m_iNumMeshes, sizeof(_int), &dwByte, nullptr);
		m_vecMeshes.resize(m_iNumMeshes);
		for (int i = 0; i < m_iNumMeshes; ++i)
		{

			AI_MeshInfo MeshInfo = {};

			ReadFile(hFile, &MeshInfo.m_MaterialIndex, sizeof(_int), &dwByte, nullptr);
			ReadFile(hFile, &MeshInfo.m_NumVertices, sizeof(_int), &dwByte, nullptr);
			ReadFile(hFile, &MeshInfo.m_NumFaces, sizeof(_int), &dwByte, nullptr);
			{
				_int iLengthName = { 0 };
				ReadFile(hFile, &iLengthName, sizeof(_int), &dwByte, nullptr);
				ReadFile(hFile, &MeshInfo.m_Name, iLengthName, &dwByte, nullptr);
			}

			MeshInfo.m_vecPositions.resize(MeshInfo.m_NumVertices);
			MeshInfo.m_vecNormals.resize(MeshInfo.m_NumVertices);
			MeshInfo.m_vecTexcoords.resize(MeshInfo.m_NumVertices);
			MeshInfo.m_vecTangents.resize(MeshInfo.m_NumVertices);
			for (int j = 0; j < MeshInfo.m_NumVertices; ++j)
			{
				_float3 vPosition = {};
				_float3 vNormal = {};
				_float2 vTexcoord = {};
				_float3 vTangent = {};
				ReadFile(hFile, &vPosition, sizeof(_float3), &dwByte, nullptr);
				ReadFile(hFile, &vNormal, sizeof(_float3), &dwByte, nullptr);
				ReadFile(hFile, &vTexcoord, sizeof(_float2), &dwByte, nullptr);
				ReadFile(hFile, &vTangent, sizeof(_float3), &dwByte, nullptr);

				MeshInfo.m_vecPositions[j] = vPosition;
				MeshInfo.m_vecNormals[j] = vNormal;
				MeshInfo.m_vecTexcoords[j] = vTexcoord;
				MeshInfo.m_vecTangents[j] = vTangent;
			}
			MeshInfo.m_vecIndices.resize(MeshInfo.m_NumFaces);
			for (int j = 0; j < MeshInfo.m_NumFaces; ++j)
			{
				int indices[3];
				ReadFile(hFile, indices, sizeof(indices), &dwByte, nullptr);
				Indices element = Indices(indices[0], indices[1], indices[2]);

				MeshInfo.m_vecIndices[j] = element;
			}

			m_vecMeshes[i] = MeshInfo;

		}
		ReadFile(hFile, &m_iNumMaterials, sizeof(_uint), &dwByte, nullptr);

		m_vecMaterials.resize(m_iNumMaterials);
		for (int i = 0; i < m_iNumMaterials; ++i)
		{
			AI_MaterialInfo MaterialInfo = {};
			MaterialInfo.m_vecNumTextures.resize(AI_TEXTURE_TYPE_MAX);
			MaterialInfo.m_vecTexturePaths.resize(AI_TEXTURE_TYPE_MAX);
			for (size_t j = 1; j < AI_TEXTURE_TYPE_MAX; j++)//텍스처 타입마다 택스처의 개수가 몇개인지 저장한다.
			{
				_uint iNumTextures = { 0 };
				ReadFile(hFile, &iNumTextures, sizeof(_uint), &dwByte, nullptr);
				MaterialInfo.m_vecNumTextures[j] = (iNumTextures);
				for (int k = 0; k < iNumTextures; ++k)
				{
					wchar_t TexPath[MAX_PATH] = L"";
					int iLength = { 0 };
					ReadFile(hFile, &iLength, sizeof(_uint), &dwByte, nullptr);
					ReadFile(hFile, &TexPath, iLength, &dwByte, nullptr);
					FilePath filePath;
					memcpy(filePath.FilePath, TexPath, MAX_PATH);
					MaterialInfo.m_vecTexturePaths[j].push_back(filePath);
				}
			}
			m_vecMaterials[i] = MaterialInfo;
		}

		CloseHandle(hFile);
		return S_OK;
}
HRESULT AI_Info::Initialize_Anim(wchar_t* inputFilePath)
{
	HANDLE hFile = CreateFile(inputFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile) {
		MSG_BOX("File Read :Failed");
		return E_FAIL;
	}
	_ulong	dwByte(0);
	_ulong   dwSize(0);
	ReadFile(hFile, &m_iNumMeshes, sizeof(_uint), &dwByte, nullptr);
	m_vecMeshes.resize(m_iNumMeshes);
	for (int i = 0; i < m_iNumMeshes; ++i)
	{

		AI_MeshInfo MeshInfo = {};

		ReadFile(hFile, &MeshInfo.m_MaterialIndex, sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, &MeshInfo.m_NumVertices, sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, &MeshInfo.m_NumFaces, sizeof(_int), &dwByte, nullptr);
		{
			_int iLengthName = { 0 };
			ReadFile(hFile, &iLengthName, sizeof(_int), &dwByte, nullptr);
			ReadFile(hFile, &MeshInfo.m_Name, iLengthName, &dwByte, nullptr);
		}

		MeshInfo.m_vecPositions.resize(MeshInfo.m_NumVertices);
		MeshInfo.m_vecNormals.resize(MeshInfo.m_NumVertices);
		MeshInfo.m_vecTexcoords.resize(MeshInfo.m_NumVertices);
		MeshInfo.m_vecTangents.resize(MeshInfo.m_NumVertices);
		for (int j = 0; j < MeshInfo.m_NumVertices; ++j)
		{
			_float3 vPosition = {};
			_float3 vNormal = {};
			_float2 vTexcoord = {};
			_float3 vTangent = {};
			ReadFile(hFile, &vPosition, sizeof(_float3), &dwByte, nullptr);
			ReadFile(hFile, &vNormal, sizeof(_float3), &dwByte, nullptr);
			ReadFile(hFile, &vTexcoord, sizeof(_float2), &dwByte, nullptr);
			ReadFile(hFile, &vTangent, sizeof(_float3), &dwByte, nullptr);

			MeshInfo.m_vecPositions[j] = vPosition;
			MeshInfo.m_vecNormals[j] = vNormal;
			MeshInfo.m_vecTexcoords[j] = vTexcoord;
			MeshInfo.m_vecTangents[j] = vTangent;
		}
		/*인덱스 채우기 전에 과정 추가*/
		/*여기까지는 과정 동일 anim에서는 2가지 정점 정보를 더 가진다 .*/


		ReadFile(hFile, &MeshInfo.m_iNumBones, sizeof(_uint), &dwByte, nullptr);
		_uint iNumBones = MeshInfo.m_iNumBones;
		MeshInfo.m_vecOffsetMatrices.resize(iNumBones);
		MeshInfo.m_vecBoneNames.resize(iNumBones);
		MeshInfo.m_vecNumWeights.resize(iNumBones);
		MeshInfo.m_vecWeights.resize(iNumBones);
		MeshInfo.m_vecWeightsID.resize(iNumBones);

		for (size_t i = 0; i < MeshInfo.m_iNumBones; ++i) {

			_float4x4 OffsetMatrix;
			XMStoreFloat4x4(&OffsetMatrix, XMMatrixIdentity());
			ReadFile(hFile, &OffsetMatrix, sizeof(_float4x4), &dwByte, nullptr);
			MeshInfo.m_vecOffsetMatrices[i] = OffsetMatrix;


			{
				_char			m_szName[MAX_PATH] = "";
				_int LenName = 0;
				ReadFile(hFile, &LenName, sizeof(_int), &dwByte, nullptr);
				ReadFile(hFile, m_szName, LenName, &dwByte, nullptr);
				memcpy(&MeshInfo.m_vecBoneNames[i].Name, m_szName,LenName);
			}
			ReadFile(hFile, &MeshInfo.m_vecNumWeights[i], sizeof(_int), &dwByte, nullptr);
			
			MeshInfo.m_vecWeights[i].resize(MeshInfo.m_vecNumWeights[i]);
			MeshInfo.m_vecWeightsID[i].resize(MeshInfo.m_vecNumWeights[i]);

			for (size_t j = 0; j < MeshInfo.m_vecNumWeights[i]; ++j)//해당하는 본의 가중치는 aiBone[나](정점에 영향을 주는)가 준다.
			{

				ReadFile(hFile, &MeshInfo.m_vecWeightsID[i][j], sizeof(_uint), &dwByte, nullptr); //j번째 정점의 idx
				ReadFile(hFile, &MeshInfo.m_vecWeights[i][j], sizeof(_float), &dwByte, nullptr);//jw

			}



		}






		MeshInfo.m_vecIndices.resize(MeshInfo.m_NumFaces);
		for (int j = 0; j < MeshInfo.m_NumFaces; ++j)
		{
			int indices[3];
			ReadFile(hFile, indices, sizeof(indices), &dwByte, nullptr);
			Indices element = Indices(indices[0], indices[1], indices[2]);

			MeshInfo.m_vecIndices[j] = element;
		}

		m_vecMeshes[i] = MeshInfo;

	}
	

	/*머테리얼 정보를 채움니다. 애님과 과정 동일*/

	ReadFile(hFile, &m_iNumMaterials, sizeof(_uint), &dwByte, nullptr);

	m_vecMaterials.resize(m_iNumMaterials);
	for (int i = 0; i < m_iNumMaterials; ++i)
	{
		AI_MaterialInfo MaterialInfo = {};
		MaterialInfo.m_vecNumTextures.resize(AI_TEXTURE_TYPE_MAX);
		MaterialInfo.m_vecTexturePaths.resize(AI_TEXTURE_TYPE_MAX);
		for (size_t j = 1; j < AI_TEXTURE_TYPE_MAX; j++)//텍스처 타입마다 택스처의 개수가 몇개인지 저장한다.
		{
			_uint iNumTextures = { 0 };
			ReadFile(hFile, &iNumTextures, sizeof(_uint), &dwByte, nullptr);
			MaterialInfo.m_vecNumTextures[j] = (iNumTextures);
			for (int k = 0; k < iNumTextures; ++k)
			{
				wchar_t TexPath[MAX_PATH] = L"";
				int iLength = { 0 };
				ReadFile(hFile, &iLength, sizeof(_uint), &dwByte, nullptr);
				ReadFile(hFile, &TexPath, iLength, &dwByte, nullptr);
				FilePath filePath;
				memcpy(filePath.FilePath, TexPath, MAX_PATH);
				MaterialInfo.m_vecTexturePaths[j].push_back(filePath);
			}
		}
		m_vecMaterials[i] = MaterialInfo;
	}

	/* 본정보 읽어옴*/
 	Read_Bones(hFile, &m_RootNode);
	
	///* 애니메이션 정보 읽어옴*/
	ReadFile(hFile, &m_iNumAnimations, sizeof(_uint), &dwByte, nullptr);
	m_vecAnims.resize(m_iNumAnimations);
	

	//AI_Animation Anim = {};
	//AI_Channel stChannel = {};
	for (int i = 0; i < m_iNumAnimations; ++i)
	{
		AI_Animation Anim = {};
		{
			int iLenName = { 0 };
			ReadFile(hFile, &iLenName, sizeof(_int), &dwByte, nullptr);
			ReadFile(hFile, &Anim.m_szName, iLenName, &dwByte, nullptr);
		}

		double fDuration = 0.f;
		double fTickPerSecond = 0.f;
		ReadFile(hFile, &fDuration, sizeof(_double), &dwByte, nullptr);
		ReadFile(hFile, &fTickPerSecond, sizeof(_double), &dwByte, nullptr);
		
		Anim.m_fDuration =(_float) fDuration;
		Anim.m_fTickPerSecond = (_float)fTickPerSecond;
		ReadFile(hFile, &Anim.m_iNumChannels, sizeof(_uint), &dwByte, nullptr);
		Anim.m_fTickPerSecond = 1.f;
		int iNumChannel = Anim.m_iNumChannels;
		Anim.m_vecChannels.resize(iNumChannel);

		for (int j = 0; j < iNumChannel; ++j)
		{
			AI_Channel stChannel = {};
			
			{
				int iLenName = { 0 };
				ReadFile(hFile, &iLenName, sizeof(_int), &dwByte, nullptr);
				ReadFile(hFile, &stChannel.m_szName, iLenName, &dwByte, nullptr);
			}
			ReadFile(hFile, &stChannel.m_NumPositionKeys, sizeof(_uint), &dwByte, nullptr);
			
			for (int k = 0; k < stChannel.m_NumPositionKeys; ++k)
			{

				AI_PositionKey stPositionKey = {};
				ReadFile(hFile, &stPositionKey.m_Value, sizeof(_float3), &dwByte, nullptr);
				_double time = 0.f;
				ReadFile(hFile, &time, sizeof(_double), &dwByte, nullptr);
				
				stPositionKey.KeyFramePos = (_float)time;
				stChannel.m_PositionKeys.push_back(stPositionKey);
			}

			ReadFile(hFile, &stChannel.m_NumScalingKeys, sizeof(_uint), &dwByte, nullptr);
			
			for (int k = 0; k < stChannel.m_NumScalingKeys; ++k)
			{
				AI_ScalingKey stScalingKey = {};
				ReadFile(hFile, &stScalingKey.m_Value, sizeof(_float3), &dwByte, nullptr);
				_double time = 0.f;
				ReadFile(hFile, &time, sizeof(_double), &dwByte, nullptr);
				stScalingKey.KeyFramePos = (_float)time;
				stChannel.m_ScalingKeys.push_back(stScalingKey);
			}

			ReadFile(hFile, &stChannel.m_NumRotationKeys, sizeof(_uint), &dwByte, nullptr);
			
			for (int k = 0; k < stChannel.m_NumRotationKeys; ++k)
			{
				AI_RotationKey stRotationKey = {};
				
				
				ReadFile(hFile, &stRotationKey.m_Value, sizeof(_float4), &dwByte, nullptr);
				_double time = 0.f;
				ReadFile(hFile, &time, sizeof(_double), &dwByte, nullptr);
				stRotationKey.KeyFramePos = (_float)time;
				stChannel.m_RotationKeys.push_back(stRotationKey);
			}
			Anim.m_vecChannels[j]=stChannel;
			stChannel.m_PositionKeys.clear();
			stChannel.m_RotationKeys.clear();
			stChannel.m_ScalingKeys.clear();
		}
		
 		m_vecAnims[i]=Anim;

		//Anim.m_vecChannels.clear();
		
	}


	
	CloseHandle(hFile);
	return S_OK;

}

HRESULT AI_Info::Read_Bones(HANDLE hFile, AI_Node** pNode)
{


	*pNode = new AI_Node();
	_ulong dwByte = 0;
	{
		_char szName[MAX_PATH] = "";
		_int LenName = 0;
		ReadFile(hFile, &LenName, sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, &(*pNode)->m_szName, LenName, &dwByte, nullptr);
	}

	ReadFile(hFile, &(*pNode)->m_TransformationMatrix, sizeof(_float4x4), &dwByte, nullptr);
	//전치 시킨게 들어옴

	ReadFile(hFile, &(*pNode)->m_NumChildren, sizeof(_int), &dwByte, nullptr);


	(*pNode)->m_vecChildren.resize((*pNode)->m_NumChildren);
	for (int i = 0; i < (*pNode)->m_NumChildren; ++i)
	{
		Read_Bones(hFile, &(*pNode)->m_vecChildren[i]);
	}




	return S_OK;
}

void AI_Info::ClearNodes(AI_Node* rootNode)
{
	for (int i = 0; i < rootNode->m_NumChildren; ++i)
	{
		ClearNodes(rootNode->m_vecChildren[i]);
	}

	Safe_Delete(rootNode);
}

