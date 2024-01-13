#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CComponent abstract : public std::enable_shared_from_this<CComponent>
{
public:
	CComponent(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);
	CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;

public:
	virtual HRESULT Initialize();

protected:
	wrl::ComPtr<ID3D11Device> m_pDevice;
	wrl::ComPtr<ID3D11DeviceContext> m_pContext;

};

END