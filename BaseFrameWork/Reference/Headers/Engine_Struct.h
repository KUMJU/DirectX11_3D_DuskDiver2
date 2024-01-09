#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Macro.h"

namespace Engine
{
	typedef struct 
	{	
		HWND			hWnd;
		unsigned int	iWinSizeX, iWinSizeY;
		bool			isWindowed;
	} GRAPHIC_DESC;


	typedef struct
	{
		enum TYPE { TYPE_DIRECTIONAL, TYPE_POINT, TYPE_END };

		TYPE			eType;
		XMFLOAT4		vDirection;
		XMFLOAT4		vPosition;
		float			fRange;
		XMFLOAT4		vDiffuse;
		XMFLOAT4		vAmbient;
		XMFLOAT4		vSpecular;
	
	}LIGHT_DESC;

	typedef struct ENGINE_DLL VTXPOSTEX
	{
		XMFLOAT3 vPositon;
		XMFLOAT2 vTexcoord;

		static const unsigned int iNumElements = { 2 };
		static const D3D11_INPUT_ELEMENT_DESC Elements[2];

	}VTXPOSTEX;

	typedef struct ENGINE_DLL VTXNORTEX
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;

		static const unsigned int	iNumElements = { 3 };
		static const D3D11_INPUT_ELEMENT_DESC	Elements[3];
	}VTXNORTEX;

	typedef struct ENGINE_DLL VTXMESH
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
		XMFLOAT3		vTangent;

		static const unsigned int	iNumElements = { 4 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[4];
	}VTXMESH;

	typedef struct ENGINE_DLL VTXANIMMESH
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
		XMFLOAT3		vTangent;

		/* 이 정점은 어떤 뼈들(최대 4개)의 상태를 받아서 변환되어야하는가?! */
		/* iIndex = 각각의 메시가 사용하는 뼈들의 인덱스 */
		XMUINT4			vBlendIndices;
		/* 각각의 뼈의 상태를 몇 퍼센트나 이용할지?!(가중치 0 ~ 1) */
		/* 1 = x + y + z + w */
		XMFLOAT4		vBlendWeights;


		static const unsigned int	iNumElements = { 6 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[6];
	}VTXANIMMESH;

}

#endif // Engine_Struct_h__
