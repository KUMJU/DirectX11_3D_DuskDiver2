#include "Collider.h"

CCollider::CCollider(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
	:CComponent(_pDevice, _pContext)
{
}

HRESULT CCollider::Initialize()
{
	return S_OK;
}

shared_ptr<CCollider> CCollider::Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CCollider> pInstance = make_shared<CCollider>();

	if (FAILED(pInstance->Initialize()))
		MSG_BOX("Failed to Create : CCollider");

	return pInstance;
}
