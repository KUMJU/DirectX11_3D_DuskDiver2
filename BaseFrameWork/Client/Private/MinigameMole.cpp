#include "pch.h"
#include "MinigameMole.h"

CMinigameMole::CMinigameMole()
{
}

HRESULT CMinigameMole::Initialize(CTransform::TRANSFORM_DESC* _pDesc)
{
    return S_OK;
}

void CMinigameMole::PriorityTick(_float _fTimeDelta)
{
}

void CMinigameMole::Tick(_float _fTimeDelta)
{
}

void CMinigameMole::LateTick(_float _fTimeDelta)
{
}

HRESULT CMinigameMole::Render()
{
    return S_OK;
}

shared_ptr<CMinigameMole> CMinigameMole::Create()
{
    shared_ptr<CMinigameMole> pInstance = make_shared<CMinigameMole>();

    if (FAILED(pInstance->Initialize(nullptr)))
        MSG_BOX("Failed to Create : CMinigameMole");

    return pInstance;
}
