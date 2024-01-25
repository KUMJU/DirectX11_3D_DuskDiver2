#include "GameInstance.h"
#include "GraphicDev.h"
#include "LevelMgr.h"
#include "ObjectManager.h"
#include "Renderer.h"
#include "TimerMgr.h"
#include "InputDevice.h"
#include "PickingMgr.h"
#include "LightMgr.h"

_float g_fSlowWeight = 1.f;

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{
}

CGameInstance::~CGameInstance()
{
}

HRESULT CGameInstance::InitializeEngine(HINSTANCE hInst, _uint iNumLevels, _uint _iScreenX, _uint _iScreenY, const GRAPHIC_DESC& _GraphicDesc)
{
	/* 그래픽 초기화. */
	m_pGraphicDev = CGraphicDev::Create(_GraphicDesc);
	if (nullptr == m_pGraphicDev)
		return E_FAIL;

	m_pInputDev = CInputDevice::Create(hInst, _GraphicDesc.hWnd);
	if (!m_pInputDev)
		return E_FAIL;

	m_pPipeLine = CPipeLine::Create();
	if (!m_pPipeLine)
		return E_FAIL;

	m_pRenderer = CRenderer::Create();
	if (!m_pRenderer)
		return E_FAIL;

	/* 입력장치 초기화. */
	m_pLevelMgr = CLevelMgr::Create();
	if (nullptr == m_pLevelMgr)
		return E_FAIL;

	m_pTimerMgr = CTimerMgr::Create();
	if (nullptr == m_pTimerMgr)
		return E_FAIL;

	/* 오브젝트 매니져  리저브*/
	m_pObjectMgr = CObjectManager::Create(iNumLevels);
	if (nullptr == m_pObjectMgr)
		return E_FAIL;

	m_pPickingMgr = CPickingMgr::Create(_iScreenX, _iScreenY);
	if (nullptr == m_pPickingMgr)
		return E_FAIL;

	m_pLightMgr = CLightMgr::Create(m_pGraphicDev->GetDeviceInfo(), m_pGraphicDev->GetDeviceContextInfo());
	if (nullptr == m_pLightMgr)
		return E_FAIL;

	m_pResMgr = CResourceMgr::Create(m_pGraphicDev->GetDeviceInfo(), m_pGraphicDev->GetDeviceContextInfo());
	if (nullptr == m_pResMgr)
		return E_FAIL;

	m_pCollisionMgr = CCollisionMgr::Create();
	if (nullptr == m_pCollisionMgr)
		return E_FAIL;

	return S_OK;
}

void CGameInstance::TickEngine(_float _fTimeDelta)
{
	if (nullptr == m_pLevelMgr ||
		nullptr == m_pObjectMgr)
		return;

	m_pInputDev->Tick();

	m_pObjectMgr->PriorityTick(_fTimeDelta);

	m_pPipeLine->Tick();

	m_pObjectMgr->Tick(_fTimeDelta);

	m_pCollisionMgr->LateTick();

	m_pObjectMgr->LateTick(_fTimeDelta);

	m_pLevelMgr->Tick(_fTimeDelta);
}

HRESULT CGameInstance::Draw()
{
	if (nullptr == m_pGraphicDev)
		return E_FAIL;

	m_pLevelMgr->Render();
	return m_pRenderer->Render();
}

HRESULT CGameInstance::ClearBackBufferView(const _float4& _vClearColor)
{
	if (nullptr == m_pGraphicDev)
		return E_FAIL;

	return m_pGraphicDev->ClearBackBufferView(_vClearColor);
}

HRESULT CGameInstance::ClearDepthStencilView()
{
	if (nullptr == m_pGraphicDev)
		return E_FAIL;

	return m_pGraphicDev->ClearDepthStencilView();
}

HRESULT CGameInstance::Present()
{
	if (nullptr == m_pGraphicDev)
		return E_FAIL;

	return m_pGraphicDev->Present();
}

void CGameInstance::Clear(_uint iLevelIndex)
{
	if (nullptr == m_pObjectMgr)
		return;

	m_pObjectMgr->Clear(iLevelIndex);

}

_byte CGameInstance::GetDIKeyState(_ubyte byKeyID)
{
	if (nullptr == m_pInputDev)
		return 0;

	return m_pInputDev->Get_DIKeyState(byKeyID);
}

_byte CGameInstance::GetDIMouseState(MOUSEKEYSTATE eMouse)
{
	if (nullptr == m_pInputDev)
		return 0;

	return m_pInputDev->Get_DIMouseState(eMouse);
}

_long CGameInstance::GetDIMouseMove(MOUSEMOVESTATE eMouseState)
{
	if (nullptr == m_pInputDev)
		return 0;

	return m_pInputDev->Get_DIMouseMove(eMouseState);
}

HRESULT CGameInstance::OpenLevel(_uint _iLevelIndex, shared_ptr<class CLevel> _pLevel)
{
	if (nullptr == m_pLevelMgr)
		return E_FAIL;

	return m_pLevelMgr->Open_Level(_iLevelIndex, _pLevel);
}

_uint CGameInstance::GetCurrentLevel()
{
	if (nullptr == m_pLevelMgr)
		return E_FAIL;

	return m_pLevelMgr->GetCurrentLevel();
}


HRESULT CGameInstance::AddObject(_uint _iLevelIndex, const wstring& _strLayerTag, shared_ptr<class CGameObject> _pGameObject)
{
	if (!m_pObjectMgr)
		return E_FAIL;

	return m_pObjectMgr->AddObject(_iLevelIndex, _strLayerTag, _pGameObject);
}

