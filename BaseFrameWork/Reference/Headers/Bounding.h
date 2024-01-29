#pragma once

#include "Collider.h"
#include "Engine_Defines.h"
#include "DebugDraw.h"


BEGIN(Engine)

class CBounding abstract
{

public:
	CBounding(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);
	virtual ~CBounding() = default;

public:
	virtual void Tick(_fmatrix _WorldMatrix) = 0;
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch) = 0;
	virtual _bool Intersect(CCollider::TYPE _eType, shared_ptr<CBounding> _pBounding) = 0;

//하위에서 재정의해서 사용하기
public:
	virtual shared_ptr<BoundingSphere> GetBoundingSphere() { return nullptr; };
	virtual shared_ptr<BoundingBox> GetBoundingAABB() { return nullptr; };
	virtual shared_ptr<BoundingBox> GetBoundingOBB() { return nullptr; };

protected:

	wrl::ComPtr<ID3D11Device> m_pDevice = { nullptr };
	wrl::ComPtr<ID3D11DeviceContext> m_pContext = { nullptr };

	_bool m_isColl = { false };
};

END