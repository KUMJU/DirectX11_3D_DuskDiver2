#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class CRenderer final
{
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_NONBLEND, RENDER_BLEND, RENDER_UI, RENDER_END };

public:
	CRenderer();
	virtual ~CRenderer() = default;

public:
	HRESULT Initialize();
	HRESULT AddRenderGroup(RENDERGROUP _eRenderGroup, shared_ptr<class CGameObject> _pGameObject);
	HRESULT AddUIRenderGroup(shared_ptr<class CGameObject> _pGameObject, _int _iPriorityIndex);
	HRESULT Render();

private:
	list<shared_ptr<class CGameObject>> m_RenderObjects[RENDER_END];

private:
	HRESULT RenderPriority();
	HRESULT RenderNonBlend();
	HRESULT RenderBlend();
	HRESULT RenderUI();

private:

public:
	static shared_ptr<CRenderer> Create();

};

END
