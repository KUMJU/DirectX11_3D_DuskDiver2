#include "RenderTarget.h"
#include "Shader.h"
#include "VIRect.h"

CRenderTarget::CRenderTarget(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
    :m_pDevice(_pDevice),
    m_pContext(_pContext)
{
}

HRESULT CRenderTarget::Initialize(_uint _iSizeX, _uint _iSizeY, DXGI_FORMAT _ePixelFormat, const _float4& _vClearColor)
{
    m_vClearColor = _vClearColor;

    D3D11_TEXTURE2D_DESC TextureDesc;
    ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

    TextureDesc.Width = _iSizeX;
    TextureDesc.Height = _iSizeY;
    TextureDesc.MipLevels = 1;
    TextureDesc.ArraySize = 1;
    TextureDesc.Format = _ePixelFormat;
    TextureDesc.SampleDesc.Quality = 0;
    TextureDesc.SampleDesc.Count = 1;
    TextureDesc.Usage = D3D11_USAGE_DEFAULT;
    TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    TextureDesc.CPUAccessFlags = 0;
    TextureDesc.MiscFlags = 0;

    if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &m_pTexture2D)))
        return E_FAIL;

    if (FAILED(m_pDevice->CreateRenderTargetView(m_pTexture2D.Get(), nullptr, &m_pRTV)))
        return E_FAIL;

    if (FAILED(m_pDevice->CreateShaderResourceView(m_pTexture2D.Get(), nullptr, &m_pSRV)))
        return E_FAIL;

    return S_OK;
}

HRESULT CRenderTarget::Clear()
{
    m_pContext->ClearRenderTargetView(m_pRTV.Get(), (_float*)&m_vClearColor);

    return S_OK;
}

HRESULT CRenderTarget::BindSRV(shared_ptr<class CShader> _pShader, const _char* _pConstantName)
{
    return _pShader->BindSRV(_pConstantName, m_pSRV);
}

#ifdef _DEBUG

HRESULT CRenderTarget::ReadyDebug(_float _fX, _float _fY, _float _fSizeX, _float _fSizeY)
{
    D3D11_VIEWPORT ViewPortDesc{};

    _uint iNumViewport = 1;
    m_pContext->RSGetViewports(&iNumViewport, &ViewPortDesc);

    XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

    m_WorldMatrix._11 = _fSizeX;
    m_WorldMatrix._22 = _fSizeY;
    m_WorldMatrix._41 = _fX - ViewPortDesc.Width * 0.5f;
    m_WorldMatrix._42 = -_fY + ViewPortDesc.Height * 0.5f;

    return S_OK;
}

HRESULT CRenderTarget::Render(shared_ptr<class CShader> _pShader, shared_ptr<class CVIRect> _pVIBuffer)
{
    if (FAILED(_pShader->BindMatrix("g_WorldMatrix", &m_WorldMatrix)))
        return E_FAIL;

    if (FAILED(_pShader->BindSRV("g_Texture", m_pSRV)))
        return E_FAIL;

    _pShader->Begin(0);
    _pVIBuffer->BindBuffers();
    _pVIBuffer->Render();

    return S_OK;
}

#endif

shared_ptr<CRenderTarget> CRenderTarget::Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, _uint _iSizeX, _uint _iSizeY, DXGI_FORMAT _ePixelFormat, const _float4& _vClearColor)
{
    shared_ptr<CRenderTarget> pInstance = make_shared<CRenderTarget>(_pDevice, _pContext);

    if (FAILED(pInstance->Initialize(_iSizeX, _iSizeY, _ePixelFormat, _vClearColor)))
        MSG_BOX("Failed to Create : CRenderTarget");

    return pInstance;
}

void CRenderTarget::Free()
{

}
