#pragma once
#include"Engine_Defines.h"

/*버텍스 & 인덱스*/
struct Indices
{
	Indices()
	{
		m_index[0] = 100;
		m_index[1] = 100;
		m_index[2] = 100;
	}
	Indices(_int x,_int y,_int z)
	{
		m_index[0] = x;
		m_index[1] = y;
		m_index[2] = z;
	}
	_int m_index[3];
};
/*머테리얼*/
struct FilePath
{
	wchar_t FilePath[MAX_PATH];
};
/*본*/
struct Name
{
	_char Name[MAX_PATH];
};

struct AI_MeshInfo
{
	_uint m_MaterialIndex = { 0 };
	_uint m_NumFaces = { 0 };
	_uint m_NumVertices = { 0 };
	_char m_Name[MAX_PATH];
	vector<_float3>m_vecPositions;
	vector<_float2>m_vecTexcoords;
	vector<_float3>m_vecNormals;
	vector<_float3>m_vecTangents;
	vector<Indices>m_vecIndices;
	/*for Anim*/
	/*for mesh->pAIBone*/
	_uint m_iNumBones=0; //뼈의 개수
	vector<_float4x4> m_vecOffsetMatrices;//각 뼈의 조정함수
	vector<Name> m_vecBoneNames;//각뼈의 이름
	vector<_uint> m_vecNumWeights;//각뼈가 영향을 미치는 정점개수
	vector<vector<_float>>m_vecWeights; //j번째 뼈가 k번째 정점에 가하는 가중치
	vector<vector<_int>>m_vecWeightsID;  //k번째 정점의 인덱스
};


struct AI_MaterialInfo
{
	vector<_uint>m_vecNumTextures;
	vector<vector< FilePath>>m_vecTexturePaths;
};

/*본*/
struct AI_Node {
	_char m_szName[MAX_PATH];
	_float4x4 m_TransformationMatrix;
	_uint m_NumChildren = { 0 };
	vector<AI_Node*>m_vecChildren;
	
};
/*프레임 키*/
struct AI_RotationKey
{
	_float4 m_Value;
	_float KeyFramePos;
};
struct AI_PositionKey
{
	_float3 m_Value;
	_float KeyFramePos;
};

struct AI_ScalingKey
{
	_float3 m_Value;
	_float KeyFramePos;
};



/*채널*/
struct AI_Channel
{
	
	_char m_szName[MAX_PATH];
	_uint m_NumScalingKeys;
	_uint m_NumPositionKeys;
	_uint m_NumRotationKeys;

	vector<AI_ScalingKey> m_ScalingKeys;
	vector<AI_PositionKey>m_PositionKeys;
	vector<AI_RotationKey>m_RotationKeys;

};

/*애니메이션*/
struct  AI_Animation
{
	~AI_Animation()
	{
	}
public:
	_char m_szName[MAX_PATH];
	_uint m_iNumChannels;
	_float m_fTickPerSecond;
	_float m_fDuration;
	_bool isCopied;
public:
	vector<AI_Channel> m_vecChannels;
};

struct AI_Info
{
	~AI_Info();

	_uint m_iNumMeshes = { 0 };
	_uint m_iNumMaterials = {0};
	_uint m_iNumAnimations = { 0 };

	/*For Meshes*/
	vector<AI_MeshInfo> m_vecMeshes;
	/*For Materials*/
	vector<AI_MaterialInfo>m_vecMaterials;
	/*ForBones*/
	AI_Node* m_RootNode = {nullptr};
	/*For Animations*/
	vector<AI_Animation> m_vecAnims;


	/*  NonAnim Func*/
	HRESULT Initialze_NonAnim(wchar_t* inputFilePath);
	/*Anim Func*/
	HRESULT Initialize_Anim(wchar_t* inputFilePath);
	HRESULT Read_Bones(HANDLE hHandle, AI_Node** pNode);
	/*For Clearing Nodes*/
	void ClearNodes(AI_Node* rootNode);
};