#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class CFontMgr
{
public:
	CFontMgr(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);
	~CFontMgr() = default;

public:
	HRESULT Initialize();
	HRESULT AddFont(const wstring& _strFontTag, const wstring& _strFontFilePath);
	HRESULT Render(const wstring& _strFontTag, const wstring& _strText, const _float2& _vPosition, _fvector _vColor = Colors::White, _float _fRotation = 0.f, const _float2& _vOrigin = _float2(0.f, 0.f), _float fScale = 1.f);

private:
	wrl::ComPtr<ID3D11Device> m_pDevice = { nullptr };
	wrl::ComPtr<ID3D11DeviceContext> m_pContext = { nullptr };


private:
	map<const wstring, shared_ptr<class CCustomFont>> m_Fonts;

private:
	shared_ptr<class CCustomFont> FindFont(const wstring& _strFontTag);

public:

	static shared_ptr<CFontMgr> Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);

};

END