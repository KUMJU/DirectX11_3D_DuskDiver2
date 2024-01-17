#include "pch.h"
#include "Enemy03.h"

CEnemy03::CEnemy03()
{
}


HRESULT CEnemy03::Initialize(CTransform::TRANSFORM_DESC* _pDesc)
{
	return S_OK;
}

void CEnemy03::PriorityTick(_float _fTimeDelta)
{
}

void CEnemy03::Tick(_float _fTimeDelta)
{
}

void CEnemy03::LateTick(_float _fTimeDelta)
{
}

HRESULT CEnemy03::Render()
{
	return E_NOTIMPL;
}
