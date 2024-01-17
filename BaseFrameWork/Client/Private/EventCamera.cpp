#include "pch.h"
#include "EventCamera.h"

CEventCamera::CEventCamera()
	:CCamera()
{
}

CEventCamera::~CEventCamera()
{
}

HRESULT CEventCamera::Initialize()
{
	return S_OK;
}

void CEventCamera::PriorityTick(_float fTimeDelta)
{
}

void CEventCamera::Tick(_float fTimeDelta)
{
}

void CEventCamera::LateTick(_float fTimeDelta)
{
}

HRESULT CEventCamera::Render()
{
	return S_OK;
}
