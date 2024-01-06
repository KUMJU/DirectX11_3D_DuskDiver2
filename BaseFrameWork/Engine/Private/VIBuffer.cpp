#include "VIBuffer.h"

CVIBuffer::CVIBuffer(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
    :CComponent(_pDevice, _pContext)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer& _rhs)
    :CComponent(_rhs)
    , m_pVB(_rhs.m_pVB)
    , m_pIB(_rhs.m_pIB)
    , m_iVertexStride(_rhs.m_iVertexStride)
    , m_iNumVertices(_rhs.m_iNumVertices)
    , m_iIndexStride(_rhs.m_iIndexStride)
    , m_iNumIndices(_rhs.m_iNumIndices)
    , m_iNumVertexBuffers(_rhs.m_iNumVertexBuffers)
    , m_eIndexFormat(_rhs.m_eIndexFormat)
    , m_ePrimitiveTopology(_rhs.m_ePrimitiveTopology)
{
}

HRESULT CVIBuffer::Initialize()
{
    return S_OK;
}

HRESULT CVIBuffer::Render()
{
    m_pContext->DrawIndexed(m_iNumIndices, 0, 0);

    return S_OK;
}

HRESULT CVIBuffer::BindBuffers()
{
    wrl::ComPtr<ID3D11Buffer> pVertexBuffers[] = {
        m_pVB,
    };

    _uint iVertexStrides[] = {
        m_iVertexStride,
    };

    _uint iOffsets[] = {
        0,
    };

    m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers->GetAddressOf(), iVertexStrides, iOffsets);
    m_pContext->IASetIndexBuffer(m_pIB.Get(), m_eIndexFormat, 0);
    m_pContext->IASetPrimitiveTopology(m_ePrimitiveTopology);

    {
        //사용자가 쓰는 정점이 어떤 성분을 갖고 있는지 알려주는 수단 : 형식 배치(InputLayout)
       // wrl::ComPtr<ID3D11InputLayout> pInputLayout;

        //For Shader


        /*
        struct VS_IN
        {
            float3 vPos : POSITION;
            float2 vTexcoord : TEXCOORD0;

        };

        //위에 만든 구조체가 각자 어떤 역할을 하는지 알려주는 부분
        D3D11_INPUT_ELEMENT_DESC Elements[] = {
            {"POSITION", 0 , DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD0", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        */



        /*
        D3D11_INPUT_ELEMENT_DESC 인자순서

        1. SemanticName : 셰이더에서 사용하는 이름(태그). HSLS 의미체계라고도 함
        2. SemanticIndex : 동일한 의미 체계를 가진 요소가 둘 이상 있을 경우, 그 순서에 맞게 숫자를 부여.
            예를 들어 4x4 행렬이 있다면 float4 구성요소를 가진 개체가 4개가 있다는 뜻이다. 이럴 경우 0, 1, 2, 3 ... < 순으로 순차적인 순서를 부여함
        3. Format : 데이터의 형식(POSITION의 경우 4바이트 float 3개이기에, 32비트 RGB 세개(FLOAT) 포맷을 사용)
        4.InputSlot : 공급될 정점 버퍼의 슬롯 색인
        5.AlignedByteOffset: 정점 위치 Offset(TEXCOORD0의 경우, 4x3(float3)의 뒤에 오기에 12)
        6.InputSlotClass: 인스턴싱에 쓰이는 부분. 인스턴싱이라면 D3D11_INPUT_PER_INSTANCE_DATA를 사용
        7.InstanceDataStepRate : 인스턴싱에 쓰이는 부분. 인스턴싱이 아니라면 0을 채워넣는다
        */


        /*
        m_pDevice->CreateInputLayout(Elements, 2, pShaderByteCode, pshaerByteLength, );
        m_pContext->IASetInputLayout(pInputLayout.Get());
        */

    }

    return S_OK;
}

HRESULT CVIBuffer::CreateBuffer(wrl::ComPtr<ID3D11Buffer>* ppBuffer)
{
    return m_pDevice->CreateBuffer(&m_BufferDesc, &m_InitialData, ppBuffer->GetAddressOf());
}
