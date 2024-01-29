#include "Navigation.h"
#include "Cell.h"

#include "GameInstance.h"
#include "Shader.h"

_float4x4 CNavigation::m_WorldMatrix = {};

CNavigation::CNavigation(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
	:CComponent(_pDevice, _pContext)
{
}

CNavigation::CNavigation(const CNavigation& _rhs, _uint _iStartIdx)
	:CComponent(_rhs),
	m_Cells(_rhs.m_Cells),
	m_iCurrentIndex(_iStartIdx),
	m_pShader(_rhs.m_pShader)
{
}

HRESULT CNavigation::Initialize(const wstring& _strNavigationDataFilePath)
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	_ulong dwByte = { 0 };
	HANDLE hFile = CreateFile(_strNavigationDataFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);


	if (0 == hFile)
		return E_FAIL;

	while (true) {
		_float3 vPoints[CCell::POINT_END];

		ReadFile(hFile, vPoints, sizeof(_float3) * CCell::POINT_END, &dwByte, nullptr);

		if (0 == dwByte)
			break;

		shared_ptr<CCell> pCell = CCell::Create(m_pDevice, m_pContext, vPoints, m_Cells.size());

		if (nullptr == pCell)
			return E_FAIL;

		m_Cells.push_back(pCell);
	}

	CloseHandle(hFile);

	if (FAILED(MakeNeighbors()))
		return E_FAIL;

	m_Cells;

	int a = 4;
#ifdef _DEBUG
	m_pShader = CGameInstance::GetInstance()->GetShader(TEXT("Shader_Cell"));
//	if (nullptr == m_pShader)
	//	return E_FAIL;
#endif

	return S_OK;
}

void CNavigation::Tick(_fmatrix _TerrainWorldMatrix)
{
	XMStoreFloat4x4(&m_WorldMatrix, _TerrainWorldMatrix);
}

_bool CNavigation::IsMove(_fvector vPosition, _float& _fHeight)
{
	_int		iNeighborIndex = { -1 };

	_matrix WorldInverse =  XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix));
	_vector vPos = XMVector3TransformCoord(vPosition, WorldInverse);

	if (m_Cells[m_iCurrentIndex]->isIn(vPos, &iNeighborIndex)) {
		_fHeight = m_Cells[m_iCurrentIndex]->ComputeCellHeight(vPos);
		_vector vTemp = { 0.f,_fHeight, 0.f};
		vTemp = XMVector3TransformCoord(vTemp, XMLoadFloat4x4(&m_WorldMatrix));
		_fHeight = vTemp.m128_f32[1];
		return true;
	}

	else
	{
		if (-1 != iNeighborIndex)
		{
			while (true)
			{
				if (-1 == iNeighborIndex)
					return false;

				if (true == m_Cells[iNeighborIndex]->isIn(vPos, &iNeighborIndex))
				{
					m_iCurrentIndex = iNeighborIndex;
					_fHeight = m_Cells[iNeighborIndex]->ComputeCellHeight(vPos);

					_vector vTemp = { 0.f,_fHeight, 0.f };
					vTemp = XMVector3TransformCoord(vTemp, XMLoadFloat4x4(&m_WorldMatrix));
					_fHeight = vTemp.m128_f32[1];

					return true;
				}
			}
		}

		else
			return false;
	}
}

