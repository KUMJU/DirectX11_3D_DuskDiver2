#include "pch.h"
#include "Loader.h"

#include "GameInstance.h"

CLoader::CLoader()
{
}

CLoader::~CLoader()
{
    Free();
}

typedef unsigned(__stdcall* _beginthreadex_proc_type)(void*);

_uint APIENTRY wThreadMain(void* pArg)
{
    CoInitializeEx(nullptr, 0);

    CLoader* pLoader = static_cast<CLoader*>(pArg);

    pLoader->LoadingSelector();

    return 0;
}

HRESULT CLoader::Initialize(LEVEL _eNextLevel)
{
    m_eNextLevel = _eNextLevel;

    m_pDevice = CGameInstance::GetInstance()->GetDeviceInfo();
    m_pContext = CGameInstance::GetInstance()->GetDeviceContextInfo();

    InitializeCriticalSection(&m_CriticalSection);

    m_hThread = (HANDLE)_beginthreadex(nullptr, 0, wThreadMain, this, 0, nullptr);

    if (0 == m_hThread)
        return E_FAIL;

    return S_OK;
}

HRESULT CLoader::LoadingSelector()
{
    EnterCriticalSection(&m_CriticalSection);

    HRESULT hr = {};

    switch ((m_eNextLevel))
    {

    case LEVEL_LOGO:
        hr = LoadingForLogo();
        break;

    case LEVEL_ARCADE:
        hr = LoadingForArcadeMap();
        break;

    case LEVEL_EDIT:
        hr = LoadingForEdit();
        break;

    default:
        break;
    }

    if (FAILED(hr))
        return E_FAIL;

    LeaveCriticalSection(&m_CriticalSection);

    return S_OK;
}

#ifdef _DEBUG
void CLoader::Output_LoadingText()
{
    SetWindowText(g_hWnd, m_strLoadingText.c_str());
}
#endif

HRESULT CLoader::LoadingForLogo()
{
    CGameInstance::GetInstance()->LoadLevelResource(LEVEL_STATIC);

    m_strLoadingText = TEXT("텍스쳐를(을) 로딩 중 입니다.");
    m_strLoadingText = TEXT("모델를(을) 로딩 중 입니다.");
    m_strLoadingText = TEXT("셰이더를(을) 로딩 중 입니다.");
    m_strLoadingText = TEXT("객체원형를(을) 로딩 중 입니다.");
    m_strLoadingText = TEXT("로딩이 완료되었습니다.");


    m_isFinished = true;

    return S_OK;
}

HRESULT CLoader::LoadingForArcadeMap()
{    
    CGameInstance::GetInstance()->LoadLevelResource(LEVEL_ARCADE);

    m_strLoadingText = TEXT("텍스쳐를(을) 로딩 중 입니다.");
    m_strLoadingText = TEXT("모델를(을) 로딩 중 입니다.");
    m_strLoadingText = TEXT("셰이더를(을) 로딩 중 입니다.");
    m_strLoadingText = TEXT("객체원형를(을) 로딩 중 입니다.");
    m_strLoadingText = TEXT("로딩이 완료되었습니다.");


    m_isFinished = true;

    return S_OK;
}

HRESULT CLoader::LoadingForEdit()
{
    CGameInstance::GetInstance()->LoadLevelResource(LEVEL_STATIC);
    CGameInstance::GetInstance()->LoadLevelResource(LEVEL_ARCADE);

    m_isFinished = true;
    return S_OK;
}

CLoader* CLoader::Create(LEVEL eNextLevel)
{
    CLoader* pInstance = new CLoader();

    if (FAILED(pInstance->Initialize(eNextLevel)))
    {
        MSG_BOX("Failed to Created : CLoader");
        delete pInstance;
        pInstance = nullptr;
    }

    return pInstance;
}

void CLoader::Free()
{
    WaitForSingleObject(m_hThread, INFINITE);

    DeleteCriticalSection(&m_CriticalSection);
    DeleteObject(m_hThread);
    CloseHandle(m_hThread);
}
