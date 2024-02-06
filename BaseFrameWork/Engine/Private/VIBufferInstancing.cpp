#include "VIBufferInstancing.h"

CVIBufferInstancing::CVIBufferInstancing(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
    :CVIBuffer(_pDevice, _pContext)
{
}

HRESULT CVIBufferInstancing::Initialize(INSTANCE_DESC* _desc)
{
    m_InstanceData = *_desc;
    m_RandomNumber = mt19937_64(m_RandomDevice());
      
    m_Speeds.reserve(m_iNumInstance);
    m_LifeTimes.reserve(m_iNumInstance);
    
    uniform_real_distribution<float>	SpeedRange(m_InstanceData.vSpeed.x, m_InstanceData.vSpeed.y);
    uniform_real_distribution<float>	TimeRange(m_InstanceData.vLifeTime.x, m_InstanceData.vLifeTime.y);

    for (_uint i = 0; i < m_iNumInstance; ++i)
    {
        m_Speeds.push_back(SpeedRange(m_RandomNumber));
        m_LifeTimes.push_back(TimeRange(m_RandomNumber));
    }

    return S_OK;
}

HRESULT CVIBufferInstancing::Render()
{
    m_pContext->DrawIndexedInstanced(m_iIndexCountPerInstance, m_iNumInstance, 0, 0, 0);

    return S_OK;
}

HRESULT CVIBufferInstancing::Bind_Buffers()
{
    wrl::ComPtr<ID3D11Buffer> pVertexBuffers[] = {
       m_pVB,
       m_pVBInstance
    };

    _uint iVertexStrides[] = {
        m_iVertexStride,
        m_iInstanceStride
    };

    _uint iOffsets[] = {
        0,
        0
    };

    m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers->GetAddressOf(), iVertexStrides, iOffsets);
    m_pContext->IASetIndexBuffer(m_pIB.Get(), m_eIndexFormat, 0);
    m_pContext->IASetPrimitiveTopology(m_ePrimitiveTopology);

    return S_OK;
}

void CVIBufferInstancing::TickDrop(_float _fTimeDelta)
{
    m_fTimeAcc += _fTimeDelta;

    if (m_InstanceData.fDuration <= m_fTimeAcc)
        m_isFinished = true;


    D3D11_MAPPED_SUBRESOURCE			SubResource{};

    uniform_real_distribution<float>	TimeRange(m_InstanceData.vLifeTime.x, m_InstanceData.vLifeTime.y);
    uniform_real_distribution<float>	WidthRange(m_InstanceData.vRange.x * -0.5f, m_InstanceData.vRange.x * 0.5f);
    uniform_real_distribution<float>	HeightRange(m_InstanceData.vRange.y * -0.5f, m_InstanceData.vRange.y * 0.5f);
    uniform_real_distribution<float>	DepthRange(m_InstanceData.vRange.z * -0.5f, m_InstanceData.vRange.z * 0.5f);

    m_pContext->Map(m_pVBInstance.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

    for (_uint i = 0; i < m_iNumInstance; ++i)
    {
        ((VTXINSTANCE*)SubResource.pData)[i].vTranslation.y -= m_Speeds[i] * _fTimeDelta;

        m_LifeTimes[i] -= _fTimeDelta;
        if (0.0f >= m_LifeTimes[i])
        {
            if (true == m_InstanceData.isLoop)
            {
                m_LifeTimes[i] = TimeRange(m_RandomNumber);
                ((VTXINSTANCE*)SubResource.pData)[i].vTranslation = _float4(
                    m_InstanceData.vCenter.x + WidthRange(m_RandomNumber),
                    m_InstanceData.vCenter.y + HeightRange(m_RandomNumber),
                    m_InstanceData.vCenter.z + DepthRange(m_RandomNumber),
                    1.f);
            }
            else
            {
                ((VTXINSTANCE*)SubResource.pData)[i].vColor.w = 0.f;
            }
        }
    }

    m_pContext->Unmap(m_pVBInstance.Get(), 0);

}

void CVIBufferInstancing::TickSpread(_float _fTimeDelta)
{
}
