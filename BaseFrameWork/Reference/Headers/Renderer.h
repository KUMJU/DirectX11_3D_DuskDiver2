#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class CRenderer final
{
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_NONBLEND, RENDER_NONLIGHT, RENDER_GLOW, RENDER_BLEND, RENDER_UI, RENDER_END };

public:
	CRenderer(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);
	virtual ~CRenderer() = default;

public:	
	HRESULT Initialize();
	HRESULT AddRenderGroup(RENDERGROUP _eRenderGroup, shared_ptr<class CGameObject> _pGameObject);
	HRESULT AddUIRenderGroup(shared_ptr<class CGameObject> _pGameObject, _int _iPriorityIndex);
	HRESULT Render();

private:
	list<shared_ptr<class CGameObject>> m_RenderObjects[RENDER_END];

private:
	shared_ptr<class CShader> m_pShader = nullptr;
	shared_ptr<class CVIRect> m_pVIBuffer = nullptr;

	_float4x4 m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;

#ifdef _DEBUG
public:
	HRESULT AddDebugComponent(shared_ptr<class CComponent> _pComponent);

private:
	list<shared_ptr<class CComponent>> m_DebugCom;

#endif


private:
	wrl::ComPtr<ID3D11Device> m_pDevice = nullptr;
	wrl::ComPtr<ID3D11DeviceContext> m_pContext = nullptr;

private:
	HRESULT RenderPriority();
	HRESULT RenderNonBlend();
	HRESULT RenderLight();
	HRESULT RenderFinal();
	HRESULT RenderNonLight();
	HRESULT RenderGlow();
	HRESULT RenderBlend();
	HRESULT RenderUI();

#ifdef _DEBUG
	HRESULT RenderDebug();
#endif // _DEBUG


public:
	void SetDebugModeOnOff() { m_bDebugOn = !m_bDebugOn; }

private:
	_bool m_bDebugOn = true;


	_float m_fScreenWidth = 0.f;
	_float m_fScreenHeight = 0.f;

public:
	static shared_ptr<CRenderer> Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);

};

END
