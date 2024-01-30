#include "CustomFont.h"

CCustomFont::CCustomFont(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
	:m_pDevice(_pDevice), m_pContext(_pContext)
{
}

HRESULT CCustomFont::Initialize(const wstring& _strFontFilePath)
{
	m_pBatch = new SpriteBatch(m_pContext.Get());
	m_pFont = new SpriteFont(m_pDevice.Get(), _strFontFilePath.c_str());

	return S_OK;
}

HRESULT CCustomFont::Render(const wstring& _strText, const _float2& _vPosition, _fvector _vColor, _float _fRotation, const _float2& _vOrigin, _float _fScale)
{

	m_pBatch->Begin();
	m_pFont->DrawString(m_pBatch.Get(), _strText.c_str(), _vPosition, _vColor, _fRotation, _vOrigin, _fScale);
	m_pBatch->End();

	return S_OK;
}

shared_ptr<CCustomFont> CCustomFont::Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, const wstring& _strFontFilePath)
{
	shared_ptr<CCustomFont> pInstance = make_shared<CCustomFont>(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_strFontFilePath)))
		MSG_BOX("Failed to Create : CCustomFont");

	return pInstance;
}
