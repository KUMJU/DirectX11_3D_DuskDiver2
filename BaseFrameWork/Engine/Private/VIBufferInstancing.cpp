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

    EffectInitSetting();

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

_bool CVIBufferInstancing::TickInstance(_float _fTimeDelta)
{
    //Particle 재생 Loop
    if (true == m_InstanceData.isLoop) {

        D3D11_MAPPED_SUBRESOURCE			SubResource{};
        uniform_real_distribution<float>	TimeRange(m_InstanceData.vLifeTime.x, m_InstanceData.vLifeTime.y);


        m_pContext->Map(m_pVBInstance.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

        for (_uint i = 0; i < m_iNumInstance; ++i)
        {
            ((VTXINSTANCE*)SubResource.pData)[i].vTranslation.x += m_Speeds[i] * _fTimeDelta * m_Directions[i].x;
            ((VTXINSTANCE*)SubResource.pData)[i].vTranslation.y += m_Speeds[i] * _fTimeDelta * m_Directions[i].y;
            ((VTXINSTANCE*)SubResource.pData)[i].vTranslation.z += m_Speeds[i] * _fTimeDelta * m_Directions[i].z;

            m_LifeTimes[i] -= _fTimeDelta;

            if (0.0f >= m_LifeTimes[i])
            {   
                m_LifeTimes[i] = TimeRange(m_RandomNumber);
                ((VTXINSTANCE*)SubResource.pData)[i].vTranslation = _float4(
                    m_InstanceData.vCenter.x + m_InitPositions[i].x,
                    m_InstanceData.vCenter.y + m_InitPositions[i].y,
                    m_InstanceData.vCenter.z + m_InitPositions[i].z,
                    1.f);

                ((VTXINSTANCE*)SubResource.pData)[i].vColor.w = m_InstanceData.vColor.w;           
            }
        }
        m_pContext->Unmap(m_pVBInstance.Get(), 0);

    }
    //Particle 재생 Loop X 
    else {

        m_fTimeAcc += _fTimeDelta;

        if (m_InstanceData.vDuration.y <= m_fTimeAcc)
            m_isFinished = true;

        if (m_fTimeAcc >= m_InstanceData.vDuration.x) {

            D3D11_MAPPED_SUBRESOURCE			SubResource{};

            uniform_real_distribution<float>	TimeRange(m_InstanceData.vLifeTime.x, m_InstanceData.vLifeTime.y);


            m_pContext->Map(m_pVBInstance.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

            for (_uint i = 0; i < m_iNumInstance; ++i)
            {
                ((VTXINSTANCE*)SubResource.pData)[i].vTranslation.x += m_Speeds[i] * _fTimeDelta * m_Directions[i].x;
                ((VTXINSTANCE*)SubResource.pData)[i].vTranslation.y += m_Speeds[i] * _fTimeDelta * m_Directions[i].y;
                ((VTXINSTANCE*)SubResource.pData)[i].vTranslation.z += m_Speeds[i] * _fTimeDelta * m_Directions[i].z;

                ((VTXINSTANCE*)SubResource.pData)[i].vColor.w = m_InstanceData.vColor.w;

                m_LifeTimes[i] -= _fTimeDelta;
                if (0.0f >= m_LifeTimes[i])
                {
                     ((VTXINSTANCE*)SubResource.pData)[i].vColor.w = 0;
                }
            }

            m_pContext->Unmap(m_pVBInstance.Get(), 0);
        }
    }    

    return m_isFinished;

}

void CVIBufferInstancing::ResetInstance()
{


    uniform_real_distribution<float>	TimeRange(m_InstanceData.vLifeTime.x, m_InstanceData.vLifeTime.y);
    D3D11_MAPPED_SUBRESOURCE			SubResource{};
    m_pContext->Map(m_pVBInstance.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

    m_isFinished = false;
    m_fTimeAcc = 0.f;

    //LifeTime + 현재 위치값 수정 
    for (size_t i = 0; i < m_iNumInstance; ++i) {

        m_LifeTimes[i] = TimeRange(m_RandomNumber);
        ((VTXINSTANCE*)SubResource.pData)[i].vTranslation = _float4(m_InstanceData.vCenter.x + m_InitPositions[i].x , 
                                                                    m_InstanceData.vCenter.y + m_InitPositions[i].y, 
                                                                    m_InstanceData.vCenter.z + m_InitPositions[i].z,
                                                                    1.f);

        ((VTXINSTANCE*)SubResource.pData)[i].vColor.w = 0;

    }

    m_pContext->Unmap(m_pVBInstance.Get(), 0);

}

void CVIBufferInstancing::EffectInitSetting()
{
    m_InitPositions.reserve(m_iNumInstance);
    m_Directions.reserve(m_iNumInstance);
        
    //초기 시작 위치를 세팅해줌
    uniform_real_distribution<float>	StartXPos(m_InstanceData.vStartPointMin.x, m_InstanceData.vStartPointMax.x);
    uniform_real_distribution<float>	StartYPos(m_InstanceData.vStartPointMin.y, m_InstanceData.vStartPointMax.y);
    uniform_real_distribution<float>	StartZPos(m_InstanceData.vStartPointMin.z, m_InstanceData.vStartPointMax.z);


    //초기 direction을 세팅해줌
    uniform_real_distribution<float>	DirectionX(m_InstanceData.vDirectionMin.x, m_InstanceData.vDirectionMax.x);
    uniform_real_distribution<float>	DirectionY(m_InstanceData.vDirectionMin.y, m_InstanceData.vDirectionMax.y);
    uniform_real_distribution<float>	DirectionZ(m_InstanceData.vDirectionMin.z, m_InstanceData.vDirectionMax.z);

    for (_uint i = 0; i < m_iNumInstance; ++i) {
        m_InitPositions.push_back({ StartXPos(m_RandomNumber),StartYPos(m_RandomNumber), StartZPos(m_RandomNumber) });
        m_Directions.push_back({ DirectionX(m_RandomNumber),DirectionY(m_RandomNumber), DirectionZ(m_RandomNumber) });
    }
}
