#pragma once

#include "Engine_Defines.h"


BEGIN(Engine)

class CTargetMgr
{
public:
	CTargetMgr(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);
	virtual ~CTargetMgr() = default;

public:
	HRESULT Initialize();
	HRESULT AddRenderTarget(const wstring& strTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);
	HRESULT AddMRT(const wstring& strMRTTag, const wstring& strTargetTag);
	HRESULT BeginMRT(const wstring& strMRTTag);
	HRESULT End_MRT();
	HRESULT BindSRV(const wstring& _strTargetTag, shared_ptr<class CShader> _pShader, const _char* _pConstantName);

	HRESULT CreateCopyBuffer(_uint iSizeX, _uint iSizeY);

#ifdef _DEBUG
public:
	HRESULT ReadyDebug(const wstring& _strTargetTag, _float _fX, _float _fY, _float _fSizeX, _float _fSizeY);
	HRESULT RenderMRT(const wstring& _strMRTTag, shared_ptr<class CShader> _pShader, shared_ptr<class CVIRect> _pVIBuffer);
#endif

public:
	HRESULT BindBackBufferSRV(shared_ptr<class CShader> _pShader, const _char* _pConstantName);

private:

	wrl::ComPtr<ID3D11Device> m_pDevice = nullptr;
	wrl::ComPtr<ID3D11DeviceContext> m_pContext = nullptr;

	wrl::ComPtr<ID3D11Texture2D> m_pCopyRes = nullptr;
	wrl::ComPtr<ID3D11ShaderResourceView> m_pBackBufferSRV = nullptr;


	ID3D11RenderTargetView* m_pBackBufferView = { nullptr };
	wrl::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView = { nullptr };


private:
	map<const wstring, shared_ptr<class CRenderTarget>> m_RenderTargets;
	map<const wstring, list<shared_ptr<class CRenderTarget>>> m_MRTs;

private:
	shared_ptr<class CRenderTarget> FindRenderTarget(const wstring& _strTargetTag);
	list<shared_ptr<class CRenderTarget>>* FindMRT(const wstring& _strMRTTag);

public:
	static shared_ptr<CTargetMgr> Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);


};

END