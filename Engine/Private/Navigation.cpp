#include "Navigation.h"
#include"Cell.h"
#include"Shader.h"
#include"GameInstance.h"

_float4x4 CNavigation::m_WorldMatrix = {};

CNavigation::CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent{pDevice, pContext}
{
}

CNavigation::CNavigation(const CNavigation& Prototype)
	:CComponent( Prototype )
	, m_Cells{ Prototype.m_Cells }
#ifdef _DEBUG
	,m_pShader{Prototype.m_pShader}
#endif

{

#ifdef _DEBUG 
	Safe_AddRef(m_pShader);
#endif
	for (auto& pCell : m_Cells)
	{
		Safe_AddRef(pCell);
	}
}

HRESULT CNavigation::Initialize_Prototype(const _tchar* pNavigationDataFile)
{
	_ulong			dwByte = {};
	HANDLE			hFile = CreateFile(pNavigationDataFile, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	int iNum = { 0 };
	ReadFile(hFile, &iNum, sizeof(_int), &dwByte, nullptr);

	while (true)
	{
		_float3		vPoints[3] = {};
		int iStage = { 0 };

		ReadFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);
		ReadFile(hFile, &iStage, sizeof(_int), &dwByte, nullptr);
		if (0 == dwByte)
			break;
									//세 정점과 인덱스 
		CCell* pCell = CCell::Create(m_pDevice, m_pContext, vPoints, m_Cells.size(),iStage);
		if (nullptr == pCell)
			return E_FAIL;

		m_Cells.push_back(pCell);
	}

	CloseHandle(hFile);

	if (FAILED(SetUp_Neighbors()))
		return E_FAIL;
	
#ifdef _DEBUG
	/* 클라가 참조하여 읽으니까 engine 참조형으로 sdk 를 읽게 한다.*/
 
	m_pShader =CShader::Create(m_pDevice , m_pContext, TEXT("../../EngineSDK/hlsl/Shader_Cell.hlsl"), VTXPOS::Elements, VTXPOS::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;
#endif
	return S_OK;
	
}

HRESULT CNavigation::Initialize_InMapTool()
{
#ifdef _DEBUG
	/* 클라가 참조하여 읽으니까 engine 참조형으로 sdk 를 읽게 한다.*/
	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../../EngineSDK/hlsl/Shader_Cell.hlsl"), VTXPOS::Elements, VTXPOS::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;
#endif

	return S_OK;
}

HRESULT CNavigation::Initialize(void* pArg)
{
	if (nullptr == pArg) //터레인인 경우 스타트인덱스가 필요없으니까 넘어간다 
		return S_OK;

	NAVIGATION_DESC* pDesc = static_cast<NAVIGATION_DESC*>(pArg);

	_matrix matIdentity = XMMatrixIdentity();
	XMStoreFloat4x4(&m_WorldMatrix, matIdentity);
	m_iCurrentIndex = pDesc->iStartIndex;

	return S_OK;
}

void CNavigation::Update(_fmatrix WorldMatrix)
{
	XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix); //터레인으로 부터 월드 행렬을 받아온다
}

_vector CNavigation::SetUp_HeightOnNavigation(_fvector vPosition)
{
	_vector			vLocalPos = XMVector3TransformCoord(vPosition, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));

	vLocalPos = XMVectorSetY(vPosition, Compute_Height(m_iCurrentIndex, vLocalPos));

	//월드로 바꿔서 벡터 반환
	return XMVector3TransformCoord(vLocalPos, XMLoadFloat4x4(&m_WorldMatrix));

	//트랜스폼에서 이용해주자 (지형 태우기)
}

