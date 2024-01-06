#include "Light.h"

CLight::CLight()
{
}

HRESULT CLight::Initialize(const LIGHT_DESC& _LightDesc)
{
	m_LightDesc = _LightDesc;
	return S_OK;
}

shared_ptr<CLight> CLight::Create(const LIGHT_DESC& LightDesc)
{
	shared_ptr<CLight> pInstance = make_shared<CLight>();

	if (FAILED(pInstance->Initialize(LightDesc)))
		MSG_BOX("Failed to Create : CLight");

	return pInstance;
}
