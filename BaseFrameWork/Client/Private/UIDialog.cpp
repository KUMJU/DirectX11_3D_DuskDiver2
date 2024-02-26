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
 

    //유모
    m_pPortraitTexCom[0] = CGameInstance::GetInstance()->GetTexture(TEXT("head_NA001_d"));
    m_Components.emplace(TEXT("Com_PortraitTexture0"), m_pTextureCom);

    //메그레즈
    m_pPortraitTexCom[1] = CGameInstance::GetInstance()->GetTexture(TEXT("head_NA017_b"));
    m_Components.emplace(TEXT("Com_PortraitTexture1"), m_pTextureCom);

    //곰
    m_pPortraitTexCom[2] = CGameInstance::GetInstance()->GetTexture(TEXT("head_NA011_a"));
    m_Components.emplace(TEXT("Com_PortraitTexture2"), m_pTextureCom);

    m_pPortraitTransCom = CTransform::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), nullptr);
    m_pPortraitTransCom->SetState(CTransform::STATE_POSITION, { UIInfo.fX - g_iWinSizeX * 0.5f - 185.f ,-UIInfo.fY + g_iWinSizeY * 0.5f , 0.f, 1.f });
    m_pPortraitTransCom->SetScaling(110.f, 150.f, 1.f);
    //Dialog Data Loading
    LoadDescriptionData();

    m_IsEnabled = false;

    return S_OK;
}

void CUIDialog::PriorityTick(_float _fTimeDelta)
{
}

void CUIDialog::Tick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;

    m_fAccTime += _fTimeDelta;

    if (m_fAccTime >= 3.f) {

        m_fAccTime = 0.f;
        ChangeNextData();

    }

}

void CUIDialog::LateTick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;

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

    if (FAILED(m_pPortraitTexCom[m_iCurrentPortaitIdx]->BindShaderResource(m_pShader, "g_Texture", 0)))
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
    CGameInstance::GetInstance()->RenderFont(TEXT("Font_Default_KR"), m_iCurrentCharName, {g_iWinSizeX * 0.5f - 125.f ,g_iWinSizeY * 0.5f + 203.f}, Colors::Aquamarine);
    CGameInstance::GetInstance()->RenderFont(TEXT("Font_Default_KR_12pt"), m_iCurrentScript, { g_iWinSizeX * 0.5f - 125.f ,g_iWinSizeY * 0.5f + 240.f }, Colors::White);


    return S_OK;
}

void CUIDialog::StartDialog(const wstring& _strDialogKey)
{
    auto FindKey = m_Scripts.find(_strDialogKey);

    if (FindKey != m_Scripts.end()) {
        m_CurrentDialog =  &FindKey->second;
        m_IsEnabled = true;
        SettingDialogInfo();
        m_fAccTime = 0.f;
    }
}

void CUIDialog::ChangeNextData()
{
    ++m_iCurrentDialogIdx;

    //다이얼로그 종료 
    if ((*m_CurrentDialog).size() == m_iCurrentDialogIdx) {
        m_IsEnabled = false;
        ResetDialogInfo();
    }
    //다음 다이얼로그 재생
    else {
        SettingDialogInfo();
    }
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
            ReadData(strFilePath, strExt);
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

    vector<DialogInfo> TempVec = {};
    TempVec.reserve(iElementNum);

    for (size_t i = 0; i < iElementNum; ++i) {


        DialogInfo info = {};

        string keyName = iter.key().asString();

        string ActorName = root[keyName]["Actor"].asString();
        string Script = root[keyName]["Script"].asString();

        _tchar szFullPath[MAX_PATH] = TEXT("");
        //변환해줄 때 플래그를 CP_UTF8로 넣어주면 한글이 멀쩡하게 읽힌다! 
        MultiByteToWideChar(CP_UTF8, 0, Script.c_str(), (_int)strlen(Script.c_str()), szFullPath, MAX_PATH);

        info.strPortraitKey = ActorName;
        info.strDialog = szFullPath;

        TempVec.push_back(info);

        ++iter;
    }

    m_Scripts.emplace(_strKeyName, TempVec);


    in.close();
}

void CUIDialog::SettingDialogInfo()
{
    if ("YM" == (*m_CurrentDialog)[m_iCurrentDialogIdx].strPortraitKey) {
        m_iCurrentPortaitIdx = 0;
        m_iCurrentCharName = TEXT("유모");
    }
    else if ("MG" == (*m_CurrentDialog)[m_iCurrentDialogIdx].strPortraitKey) {
        m_iCurrentPortaitIdx = 1;
        m_iCurrentCharName = TEXT("메그레즈");

    }
    else if ("BR" == (*m_CurrentDialog)[m_iCurrentDialogIdx].strPortraitKey) {
        m_iCurrentPortaitIdx = 2;
        m_iCurrentCharName = TEXT("수상한 곰");

    }

    m_iCurrentScript = (*m_CurrentDialog)[m_iCurrentDialogIdx].strDialog;


}

void CUIDialog::ResetDialogInfo()
{
    m_iCurrentDialogIdx = 0;
    m_iCurrentDialogLastIdx = 0;
    m_fAccTime = 0.f;
    m_CurrentDialog = nullptr;
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
