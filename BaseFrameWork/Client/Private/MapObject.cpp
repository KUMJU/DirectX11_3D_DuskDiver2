#include "pch.h"
#include "MapObject.h"

CMapObject::CMapObject()
{
}

HRESULT CMapObject::Initialize(const wstring& _ModelKey, CTransform::TRANSFORM_DESC* _pDesc)
{
    return S_OK;
}

void CMapObject::PriorityTick(_float _fTimeDelta)
{
}

void CMapObject::Tick(_float _fTimeDelta)
{
}

void CMapObject::LateTick(_float _fTimeDelta)
{
}

HRESULT CMapObject::Render()
{
    return S_OK;
}

HRESULT CMapObject::AddComponent()
{
    return S_OK;
}

HRESULT CMapObject::BindShaderResources()
{
    return S_OK;
}

shared_ptr<CMapObject> CMapObject::Create(const wstring& _ModelKey, CTransform::TRANSFORM_DESC* _pDesc)
{
    return shared_ptr<CMapObject>();
}
