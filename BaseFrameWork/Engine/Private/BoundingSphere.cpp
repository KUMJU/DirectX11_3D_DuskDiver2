#include "BoundingSphere.h"

CBoundingSphere::CBoundingSphere(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
    :CBounding(_pDevice, _pContext)
{
}

HRESULT CBoundingSphere::Initialize(CCollider::COLLIDER_DESC _desc)
{

    m_pOriginalSphere = make_shared<BoundingSphere>(_desc.vCenter, _desc.fRadius);
    m_Sphere = make_shared<BoundingSphere>(*(m_pOriginalSphere.get()));

    return S_OK;
}

void CBoundingSphere::Tick(_fmatrix WorldMatrix)
{
    m_pOriginalSphere->Transform(*(m_Sphere.get()), WorldMatrix);

}

HRESULT CBoundingSphere::Render(PrimitiveBatch<VertexPositionColor>* pBatch)
{
    DX::Draw(pBatch, (*m_Sphere.get()), m_isColl == true ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f));
    return S_OK;
}

_bool CBoundingSphere::Intersect(CCollider::TYPE eType, shared_ptr<CBounding> _pBounding)
{
    m_isColl = false;

    switch (eType)
    {
    case Engine::CCollider::TYPE_SPHERE:
    {
        shared_ptr<BoundingSphere> TagetSphere = _pBounding->GetBoundingSphere();
        m_isColl = m_Sphere->Intersects(*(TagetSphere.get()));

        break;
    }
    case Engine::CCollider::TYPE_AABB:

    {
        shared_ptr<BoundingBox> TargetAABB = _pBounding->GetBoundingAABB();
        m_isColl = m_Sphere->Intersects(*(TargetAABB.get()));

        break;
    }
    case Engine::CCollider::TYPE_OBB:

        shared_ptr<BoundingOrientedBox> TargetOBB = _pBounding->GetBoundingOBB();
        m_isColl = m_Sphere->Intersects(*(TargetOBB.get()));

        break;
    }

    return m_isColl;
}

shared_ptr<CBoundingSphere> CBoundingSphere::Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, CCollider::COLLIDER_DESC _desc)
{
    shared_ptr<CBoundingSphere> pInstance = make_shared<CBoundingSphere>(_pDevice, _pContext);

    if (FAILED(pInstance->Initialize(_desc)))
        MSG_BOX("Failed to Create : CBoundingSphere");

    return pInstance;
}
