#include "Cell.h"

#include "VIBufferCell.h"

CCell::CCell(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
	:m_pDevice(_pDevice) , m_pContext(_pContext)
{
}

HRESULT CCell::Initialize(const _float3* _pPonints, _uint _iCellIndex)
{
	for (size_t i = 0; i < POINT_END; ++i)
		m_vPoints[i] = _pPonints[i];

	m_iIndex = _iCellIndex;

	XMStoreFloat3(&m_vNormals[LINE_AB], XMLoadFloat3(&m_vPoints[POINT_B]) - XMLoadFloat3(&m_vPoints[POINT_A]));
	_float3 vectorAB = _float3(m_vNormals[LINE_AB].z * -1.f, 0.f, m_vNormals[LINE_AB].x);
	_vector normalAB = XMLoadFloat3(&vectorAB);

	XMStoreFloat3(&m_vNormals[LINE_AB], XMVector3Normalize(normalAB));

	XMStoreFloat3(&m_vNormals[LINE_BC], XMLoadFloat3(&m_vPoints[POINT_C]) - XMLoadFloat3(&m_vPoints[POINT_B]));
	_float3 vectorBC = _float3(m_vNormals[LINE_BC].z * -1.f, 0.f, m_vNormals[LINE_BC].x);
	_vector normalBC = XMLoadFloat3(&vectorBC);
	XMStoreFloat3(&m_vNormals[LINE_BC], XMVector3Normalize(normalBC));

	XMStoreFloat3(&m_vNormals[LINE_CA], XMLoadFloat3(&m_vPoints[POINT_A]) - XMLoadFloat3(&m_vPoints[POINT_C]));
	_float3 vectorCA = _float3(m_vNormals[LINE_CA].z * -1.f, 0.f, m_vNormals[LINE_CA].x);
	_vector normalCA = XMLoadFloat3(&vectorCA);
	XMStoreFloat3(&m_vNormals[LINE_CA], XMVector3Normalize(normalCA));

#ifdef _DEBUG
	m_pVIBuffer = CVIBufferCell::Create(m_pDevice, m_pContext, m_vPoints);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;
#endif

	return S_OK;
}

_bool CCell::ComparePoints(const _float3& _vSourPoint, const _float3& _vDestPoint)
{
	if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(&_vSourPoint)))
	{
		//XMVectorEqual => 각 요소 값 하나하나를 비교한 후 요소의 위치에 0 또는 1을 넣은 벡터를 반환
		//XMVector3Equal => 벡터의 세 요소를 비교한 후 완전히 같으면 true, 아니면 false 반환
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(&_vDestPoint)))
			return true;
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(&_vDestPoint)))
			return true;

	}

	if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(&_vSourPoint)))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(&_vDestPoint)))
			return true;
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(&_vDestPoint)))
			return true;

	}

	if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(&_vSourPoint)))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(&_vDestPoint)))
			return true;
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(&_vDestPoint)))
			return true;

	}

	return false;
}

_bool CCell::isIn(_fvector _vPosition, _int* _pNeighborIndex)
{
	for (size_t i = 0; i < LINE_END; i++) {
		_vector vDir = XMVector3Normalize(_vPosition - XMLoadFloat3(&m_vPoints[i]));

		if (0 < XMVectorGetX(XMVector3Dot(XMLoadFloat3(&m_vNormals[i]), vDir)))
		{
			*_pNeighborIndex = m_iNeighborIndices[i];
			return false;
		}
	}

	return true;
}

#ifdef _DEBUG

HRESULT CCell::Render()
{
	//m_pVIBuffer->BindBuffers();
	//m_pVIBuffer->Render();

	return S_OK;
}
#endif

_float CCell::ComputeCellHeight(_fvector _vPos)
{
	_float3 vTargetPos;
	XMStoreFloat3(&vTargetPos, _vPos);

	_vector		vPlane;

	vPlane = XMPlaneFromPoints(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(& m_vPoints[POINT_C]));

	return (-vPlane.m128_f32[0] * vTargetPos.x - vPlane.m128_f32[2] * vTargetPos.z - vPlane.m128_f32[3]) / vPlane.m128_f32[1];

}

shared_ptr<CCell> CCell::Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, const _float3* pPoints, _uint iCellIndex)
{
	shared_ptr<CCell> pInstance = make_shared<CCell>(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize(pPoints, iCellIndex))) {

		MSG_BOX("Faile to Create : CCell");
	}

	return pInstance;
}
