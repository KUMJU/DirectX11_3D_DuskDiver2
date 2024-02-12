#include "BoundingOBB.h"

CBoundingOBB::CBoundingOBB(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
    :CBounding(_pDevice, _pContext)
{
}

HRESULT CBoundingOBB::Initialize(CCollider::COLLIDER_DESC _desc)
{


    _float4		vRotation;
    XMStoreFloat4(&vRotation, XMQuaternionRotationRollPitchYaw(_desc.vRadians.x, _desc.vRadians.y, _desc.vRadians.z));
   
    m_pOriginalBox = make_shared<BoundingOrientedBox>(_desc.vCenter, _desc.vExtents, vRotation);
    m_Box = make_shared<BoundingOrientedBox>(*(m_pOriginalBox.get()));

    return S_OK;
}

void CBoundingOBB::Tick(_fmatrix WorldMatrix)
{
    m_pOriginalBox->Transform(*m_Box, WorldMatrix);

}

HRESULT CBoundingOBB::Render(PrimitiveBatch<VertexPositionColor>* pBatch)
{
    DX::Draw(pBatch, (*m_Box.get()), m_isColl == true ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f));

    return S_OK;
}

_bool CBoundingOBB::Intersect(CCollider::TYPE eType, shared_ptr<CBounding> _pBounding)
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

shared_ptr<CBoundingOBB> CBoundingOBB::Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, CCollider::COLLIDER_DESC _desc)
{
    shared_ptr<CBoundingOBB> pInstance = make_shared<CBoundingOBB>(_pDevice, _pContext);

    if (FAILED(pInstance->Initialize(_desc)))
        MSG_BOX("Failed to Create : CBoundingOBB");

    return pInstance;
}
