#include "PickingMgr.h"
#include "GameInstance.h"

#include "VITerrain.h"
#include "Transform.h"

#include "Mesh.h"
#include "Model.h"

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
	_vector vRayPos = { 0.f, 0.f, 0.f };
	//_vector vRayPos = XMLoadFloat4(&CamPos);

	_vector vRayDir = vMousePos - vRayPos;

	vRayPos = XMVector3TransformCoord(vRayPos, ViewMatInverse);
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
	_matrix worldMatrix = _pTransCom->GetWorldMatrixInverse();

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

_float3 CPickingMgr::MeshPicking(POINT _ptMouse, shared_ptr<class CModel> _pMeshCom, shared_ptr<class CTransform> _pTransCom, _float& _pDistance)
{
	SetRay(_ptMouse);

	_float fDist = 0.f;

	//ray를 local로 내리기
	_matrix worldMatrix = _pTransCom->GetWorldMatrixInverse();

	_vector vRayPos = XMLoadFloat3(&m_tRay.vRayPos);
	_vector vRayDir = XMLoadFloat3(&m_tRay.vRayDir);

	vRayPos = XMVector3TransformCoord(vRayPos, worldMatrix);
	vRayDir = XMVector3TransformNormal(vRayDir, worldMatrix);

	vRayDir = XMVector3Normalize(vRayDir);

	vector<shared_ptr<CMesh>> vMeshes = _pMeshCom->GetMeshes();

	_ulong iIndex = 0;

	_float fMinDistance = 99999.f;
	_float3 NearPickPos = _float3(0.f, 0.f, 0.f);

	_float4 OriginCamPos = CGameInstance::GetInstance()->GetCamPosition();
	_vector CamPos = { OriginCamPos.x, OriginCamPos.y, OriginCamPos.z };

	_vector dd = XMVector3TransformCoord(CamPos, worldMatrix);

	for (auto& iter : vMeshes) {

		vector<_float3> verticesPos = iter->GetVerticesPos();
		_uint iFaceNum = iter->GetFaceNum();
		vector<_uint> vIndicesVector = iter->GetIndicesNumVector();

		for (_uint i = 0; i < vIndicesVector.size();) {
			if (DirectX::TriangleTests::Intersects(vRayPos, vRayDir, XMLoadFloat3(&verticesPos[vIndicesVector[i++]]), XMLoadFloat3(&verticesPos[vIndicesVector[i++]]), XMLoadFloat3(&verticesPos[vIndicesVector[i++]]), fDist)) {

				_float3 pickPos = _float3(vRayPos.m128_f32[0] + vRayDir.m128_f32[0] * fDist, vRayPos.m128_f32[1] + vRayDir.m128_f32[1] * fDist, vRayPos.m128_f32[2] + vRayDir.m128_f32[2] * fDist);
				_float fDistance = XMVector3Length(XMLoadFloat3(&pickPos) - dd).m128_f32[0];


				if (fDistance < fMinDistance) {
					fMinDistance = fDistance;
					NearPickPos = pickPos;
					_pDistance = fDistance;

					NearPickPos.y += 0.003f;
				}
			}
		}
	}


	return NearPickPos;
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
