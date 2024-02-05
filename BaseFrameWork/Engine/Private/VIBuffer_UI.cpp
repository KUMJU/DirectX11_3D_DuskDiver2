#include "VIBuffer_UI.h"

CVIBuffer_UI::CVIBuffer_UI(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
    :CVIBuffer(_pDevice, _pContext)
{
}

HRESULT CVIBuffer_UI::Initialize()
{
    m_iNumVertexBuffers = 1;
    m_iVertexStride = sizeof(VTXPOINT);
    m_iNumVertices = 1;

    m_iIndexStride = 2;
    m_eIndexFormat = m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
    m_iNumIndices = 1;
    m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

    /****** Vertex Buffer ******/

    m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
    m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
    m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    m_BufferDesc.CPUAccessFlags = 0;
    m_BufferDesc.MiscFlags = 0;
    m_BufferDesc.StructureByteStride = m_iVertexStride;


    //버텍스 버퍼를 생성해야하는 만큼 할당
    VTXPOINT* pVertices = new VTXPOINT[m_iNumVertices];
    ZeroMemory(pVertices, sizeof(VTXPOINT) * m_iNumVertices);
    
    pVertices[0].vPosition = _float3(0.f, 0.f, 0.f);
    pVertices[0].vPSize = _float2(1.f, 1.f);

    m_InitialData.pSysMem = pVertices;

    if (FAILED(__super::CreateBuffer(&m_pVB)))
        return E_FAIL;

    delete[] pVertices;
    pVertices = nullptr;


    /****** Index Buffer ******/

    m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
    m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
    m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    m_BufferDesc.CPUAccessFlags = 0;
    m_BufferDesc.MiscFlags = 0;
    m_BufferDesc.StructureByteStride = sizeof(_ushort);

    _ushort* pIndices = new _ushort[m_iNumIndices];
    ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

    pIndices[0] = 0;

    m_InitialData.pSysMem = pIndices;

    if (FAILED(__super::CreateBuffer(&m_pIB)))
        return E_FAIL;

    delete[] pIndices;
    pIndices = nullptr;

    return S_OK;
}


shared_ptr<CVIBuffer_UI> CVIBuffer_UI::Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CVIBuffer_UI> pInstance = make_shared<CVIBuffer_UI>(_pDevice, _pContext);

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CVIBuffer_UI");
     
    return pInstance;
}
