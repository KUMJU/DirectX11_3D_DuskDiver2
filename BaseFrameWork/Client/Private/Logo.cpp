#include "pch.h"
#include "Logo.h"

#include "LevelLoading.h"
#include "GameInstance.h"
#include "BackGround.h"

#include "UIBackGround.h"

CLogo::CLogo()
    :CLevel()
{
}

CLogo::~CLogo()
{
}

void CLogo::PreInit()
{
}

HRESULT CLogo::Initialize()
{
    if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
        return E_FAIL;

    return S_OK;
}

void CLogo::Tick(_float _fTimeDelta)
{
    if (m_bKeyDeb) {
        m_fKeyDebTime += _fTimeDelta;

        if (m_fKeyDebTime > 0.2f) {
            m_bKeyDeb = false;
            m_fKeyDebTime = 0.f;
        }
    }

    KeyInput();
}

HRESULT CLogo::Render()
{
#ifdef _DEBUG
    SetWindowText(g_hWnd, TEXT("로고레벨입니다."));
#endif

    return S_OK;
}

HRESULT CLogo::Ready_Layer_BackGround(const wstring& strLayerTag)
{
    CUIBackGround::tagUIInfo UITagInfo;
    UITagInfo.fSizeX = g_iWinSizeX;
    UITagInfo.fSizeY = g_iWinSizeY;
    UITagInfo.fX = g_iWinSizeX * 0.5f;
    UITagInfo.fY = g_iWinSizeY * 0.5f;

    if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_LOGO, strLayerTag, CUIBackGround::Create(UITagInfo, TEXT("title_BackGround"), 0))))
        return E_FAIL;

    UITagInfo.fSizeX = 350.f;
    UITagInfo.fSizeY = 115.f;
    UITagInfo.fX = g_iWinSizeX * 0.5f - 400.f;
    UITagInfo.fY = g_iWinSizeY * 0.5f - 250.f;

    if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_LOGO, strLayerTag, CUIBackGround::Create(UITagInfo, TEXT("title_logo"), 1))))
        return E_FAIL;

    UITagInfo.fSizeX = 155.f;
    UITagInfo.fSizeY = 70.f;
    UITagInfo.fX = g_iWinSizeX * 0.5f - 400.f;
    UITagInfo.fY = g_iWinSizeY * 0.5f - 130.f;

    m_pFirstMenu = CUIBackGround::Create(UITagInfo, TEXT("Start"), 1);
    m_pFirstMenu->SetSelectChange();

    if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_LOGO, strLayerTag, m_pFirstMenu)))
        return E_FAIL;

    UITagInfo.fSizeX = 155.f;
    UITagInfo.fSizeY = 70.f;
    UITagInfo.fX = g_iWinSizeX * 0.5f - 400.f;
    UITagInfo.fY = g_iWinSizeY * 0.5f - 50.f;

    m_pSecondMenu = CUIBackGround::Create(UITagInfo, TEXT("Map"), 1);

    if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_LOGO, strLayerTag, m_pSecondMenu)))
        return E_FAIL;

    return S_OK;
}

void CLogo::KeyInput()
{

    if (GetKeyState(VK_RETURN) & 0x8000)
    {
        if (0 == m_iSelectIndex) {
            if (FAILED(CGameInstance::GetInstance()->OpenLevel(LEVEL_LOADING, CLevelLoading::Create(LEVEL_ARCADE))))
                return;
        }
        else {
            if (FAILED(CGameInstance::GetInstance()->OpenLevel(LEVEL_LOADING, CLevelLoading::Create(LEVEL_EDIT))))
                return;
        }
       
    }

    if (m_bKeyDeb)
        return;

    if ((GetKeyState(VK_UP) & 0x8000) || (GetKeyState(VK_DOWN) & 0x8000))
    {
        if (0 == m_iSelectIndex) {
            m_iSelectIndex = 1;
        }
        else {
            m_iSelectIndex = 0;
        }

        m_pFirstMenu->SetSelectChange();
        m_pSecondMenu->SetSelectChange();

        m_bKeyDeb = true;
    }


}

shared_ptr<CLogo> CLogo::Create()
{
    shared_ptr<CLogo> pInstance = make_shared<CLogo>();

    if(FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created : CLogo");
    }

    return pInstance;
}
