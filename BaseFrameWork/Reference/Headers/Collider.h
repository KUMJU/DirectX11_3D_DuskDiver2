#pragma once

#include "Engine_Defines.h"
#include "Component.h"

BEGIN(Engine)


class ENGINE_DLL CCollider : public CComponent
{
public:
	enum TYPE { TYPE_SPHERE, TYPE_AABB, TYPE_OBB, TYPE_END };

public:
	CCollider(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);
	virtual ~CCollider() = default;

public:
	HRESULT Initialize();

private:

	shared_ptr<class CBounding> m_pBounding = nullptr;

public:

	static shared_ptr<CCollider> Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);


};

END