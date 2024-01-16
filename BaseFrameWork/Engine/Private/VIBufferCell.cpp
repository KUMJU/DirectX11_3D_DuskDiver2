#include "VIBufferCell.h"

CVIBufferCell::CVIBufferCell(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
    :CVIBuffer(_pDevice, _pContext)
{
}

HRESULT CVIBufferCell::Initialize(const _float3* _pPoints)
{
    m_iNumVertexBuffers = 1;
    m_iVertexStride = sizeof(VTXPOS);
    m_iNumVertices = 3;

    m_iIndexStride = 2;
    m_eIndexFormat = m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
    m_iNumIndices = 4;
    m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;


    m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
    m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
    m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    m_BufferDesc.CPUAccessFlags = 0;
    m_BufferDesc.MiscFlags = 0;
    m_BufferDesc.StructureByteStride = m_iVertexStride;

    VTXPOS* pVertices = new VTXPOS[m_iNumVertices];
    ZeroMemory(pVertices, sizeof(VTXPOS) * m_iNumVertices);

    pVertices[0].vPosition = _pPoints[0];
    pVertices[1].vPosition = _pPoints[1];
    pVertices[2].vPosition = _pPoints[2];


    m_InitialData.pSysMem = pVertices;

    if (FAILED(__super::CreateBuffer(&m_pVB)))
        return E_FAIL;

    Safe_Delete(pVertices);

    m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
    m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
    m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    m_BufferDesc.CPUAccessFlags = 0;
    m_BufferDesc.MiscFlags = 0;
    m_BufferDesc.StructureByteStride = 0;

    _ushort* pIndices = new _ushort[m_iNumIndices];
    ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

    pIndices[0] = 0;
    pIndices[1] = 1;
    pIndices[2] = 2;
    pIndices[3] = 0;

    m_InitialData.pSysMem = pIndices;

    if (FAILED(__super::CreateBuffer(&m_pIB)))
        return E_FAIL;

    Safe_Delete_Array(pIndices);


    return S_OK;
}

shared_ptr<CVIBufferCell> CVIBufferCell::Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, const _float3* _pPoints)
{
    shared_ptr<CVIBufferCell> pInstance = make_shared<CVIBufferCell>(_pDevice, _pContext);
    
    if (FAILED(pInstance->Initialize(_pPoints)))
        MSG_BOX("Failed to Create : CVIBufferCell");

    return pInstance;
}
