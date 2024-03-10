#include "TargetMgr.h"
#include "RenderTarget.h"
#include "Shader.h"

CTargetMgr::CTargetMgr(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
    :m_pDevice(_pDevice) ,
    m_pContext(_pContext)
{
}

HRESULT CTargetMgr::Initialize()
{
    return S_OK;
}

HRESULT CTargetMgr::AddRenderTarget(const wstring& strTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
    if (FindRenderTarget(strTargetTag))
        return E_FAIL;



    shared_ptr<CRenderTarget> pRenderTarget = CRenderTarget::Create(m_pDevice, m_pContext, iSizeX, iSizeY, ePixelFormat, vClearColor);

    if (!pRenderTarget)
        return E_FAIL;

    m_RenderTargets.emplace(strTargetTag, pRenderTarget);

    return S_OK;
}

HRESULT CTargetMgr::AddMRT(const wstring& strMRTTag, const wstring& strTargetTag)
{
    shared_ptr<CRenderTarget> pRenderTarget = FindRenderTarget(strTargetTag);
    
    if (!pRenderTarget)
        return E_FAIL;

    list<shared_ptr<CRenderTarget>>* pMRTList = FindMRT(strMRTTag);

    if (!pMRTList) {

        list<shared_ptr<CRenderTarget>> MRTList;
        MRTList.push_back(pRenderTarget);

        m_MRTs.emplace(strMRTTag, MRTList);

    }
    else {

        pMRTList->push_back(pRenderTarget);
    }

    return S_OK;
}

HRESULT CTargetMgr::BeginMRT(const wstring& strMRTTag)
{
    list<shared_ptr<CRenderTarget>>* pMRTList = FindMRT(strMRTTag);
    ID3D11RenderTargetView* pRTVs[8] = { nullptr };

    ID3D11ShaderResourceView* null[] = { nullptr };
    m_pContext->PSSetShaderResources(0, 1, null);

    m_pContext->OMGetRenderTargets(1, &m_pBackBufferView, &m_pDepthStencilView);

    _uint iNumRTVs = { 0 };

    for (auto& pRenderTarget : *pMRTList) {
        pRenderTarget->Clear();
        pRTVs[iNumRTVs++] = pRenderTarget->GetRTV().Get();
    }

    m_pContext->OMSetRenderTargets(iNumRTVs, pRTVs, m_pDepthStencilView.Get());

    return S_OK;
}

HRESULT CTargetMgr::End_MRT()
{
    m_pContext->OMSetRenderTargets(1, &m_pBackBufferView, m_pDepthStencilView.Get());

    m_pBackBufferView->Release();

    return S_OK;
}

HRESULT CTargetMgr::BindSRV(const wstring& _strTargetTag, shared_ptr<class CShader> _pShader, const _char* _pConstantName)
{
    shared_ptr<CRenderTarget> pRenderTarget = FindRenderTarget(_strTargetTag);

    if (!pRenderTarget)
        return E_FAIL;

    return pRenderTarget->BindSRV(_pShader, _pConstantName);
}

HRESULT CTargetMgr::CreateCopyBuffer(_uint iSizeX, _uint iSizeY)
{
    ID3D11RenderTargetView* pBackBufferRTV = nullptr;
    m_pContext->OMGetRenderTargets(1, &pBackBufferRTV, nullptr);

    D3D11_RENDER_TARGET_VIEW_DESC pBackBufferDesc;
    pBackBufferRTV->GetDesc(&pBackBufferDesc);

    D3D11_TEXTURE2D_DESC tBackBufferCopyDesc;
    ZeroMemory(&tBackBufferCopyDesc, sizeof(D3D11_TEXTURE2D_DESC));

    tBackBufferCopyDesc.Width = iSizeX;
    tBackBufferCopyDesc.Height = iSizeY;
    tBackBufferCopyDesc.MipLevels = 1;
    tBackBufferCopyDesc.ArraySize = 1;
    tBackBufferCopyDesc.Format = pBackBufferDesc.Format;
    tBackBufferCopyDesc.SampleDesc.Quality = 0;
    tBackBufferCopyDesc.SampleDesc.Count = 1;
    tBackBufferCopyDesc.Usage = D3D11_USAGE_DEFAULT;
    tBackBufferCopyDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    tBackBufferCopyDesc.CPUAccessFlags = 0;
    tBackBufferCopyDesc.MiscFlags = 0;

    if (FAILED(m_pDevice->CreateTexture2D(&tBackBufferCopyDesc, nullptr, &m_pCopyRes)))
        return E_FAIL;

    pBackBufferRTV->Release();

    return S_OK;
}

#ifdef _DEBUG

HRESULT CTargetMgr::ReadyDebug(const wstring& _strTargetTag, _float _fX, _float _fY, _float _fSizeX, _float _fSizeY)
{
    shared_ptr<CRenderTarget> pRenderTarget = FindRenderTarget(_strTargetTag);

    if (!pRenderTarget)
        return E_FAIL;

    return pRenderTarget->ReadyDebug(_fX, _fY, _fSizeX, _fSizeY);
}

HRESULT CTargetMgr::RenderMRT(const wstring& _strMRTTag, shared_ptr<class CShader> _pShader, shared_ptr<class CVIRect> _pVIBuffer)
{
    list<shared_ptr<CRenderTarget>>* pMRTList = FindMRT(_strMRTTag);

    for (auto& pRenderTarget : *pMRTList) {
        pRenderTarget->Render(_pShader, _pVIBuffer);
    }

    return S_OK;
}

#endif


HRESULT CTargetMgr::BindBackBufferSRV(shared_ptr<class CShader> _pShader, const _char* _pConstantName)
{
    ID3D11RenderTargetView* pBackBufferRTV = nullptr;
    m_pContext->OMGetRenderTargets(1, &pBackBufferRTV, nullptr);

    /* Get BackBuffer RenderTarget Texture. */
    ID3D11Resource* pBackBufferResource = nullptr;
    pBackBufferRTV->GetResource(&pBackBufferResource);

    /* Copy the BackBuffer Texture into "m_pBackBufferTextureCopy". */
    m_pContext->CopyResource(m_pCopyRes.Get(), pBackBufferResource);


    /* Make a Shader Resource View based on the copied "m_pBackBufferTextureCopy". */
    if (FAILED(m_pDevice->CreateShaderResourceView(m_pCopyRes.Get(), nullptr, &m_pBackBufferSRV)))
        return E_FAIL;

    pBackBufferRTV->Release();
    pBackBufferResource->Release();

    if (FAILED(_pShader->BindSRV(_pConstantName, m_pBackBufferSRV)))
        return E_FAIL;

    return S_OK;
}


shared_ptr<class CRenderTarget> CTargetMgr::FindRenderTarget(const wstring& _strTargetTag)
{
    auto iter = m_RenderTargets.find(_strTargetTag);

    if (iter == m_RenderTargets.end())
        return nullptr;

    return iter->second;
}

list<shared_ptr<class CRenderTarget>>* CTargetMgr::FindMRT(const wstring& _strMRTTag)
{
    auto iter = m_MRTs.find(_strMRTTag);

    if (iter == m_MRTs.end())
        return nullptr;
    return &iter->second;
}

shared_ptr<CTargetMgr> CTargetMgr::Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CTargetMgr> pInstnce = make_shared<CTargetMgr>(_pDevice, _pContext);

    if (FAILED(pInstnce->Initialize()))
        MSG_BOX("Failed to Create : CTargetMgr");

    return pInstnce;
}
