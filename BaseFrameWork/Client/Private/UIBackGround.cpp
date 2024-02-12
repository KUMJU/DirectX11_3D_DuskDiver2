#include "pch.h"
#include "UIBackGround.h"
#include "GameInstance.h"

#include "Texture.h"
#include "Shader.h"
#include "VIRect.h"

#include "VIBuffer_UI.h"

CUIBackGround::CUIBackGround()
{
}

HRESULT CUIBackGround::Initialize(tagUIInfo _info, const wstring& _TextureKey, _int _iPriorityIndex)
{
	CUI::Initialize(_info, _iPriorityIndex);
	AddComponent(_TextureKey);

	return S_OK;
}

void CUIBackGround::PriorityTick(_float _fTimeDelta)
{
}

void CUIBackGround::Tick(_float _fTimeDelta)
{
}

void CUIBackGround::LateTick(_float _fTimeDelta)
{
   CUI::LateTick(_fTimeDelta);
}

HRESULT CUIBackGround::Render()
{
    if (FAILED(m_pTransformCom->BindShaderResource(m_pShader, "g_WorldMatrix")))
        return E_FAIL;
    
    _float4x4 ViewMatrix, ProjMatrix;
    XMStoreFloat4x4(&ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

    m_pShader->BindMatrix("g_ViewMatrix", &ViewMatrix);
    m_pShader->BindMatrix("g_ProjMatrix", &ProjMatrix);

    if (FAILED(m_pTextureCom->BindShaderResource(m_pShader, "g_Texture", 0)))
        return E_FAIL;

    _int iPassNum = 0;

    if (m_IsSelect)
        iPassNum = 1;

    if (FAILED(m_pShader->Begin(iPassNum)))
        return E_FAIL;

    if (FAILED(m_VIRectCom->BindBuffers()))
        return E_FAIL;

    if (FAILED(m_VIRectCom->Render()))
        return E_FAIL;

    return S_OK;
}

void CUIBackGround::AddComponent(const wstring& _TextureKey)
{
	CUI::AddBaseComponent();

	m_pTextureCom = CGameInstance::GetInstance()->GetTexture(_TextureKey);
	m_Components.emplace(TEXT("Com_Texture"), m_pTextureCom);
}

shared_ptr<CUIBackGround> CUIBackGround::Create(tagUIInfo _info, const wstring& _TextureKey, _int _iPriorityIndex)
{
    shared_ptr<CUIBackGround> pInstance = make_shared<CUIBackGround>();

    if (pInstance->Initialize(_info, _TextureKey, _iPriorityIndex))
        MSG_BOX("Failed to Create : CUIBackGround");

    return pInstance;
}
