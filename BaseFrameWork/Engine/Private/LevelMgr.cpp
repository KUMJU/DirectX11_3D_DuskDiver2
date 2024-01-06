#include "LevelMgr.h"
#include "Level.h"
#include "GameInstance.h"

CLevelMgr::CLevelMgr()
{
}

HRESULT CLevelMgr::Initialize()
{
    return S_OK;
}

void CLevelMgr::Tick(_float fTimeDelta)
{
    if (nullptr == m_pCurrentLevel)
        return;

    m_pCurrentLevel->Tick(fTimeDelta);

}

HRESULT CLevelMgr::Render()
{
    if (nullptr == m_pCurrentLevel)
        return E_FAIL;

    m_pCurrentLevel->Render();

    return S_OK;
}

HRESULT CLevelMgr::Open_Level(_uint iLevelIndex, shared_ptr<class CLevel> pNewLevel)
{
    if (m_pCurrentLevel) {
        CGameInstance::GetInstance()->Clear(m_iLevelIndex);
        m_pCurrentLevel.reset();
    }

    m_pCurrentLevel = pNewLevel;
    m_iLevelIndex = iLevelIndex;

    return S_OK;
}

shared_ptr<CLevelMgr> CLevelMgr::Create()
{
    shared_ptr<CLevelMgr> pInstance = make_shared<CLevelMgr>();

    if (FAILED(pInstance->Initialize())) {

        MSG_BOX("Failed to Created : CLevel Manager");
    }

    return pInstance;
}

void CLevelMgr::Free()
{
    m_pCurrentLevel.reset();

}
