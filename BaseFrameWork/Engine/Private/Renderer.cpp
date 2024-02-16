#include "Renderer.h"
#include "GameObject.h"
#include "Component.h"
#include "GameInstance.h"
#include "Shader.h"
#include "VIRect.h"

CRenderer::CRenderer(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
	:m_pDevice(_pDevice),
	m_pContext(_pContext)
{
}

HRESULT CRenderer::Initialize()
{
	D3D11_VIEWPORT ViewPortDesc{};

	_uint iNumViewPorts = 1;

	m_pContext->RSGetViewports(&iNumViewPorts, &ViewPortDesc);

	/*Set RenderTarget*/
	if (FAILED(CGameInstance::GetInstance()->AddRenderTarget(TEXT("Target_Diffuse"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(1.f, 1.f, 1.f, 0.f))))
		return E_FAIL;

	if (FAILED(CGameInstance::GetInstance()->AddRenderTarget(TEXT("Target_Normal"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	if (FAILED(CGameInstance::GetInstance()->AddRenderTarget(TEXT("Target_Shade"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	if (FAILED(CGameInstance::GetInstance()->AddRenderTarget(TEXT("Target_Depth"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	if (FAILED(CGameInstance::GetInstance()->AddRenderTarget(TEXT("Target_Specular"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(CGameInstance::GetInstance()->AddRenderTarget(TEXT("Target_Glow"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(1.f, 1.f, 1.f, 0.f))))
		return E_FAIL;


	/*Set MRT*/
	if(FAILED(CGameInstance::GetInstance()->AddMRT(TEXT("MRT_GameObjects"), TEXT("Target_Diffuse"))))
		return E_FAIL;

	if (FAILED(CGameInstance::GetInstance()->AddMRT(TEXT("MRT_GameObjects"), TEXT("Target_Normal"))))
		return E_FAIL;

	if (FAILED(CGameInstance::GetInstance()->AddMRT(TEXT("MRT_GameObjects"), TEXT("Target_Depth"))))
		return E_FAIL;


	if (FAILED(CGameInstance::GetInstance()->AddMRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
		return E_FAIL;
	if (FAILED(CGameInstance::GetInstance()->AddMRT(TEXT("MRT_LightAcc"), TEXT("Target_Specular"))))
		return E_FAIL;

	if (FAILED(CGameInstance::GetInstance()->AddMRT(TEXT("MRT_Glow"), TEXT("Target_Glow"))))
		return E_FAIL;

	m_pVIBuffer = CVIRect::Create(m_pDevice, m_pContext);
	if (!m_pVIBuffer)
		return E_FAIL;

	m_pShader = CGameInstance::GetInstance()->GetShader(TEXT("Shader_Deferred"));

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
	m_WorldMatrix._11 = ViewPortDesc.Width;
	m_WorldMatrix._22 = ViewPortDesc.Height;

	m_fScreenWidth = ViewPortDesc.Width;
	m_fScreenHeight = ViewPortDesc.Height;

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewPortDesc.Width, ViewPortDesc.Height, 0.f, 1.f));

#ifdef _DEBUG
	if (FAILED(CGameInstance::GetInstance()->ReadyDebug(TEXT("Target_Diffuse"), 100.0f, 100.0f, 200.0f, 200.0f)))
		return E_FAIL;
	if (FAILED(CGameInstance::GetInstance()->ReadyDebug(TEXT("Target_Normal"), 100.0f, 300.0f, 200.0f, 200.0f)))
		return E_FAIL;
	if (FAILED(CGameInstance::GetInstance()->ReadyDebug(TEXT("Target_Depth"), 100.0f, 500.0f, 200.0f, 200.0f)))
		return E_FAIL;
	if (FAILED(CGameInstance::GetInstance()->ReadyDebug(TEXT("Target_Shade"), 300.0f, 100.0f, 200.0f, 200.0f)))
		return E_FAIL;
	if (FAILED(CGameInstance::GetInstance()->ReadyDebug(TEXT("Target_Specular"), 300.0f, 300.0f, 200.0f, 200.0f)))
		return E_FAIL;
#endif // _DEBUG


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
		m_RenderObjects[RENDER_UI].push_front(_pGameObject);
	else 
		m_RenderObjects[RENDER_UI].push_back(_pGameObject);


	//std::sort(m_RenderObjects[RENDER_UI].begin(), m_RenderObjects[RENDER_UI].end(), [&]( ) {});

	return S_OK;
}

HRESULT CRenderer::Render()
{
	wrl::ComPtr<ID3D11RenderTargetView> pBackBufferView;
	wrl::ComPtr<ID3D11DepthStencilView> pDepthStencilView;

	if (FAILED(RenderPriority()))
		return E_FAIL;


	if (FAILED(RenderNonBlend()))
		return E_FAIL;


	if (FAILED(RenderLight()))
		return E_FAIL;

	//if (FAILED(RenderGlow()))
	//	return E_FAIL;

	if (FAILED(RenderFinal()))
		return E_FAIL;


	if (FAILED(RenderNonLight()))
		return E_FAIL;


	if (FAILED(RenderBlend()))
		return E_FAIL;

	if (FAILED(RenderUI()))
		return E_FAIL;

	
#ifdef _DEBUG
	if (m_bDebugOn) {
		if (FAILED(RenderDebug()))
			return E_FAIL;

	}
#endif


	return S_OK;
}

HRESULT CRenderer::AddDebugComponent(shared_ptr<class CComponent> _pComponent)
{

	m_DebugCom.push_back(_pComponent);

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
	//Diffuset + Normal
	if (FAILED(CGameInstance::GetInstance()->BeginMRT(TEXT("MRT_GameObjects"))))
		return E_FAIL;

	for (auto& pGameObject : m_RenderObjects[RENDER_NONBLEND]) {
		if (nullptr != pGameObject)
			pGameObject->Render();
	}

	m_RenderObjects[RENDER_NONBLEND].clear();


	if (FAILED(CGameInstance::GetInstance()->EndMRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::RenderLight()
{
	if (FAILED(CGameInstance::GetInstance()->BeginMRT(TEXT("MRT_LightAcc"))))
		return E_FAIL;

	if (FAILED(m_pShader->BindMatrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->BindMatrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->BindMatrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	_float4x4 ProjMatInv = CGameInstance::GetInstance()->GetTransformFloat4x4Inverse(CPipeLine::D3DTS_PROJ);
	_float4x4 ViewMatInv = CGameInstance::GetInstance()->GetTransformFloat4x4Inverse(CPipeLine::D3DTS_VIEW);
	_float4 vCamPos = CGameInstance::GetInstance()->GetCamPosition();

	if (FAILED(m_pShader->BindMatrix("g_ProjMatrixInv", &ProjMatInv)))
		return E_FAIL;
	if (FAILED(m_pShader->BindMatrix("g_ViewMatrixInv", &ViewMatInv)))
		return E_FAIL;
	if (FAILED(m_pShader->BindRawValue("g_vCamPosition", &vCamPos, sizeof(_float4))))
		return E_FAIL;

	/* 노말 렌더타겟을 쉐이더에 던진다. */
	if (FAILED(CGameInstance::GetInstance()->BindSRV(TEXT("Target_Normal"), m_pShader, "g_NormalTexture")))
		return E_FAIL;
	if (FAILED(CGameInstance::GetInstance()->BindSRV(TEXT("Target_Depth"), m_pShader, "g_DepthTexture")))
		return E_FAIL;

	/* 빛들을 하나씩 그린다.(사각형버퍼를 셰이드타겟에 그린다.) */
	CGameInstance::GetInstance()->RenderLight(m_pShader, m_pVIBuffer);

	if (FAILED(CGameInstance::GetInstance()->EndMRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::RenderFinal()
{
	if (FAILED(m_pShader->BindMatrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->BindMatrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->BindMatrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(CGameInstance::GetInstance()->BindSRV(TEXT("Target_Diffuse"), m_pShader, "g_DiffuseTexture")))
		return E_FAIL;

	if (FAILED(CGameInstance::GetInstance()->BindSRV(TEXT("Target_Shade"), m_pShader, "g_ShadeTexture")))
		return E_FAIL;

	if (FAILED(CGameInstance::GetInstance()->BindSRV(TEXT("Target_Specular"), m_pShader, "g_SpecularTexture")))
		return E_FAIL;

	m_pShader->Begin(3);

	m_pVIBuffer->BindBuffers();
	m_pVIBuffer->Render();

	return S_OK;
}

HRESULT CRenderer::RenderNonLight()
{

	for (auto& pGameObject : m_RenderObjects[RENDER_NONLIGHT])
	{
		if (pGameObject)
			pGameObject->Render();
	}

	m_RenderObjects[RENDER_NONLIGHT].clear();

	return S_OK;
}

HRESULT CRenderer::RenderGlow()
{

	if (m_RenderObjects[RENDER_GLOW].empty())
		return S_OK;

	//각 객체에 Render를 돌려서 렌더 타겟에 전부 그린다음에 그 타겟을 디퍼드 셰이더에 던져서(가로 세로 총 2회) 완성함? 

	/*****렌더 타겟 텍스쳐에 객체 그리기 ******/
	if (FAILED(CGameInstance::GetInstance()->BeginMRT(TEXT("MRT_Glow"))))
		return E_FAIL;

	for (auto& pGameObject : m_RenderObjects[RENDER_GLOW]) {
		if (nullptr != pGameObject)
			pGameObject->Render();
	}

	m_RenderObjects[RENDER_GLOW].clear();

	if (FAILED(CGameInstance::GetInstance()->EndMRT()))
		return E_FAIL;

	/*********그린 렌더 타겟을 디퍼드 셰이더에 던져서 후처리 작업**********/
	if (FAILED(m_pShader->BindMatrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->BindMatrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->BindMatrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	//화면 해상도 바인드
	if (FAILED(m_pShader->BindRawValue("g_fScreenWidth", &m_fScreenWidth, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShader->BindRawValue("g_fScreenHeight", &m_fScreenHeight, sizeof(_float))))
		return E_FAIL;


	if (FAILED(CGameInstance::GetInstance()->BindSRV(TEXT("Target_Glow"), m_pShader, "g_Texture")))
		return E_FAIL;


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

HRESULT CRenderer::RenderDebug()
{
	if (FAILED(m_pShader->BindMatrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->BindMatrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	CGameInstance::GetInstance()->RenderMRT(TEXT("MRT_GameObjects"), m_pShader, m_pVIBuffer);
	CGameInstance::GetInstance()->RenderMRT(TEXT("MRT_LightAcc"), m_pShader, m_pVIBuffer);

	for (auto& pComponent : m_DebugCom)
	{
		pComponent->Render();
	}

	m_DebugCom.clear();

	return S_OK;
}

shared_ptr<CRenderer> CRenderer::Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CRenderer> pInstance = make_shared<CRenderer>(_pDevice,_pContext);

	if (FAILED(pInstance->Initialize())) {
		MSG_BOX("Create Failed : Renderer");
	}

	return pInstance;
}
