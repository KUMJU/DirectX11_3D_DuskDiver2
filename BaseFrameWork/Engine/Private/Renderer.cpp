#include "Renderer.h"
#include "GameObject.h"

CRenderer::CRenderer()
{
}

HRESULT CRenderer::Initialize()
{
	return S_OK;
}

HRESULT CRenderer::AddRenderGroup(RENDERGROUP _eRenderGroup, shared_ptr<class CGameObject> _pGameObject)
{
	if (_eRenderGroup >= RENDER_END)
		return E_FAIL;

	m_RenderObjects[_eRenderGroup].push_back(_pGameObject);

	return S_OK;
}

HRESULT CRenderer::AddUIRenderGroup(shared_ptr<class CGameObject> _pGameObject, _int _iPriorityIndex)
{
	if(0 == _iPriorityIndex)
		m_RenderObjects[RENDER_UI].push_back(_pGameObject);
	else 
		m_RenderObjects[RENDER_UI].push_front(_pGameObject);

	return S_OK;
}

HRESULT CRenderer::Render()
{
	if (FAILED(RenderPriority()))
		return E_FAIL;

	if (FAILED(RenderNonBlend()))
		return E_FAIL;

	if (FAILED(RenderBlend()))
		return E_FAIL;

	if (FAILED(RenderUI()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::RenderPriority()
{
	for (auto& pGameObject : m_RenderObjects[RENDER_PRIORITY]) {
		if(nullptr!= pGameObject)
			pGameObject->Render();
	}

	m_RenderObjects[RENDER_PRIORITY].clear();

	return S_OK;
}

HRESULT CRenderer::RenderNonBlend()
{
	for (auto& pGameObject : m_RenderObjects[RENDER_NONBLEND]) {
		if (nullptr != pGameObject)
			pGameObject->Render();
	}

	m_RenderObjects[RENDER_NONBLEND].clear();

	return S_OK;
}

HRESULT CRenderer::RenderBlend()
{
	for (auto& pGameObject : m_RenderObjects[RENDER_BLEND]) {
		if (nullptr != pGameObject)
			pGameObject->Render();
	}

	m_RenderObjects[RENDER_BLEND].clear();

	return S_OK;
}

HRESULT CRenderer::RenderUI()
{
	for (auto& pGameObject : m_RenderObjects[RENDER_UI]) {
		if (nullptr != pGameObject)
			pGameObject->Render();
	}

	m_RenderObjects[RENDER_UI].clear();

	return S_OK;
}

shared_ptr<CRenderer> CRenderer::Create()
{
	shared_ptr<CRenderer> pInstance = make_shared<CRenderer>();

	if (FAILED(pInstance->Initialize())) {
		MSG_BOX("Create Failed : Renderer");
	}

	return pInstance;
}
