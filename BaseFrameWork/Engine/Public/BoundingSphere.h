#pragma once

#include "Bounding.h"

BEGIN(Engine)

class CBoundingSphere final : public CBounding
{


public:
	CBoundingSphere(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);
	virtual ~CBoundingSphere() = default;


public:
	virtual HRESULT Initialize(CCollider::COLLIDER_DESC _desc);

	virtual void Tick(_fmatrix WorldMatrix) override;
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch) override;
	_bool Intersect(CCollider::TYPE eType, shared_ptr<CBounding> _pBounding);

public:
	virtual shared_ptr<BoundingSphere> GetBoundingSphere() { return m_Sphere; }

private:
	shared_ptr<BoundingSphere> m_pOriginalSphere = { nullptr };
	shared_ptr<BoundingSphere> m_Sphere = { nullptr };

public:
	static shared_ptr<CBoundingSphere> Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, CCollider::COLLIDER_DESC _desc);

};

END