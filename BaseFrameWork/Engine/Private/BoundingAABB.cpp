#include "BoundingAABB.h"

CBoundingAABB::CBoundingAABB(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
    :CBounding(_pDevice, _pContext)
{
}

HRESULT CBoundingAABB::Initialize(CCollider::COLLIDER_DESC _desc)
{
    m_pOriginalBox = make_shared<BoundingBox>(_desc.vCenter, _desc.vExtents);
    m_Box = make_shared<BoundingBox>(*(m_pOriginalBox.get()));

    return S_OK;
}

void CBoundingAABB::Tick(_fmatrix WorldMatrix)
{
    _matrix		TransformMatrix = WorldMatrix;

    TransformMatrix.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVector3Length(TransformMatrix.r[0]).m128_f32[0];
    TransformMatrix.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVector3Length(TransformMatrix.r[1]).m128_f32[0];
    TransformMatrix.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVector3Length(TransformMatrix.r[2]).m128_f32[0];

    m_pOriginalBox->Transform(*(m_Box.get()), TransformMatrix);

}

HRESULT CBoundingAABB::Render(PrimitiveBatch<VertexPositionColor>* pBatch)
{

    DX::Draw(pBatch, (*m_Box.get()), m_isColl == true ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f));

    return S_OK;
}

_bool CBoundingAABB::Intersect(CCollider::TYPE eType, shared_ptr<CBounding> _pBounding)
{
    m_isColl = false;

    switch (eType)
    {
    case Engine::CCollider::TYPE_SPHERE:
    {
        shared_ptr<BoundingSphere> TagetSphere = _pBounding->GetBoundingSphere();
        m_isColl = m_Box->Intersects(*(TagetSphere.get()));

        break;
    }
    case Engine::CCollider::TYPE_AABB:

    {
        shared_ptr<BoundingBox> TargetAABB = _pBounding->GetBoundingAABB();
        m_isColl = m_Box->Intersects(*(TargetAABB.get())); 

        break;
    }
    case Engine::CCollider::TYPE_OBB:

        shared_ptr<BoundingOrientedBox> TargetOBB = _pBounding->GetBoundingOBB();
        m_isColl = m_Box->Intersects(*(TargetOBB.get()));
        break;
    }

    return m_isColl;
}

shared_ptr<CBoundingAABB> CBoundingAABB::Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, CCollider::COLLIDER_DESC _desc)
{
    shared_ptr<CBoundingAABB> pInstance = make_shared<CBoundingAABB>(_pDevice, _pContext);

    if (FAILED(pInstance->Initialize(_desc)))
        MSG_BOX("Failed to Create : CBoundingAABB");

    return pInstance;
}
