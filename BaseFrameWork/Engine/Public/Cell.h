#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class CCell final
{
public:
	enum POINTS { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINES { LINE_AB, LINE_BC, LINE_CA, LINE_END };

public:
	CCell(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);
	~CCell() = default;

public:
	_float3 GetPoint(POINTS _ePoint) {
		return m_vPoints[_ePoint];
	}

	void SetNeighbor(LINES _eLine, const shared_ptr<CCell> _pNeighbor) {

		m_iNeighborIndices[_eLine] = _pNeighbor->m_iIndex;
	}

public:
	HRESULT Initialize(const _float3* _pPonints, _uint _iCellIndex);
	_bool ComparePoints(const _float3& _vSourPoint, const _float3& _vDestPoint);
	_bool isIn(_fvector _vPosition, _int* _pNeighborIndex);

#ifdef _DEBUG

public:
	HRESULT Render();
#endif

private:
	wrl::ComPtr<ID3D11Device> m_pDevice = { nullptr };
	wrl::ComPtr<ID3D11DeviceContext> m_pContext = { nullptr };

private:
	_float3 m_vPoints[POINT_END];
	_uint m_iIndex = { 0 };
	_float3 m_vNormals[LINE_END];

	_int m_iNeighborIndices[LINE_END] = { -1, -1, -1 };

#ifdef _DEBUG
private:
	shared_ptr<class CVIBufferCell> m_pVIBuffer = { nullptr };
#endif

public:
	static shared_ptr<CCell> Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, const _float3* pPoints, _uint iCellIndex);

};

END