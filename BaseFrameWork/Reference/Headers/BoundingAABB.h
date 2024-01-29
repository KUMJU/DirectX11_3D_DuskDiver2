#pragma once

#include "Bounding.h"

BEGIN(Engine)

class CBoundingAABB : public CBounding
{
public:
	CBoundingAABB(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);
	virtual ~CBoundingAABB() = default;


public:
	virtual HRESULT Initialize(CCollider::COLLIDER_DESC _desc);

	virtual void Tick(_fmatrix WorldMatrix) override;
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch) override;
	_bool Intersect(CCollider::TYPE eType, shared_ptr<CBounding> _pBounding);

public:
	virtual shared_ptr<BoundingBox> GetBoundingAABB() { return m_Box; }

private:
	shared_ptr<BoundingBox> m_pOriginalBox = { nullptr };
	shared_ptr<BoundingBox> m_Box = { nullptr };

public:
	static shared_ptr<CBoundingAABB> Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, CCollider::COLLIDER_DESC _desc);




};

END