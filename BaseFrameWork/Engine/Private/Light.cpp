#include "Light.h"
#include "Shader.h"
#include "VIRect.h"

CLight::CLight()
{
}

HRESULT CLight::Initialize(const LIGHT_DESC& _LightDesc)
{
	m_LightDesc = _LightDesc;
	return S_OK;
}

HRESULT CLight::Render(shared_ptr<class CShader> _pShader, shared_ptr<class CVIRect> _pVIBuffer)
{
	_uint		iPassIndex = { 0 };

	if (LIGHT_DESC::TYPE_DIRECTIONAL == m_LightDesc.eType)
	{
		iPassIndex = 1;

		if (FAILED(_pShader->BindRawValue("g_vLightDir", &m_LightDesc.vDirection, sizeof(_float4))))
			return E_FAIL;
	}
	else if (LIGHT_DESC::TYPE_POINT == m_LightDesc.eType)
	{
		iPassIndex = 2;

		if (FAILED(_pShader->BindRawValue("g_vLightPos", &m_LightDesc.vPosition, sizeof(_float4))))
			return E_FAIL;

		if (FAILED(_pShader->BindRawValue("g_fLightRange", &m_LightDesc.fRange, sizeof(_float))))
			return E_FAIL;

	}

	if (FAILED(_pShader->BindRawValue("g_vLightDiffuse", &m_LightDesc.vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(_pShader->BindRawValue("g_vLightAmbient", &m_LightDesc.vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(_pShader->BindRawValue("g_vLightSpecular", &m_LightDesc.vSpecular, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(_pShader->Begin(iPassIndex)))
		return E_FAIL;

	if (FAILED(_pVIBuffer->BindBuffers()))
		return E_FAIL;

	if (FAILED(_pVIBuffer->Render()))
		return E_FAIL;

	return S_OK;
}

shared_ptr<CLight> CLight::Create(const LIGHT_DESC& LightDesc)
{
	shared_ptr<CLight> pInstance = make_shared<CLight>();

	if (FAILED(pInstance->Initialize(LightDesc)))
		MSG_BOX("Failed to Create : CLight");

	return pInstance;
}
