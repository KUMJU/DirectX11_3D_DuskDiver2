#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class CRenderer final
{
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_NONBLEND, RENDER_NONLIGHT, RENDER_GLOW , RENDER_TRAIL, RENDER_BLEND, RENDER_DISTORTION, RENDER_UI, RENDER_END };

public:
	enum UIGROUP { UI_SCREEN, UI_BACKGROUND, UI_CONTENTS, UI_EFFECT, UI_PRIORITY, UI_END};

public:
	CRenderer(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);
	virtual ~CRenderer() = default;

public:	
	HRESULT Initialize();
	HRESULT AddRenderGroup(RENDERGROUP _eRenderGroup, shared_ptr<class CGameObject> _pGameObject);
	HRESULT AddUIRenderGroup(shared_ptr<class CGameObject> _pGameObject, UIGROUP _eUIGroup);
	HRESULT Render();

private:
	list<shared_ptr<class CGameObject>> m_RenderObjects[RENDER_END];
	list<shared_ptr<class CGameObject>> m_RenderUIs[UI_END];

private:
	shared_ptr<class CShader> m_pShader = nullptr;
	shared_ptr<class CShader> m_pPostProcessShader = nullptr;

	shared_ptr<class CVIRect> m_pVIBuffer = nullptr;

	_float4x4 m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;

#ifdef _DEBUG
public:
	HRESULT AddDebugComponent(shared_ptr<class CComponent> _pComponent);
	void SetZoomBlur(_float _fZoomPower, _float _fZoomAccTime);

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
	HRESULT RenderOutLine();
	HRESULT RenderGlow();
	HRESULT RenderBlend();
	HRESULT RenderMotionTrail();
	HRESULT RenderDistortion();
	HRESULT RenderZoomBlur();
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

	_bool m_bZoomBlur = false;
	_float m_fZoomBlurPower = 0.f;
	_float m_fBlurAccTime = 0.f;
	_float m_fBlurTotalTime = 0.f;

public:
	static shared_ptr<CRenderer> Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);

};

END
