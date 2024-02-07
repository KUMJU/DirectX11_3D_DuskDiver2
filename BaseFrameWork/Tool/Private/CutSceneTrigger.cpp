#include "pch.h"
#include "CutSceneTrigger.h"

CCutSceneTrigger::CCutSceneTrigger()
{
}

HRESULT CCutSceneTrigger::Initialize(_float3 _vTriggerPos)
{
	return E_NOTIMPL;
}

void CCutSceneTrigger::PriorityTick(_float _fTimeDelta)
{
}

void CCutSceneTrigger::Tick(_float _fTimeDelta)
{
}

void CCutSceneTrigger::LateTick(_float _fTimeDelta)
{
}

HRESULT CCutSceneTrigger::Render()
{
	return E_NOTIMPL;
}

shared_ptr<CCutSceneTrigger> CCutSceneTrigger::Create(_float3 _vTriggerPos)
{
	return shared_ptr<CCutSceneTrigger>();
}
