#pragma once

#include "Bounding.h"

BEGIN(Engine)

class CBoundingOBB : public CBounding
{
public:
	CBoundingOBB(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);
	virtual ~CBoundingOBB() = default;


public:
	virtual HRESULT Initialize(CCollider::COLLIDER_DESC _desc);

	virtual void Tick(_fmatrix WorldMatrix) override;
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch) override;
	_bool Intersect(CCollider::TYPE eType, shared_ptr<CBounding> _pBounding);

public:
	virtual shared_ptr<BoundingOrientedBox> GetBoundingOBB() { return m_Box; }

private:
	shared_ptr<BoundingOrientedBox> m_pOriginalBox = { nullptr };
	shared_ptr<BoundingOrientedBox> m_Box = { nullptr };

public:
	static shared_ptr<CBoundingOBB> Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, CCollider::COLLIDER_DESC _desc);


};

END