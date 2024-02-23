#include "pch.h"
#include "UICommandGame.h"

CUICommandGame::CUICommandGame()
{
}

HRESULT CUICommandGame::Initialize()
{
    return S_OK;
}

void CUICommandGame::PriorityTick(_float _fTimeDelta)
{
}

void CUICommandGame::Tick(_float _fTimeDelta)
{
}

void CUICommandGame::LateTick(_float _fTimeDelta)
{
    CUI::LateTick(_fTimeDelta);
}

HRESULT CUICommandGame::Render()
{
    return S_OK;
}

shared_ptr<CUICommandGame> CUICommandGame::Create()
{
    shared_ptr<CUICommandGame> pInstance = make_shared<CUICommandGame>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CUICommandGame");

    return pInstance;
}
