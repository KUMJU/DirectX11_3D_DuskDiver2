#include "pch.h"
#include "UIDialog.h"

#include "GameInstance.h"
#include "Shader.h"
#include "Texture.h"
#include "VIBuffer_UI.h"

#include <io.h>
#include "Json/json.h"
#include "Json/json-forwards.h"
#include <fstream>

CUIDialog::CUIDialog()
{
}

HRESULT CUIDialog::Initialize()
{
    CUI::tagUIInfo UIInfo = {};

    UIInfo.fSizeX = 540.f;
    UIInfo.fSizeY = 140.f;
    UIInfo.fX = g_iWinSizeX * 0.5f;
    UIInfo.fY = g_iWinSizeY * 0.5f + 250.f;

    __super::Initialize(UIInfo, 2);
    CUI::AddBaseComponent();

    //배경이미지
    m_pTextureCom = CGameInstance::GetInstance()->GetTexture(TEXT("hud_dialogue_bg"));
    m_Components.emplace(TEXT("Com_Texture"), m_pTextureCom);

    m_eUIGroup = CRenderer::UI_CONTENTS;

    //포트레잇
 

    m_pPortraitTexCom = CGameInstance::GetInstance()->GetTexture(TEXT("head_NA001_d"));
    m_Components.emplace(TEXT("Com_PortraitTexture"), m_pTextureCom);

    m_pPortraitTransCom = CTransform::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), nullptr);
    m_pPortraitTransCom->SetState(CTransform::STATE_POSITION, { UIInfo.fX - g_iWinSizeX * 0.5f - 185.f ,-UIInfo.fY + g_iWinSizeY * 0.5f , 0.f, 1.f });
    m_pPortraitTransCom->SetScaling(110.f, 150.f, 1.f);
    //Dialog Data Loading
    LoadDescriptionData();

    return S_OK;
}

void CUIDialog::PriorityTick(_float _fTimeDelta)
{
}

void CUIDialog::Tick(_float _fTimeDelta)
{
}

void CUIDialog::LateTick(_float _fTimeDelta)
{
    __super::LateTick(_fTimeDelta);
}

HRESULT CUIDialog::Render()
{
    _float4x4 ViewMatrix, ProjMatrix;
    XMStoreFloat4x4(&ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));


    if (FAILED(m_pTransformCom->BindShaderResource(m_pShader, "g_WorldMatrix")))
        return E_FAIL;

    m_pShader->BindMatrix("g_ViewMatrix", &ViewMatrix);
    m_pShader->BindMatrix("g_ProjMatrix", &ProjMatrix);

    if (FAILED(m_pTextureCom->BindShaderResource(m_pShader, "g_Texture", 0)))
        return E_FAIL;

    if (FAILED(m_pShader->Begin(0)))
        return E_FAIL;

    if (FAILED(m_VIRectCom->BindBuffers()))
        return E_FAIL;

    if (FAILED(m_VIRectCom->Render()))
        return E_FAIL;


    //포트레잇

    if (FAILED(m_pPortraitTexCom->BindShaderResource(m_pShader, "g_Texture", 0)))
        return E_FAIL;

    if (FAILED(m_pPortraitTransCom->BindShaderResource(m_pShader, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShader->Begin(0)))
        return E_FAIL;

    if (FAILED(m_VIRectCom->BindBuffers()))
        return E_FAIL;

    if (FAILED(m_VIRectCom->Render()))
        return E_FAIL;

   // wstring strSkillRate = to_wstring((_int)((m_fCurrentRate) * 100) + 100) + TEXT("%");
    CGameInstance::GetInstance()->RenderFont(TEXT("Font_Default_KR"), TEXT("유모"), {g_iWinSizeX * 0.5f - 125.f ,g_iWinSizeY * 0.5f + 203.f}, Colors::Aquamarine);
    CGameInstance::GetInstance()->RenderFont(TEXT("Font_Default_KR_12pt"), TEXT("다이얼로그 테스트입니다.\n줄바꿈도 해보겠습니다"), { g_iWinSizeX * 0.5f - 125.f ,g_iWinSizeY * 0.5f + 240.f }, Colors::White);


    return S_OK;
}

HRESULT CUIDialog::LoadDescriptionData()
{
    wstring strExt = TEXT("*.*");
    wstring strFullPath = m_strBasePath + strExt;

    _wfinddata_t fd;
    intptr_t lHandle;

    if (-1 == (lHandle = _wfindfirst(strFullPath.c_str(), &fd)))
        return E_FAIL;

    while (0 == _wfindnext(lHandle, &fd))
    {
        const wstring& strName = fd.name;

        if (IsFileOrDir(fd) && fd.size != 0 && fd.name[0] != '.') {
            wstring strName = fd.name;
            wstring strExt = EraseExt(strName);
            wstring strFilePath = m_strBasePath + strName;
            ReadData(strFilePath, strName);
        }
    }

    _findclose(lHandle);
    return S_OK;
}

void CUIDialog::ReadData(const wstring& _strFullPath, const wstring& _strKeyName)
{
    ifstream in(_strFullPath);

    Json::Value root;

    in >> root;
    _int iElementNum = root.size();
    auto iter = root.begin();


    for (size_t i = 0; i < iElementNum; ++i) {

        string keyName = iter.key().asString();

        string ActorName = root[keyName]["Actor"].asString();
        string Script = root[keyName]["Script"].asString();


        int a = 5;

        ++iter;
    }
}

_bool CUIDialog::IsFileOrDir(_wfinddata_t _fd)
{
    if (_fd.attrib & _A_SUBDIR)
        return false; //디렉토리
    else
        return true; //파일

}

wstring CUIDialog::EraseExt(const wstring& _strFileName)
{
    size_t iCount = _strFileName.find_last_of(TEXT("."));
    return _strFileName.substr(0, iCount);
}


shared_ptr<CUIDialog> CUIDialog::Create()
{
    shared_ptr<CUIDialog> pInstance = make_shared<CUIDialog>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CUIDialog");

    return pInstance;
}
