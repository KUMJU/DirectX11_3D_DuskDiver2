#include "VIRect.h"

CVIRect::CVIRect(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
    :CVIBuffer(_pDevice, _pContext)
{
}

CVIRect::CVIRect(const CVIRect& _rhs)
    :CVIBuffer(_rhs)
{
}


HRESULT CVIRect::Initialize()
{
    /*
DX11은 별도의 FVF가 존재하지 않음.
사용자가 직접 만들어서 직접 시스템에 알려줘야한다는 뜻이기도 함
*/

// 정점 버퍼 생성 순서 1 ) BufferDesc 채우기

//생성하는 버텍스 버퍼 갯수
    m_iNumVertexBuffers = 1;
    //버텍스 버퍼의 크기
    m_iVertexStride = sizeof(VTXPOSTEX);
    //생성되는 버텍스 갯수 
    m_iNumVertices = 4;

    //인덱스 버퍼 하나의 크기. short(2) 또는 int(4)를 가질 수 있다. 
    // unsinged short의 최댓값인 65,535 이하의 경우일 때는 그냥 2를 쓰는 것이 보편적
    m_iIndexStride = 2;
    m_eIndexFormat = m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
    m_iNumIndices = 6;
    m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    /****** Vertex Buffer ******/

    m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
    m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
    m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    /*보통 버퍼를 동적으로 생성할 때 사용하는 flag. 이번 생성 때는 정적 생성이기에 0으로 지정해준다*/
    m_BufferDesc.CPUAccessFlags = 0;
    m_BufferDesc.MiscFlags = 0;
    /**/

    m_BufferDesc.StructureByteStride = m_iVertexStride;

    // 정점 버퍼 생성 순서 2 ) 버퍼 초기화를 위한 버텍스 배열을 생성한 후 D3D11_SUBRESOURCE_DATA에 채운다

    //버텍스 버퍼를 생성해야하는 만큼 할당
    VTXPOSTEX* pVertices = new VTXPOSTEX[m_iNumVertices];
    ZeroMemory(pVertices, sizeof(VTXPOSTEX) * m_iNumVertices);

    pVertices[0].vPositon = _float3(-0.5f, 0.5f, 0.f);
    pVertices[0].vTexcoord = _float2(0.f, 0.f);

    pVertices[1].vPositon = _float3(0.5f, 0.5f, 0.f);
    pVertices[1].vTexcoord = _float2(1.f, 0.f);

    pVertices[2].vPositon = _float3(0.5f, -0.5f, 0.f);
    pVertices[2].vTexcoord = _float2(1.f, 1.f);

    pVertices[3].vPositon = _float3(-0.5f, -0.5f, 0.f);
    pVertices[3].vTexcoord = _float2(0.f, 1.f);

    m_InitialData.pSysMem = pVertices;

    // 정점 버퍼 생성 순서 3) 채운 내용을 기반으로 버퍼 생성 

    if (FAILED(__super::CreateBuffer(&m_pVB)))
        return E_FAIL;


    //오해하지 말자. pVertices를 통해서 m_pVB에 할당해줄뿐, pVertices에 직접 할당해주는 것이 아님
    //그래서 다 쓰고 나서 하단 부분에서 해제를 해줘야만 함 
    delete[] pVertices;
    pVertices = nullptr;


    /****** Index Buffer ******/

    m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
    m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
    m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    m_BufferDesc.CPUAccessFlags = 0;
    m_BufferDesc.MiscFlags = 0;
    //구조적 버퍼에는 stride와 같은 크기를 채워주고 나머지 다른 버퍼에는 0을 채워넣음
    m_BufferDesc.StructureByteStride = 0;

    _ushort* pIndices = new _ushort[m_iNumIndices];
    ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

    pIndices[0] = 0;
    pIndices[1] = 1;
    pIndices[2] = 2;

    pIndices[3] = 0;
    pIndices[4] = 2;
    pIndices[5] = 3;

    m_InitialData.pSysMem = pIndices;

    if (FAILED(__super::CreateBuffer(&m_pIB)))
        return E_FAIL;

    delete[] pIndices;
    pIndices = nullptr;

    return S_OK;
}

shared_ptr<CVIRect> CVIRect::Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CVIRect> pInstance = make_shared<CVIRect>(_pDevice, _pContext);

    if (FAILED(pInstance->Initialize())) {
        MSG_BOX("Failed to Created : CVIRect");
    }

    return pInstance;
}

