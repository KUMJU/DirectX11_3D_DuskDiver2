#include "Navigation.h"
#include "Cell.h"

#include "GameInstance.h"

_float4x4 CNavigation::m_WorldMatrix = {};

CNavigation::CNavigation(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
	:CComponent(_pDevice, _pContext)
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

#ifdef _DEBUG
	//m_pShader = 
//	if (nullptr == m_pShader)
	//	return E_FAIL;
#endif

	return S_OK;
}

void CNavigation::Tick(_fmatrix _TerrainWorldMatrix)
{
	XMStoreFloat4x4(&m_WorldMatrix, _TerrainWorldMatrix);
}

_bool CNavigation::IsMove(_fvector vPosition)
{
	_int		iNeighborIndex = { -1 };

	if (m_Cells[m_iCurrentIndex]->isIn(vPosition, &iNeighborIndex))
		return true;

	else
	{
		if (-1 != iNeighborIndex)
		{
			while (true)
			{
				if (-1 == iNeighborIndex)
					return false;

				if (true == m_Cells[iNeighborIndex]->isIn(vPosition, &iNeighborIndex))
				{
					m_iCurrentIndex = iNeighborIndex;
					return true;
				}
			}
		}

		else
			return false;
	}
}

HRESULT CNavigation::Render()
{
	return S_OK;
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
