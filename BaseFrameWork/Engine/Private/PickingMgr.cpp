#include "PickingMgr.h"
#include "GameInstance.h"

#include "VITerrain.h"
#include "Transform.h"

CPickingMgr::CPickingMgr()
{
}

void CPickingMgr::Initialize(_uint _iScreenX, _uint _iScreenY)
{
	m_iScreenX = _iScreenX;
	m_iScreenY = _iScreenY;
}

void CPickingMgr::SetRay(POINT _ptMouse)
{
	_vector vMousePos = { (_float)_ptMouse.x / ((_float)m_iScreenX / 2.f) - 1 , (_float)_ptMouse.y / ((_float)m_iScreenY / -2.f) + 1 , 0.f, 1.f };
	
	_matrix ProjMatInverse = CGameInstance::GetInstance()->GetTransformMatrixInverse(CPipeLine::D3DTS_PROJ);
	_matrix ViewMatInverse = CGameInstance::GetInstance()->GetTransformMatrixInverse(CPipeLine::D3DTS_VIEW);

	vMousePos = XMVector3TransformCoord(vMousePos, ProjMatInverse);

	_float4 CamPos = CGameInstance::GetInstance()->GetCamPosition();
	_vector vRayPos = XMLoadFloat4(&CamPos);

	_vector vRayDir = vMousePos;

	vRayDir = XMVector3TransformNormal(vRayDir, ViewMatInverse);

	XMStoreFloat3(&m_tRay.vRayPos, vRayPos);
	XMStoreFloat3(&m_tRay.vRayDir, XMVector3Normalize(vRayDir));

}

_float4 CPickingMgr::TerrainPicking(POINT _ptMouse, shared_ptr<class CVITerrain> _pTerrainCom, shared_ptr<class CTransform> _pTransCom)
{
	SetRay(_ptMouse);

	CVITerrain::TerrainDesc desc = _pTerrainCom->GetTerrainDesc();

	_ulong dwVtxIdx[3]{};
	_float fDist = 0.f;

	//ray를 local로 내리기
	_matrix worldMatrix = _pTransCom->GetWorldMatrix();
	worldMatrix = XMMatrixInverse(nullptr, worldMatrix);

	vector<_float3> verticesPos = _pTerrainCom->GetVerticesPos();

	_vector vRayPos = XMLoadFloat3(&m_tRay.vRayPos);
	_vector vRayDir = XMLoadFloat3(&m_tRay.vRayDir);

	vRayPos = XMVector3TransformCoord(vRayPos, worldMatrix);
	vRayDir = XMVector3TransformNormal(vRayDir, worldMatrix);

	for (_uint i = 0; i < desc.iVerticesZ - 1; ++i) {

		for (_uint j = 0; j < desc.iVerticesX -1 ; ++j) {

			_ulong iIndex = (_ulong)(i * desc.iVerticesX + j);

			//오른쪽 위
			dwVtxIdx[0] = iIndex + desc.iVerticesX;
			dwVtxIdx[1] = iIndex + desc.iVerticesX + 1;
			dwVtxIdx[2] = iIndex + 1;
		
			//반직선이 관통할때
			if (DirectX::TriangleTests::Intersects(vRayPos, vRayDir, XMLoadFloat3(&verticesPos[dwVtxIdx[1]]), XMLoadFloat3(&verticesPos[dwVtxIdx[0]]), XMLoadFloat3(&verticesPos[dwVtxIdx[2]]), fDist)) {

				_float4 pickPos = _float4(vRayPos.m128_f32[0] + vRayDir.m128_f32[0] * fDist, vRayPos.m128_f32[1] + vRayDir.m128_f32[1] * fDist, vRayPos.m128_f32[2] + vRayDir.m128_f32[2] * fDist, 1.f);
				return pickPos;
			}

			//왼쪽 아래
			dwVtxIdx[0] = iIndex + desc.iVerticesX;
			dwVtxIdx[1] = iIndex + 1;
			dwVtxIdx[2] = iIndex;
		
			if (DirectX::TriangleTests::Intersects(vRayPos, vRayDir, XMLoadFloat3(&verticesPos[dwVtxIdx[1]]), XMLoadFloat3(&verticesPos[dwVtxIdx[0]]), XMLoadFloat3(&verticesPos[dwVtxIdx[2]]), fDist)) {

				_float4 pickPos = _float4(vRayPos.m128_f32[0] + vRayDir.m128_f32[0] * fDist, vRayPos.m128_f32[1] + vRayDir.m128_f32[1] * fDist, vRayPos.m128_f32[2] + vRayDir.m128_f32[2] * fDist, 1.f);
				return pickPos;
			}
		}
	}

	return _float4();
}

_bool CPickingMgr::ObjectPicking()
{
	return _bool();
}

shared_ptr<CPickingMgr> CPickingMgr::Create(_uint _iScreenX, _uint _iScreenY)
{
	shared_ptr<CPickingMgr> pInstance = make_shared<CPickingMgr>();
	pInstance->Initialize(_iScreenX, _iScreenY);
	
	return pInstance;
}
