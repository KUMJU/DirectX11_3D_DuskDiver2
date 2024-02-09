#include "VIBufferPoint.h"

CVIBufferPoint::CVIBufferPoint(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
    :CVIBufferInstancing(_pDevice, _pContext)
{
}

HRESULT CVIBufferPoint::Initialize(_uint _iNumInstance, CVIBufferInstancing::INSTANCE_DESC* _pDesc)
{
	m_iNumVertexBuffers = 2;
	m_iVertexStride = sizeof(VTXPOINT);
	m_iNumVertices = 1;

	m_iNumInstance = _iNumInstance;
	m_iIndexCountPerInstance = 1;
	m_iIndexStride = 2;
	m_eIndexFormat = m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	m_iNumIndices = m_iIndexCountPerInstance * m_iNumInstance;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

#pragma region VERTEX_BUFFER
	/* 정점버퍼를 만든다.*/
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;

	VTXPOINT* pVertices = new VTXPOINT[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXPOINT) * m_iNumVertices);

	pVertices[0].vPosition = _float3(0.0f, 0.0f, 0.f);
	pVertices[0].vPSize = _float2(1.f, 1.f);

	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::CreateBuffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion

#pragma region INDEX_BUFFER
	/* 인덱스버퍼를 만든다.*/
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	_ushort* pIndices = new _ushort[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

	m_InitialData.pSysMem = pIndices;

	if (FAILED(__super::CreateBuffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion

#pragma region INSTANCE_BUFFER

	if (FAILED(__super::Initialize(_pDesc)))
		return E_FAIL;

	m_iInstanceStride = sizeof(VTXINSTANCE);

	/* 정점버퍼를 만든다.*/
	m_BufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;

	VTXINSTANCE* pVertices2 = new VTXINSTANCE[m_iNumInstance];
	ZeroMemory(pVertices2, sizeof(VTXINSTANCE) * m_iNumInstance);

	uniform_real_distribution<float>	SizeRange(m_InstanceData.vSize.x, m_InstanceData.vSize.y);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		_float		fSize = SizeRange(m_RandomNumber);
		pVertices2[i].vRight = _float4(fSize, 0.f, 0.f, 0.f);
		pVertices2[i].vUp = _float4(0.f, fSize, 0.f, 0.f);
		pVertices2[i].vLook = _float4(0.f, 0.f, fSize, 0.f);
		pVertices2[i].vTranslation = _float4(
			m_InstanceData.vCenter.x + m_InitPositions[i].x,
			m_InstanceData.vCenter.y + m_InitPositions[i].y,
			m_InstanceData.vCenter.z + m_InitPositions[i].z,
			1.f);

		pVertices2[i].vColor = m_InstanceData.vColor;
		pVertices2[i].vColor.w = 0;
	}

	m_InitialData.pSysMem = pVertices2;

	if (FAILED(__super::CreateBuffer(&m_pVBInstance)))
		return E_FAIL;

	Safe_Delete_Array(pVertices2);


#pragma endregion


    return S_OK;
}

shared_ptr<CVIBufferPoint> CVIBufferPoint::Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, _uint _iNumInstance, CVIBufferInstancing::INSTANCE_DESC* _pDesc)
{
    shared_ptr<CVIBufferPoint> pInstance = make_shared<CVIBufferPoint>(_pDevice, _pContext);

    if (FAILED(pInstance->Initialize(_iNumInstance, _pDesc)))
        MSG_BOX("Failed to Create : CVIBufferPoint");

    return pInstance;
}
