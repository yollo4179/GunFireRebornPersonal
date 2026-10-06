#pragma once
#include"Component.h"
#include"Cell.h"
/* 모든 객체가 네비게이션을 가지고 얕은복사로 네비게이션이 가진 셀들을 공유한다*/
BEGIN(Engine)
class ENGINE_DLL CNavigation final :public CComponent
{
public:
	typedef struct
	{
		_int iStartIndex = { -1 };
	}NAVIGATION_DESC;
private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigation(const CNavigation& Prototype);
	virtual ~CNavigation() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pNavigationDataFile);
	virtual HRESULT Initialize_InMapTool();
	virtual HRESULT Initialize(void* pArg);
	virtual void Update(_fmatrix WorldMatrix);
public:
	/* 객체의 월드 포지션을 가져와 셀에서 비교후 높이를 포함함 위치값 반환*/
	_vector SetUp_HeightOnNavigation(_fvector vPosition);
	_bool isMove(_fvector vWorldPos);
	/*For Maptool*/
	HRESULT Add_Cells(TRIANGLE_VERTICES);
	void Edit_CurCell(TRIANGLE_VERTICES);
	void Pop_Cell();

	/*For Transform SlidingVector*/
	_vector Get_Normal_From_CurCell();
	_float3 Get_A_Point_From_CurCell(CCell::POINT _Point);
	/*_vector Get_Line_To_Slide_From_CurCell(_fvector vObjPos); */
	pair<_vector, _vector> Get_Elements_For_Sliding_FromCurCell(_fvector vObjPos);
	_bool  Check_Near_To_Points(_fvector vObjPos);
	_int Get_Index() { return m_iCurrentIndex; }
	void Set_Index(int iIndex) { m_iCurrentIndex = iIndex; };
#ifdef _DEBUG
public:
	virtual HRESULT Render();
#endif

private:
	vector<class CCell*>	m_Cells;
	static _float4x4		m_WorldMatrix; //지형의 월드행렬 (셀들은 로컬여역에 잇기때문에) 테레인을 월점 기준으로 이동시키고, 플레이어도따라서 움직여서 셀(로컬) 기준으로 맞춰서 위치를 판단해야함
	_int					m_iCurrentIndex = { -1 }; //플레이어(몬스터)의 현재위치한 인덱스 (얕복 안하고 고유하게 가짐 ) -> 테레인은 예외처리해줘야됨 (안터트리려면)


#ifdef _DEBUG
private:
	class CShader* m_pShader = { nullptr };
#endif

private:
	HRESULT SetUp_Neighbors();
	//내 셀의 (벡터) 인덱스를 주면 벡터에서 셀에 포지션을 넘겨 높이값을 넘겨줌(y)
	_float Compute_Height(_int iCellIndex, _fvector vPosition);

public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pNavigationDataFile = nullptr);
	virtual CComponent* Clone(void* pArg)override;
	virtual void Free() override;


};
END

