#pragma once
#include"GameObject.h"
#include"Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
class CVIBuffer_Collider;
END


BEGIN(Client)
class CMonster : public CGameObject
{
public:
	struct Name
	{
		_char Name[MAX_PATH];
	};
	typedef struct
	{
		int iNumStoredAnims;//  반복문, <cur index, nextIndex>,< AnimName, distance>
		map<pair<_uint ,_uint> , pair< Name ,_float>> m_mapDistances;  //-> client vector[CuranimIndex][nextAnim]=flaot -> 구조체 째로 말고 따로따로
	}ANIM_DESC;

	typedef struct :public CGameObject::GAMEOBJECT_DESC{
		_float4 vPosition;
		_float3 vScale;
		_float fAngle;
		_int iIndex;
		ANIM_DESC eANIM_Desc;
	}MONSTER_DESC;

protected:
	explicit CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CMonster(const CMonster& Prototype);
	virtual ~CMonster() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	_uint Get_AnimsNum() { return iNumAnims; };
	_uint Get_AddedAnimsNum(){ return iAddedNumAnims; };
		
	void StartAnimToAnim(); //버튼 클릭시에만 작동하게 만들자(트리거)[애니메이션 재생모드 트리거도 on모드로]
	_bool Play_AnimToAnim(_uint iAnimIndex, _uint iNextAnimIndex,_float fDuration);//얘는 버튼 밖으로 뺴자
	_bool PlayNowAnim(_uint iAnimIndex, _bool isLoop);

protected:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	_uint iNumAnims = { 0 };
	_uint iAddedNumAnims = { 0 }; //anim_to_anim
	_bool m_isChangeing = { false };

public:
	_bool m_bAnim_End = { false };//이거로 애님 끝낫는지확인해서 반복호출 막자.
	/* Store for Anim Desc*/
	ANIM_DESC m_eAnim_Desc;
public:
	/*for Both*/
	MONSTER_DESC m_Desc;
	/*for AnimTool;  CHECK Anim*/
	bool m_isVisible = {false};
	/* 1 Anim*/
	_int m_iNumAnims = { 0 };
	_int m_iCurAnimIndex = { 0 };
	/*Anim To Anim*/
	_int m_iCurAnimToAnimIndex = { 0 };
	_float m_fDistanceForStore = { false };
	_int m_StartAnimIndex = { 0 };
	_int m_EndAnimIndex = { 0 };
	char m_AnimName[MAX_PATH]="";

	bool m_bStartAnim_Begin = { false };
	bool m_bStartAnim_End = { false };
	_uint CallStartAnim = { 0 };
	bool m_bChangingAnim_Begin = { false };
	bool m_bChangingAnim_End = { false };
	_uint CallChangingAnim = { 0 };
	bool m_bEndAnim_Begin = { false };
	bool m_bEndAnim_End = { false };
	_uint CallEndAnim = { 0 };
	_float m_fAccTimeDelta = 0.f;
	_float m_fTimeDelta = 0.f;

protected:
	
public:
	
protected:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
protected:
	

public:
	static CMonster* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free()override;

};

END
