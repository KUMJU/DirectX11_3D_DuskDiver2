#pragma once

#include "VIBufferInstancing.h"

BEGIN(Engine)


class ENGINE_DLL CVIBufferPoint : public CVIBufferInstancing
{
public:
	CVIBufferPoint(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);
	virtual ~CVIBufferPoint() = default;

public:
	virtual HRESULT Initialize(_uint _iNumInstance ,CVIBufferInstancing::INSTANCE_DESC* _pDesc);

public:
	static shared_ptr<CVIBufferPoint> Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, _uint _iNumInstance, CVIBufferInstancing::INSTANCE_DESC* _pDesc);

};

END