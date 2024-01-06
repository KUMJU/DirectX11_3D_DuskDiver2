#include "Level.h"
#include "GameInstance.h"

CLevel::CLevel()
{
}

CLevel::~CLevel()
{
    Free();
}

void CLevel::PreInit()
{
}

HRESULT CLevel::Initialize()
{
    return S_OK;
}

void CLevel::Tick(_float _fTimeDelta)
{
}

void CLevel::LateTick()
{
}

HRESULT CLevel::Render()
{
    return S_OK;
}

void CLevel::Free()
 {
}