_bool CNavigation::isMove(_fvector vWorldPos)
{

	_vector			vLocalPos = XMVector3TransformCoord(vWorldPos, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));//터레인 월드의 역향렬을 곱해서 터레인을 로컬로 움직이고 같은 이동량을 플레이어 몬스터에 둬서 (로컬에있는 셀을 기준으로 네비메시판단)
	_int			iNeighborIndex = { -1 };

	if (true == m_Cells[m_iCurrentIndex]->isIn(vLocalPos,iNeighborIndex))
	{
		return true;//내부에 있으면 움직여
	}
	else
	{
		/*나간 방향으로 다른 셀과 연결되지 않은 경우*/
		if(-1 ==iNeighborIndex)
			return false;//외분데 연결 안되있으면 움직이지마 

		/*
		if(-1 != iNeghborIndex)
		{
			m_iCurrentIndex =iNeghvorIndex ;
			return true ;
			//-> 한틱에 한 셀만 이동 체크 하고 이동 허용, 근데 델타 타임이동량이 너무 크다면 ? 
			//-> 이동 허용후 한번 더 이동하는데   먹은 피자 모양의 셀이모인공간에서 길이 없는데 이동을 허용했다? 셀에 갖혀서 iCurIndex동기화가 틀어진다.->이동도 안되겠지..
		}
		*/
	
	/* 먹은 피자모양의 셀들로부터 셀에 갇히는것을 방지하기위해서 다음틱에 걍 이동량만큼 최종 위치에 존재하는 셀의 인덱스를 추적 조사하여 갈수 있는지 판단 (최종 이동량이 IN인 경우만 허용)*/
		while (true)
		{
			if (-1 == iNeighborIndex)//피자에 갇히기 싫으면 이거 꼭 필요
				return false;

			if (true == m_Cells[iNeighborIndex]->isIn(vLocalPos, iNeighborIndex))
			{
				m_iCurrentIndex = iNeighborIndex;
				return true;
			}
			//isIn이끝나면iNeighborIndex에는 연결된 다음 셀의 인덱스가 자동으로 저장 
		}
		return true;
	}
	
}
/*For Maptool*/
HRESULT CNavigation::Add_Cells(TRIANGLE_VERTICES Triangle)
{

	_float3		vPoints[3] = {};

	memcpy(vPoints, &Triangle, sizeof(TRIANGLE_VERTICES));
	
	//세 정점과 인덱스 
	CCell* pCell = CCell::Create(m_pDevice, m_pContext, vPoints, m_Cells.size());
	if (nullptr == pCell)
		return E_FAIL;

	m_Cells.push_back(pCell);

	if (FAILED(SetUp_Neighbors()))
		return E_FAIL;


	return S_OK;
}
void CNavigation::Edit_CurCell(TRIANGLE_VERTICES TriangleNew)
{
	if (m_Cells.size() > 0)
	{
		m_Cells[m_Cells.size() - 1]->AdjustPoints(TriangleNew);
	}
}
void CNavigation::Pop_Cell()
{
	if (m_Cells.size() > 0) 
	{
		Safe_Release(m_Cells[m_Cells.size() - 1]);
		m_Cells.pop_back();
	}
}
_vector CNavigation::Get_Normal_From_CurCell()
{
	if (-1 == m_iCurrentIndex)
		return _vector();
	if (m_iCurrentIndex >= m_Cells.size())
		return _vector();

	return m_Cells[m_iCurrentIndex]->Get_Normal();
	
}

_float3 CNavigation::Get_A_Point_From_CurCell(CCell::POINT _Point)
{
	return m_Cells[m_iCurrentIndex]->Get_Point(_Point);
}
//
//_vector CNavigation::Get_Line_To_Slide_From_CurCell(_fvector vObjWorldPos)
//{
//	_vector			vLocalObjPos = XMVector3TransformCoord(vObjWorldPos, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));
//	if (-1 == m_iCurrentIndex)
//		return _vector();
//	if (m_iCurrentIndex >= m_Cells.size())
//		return _vector();
//
//	return m_Cells[m_iCurrentIndex]->Get_Line_To_Slide(vLocalObjPos);
//
//}

pair<_vector, _vector> CNavigation::Get_Elements_For_Sliding_FromCurCell(_fvector vObjWorldPos)
{

		_vector			vLocalObjPos = XMVector3TransformCoord(vObjWorldPos, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));
	if (-1 == m_iCurrentIndex)
		return {_vector(), _vector()};
	if (m_iCurrentIndex >= m_Cells.size())
		return { _vector(), _vector() };

	return m_Cells[m_iCurrentIndex]->Get_Elements_For_Sliding(vLocalObjPos);
}

