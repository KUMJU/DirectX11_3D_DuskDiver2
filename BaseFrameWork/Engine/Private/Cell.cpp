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

	_float3 vectorAB = _float3(m_vNormals[LINE_AB].z * -1.f, 0.f, m_vNormals[LINE_AB].x);
	_vector normalAB = XMLoadFloat3(&vectorAB);

	_float3 vectorBC = _float3(m_vNormals[LINE_BC].z * -1.f, 0.f, m_vNormals[LINE_BC].x);
	_vector normalBC = XMLoadFloat3(&vectorAB);

	_float3 vectorCA = _float3(m_vNormals[LINE_CA].z * -1.f, 0.f, m_vNormals[LINE_CA].x);
	_vector normalCA = XMLoadFloat3(&vectorAB);

	XMStoreFloat3(&m_vNormals[LINE_AB], XMLoadFloat3(&m_vPoints[POINT_B]) - XMLoadFloat3(&m_vPoints[POINT_A]));
	XMStoreFloat3(&m_vNormals[LINE_AB], XMVector3Normalize(normalAB));

	XMStoreFloat3(&m_vNormals[LINE_BC], XMLoadFloat3(&m_vPoints[POINT_C]) - XMLoadFloat3(&m_vPoints[POINT_B]));
	XMStoreFloat3(&m_vNormals[LINE_BC], XMVector3Normalize(normalBC));

	XMStoreFloat3(&m_vNormals[LINE_CA], XMLoadFloat3(&m_vPoints[POINT_A]) - XMLoadFloat3(&m_vPoints[POINT_C]));
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
		//XMVectorEqual => �� ��� �� �ϳ��ϳ��� ���� �� ����� ��ġ�� 0 �Ǵ� 1�� ���� ���͸� ��ȯ
		//XMVector3Equal => ������ �� ��Ҹ� ���� �� ������ ������ true, �ƴϸ� false ��ȯ
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(&_vDestPoint)))
			return true;
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(&_vDestPoint)))
			return true;

	}

	if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(&_vSourPoint)))
	{
		//XMVectorEqual => �� ��� �� �ϳ��ϳ��� ���� �� ����� ��ġ�� 0 �Ǵ� 1�� ���� ���͸� ��ȯ
		//XMVector3Equal => ������ �� ��Ҹ� ���� �� ������ ������ true, �ƴϸ� false ��ȯ
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(&_vDestPoint)))
			return true;
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(&_vDestPoint)))
			return true;

	}

	if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(&_vSourPoint)))
	{
		//XMVectorEqual => �� ��� �� �ϳ��ϳ��� ���� �� ����� ��ġ�� 0 �Ǵ� 1�� ���� ���͸� ��ȯ
		//XMVector3Equal => ������ �� ��Ҹ� ���� �� ������ ������ true, �ƴϸ� false ��ȯ
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

	//m_pVIBuffer->

	return E_NOTIMPL;
}
#endif

shared_ptr<CCell> CCell::Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, const _float3* pPoints, _uint iCellIndex)
{
	shared_ptr<CCell> pInstance = make_shared<CCell>(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize(pPoints, iCellIndex))) {

		MSG_BOX("Faile to Create : CCell");
	}

	return pInstance;
}
