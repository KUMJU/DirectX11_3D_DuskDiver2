#include "Texture.h"
#include "Shader.h"
#include "GameInstance.h"

CTexture::CTexture(wrl::ComPtr<ID3D11Device> pDevice,wrl::ComPtr<ID3D11DeviceContext> pContext)
	:CComponent(pDevice, pContext)
{
}


HRESULT CTexture::Initialize(const wstring& _strTextureFilePath, _uint _iNumTextures)
{
	wrl::ComPtr<ID3D11Device> pDevice = CGameInstance::GetInstance()->GetDeviceInfo();

	m_iNumTextures = _iNumTextures;
	_tchar szEXT[MAX_PATH] = TEXT("");

	_wsplitpath_s(_strTextureFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szEXT, MAX_PATH);

	for (size_t i = 0; i < _iNumTextures; i++) {

		//_tchar szTextureFilePath[MAX_PATH] = TEXT("");

		//wsprintf(szTextureFilePath, _strTextureFilePath.c_str(), i);

		HRESULT hr = { 0 };

		wrl::ComPtr<ID3D11ShaderResourceView> pSRV = { nullptr };

		if (false == lstrcmp(szEXT, TEXT(".dds")))
			hr = CreateDDSTextureFromFile(pDevice.Get(), _strTextureFilePath.c_str(), nullptr, &pSRV);
		else if (false == lstrcmp(szEXT, TEXT(".tga")))
			hr = E_FAIL;
		else
			hr = CreateWICTextureFromFile(pDevice.Get(), _strTextureFilePath.c_str(), nullptr, &pSRV);


		if (FAILED(hr))
			return E_FAIL;

		m_SRVs.push_back(pSRV);

	}

	return S_OK;
}

HRESULT CTexture::BindShaderResource(shared_ptr<class CShader> _pShader, const _char* _pConstantName, _uint _iIndex)
{
	if (_iIndex >= m_iNumTextures)
		return E_FAIL;

	return _pShader->BindSRV(_pConstantName, m_SRVs[_iIndex]);
}

HRESULT CTexture::BindShaderResources(shared_ptr<class CShader> _pShader, const _char* _pConstantName)
{
	//배열 주소 넘기듯 벡터 맨 앞 주소를 넘겨 순차적으로 돌 수 있도록함
	return _pShader->BindSRVs(_pConstantName, m_SRVs.front().GetAddressOf(), m_iNumTextures);
}

shared_ptr<CTexture> CTexture::Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, const wstring& _strTextureFilepath, _uint _iNumTextures)
{
	shared_ptr<CTexture> pInstance = make_shared<CTexture>(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_strTextureFilepath, _iNumTextures)))
		MSG_BOX("Failed to Created : CTexture");

	return pInstance;
}
