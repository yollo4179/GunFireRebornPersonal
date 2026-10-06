#pragma once
#include"Base.h"
BEGIN(Engine)
class CCell final : public CBase
{
public:
	//내가가진 삼각형의 포인트의 인덱스 {시계방향으로 A B C} 포인트는 배열로 관리
	enum POINT {POINT_A, POINT_B, POINT_C ,POINT_END};
	//이웃한 셀의 삼각형과의 공통요소를 표현하기위해 line으로 표기한다 라인도 배열로 관리
	enum LINE {LINE_AB, LINE_BC,LINE_CA, LINE_END};

private:
	CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CCell() = default;
public:
	
	/* 매개변수의 값에따라 A B C 정점중 하나를 리턴*/
	_float3 Get_Point(POINT ePoint)const
	{
		return m_vPoints[ePoint];
	}
	/*Line이 겹친다면 겹치는 라인(배열)에 이웃 인덱스를 등록한다*/
	void Set_Neighbor(LINE eLine, CCell* pNeighborCell)
	{
		m_iNeighborIndices[eLine] = pNeighborCell->m_iIndex;
	}
	
	//3개의 정점의 위치 (배열로 넘긴다 ), 내 삼각형의 인덱스
	HRESULT Initialize(const _float3* pPoints, _int iIndex,int iStage);
	_bool isIn(_fvector vLocalPos, _int& iNeghborIndex);

	/* 원시적인 타입은 콜바이Value */
	/* 사용자정의 타입은 디폴트로는 콜바이레퍼런스 */
	/* 특수한 경우에는 콜바이 포인터 */

	/*상대방 삼각형의 두점을 매개변수로 넘긴다면-> 나의 삼각형과 라인이 겹치는지 확인 (src)A B -> desc(a b,a c)(b a,b c)(c a, c b)()*/
	_bool Compare_Points(const _float3& SourPoint, const _float3& DestPoint);
	/*객체의 로컬 위치를 넘기면, 세 정점에 의한 평면을 만들어 x,z 값만 넘겨 y 값을  넘긴다 */
	_float Compute_Height(_fvector vPosition);
	
	void AdjustPoints(TRIANGLE_VERTICES vTriangle);
	/*For Sliding vector*/
	_vector Get_Normal();
	void    Set_Normal();
	pair<_vector, _vector> Get_Elements_For_Sliding(_fvector vObjPos);
	_vector Get_Src_Of_SlidingVector();
	_bool Check_Near_To_Points(_fvector vObjPos);


#ifdef _DEBUG
public:
	HRESULT Render();
#endif

private:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext{ nullptr };

private:
	_int						m_iIndex = { -1 }; //인접한 셀이 없다면 인접 했어야 했지만 없는 셀의 인덱스는 -1
	_float3						m_vPoints[POINT_END] = {};//내 셀의 3개의 정점 
	_int						m_iNeighborIndices[LINE_END] = { -1,-1,-1 };//line 에는 인접한 셀의 인덱스를 저장한다
	_int						m_iStage = 0;
	
	_float3						m_Normal = {};
	_float3						m_vSrcOfSlidingVector = {};


#ifdef _DEBUG
private:
	class CVIBuffer_Cell* m_pVIBuffer = { nullptr };
#endif

public:
	static CCell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints, _int iIndex,int iStage=0);
	virtual void Free()override;
};END

