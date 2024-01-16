#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBufferCell final: public CVIBuffer
{
public:
	CVIBufferCell(wrl::ComPtr<ID3D11Device> _pDevice , wrl::ComPtr<ID3D11DeviceContext> m_pContext);
	virtual ~CVIBufferCell() = default;

public:
	virtual HRESULT Initialize(const _float3* _pPoints);

public:
	static shared_ptr<CVIBufferCell> Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> m_pContext, const _float3* _pPoints);

};

END