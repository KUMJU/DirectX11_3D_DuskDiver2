#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class CLight
{
public:
	CLight();
	~CLight() = default;

public:
	const LIGHT_DESC* GetLightDesc() const {
		return &m_LightDesc;
	}

public:
	HRESULT Initialize(const LIGHT_DESC& _LightDesc);

private:
	LIGHT_DESC m_LightDesc = {};

public:
	static shared_ptr<CLight> Create(const LIGHT_DESC& LightDesc);

};

END