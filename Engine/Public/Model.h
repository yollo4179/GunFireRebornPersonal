#pragma once
#include"Component.h"
#include"Engine_Defines.h"
#include"AI_Info.h"

BEGIN(Engine)
class ENGINE_DLL CModel final :public CComponent
{
public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };

private:
	explicit CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CModel(const CModel& Prototype);
	virtual ~CModel() = default;

public:
	_uint Get_NumMeshes()const {
		return m_iNumMeshes;
	}
	_uint Get_BoneIndex(const _char* pBoneName) const ;//->본의 이름으로 내 벡터의 본을 탐색(정점에 영향을 미치는 4가지 본을 찾을때...) 

	/*파트오브젝트가 따라다닐 본매트릭스를 가져오기위해 필요-> 장착할 본매트릭스 (따라다닐)*/
	const _float4x4* Get_BoneMatrix(const _char* pBoneName)const;
public:
	void SetUp_Animation(_uint iAnimIndex, _bool isLoop);
	
	
public:
	virtual HRESULT Initialize_Prototype(TYPE eModelType, const _char* pModelFilePath,_fmatrix PreTransformMatrix, _tchar* biModelFilePath=nullptr,bool isBinary=false);
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render(_uint  iMeshIndex );

public:
	_bool Play_Animation(_float fTimeDelta,_bool& isChanging);//애니메이션 이 끝났는지도 불값으로 주자. 

	/*For Rendering*/
public:
	
	HRESULT Bind_BoneMatrices(class CShader* pShader, const _char* pConstantName, _uint iMeshIndex);
	HRESULT Bind_Material(class CShader* pShader, const _char* pConstantName, _uint iMeshIndex, aiTextureType eType, _uint iTextureIndex = 0); 
	/*For Anims*/
	void Set_AnimIndex(_uint i) { m_iCurrentAnimIndex = (i< m_iNumAnimations)?i : 0; }
	_uint Get_AnimNums() { return m_iNumAnimations; };
	_uint Get_Current_AnimIndex() { return m_iCurrentAnimIndex; };
	/*For Moving NextAnim*/
	void Move_NextAnim(_uint NextAnimIndex,_float fDistanceAnimToAnim=0.2f,_bool isNextAnimationLoop=false, _bool SlerpWhileSlerp=false);
	/* For SettingAnim in Anim Tool*/
	_uint Get_AnimNums() const { return m_iNumAnimations; };
	/* Anim Speed */
	void Set_AnimSpeed(int iIndex, _float fRatio);
	/*For NaviMesh*/
	void Add_Triangle(TRIANGLE_VERTICES Triangle);
	
	/*for importing fbx and extracting binaryInfo*/
private: 
	Assimp::Importer			m_Importer;
	const aiScene* m_pAIScene = { nullptr };
	TYPE						m_eModelType;
	/*For Meshes*/
private:
	_uint					m_iNumMeshes = { 0 };
	vector<class CMesh*>	m_Meshes;
	/*For Bones*/
private:
	vector<class CBone*>		m_Bones; 
	
	/*For Adjusting PrePosition and Angle if The Model's Original is somewhere else than local (0,0) */
private:
	_float4x4				m_PreTransformMatrix = {};
	/* For Materials*/
private:
	_uint					m_iNumMaterials = {};
	vector<vector<vector<ID3D11ShaderResourceView*>>> m_Materials;
	
	/*ForAnimations*/
private:
	_uint						m_iCurrentAnimIndex = {0};
	_bool						m_isLoop = { false };
	_uint						m_iNumAnimations = {};  
	vector<class CAnimation*>	m_Animations;   
	_bool						m_isFinished = { false};//PlayAnimation 에 이값 줄거임

	/*ForChange Animation Motion*/
	_bool						m_isChangingMotion = { false };
	_uint						m_iNextAnimIndex = { 0 };
	_bool						m_isNextAnimationLoop = { false };
	
	/*ForBinary Info*/
	AI_Info m_aiInfo;
	bool m_isBinary=false;


public:
	/*For Maptool :Making Navigation Mesh*/
	vector<TRIANGLE_VERTICES> m_vecTriangles;
	

private:
	HRESULT Ready_Meshes();
	HRESULT  Ready_Materials(const _char* pModelFilePath);//모델 파일로부터(메터리얼로부터 정보를 가져와 저장한다.(diffuse,specular, ambient,emissive텍스처....등등))
	HRESULT Ready_Bones(const aiNode* pNode,_int iParentBoneIndex);
	HRESULT Ready_Animations();

	/*for binary*/
	HRESULT Ready_Meshes_Bin();
	HRESULT Ready_Materials_Bin();
	HRESULT Ready_Bones_Bin( AI_Node* pAINode, _int iParentBoneIndex);
	HRESULT Ready_Animations_Bin();

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eModelType, const _char* pModelFilePath,_fmatrix PreTransformMatrix, _tchar* biModelFilePath=nullptr,bool isBinary = false);
	virtual CComponent* Clone(void* pArg)override;
	virtual void Free() override;

public:
	static HRESULT Make_Binary_NonAnim(TYPE eModelType, const _char* pModelFilePath, _tchar* OutputFilePath,_fmatrix PreTranformMatrix);
	static HRESULT Make_Binary_Anim(TYPE eModelType, const _char* pModelFilePath, _tchar* OutputFilePath, _fmatrix PreTranformMatrix);
	static HRESULT Write_Bones_Anim(HANDLE hFile, const aiNode* pNode);
};
END

