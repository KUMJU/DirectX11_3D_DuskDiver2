#pragma once

#include "Engine_Defines.h"
#include "Renderer.h"
#include "PipeLine.h"
#include "ResourceMgr.h"
#include "CollisionMgr.h"

BEGIN(Engine)

class CGraphicDev;
class CLayers;


class ENGINE_DLL CGameInstance
{
	DECLARE_SINGLETON(CGameInstance)

private:
	CGameInstance();
	~CGameInstance();

public:
	HRESULT InitializeEngine(HINSTANCE hInst, _uint iNumLevels, _uint _iScreenX, _uint _iScreenY, const GRAPHIC_DESC& _GraphicDesc);
	void TickEngine(_float _fTimeDelta);


	HRESULT Draw(); 
	HRESULT ClearBackBufferView(const _float4& _vClearColor);
	HRESULT ClearDepthStencilView();
	HRESULT Present();

	void Clear(_uint iLevelIndex);

public: /* For.Input_Device */
	_byte	GetDIKeyState(_ubyte byKeyID);
	_byte	GetDIMouseState(MOUSEKEYSTATE eMouse);
	_long	GetDIMouseMove(MOUSEMOVESTATE eMouseState);

/*LevelManager*/
public:
	HRESULT OpenLevel(_uint _iLevelIndex, shared_ptr<class CLevel> _pLevel);
	_uint GetCurrentLevel();

/*ObjectManager*/
public:                                   
	HRESULT AddObject(_uint _iLevelIndex, const wstring& _strLayerTag, shared_ptr<class CGameObject> _pGameObject);
	shared_ptr<CLayers> FindLayer(const wstring& _strLayerName);

/*TimeManager*/
public:
	_float ComputeTimeDelta(const wstring& pTimer);
	HRESULT AddTimer(const wstring& pTimerTag);

public: /* For.PipeLine */
	void SetTransform(CPipeLine::D3DTRANSFORMSTATE _eState, _fmatrix _TransformMatrix);
	_float4x4 GetTransformFloat4x4(CPipeLine::D3DTRANSFORMSTATE _eState);
	_matrix GetTransformMatrix(CPipeLine::D3DTRANSFORMSTATE _eState);
	_float4x4 GetTransformFloat4x4Inverse(CPipeLine::D3DTRANSFORMSTATE _eState);
	_matrix GetTransformMatrixInverse(CPipeLine::D3DTRANSFORMSTATE _eState);
	_float4 GetCamPosition();

/*Renderer*/
public:
	HRESULT AddRenderGroup(CRenderer::RENDERGROUP _eRenderGroup, shared_ptr<class CGameObject> _pGameObject);
	HRESULT AddUIRenderGroup(shared_ptr<class CGameObject> _pGameObject, _int _iPriorityIdx);

/*Picking*/
public:
	_float4 TerrainPicking(POINT _ptMouse, shared_ptr<class CVITerrain> _pTerrainCom, shared_ptr<class CTransform> _pTransCom);
	_float3 MeshPicking(POINT _ptMouse, shared_ptr<class CModel> _pModelCom, shared_ptr<class CTransform> _pTransCom, _float& _pDistance);

/*Light Mgr*/
public:
	HRESULT AddLight(const LIGHT_DESC& _LightDesc);
	const LIGHT_DESC* GetLightDesc(_uint iIndex) const;

/*Resource Mgr*/
public:
	void LoadLevelResource(_uint _iLevelIndex);
	shared_ptr<class CTexture> GetTexture(const wstring& _strTextureKey);
	shared_ptr<class CModel> GetModel(const wstring& _strModelKey);
	shared_ptr<class CShader> GetShader(const wstring& _strShaderKey);

	map <wstring, CResourceMgr::ResourceDesc<class CModel>>* GetModels();

	void LoadResourceForTool();

/*Collision Mgr*/
public:

	void AddCollider(CCollisionMgr::COLTYPE_GROUP _eColGroup, shared_ptr<CCollider> _pCollider);


private:
	shared_ptr<class CGraphicDev> m_pGraphicDev = nullptr;
	shared_ptr<class CInputDevice> m_pInputDev = nullptr;
	shared_ptr<class CLevelMgr> m_pLevelMgr = nullptr;
	shared_ptr<class CObjectManager> m_pObjectMgr = nullptr;
	shared_ptr<class CRenderer> m_pRenderer = nullptr;
	shared_ptr<class CTimerMgr> m_pTimerMgr = nullptr;
	shared_ptr<CPipeLine> m_pPipeLine = nullptr;
	shared_ptr<class CPickingMgr> m_pPickingMgr = nullptr;
	shared_ptr<class CLightMgr> m_pLightMgr = nullptr;
	shared_ptr<class CResourceMgr> m_pResMgr = nullptr;
	shared_ptr<CCollisionMgr> m_pCollisionMgr = nullptr;


private:

public:
	wrl::ComPtr<ID3D11Device> GetDeviceInfo();
	wrl::ComPtr<ID3D11DeviceContext> GetDeviceContextInfo();

public:
	void Free();
	static void ReleaseManager();
};

END
