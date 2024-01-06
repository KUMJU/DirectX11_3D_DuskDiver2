#include "pch.h"
#include "UI.h"

#include "Shader.h"
#include "VIRect.h"

#include "GameInstance.h"

CUI::CUI()
    :CGameObject()
{
}

void CUI::PreInitialize()
{
}

HRESULT CUI::Initialize(tagUIInfo _UIinfo, _int _iPriorityIndex)
{
    if (FAILED(__super::Initialize(nullptr)))
        return E_FAIL;

    m_iPriorityIndex = _iPriorityIndex;

    m_pUIDefaultInfo = _UIinfo;
    m_pTransformCom->SetScaling(m_pUIDefaultInfo.fSizeX, m_pUIDefaultInfo.fSizeY, 1.f);
    m_pTransformCom->SetState(CTransform::STATE_POSITION, XMVectorSet(m_pUIDefaultInfo.fX - g_iWinSizeX * 0.5f, -m_pUIDefaultInfo.fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

    return S_OK;
}

void CUI::PriorityTick(_float _fTimeDelta)
{
}

void CUI::Tick(_float _fTimeDelta)
{
}

void CUI::LateTick(_float _fTimeDelta)
{
    if (FAILED(CGameInstance::GetInstance()->AddUIRenderGroup(shared_from_this(), m_iPriorityIndex)))
        return;
}

HRESULT CUI::Render()
{
    return S_OK;
}

void CUI::AddBaseComponent()
{
    wrl::ComPtr<ID3D11Device> pDevice = CGameInstance::GetInstance()->GetDeviceInfo();
    wrl::ComPtr<ID3D11DeviceContext> pContext = CGameInstance::GetInstance()->GetDeviceContextInfo();

    m_VIRectCom = CVIRect::Create(pDevice, pContext);
    m_Components.emplace(TEXT("Com_VIRect"), m_VIRectCom);

    m_pShader = CGameInstance::GetInstance()->GetShader(TEXT("Shader_VtxPosTex"));
}
