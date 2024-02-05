#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_UI: public CVIBuffer
{
public:
	CVIBuffer_UI(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);
	virtual ~CVIBuffer_UI() = default;

public:
	virtual HRESULT Initialize() override;

public:
	static shared_ptr<CVIBuffer_UI> Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);

};

END