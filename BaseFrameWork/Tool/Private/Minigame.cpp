#include "pch.h"
#include "Minigame.h"

CMinigame::CMinigame()
{
}

HRESULT CMinigame::Initialize()
{
	__super::Initialize(nullptr);
	m_IsEnabled = false;
	m_bProcessing = false;

	return S_OK;
}

void CMinigame::PriorityTick(_float _fTimeDelta)
{
}

void CMinigame::Tick(_float _fTimeDelta)
{
}

void CMinigame::LateTick(_float _fTimeDelta)
{
}

HRESULT CMinigame::Render()
{
	return S_OK;
}

void CMinigame::GameStart()
{
	m_bProcessing = true;
}

void CMinigame::GameEnd()
{
	m_bProcessing = false;
	m_bDone = true;
}
