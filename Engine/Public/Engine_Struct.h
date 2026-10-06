#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Typedef.h"

namespace Engine
{
	typedef struct
	{
		HINSTANCE		hInstance;
		HWND			hWnd;
		bool			isWindowed;
		unsigned int	iNumLevels;
		unsigned int	iViewportWidth;
		unsigned int	iViewportHeight;

	}ENGINE_DESC;


	typedef struct
	{
		enum TYPE {TYPE_POINT, TYPE_DIRECTIONAL, TYPE_END};


		TYPE		eType;
		XMFLOAT4	vDirection; 
		XMFLOAT4	vPosition;
		float		fRange;
		XMFLOAT4	vDiffuse;
		XMFLOAT4	vAmbient;
		XMFLOAT4	vSpecular;
	}LIGHT_DESC;

	typedef struct
	{
		XMFLOAT4	vDiffuse;
		XMFLOAT4	vAmbient;
		XMFLOAT4	vSpecular;

	}METERIAL_DESC;

	/*Light.Diffuse * Mtrl.vDiffuse;*/

	typedef struct ENGINE_DLL
	{
		XMFLOAT3		vPosition;

		static const unsigned int					iNumElements = { 1 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[1];
	}VTXPOS;


	typedef struct ENGINE_DLL
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vSize;
	}VTXPOINT;

	typedef struct ENGINE_DLL
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexcoord;

		static const unsigned int					iNumElements = { 2 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[2];
	}VTXPOSTEX;

	typedef struct ENGINE_DLL
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;

		static const unsigned int					iNumElements = { 3 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXNORTEX;

	typedef struct ENGINE_DLL
	{

		XMFLOAT3	vPosition;
		XMFLOAT3    vTexcoord;

		static const unsigned int					iNumElements = { 2 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXCUBE;

	typedef struct ENGINE_DLL
	{

		XMFLOAT3	vPosition;
		XMFLOAT3    vColor;

		static const unsigned int					iNumElements = { 2 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXCUBECOL;

	/* vertex for nonamination*/
	typedef struct ENGINE_DLL
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
		XMFLOAT3		vTangent;

		static const unsigned int					iNumElements = { 4 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXMESH;
	/* vertex for animation*/
	typedef struct ENGINE_DLL
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
		XMFLOAT3		vTangent;
		XMUINT4			vBlendIndices; //메시가 가지고 있는 정점에 영향을 주는 뼈의 인덱스
		XMFLOAT4		vBlendWeights;  //그 뼈가 영향을 주는 정도를 합이 1이나오도록 배울을 배분함-> blend indices 는 0~4
		static const unsigned int					iNumElements = { 6 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXANIMMESH;

	
	//ASSIMP에서 제공하는 키프레임마다의 애니메이션 상태정보는 
	//행렬이 아니라 벡터로 SRT를 제공한다 ( 선형보간 하려면 따로 해야됨 )
	//행렬 통째로 엮여 있으면 한꺼번에 선형보간 처리를 할수 가없다 
	typedef struct
	{
		XMFLOAT3 vScale;
		XMFLOAT4 vRotation;
		XMFLOAT3 vPosition;
		float	fKeyFramePosition;
	}KEYFRAME;


	typedef struct ENGINE_DLL
	{
		XMFLOAT3 vPointA;
		XMFLOAT3 vPointB;
		XMFLOAT3 vPointC;
	}TRIANGLE_VERTICES;



	/* v가각ㄱ의 파티클인스턴스의 움직임을 로컬에서 제어하기위한 (버텍스 버퍼에 저장할것)트랜스폼 구조체*/
	typedef struct ENGINE_DLL
	{
		XMFLOAT4 vRight, vUp, vLook, vTranslation;
		XMFLOAT2 vLifeTime; //x에는 현재 lifetime 상태 ,y에는 제한값
	}VTXINSTANCE;
	typedef struct ENGINE_DLL
	{
		static const unsigned int iNumElements = { 7 };
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXRECTINSTANCE;

	typedef struct ENGINE_DLL
	{
		static const unsigned int iNumElements = { 7 };
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXPOINTINSTANCE;

	/* Trail전용 vertices*/
	typedef struct ENGINE_DLL
	{
		XMFLOAT3 vPosition;
		XMFLOAT2 vTexcoord;
		
		static const unsigned int iNumElements = { 2 };
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXTRAIL;

}


#endif // Engine_Struct_h__
