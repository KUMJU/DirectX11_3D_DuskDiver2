#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class CRenderTarget
{
public:
	CRenderTarget(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);
	virtual ~CRenderTarget() = default;

public:
	
	wrl::ComPtr<ID3D11RenderTargetView> GetRTV() const {
		return m_pRTV;
	}

public:
	HRESULT Initialize(_uint _iSizeX, _uint _iSizeY, DXGI_FORMAT _ePixelFormat, const _float4& _vClearColor);
	HRESULT Clear();
	HRESULT BindSRV(shared_ptr<class CShader> _pShader, const _char* _pConstantName);


#ifdef _DEBUG
public:
	HRESULT ReadyDebug(_float _fX, _float _fY, _float _fSizeX, _float _fSizeY);
	HRESULT Render(shared_ptr<class CShader> _pShader, shared_ptr<class CVIRect> _pVIBuffer);
#endif

private:
	wrl::ComPtr<ID3D11Device> m_pDevice = nullptr;
	wrl::ComPtr<ID3D11DeviceContext> m_pContext = nullptr;

	wrl::ComPtr<ID3D11Texture2D> m_pTexture2D = nullptr;
	wrl::ComPtr<ID3D11RenderTargetView> m_pRTV = nullptr;
	wrl::ComPtr<ID3D11ShaderResourceView> m_pSRV = nullptr;

	_float4 m_vClearColor = _float4();

#ifdef _DEBUG
private:
	_float4x4 m_WorldMatrix = _float4x4();
#endif

public:
	static shared_ptr<CRenderTarget> Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, _uint _iSizeX, _uint _iSizeY, DXGI_FORMAT _ePixelFormat, const _float4& _vClearColor);
	void Free();
};

END