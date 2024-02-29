#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class CLightMgr final
{
public: 
	CLightMgr(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);
	~CLightMgr() = default;

public:
	LIGHT_DESC* GetLightDesc(_uint _iIndex);

	void SetLightEnable(_uint _iIndex, _bool _IsEnabled);

public:
	HRESULT Initialize();
	HRESULT AddLight(const LIGHT_DESC& _LightDesc, _uint* _iIndex = nullptr);
	HRESULT Render(shared_ptr<class CShader> _pShader, shared_ptr<class CVIRect> _pVIBuffer);

private:
	wrl::ComPtr<ID3D11Device> m_pDevice = nullptr;
	wrl::ComPtr<ID3D11DeviceContext> m_pContext = nullptr;

	list<shared_ptr<class CLight>> m_Lights;

public:

	static shared_ptr<CLightMgr> Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);


};

END