#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture : public CComponent
{
public:
	CTexture(wrl::ComPtr<ID3D11Device> pDevice, wrl::ComPtr<ID3D11DeviceContext> pContext);
	virtual ~CTexture() = default;

public:
	virtual HRESULT Initialize(const wstring& _strTextureFilePath, _uint _iNumTextures);

public:
	HRESULT BindShaderResource(shared_ptr<class CShader> _pShader, const _char* _pConstantName, _uint _iIndex);
	HRESULT BindShaderResources(shared_ptr<class CShader> _pShader, const _char* _pConstantName);

private:
	_uint m_iNumTextures = { 0 };
	vector<wrl::ComPtr<ID3D11ShaderResourceView>> m_SRVs;

public:
	static shared_ptr<CTexture> Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, const wstring& _strTextureFilepath, _uint _iNumTextures);

};

END