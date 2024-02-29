#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class CLight
{
public:
	CLight();
	~CLight() = default;

public:
	LIGHT_DESC* GetLightDesc() {
		return &m_LightDesc;
	}

	void SetLightDesc(LIGHT_DESC _LightDesc) {
		m_LightDesc = _LightDesc;
	}

public:
	void SetEnable(_bool _IsEnabled) { m_bEnable = _IsEnabled; }

public:
	HRESULT Initialize(const LIGHT_DESC& _LightDesc);
	HRESULT Render(shared_ptr<class CShader> _pShader, shared_ptr<class CVIRect> _pVIBuffer);

private:

	_bool m_bEnable = true;
	LIGHT_DESC m_LightDesc = {};

public:
	static shared_ptr<CLight> Create(const LIGHT_DESC& LightDesc);

};

END