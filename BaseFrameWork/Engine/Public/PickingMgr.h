#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class CPickingMgr
{
public:
	struct LayInfo {
		_float3 vRayDir;
		_float3 vRayPos;
	};

public:
	CPickingMgr();
	~CPickingMgr() = default;

public:
	void Initialize(_uint _iScreenX, _uint _iScreenY);
	//Make Ray : 공통적인 Ray생성 -> 만들어진 ray를 m_tRay에 저장
	void SetRay(POINT _ptMouse);

	//터레인, 버퍼 피킹
	_float4 TerrainPicking(POINT _ptMouse, shared_ptr<class CVITerrain> _pTerrainCom, shared_ptr<class CTransform> _pTransCom);
	//오브젝트 피킹(메쉬)
	_float3 MeshPicking(POINT _ptMouse, shared_ptr<class CModel> _pMeshCom, shared_ptr<class CTransform> _pTransCom, _float& _pDistance);
	
	_bool ObjectPicking();

private:

	_uint m_iScreenX  = 0;
	_uint m_iScreenY = 0;

	LayInfo m_tRay = {};

public:

	static shared_ptr<CPickingMgr> Create(_uint _iScreenX, _uint _iScreenY);

};

END