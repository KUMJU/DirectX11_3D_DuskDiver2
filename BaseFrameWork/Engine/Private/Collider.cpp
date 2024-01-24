#include "Collider.h"

#include "BoundingSphere.h"
#include "GameInstance.h"


CCollider::CCollider(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
	:CComponent(_pDevice, _pContext)
{
}

HRESULT CCollider::Initialize(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, CCollider::TYPE _eType, COLLIDER_DESC _desc)
{
	m_eType = _eType;

#ifdef _DEBUG
	m_pBatch = make_shared<PrimitiveBatch<VertexPositionColor>>(_pContext.Get());
	m_pEffect = make_shared<BasicEffect>(_pDevice.Get());
		//new BasicEffect(_pDevice.Get());

	m_pEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCode = { nullptr };
	size_t		iByteCodeLength = { 0 };

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &iByteCodeLength);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount,
		pShaderByteCode, iByteCodeLength, &m_pInputLayout)))
		return E_FAIL;
#endif


	switch (_eType)
	{
	case Engine::CCollider::TYPE_SPHERE:

		m_pBounding = CBoundingSphere::Create(_pDevice, _pContext, _desc);
		break;
	case Engine::CCollider::TYPE_AABB:
		break;
	case Engine::CCollider::TYPE_OBB:
		break;
	case Engine::CCollider::TYPE_END:
		break;
	default:
		break;
	}

	return S_OK;
}

void CCollider::Tick(_fmatrix _WorldMatrix)
{
	m_pBounding->Tick(_WorldMatrix);
}

HRESULT CCollider::Render()
{
	m_pEffect->SetWorld(XMMatrixIdentity());
	m_pEffect->SetView(CGameInstance::GetInstance()->GetTransformMatrix(CPipeLine::D3DTS_VIEW));
	m_pEffect->SetProjection(CGameInstance::GetInstance()->GetTransformMatrix(CPipeLine::D3DTS_PROJ));

	m_pContext->IASetInputLayout(m_pInputLayout.Get());

	m_pEffect->Apply(m_pContext.Get());

	m_pBatch->Begin();

	m_pBounding->Render(m_pBatch.get());

	m_pBatch->End();

	return S_OK;
}

_float CCollider::GetRadius()
{
	return m_pBounding->GetBoundingSphere()->Radius;
}

_bool CCollider::Intersect(TYPE _eType, shared_ptr<CBounding> _pTargetCollider)
{
	return m_pBounding->Intersect(_eType, _pTargetCollider);
}

shared_ptr<CCollider> CCollider::Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, CCollider::TYPE _eType, CCollider::COLLIDER_DESC _desc)
{
	shared_ptr<CCollider> pInstance = make_shared<CCollider>(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_pDevice, _pContext, _eType, _desc)))
		MSG_BOX("Failed to Create : CCollider");

	return pInstance;
}
