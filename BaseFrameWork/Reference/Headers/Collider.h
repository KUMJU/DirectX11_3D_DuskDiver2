#pragma once

#include "Engine_Defines.h"
#include "Component.h"

BEGIN(Engine)


class ENGINE_DLL CCollider : public CComponent
{
public:
	struct COLLIDER_DESC {

		_float3 vExtents; //for AABB 
		_float fRadius; //for Sphere
		_float3 vRadians; //for OBB
		_float3 vCenter;
	};

public:
	enum TYPE { TYPE_SPHERE, TYPE_AABB, TYPE_OBB, TYPE_END };

public:
	enum EOBJTYPE { OBJ_PLAYER, OBJ_MONSTER, OBJ_PLRPROJ, OBJ_MONPROJ, OBJ_END};

public:
	CCollider(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);
	virtual ~CCollider() = default;

public:
	HRESULT Initialize(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, CCollider::TYPE _eType, COLLIDER_DESC _desc);
	void Tick(_fmatrix _WorldMatrix);
	HRESULT Render();

public:
	_float GetRadius();
	_float3 GetExtents();

public:
	_bool Intersect(TYPE _eType, shared_ptr<class CBounding> _pTargetCollider);
	shared_ptr<class CGameObject> GetOwner() { return m_pOwner.lock(); }
	void SetOwner(shared_ptr<class CGameObject> _pOwner) { m_pOwner = _pOwner; }

public:
	CCollider::TYPE GetColliderType() { return m_eType; }
	shared_ptr<class CBounding>  GetBounding() { return m_pBounding; }
	
private:

	CCollider::TYPE m_eType = { TYPE_END };
	shared_ptr<class CBounding> m_pBounding = { nullptr };

	//콜라이더의 소유 객체의 주소를 갖고 있는다
	weak_ptr<class CGameObject> m_pOwner;

//디버깅 렌더용
#ifdef _DEBUG
private:

	shared_ptr<PrimitiveBatch<VertexPositionColor>> m_pBatch = { nullptr };
	shared_ptr<BasicEffect> m_pEffect = { nullptr };
	wrl::ComPtr<ID3D11InputLayout> m_pInputLayout = { nullptr };

#endif

public:
	static shared_ptr<CCollider> Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext , CCollider::TYPE _eType, COLLIDER_DESC _desc);
};

END