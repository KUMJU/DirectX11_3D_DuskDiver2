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
        //����ڰ� ���� ������ � ������ ���� �ִ��� �˷��ִ� ���� : ���� ��ġ(InputLayout)
       // wrl::ComPtr<ID3D11InputLayout> pInputLayout;

        //For Shader


        /*
        struct VS_IN
        {
            float3 vPos : POSITION;
            float2 vTexcoord : TEXCOORD0;

        };

        //���� ���� ����ü�� ���� � ������ �ϴ��� �˷��ִ� �κ�
        D3D11_INPUT_ELEMENT_DESC Elements[] = {
            {"POSITION", 0 , DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD0", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        */



        /*
        D3D11_INPUT_ELEMENT_DESC ���ڼ���

        1. SemanticName : ���̴����� ����ϴ� �̸�(�±�). HSLS �ǹ�ü���� ��
        2. SemanticIndex : ������ �ǹ� ü�踦 ���� ��Ұ� �� �̻� ���� ���, �� ������ �°� ���ڸ� �ο�.
            ���� ��� 4x4 ����� �ִٸ� float4 ������Ҹ� ���� ��ü�� 4���� �ִٴ� ���̴�. �̷� ��� 0, 1, 2, 3 ... < ������ �������� ������ �ο���
        3. Format : �������� ����(POSITION�� ��� 4����Ʈ float 3���̱⿡, 32��Ʈ RGB ����(FLOAT) ������ ���)
        4.InputSlot : ���޵� ���� ������ ���� ����
        5.AlignedByteOffset: ���� ��ġ Offset(TEXCOORD0�� ���, 4x3(float3)�� �ڿ� ���⿡ 12)
        6.InputSlotClass: �ν��Ͻ̿� ���̴� �κ�. �ν��Ͻ��̶�� D3D11_INPUT_PER_INSTANCE_DATA�� ���
        7.InstanceDataStepRate : �ν��Ͻ̿� ���̴� �κ�. �ν��Ͻ��� �ƴ϶�� 0�� ä���ִ´�
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