shared_ptr<CLayers> CGameInstance::FindLayer(const wstring& _strLayerName)
{
	if (!m_pObjectMgr)
		return nullptr;

	return m_pObjectMgr->FindLayer(m_pLevelMgr->GetCurrentLevel(),_strLayerName);
}

_float CGameInstance::ComputeTimeDelta(const wstring& pTimer)
{
	if (!m_pTimerMgr)
		return 0.f;

	return m_pTimerMgr->ComputeTimeDelta(pTimer);
}

HRESULT CGameInstance::AddTimer(const wstring& pTimerTag)
{
	if (!m_pTimerMgr)
		return E_FAIL;

	return m_pTimerMgr->AddTimer(pTimerTag);
}

void CGameInstance::SetTransform(CPipeLine::D3DTRANSFORMSTATE _eState, _fmatrix _TransformMatrix)
{
	m_pPipeLine->SetTransform(_eState, _TransformMatrix);
}

_float4x4 CGameInstance::GetTransformFloat4x4(CPipeLine::D3DTRANSFORMSTATE _eState)
{
	return m_pPipeLine->GetTransformFloat4x4(_eState);
}

_matrix CGameInstance::GetTransformMatrix(CPipeLine::D3DTRANSFORMSTATE _eState)
{
	return m_pPipeLine->GetTransformMatrix(_eState);
}

_float4x4 CGameInstance::GetTransformFloat4x4Inverse(CPipeLine::D3DTRANSFORMSTATE _eState)
{
	return m_pPipeLine->GetTransformFloat4x4Inverse(_eState);
}

_matrix CGameInstance::GetTransformMatrixInverse(CPipeLine::D3DTRANSFORMSTATE _eState)
{
	return m_pPipeLine->GetTransformMatrixInverse(_eState);
}

_float4 CGameInstance::GetCamPosition()
{
	return m_pPipeLine->GetCamPosition();
}

HRESULT CGameInstance::AddRenderGroup(CRenderer::RENDERGROUP _eRenderGroup, shared_ptr<class CGameObject> _pGameObject)
{
	if (!m_pRenderer)
		return E_FAIL;

	return m_pRenderer->AddRenderGroup(_eRenderGroup, _pGameObject);
}

HRESULT CGameInstance::AddUIRenderGroup(shared_ptr<class CGameObject> _pGameObject, _int _iPriorityIdx)
{
	if (!m_pRenderer)
		return E_FAIL;

	return m_pRenderer->AddUIRenderGroup( _pGameObject, _iPriorityIdx);
}

_float4 CGameInstance::TerrainPicking(POINT _ptMouse, shared_ptr<class CVITerrain> _pTerrainCom, shared_ptr<class CTransform> _pTransCom)
{
	if (!m_pPickingMgr)
		return _float4();

	return m_pPickingMgr->TerrainPicking(_ptMouse, _pTerrainCom, _pTransCom);
}

_float3 CGameInstance::MeshPicking(POINT _ptMouse, shared_ptr<class CModel> _pModelCom, shared_ptr<class CTransform> _pTransCom, _float& _pDistance)
{
	if (!m_pPickingMgr)
		return _float3();

	return m_pPickingMgr->MeshPicking(_ptMouse, _pModelCom, _pTransCom, _pDistance);
}

HRESULT CGameInstance::AddLight(const LIGHT_DESC& _LightDesc)
{
	if (!m_pLightMgr)
		return E_FAIL;


	return m_pLightMgr->AddLight(_LightDesc);
}

const LIGHT_DESC* CGameInstance::GetLightDesc(_uint iIndex) const
{
	return m_pLightMgr->GetLightDesc(iIndex);
}

void CGameInstance::LoadLevelResource(_uint _iLevelIndex)
{
	if (!m_pResMgr)
		return;

	m_pResMgr->LoadLevelResource(_iLevelIndex);
}

shared_ptr<class CTexture> CGameInstance::GetTexture(const wstring& _strTextureKey)
{
	if (!m_pResMgr)
		return nullptr;

	return 	m_pResMgr->GetTexture(_strTextureKey);
}

shared_ptr<class CModel> CGameInstance::GetModel(const wstring& _strModelKey)
{
	if (!m_pResMgr)
		return nullptr;

	return m_pResMgr->GetModel(_strModelKey);
}

shared_ptr<class CShader> CGameInstance::GetShader(const wstring& _strShaderKey)
{
	if (!m_pResMgr)
		return nullptr;

	return m_pResMgr->GetShader(_strShaderKey);
}

map<wstring, CResourceMgr::ResourceDesc<class CModel>>* CGameInstance::GetModels()
{
	return m_pResMgr->GetModels();
}

void CGameInstance::LoadResourceForTool()
{
	m_pResMgr->LoaderForTool();
}

void CGameInstance::AddCollider(CCollisionMgr::COLTYPE_GROUP _eColGroup, shared_ptr<CCollider> _pCollider)
{

	if (!m_pCollisionMgr)
		return;

	m_pCollisionMgr->AddColliderList(_eColGroup, _pCollider);

}

wrl::ComPtr<ID3D11Device> CGameInstance::GetDeviceInfo()
{
	return m_pGraphicDev->GetDeviceInfo();
}

wrl::ComPtr<ID3D11DeviceContext> CGameInstance::GetDeviceContextInfo()
{
	return m_pGraphicDev->GetDeviceContextInfo();
}

void CGameInstance::Free()
{
	ULONG p22 = m_pInstance.use_count();
	m_pLevelMgr.reset();
	m_pObjectMgr.reset();
 }

void CGameInstance::ReleaseManager()
{
	DestroyInstance();
}
