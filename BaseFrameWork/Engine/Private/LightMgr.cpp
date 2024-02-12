#include "LightMgr.h"
#include "Light.h"

CLightMgr::CLightMgr(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
    :m_pDevice(_pDevice), m_pContext(_pContext)
{
}

const LIGHT_DESC* CLightMgr::GetLightDesc(_uint _iIndex) const
{
    auto iter = m_Lights.begin();

    for (size_t i = 0; i < _iIndex; ++i)
        ++iter;

    return (*iter)->GetLightDesc();
}

HRESULT CLightMgr::Initialize()
{
    return S_OK;
}

HRESULT CLightMgr::AddLight(const LIGHT_DESC& _LightDesc)
{
    shared_ptr<CLight> pLight = CLight::Create(_LightDesc);
    if (!pLight)
        return E_FAIL;

    m_Lights.push_back(pLight);

    return S_OK;
}

HRESULT CLightMgr::Render(shared_ptr<class CShader> _pShader, shared_ptr<class CVIRect> _pVIBuffer)
{
    for (auto& pLight : m_Lights)
        pLight->Render(_pShader, _pVIBuffer);

    return S_OK;
}

shared_ptr<CLightMgr> CLightMgr::Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CLightMgr> pInstance = make_shared<CLightMgr>(_pDevice, _pContext);

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CLightMgr");

    return pInstance;
}