void CNavigation::CalcCurrentPos(_fvector _vPos)
{

	XMVector3TransformCoord(_vPos,XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));
	_matrix WorldMat = XMLoadFloat4x4(&m_WorldMatrix);

	for (auto& iter : m_Cells) {

		_float3* _pPoints = iter->GetPoints();

		_vector vPoint1 = XMVector3TransformCoord(XMLoadFloat3(&_pPoints[0]), WorldMat);
		_vector vPoint2 = XMVector3TransformCoord(XMLoadFloat3(&_pPoints[1]), WorldMat);
		_vector vPoint3 = XMVector3TransformCoord(XMLoadFloat3(&_pPoints[2]), WorldMat);

		_float fMaxX = max(max(vPoint1.m128_f32[0], vPoint2.m128_f32[0]), vPoint3.m128_f32[0]);
		_float fMinX = min(min(vPoint1.m128_f32[0], vPoint2.m128_f32[0]), vPoint3.m128_f32[0]);

		_float fMaxZ = max(max(vPoint1.m128_f32[2], vPoint2.m128_f32[2]), vPoint3.m128_f32[2]);
		_float fMinZ = min(min(vPoint1.m128_f32[2], vPoint2.m128_f32[2]), vPoint3.m128_f32[2]);

		if (_vPos.m128_f32[0] >= fMinX && _vPos.m128_f32[0] <= fMaxX &&
			_vPos.m128_f32[2] >= fMinZ && _vPos.m128_f32[2] <= fMaxZ) {

			m_iCurrentIndex =  iter->GetIndex();
			break;
		}
	}
}

HRESULT CNavigation::Render()
{
	_float4x4 identity;

	_matrix temp = XMMatrixIdentity();


	XMStoreFloat4x4(&identity, temp);

	if (FAILED(m_pShader->BindMatrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;

	_float4x4 ViewMat = CGameInstance::GetInstance()->GetTransformFloat4x4(CPipeLine::D3DTS_VIEW);

	if (FAILED(m_pShader->BindMatrix("g_ViewMatrix", &ViewMat)))
		return E_FAIL;

	_float4x4 ProjMat = CGameInstance::GetInstance()->GetTransformFloat4x4(CPipeLine::D3DTS_PROJ);

	if (FAILED(m_pShader->BindMatrix("g_ProjMatrix", &ProjMat)))
		return E_FAIL;

	_float4 CamPos = CGameInstance::GetInstance()->GetCamPosition();
	_float4 vColor = _float4(0.f, 1.f, 0.f, 1.f);

	if (FAILED(m_pShader->BindRawValue("g_vColor", &vColor, sizeof(_float4))))
		return E_FAIL;

	m_pShader->Begin(0);


	for (auto& pCell : m_Cells)
	{
		if (nullptr != pCell)
			pCell->Render();
	}


	//m_Cells[m_iCurrentIndex]->Render();

	//if (-1 == m_iCurrentIndex)
	//{
	//	for (auto& pCell : m_Cells)
	//	{
	//		if (nullptr != pCell)
	//			pCell->Render();
	//	}
	//}
	//else
	//	m_Cells[m_iCurrentIndex]->Render();

	return S_OK;
}

_float CNavigation::ComputeHeight(_fvector vPosition)
{

	return _float();
}

HRESULT CNavigation::MakeNeighbors()
{
	for (auto& pSourCell : m_Cells)
	{
		for (auto& pDestCell : m_Cells)
		{
			if (pSourCell == pDestCell)
				continue;

			if (true == pDestCell->ComparePoints(pSourCell->GetPoint(CCell::POINT_A), pSourCell->GetPoint(CCell::POINT_B)))
				pSourCell->SetNeighbor(CCell::LINE_AB, pDestCell);
			if (true == pDestCell->ComparePoints(pSourCell->GetPoint(CCell::POINT_B), pSourCell->GetPoint(CCell::POINT_C)))
				pSourCell->SetNeighbor(CCell::LINE_BC, pDestCell);
			if (true == pDestCell->ComparePoints(pSourCell->GetPoint(CCell::POINT_C), pSourCell->GetPoint(CCell::POINT_A)))
				pSourCell->SetNeighbor(CCell::LINE_CA, pDestCell);
		}
	}
	return S_OK;
}

shared_ptr<CNavigation> CNavigation::Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, const wstring& _strNavigationDataFilePath)
{
	shared_ptr<CNavigation> pInstance = make_shared<CNavigation>(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_strNavigationDataFilePath)))
		MSG_BOX("Failed to Create : CNavigation");


	return pInstance;
}

shared_ptr<CNavigation> CNavigation::Clone(shared_ptr<CNavigation> _rhs, _uint _startIndex)
{
	shared_ptr<CNavigation> pInstance = make_shared<CNavigation>(*(_rhs.get()), _startIndex);

	return pInstance;
}