_bool CNavigation::Check_Near_To_Points(_fvector vObjWorldPos)
{
	_vector			vLocalObjPos = XMVector3TransformCoord(vObjWorldPos, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));
	if (-1 == m_iCurrentIndex)
		return false;
	if (m_iCurrentIndex >= m_Cells.size())
		return false;
	return m_Cells[m_iCurrentIndex]->Check_Near_To_Points(vLocalObjPos);
}

#ifdef _DEBUG

HRESULT CNavigation::Render()
{
	//테레인의 월드를 곱해서 ->네비메시를 (보이는것은) 지향으로 옯긴다 
	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	m_pShader->Begin(0);

	for (auto& pCell : m_Cells)
	{
		if (nullptr != pCell)
			pCell->Render();
	}

	return S_OK;
}

#endif

/*벡터에 셀을 다 넣었으면 네비게이션에서 neghbor을 설정하자*/
HRESULT CNavigation::SetUp_Neighbors()
{
	for (auto& pSourceCell : m_Cells)//(타겟 셀(이웃한 3개 셀을 찾아 src에 연결))
	{
		for (auto& pDestCell : m_Cells)//(타겟셀에 이웃한 3개이하의 셀을 찾기위한 이중 for문)
		{
			/*(셀의 주소가 같다면)나 자신은 넘어간다 _> 물론 3건분 모두 걉칠테니*/
			if (pSourceCell == pDestCell)
			{
				continue;
			}
			/*dest 셀이  타겟셀의 A->B 선분을 가지고 있는가?*/
			if (true == pDestCell->Compare_Points(pSourceCell->Get_Point(CCell::POINT_A), pSourceCell->Get_Point(CCell::POINT_B)))
			{
				/*AB성분 겹치니까 DEST의 인덱스를 AB에 등록해 */
				pSourceCell->Set_Neighbor(CCell::LINE_AB, pDestCell);
			}

			else if (true == pDestCell->Compare_Points(pSourceCell->Get_Point(CCell::POINT_B), pSourceCell->Get_Point(CCell::POINT_C)))
			{
				pSourceCell->Set_Neighbor(CCell::LINE_BC, pDestCell);
			}
			else if (true == pDestCell->Compare_Points(pSourceCell->Get_Point(CCell::POINT_C), pSourceCell->Get_Point(CCell::POINT_A)))
			{
				pSourceCell->Set_Neighbor(CCell::LINE_CA, pDestCell);
			}
		}
	}

	return S_OK;
}
_float CNavigation::Compute_Height(_int iCellIndex, _fvector vPosition)
{
	/* 혹시 잘못된 인덱스(-1)를 넣으면 일단 높이 0을 반환하자*/
	if (-1 == iCellIndex)
	{
		return 0;
	}

	//->셀에는 당연히 로컬 포스를 넘겨야된다.-> ComputeHeight는 로컬 포스 계산 후 네비게이션 안에서만 사용하자  
	return m_Cells[iCellIndex]->Compute_Height(vPosition);
}
CNavigation* CNavigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pNavigationDataFile)
{
	CNavigation* pInstance = new CNavigation(pDevice, pContext);

	if (nullptr == pNavigationDataFile)
	{
		if (FAILED(pInstance->Initialize_InMapTool()))
		{
			MSG_BOX("Failed to Created : CNavigation-> Initialize InMaptool");
			Safe_Release(pInstance);
		}
		return pInstance;

	}

	if (FAILED(pInstance->Initialize_Prototype(pNavigationDataFile)))
	{
		MSG_BOX("Failed to Created : CNavigation");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CNavigation::Clone(void* pArg)
{
	CNavigation* pInstance = new CNavigation(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavigation::Free()
{
	__super::Free();



#ifdef _DEBUG
	Safe_Release(m_pShader);
#endif

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);

	m_Cells.clear();
}
