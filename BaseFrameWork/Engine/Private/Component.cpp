#include "Component.h"

CComponent::CComponent(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
	:m_pDevice(_pDevice) , m_pContext(_pContext)
{
}

CComponent::CComponent(const CComponent& rhs)
	:m_pDevice(rhs.m_pDevice), m_pContext(rhs.m_pContext)
{
}


HRESULT CComponent::Initialize()
{
	return S_OK;
}
