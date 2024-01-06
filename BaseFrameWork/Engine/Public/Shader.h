#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CShader final
{
public:
	CShader(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);
	virtual ~CShader() = default;

public:
	virtual HRESULT Initialize(const wstring& strShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements);

public:
	HRESULT Begin(_uint iPassIndex, _uint iTechniqueIndex = 0);
	HRESULT BindRawValue(const _char* _pConstantName, const void* _pData, _uint _iLength);
	HRESULT BindMatrix(const _char* pConstantName, const _float4x4* pMatrix);
	HRESULT BindMatrices(const _char* pConstantName, const _float4x4* pMatrices, _uint iNumMatrices);
	HRESULT BindSRV(const _char* pConstantName, wrl::ComPtr<ID3D11ShaderResourceView> _pSRV);
	HRESULT BindSRVs(const _char* pConstantName, ID3D11ShaderResourceView** _pSRV, _uint _iNumTextures);

private:
	wrl::ComPtr<ID3DX11Effect> m_pEffect = { nullptr };
	vector<wrl::ComPtr<ID3D11InputLayout>> m_InputLayouts;

private:
	wrl::ComPtr<ID3D11Device> m_pDevice = nullptr;
	wrl::ComPtr<ID3D11DeviceContext> m_pContext = nullptr;

public:
	static shared_ptr<CShader> Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, const wstring& strShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements);

};

END