#include "pch.h"
#include "ArcadeMap.h"

#include "GameInstance.h"
#include "Terrain.h"
#include "CameraFree.h"
#include "Dummy.h"

#include "Engine_Defines.h"

#include "MapLoader.h"

CArcadeMap::CArcadeMap()
	:CLevel()
{
}

CArcadeMap::~CArcadeMap()
{
}

void CArcadeMap::PreInit()
{
}

HRESULT CArcadeMap::Initialize()
{
	
	if (FAILED(ReadyLight()))
		return E_FAIL;

	CMapLoader::GetInstance()->LoadMapData("../Bin/DataFiles/ArcadeMap.json");

	if (FAILED(ReadyLayerCamera(TEXT("Layer_Camera"))))
		return E_FAIL;

	//if (FAILED(ReadyLayerPlayer(TEXT("Layer_Player"))))
	//	return E_FAIL;

	return S_OK;
}

void CArcadeMap::Tick(_float _fTimeDelta)
{
}
 
HRESULT CArcadeMap::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("게임 플레이입니다."));
#endif

	return S_OK;
}

HRESULT CArcadeMap::ReadyLayerBackGround(const wstring& _strLayerTag)
{
	if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, _strLayerTag, CTerrain::Create(TEXT("../Bin/Resources/Base/Textures/Terrain/Height1.bmp")))))
		return E_FAIL;

	return S_OK;
}

HRESULT CArcadeMap::ReadyLayerCamera(const wstring& _strLayerTag)
{
	CCameraFree::CAMERAFREE_DESC pCamFreeDesc = {};
	pCamFreeDesc.fMouseSensor = 0.05f;

	CCameraFree::CAMERA_DESC pCamDesc = {};
	pCamDesc.vEye = _float4(0.0f, 30.f, -25.f, 1.f);
	pCamDesc.vAt = _float4(0.0f, 0.0f, 0.f, 1.f);
	pCamDesc.fFovy = XMConvertToRadians(60.0f);
	pCamDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	pCamDesc.fNear = 0.2f;
	pCamDesc.fFar = 1000.f;

	CTransform::TRANSFORM_DESC pTransDesc = {};
	pTransDesc.fSpeedPerSet = 20.f;
	pTransDesc.fRotationPerSet = XMConvertToRadians(90.0f);

	if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, _strLayerTag, CCameraFree::Create(&pCamFreeDesc, &pCamDesc, &pTransDesc))))
		return E_FAIL;

	return S_OK;
}

HRESULT CArcadeMap::ReadyLayerPlayer(const wstring& _strLayerTag)
{
	if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, _strLayerTag,CDummy::Create(TEXT("AssetsA")))))
		return E_FAIL;

	return S_OK;
}

HRESULT CArcadeMap::ReadyLight()
{
	LIGHT_DESC LightDesc = {};

	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(CGameInstance::GetInstance()->AddLight(LightDesc)))
		return E_FAIL;

	return S_OK;
}

shared_ptr<CArcadeMap> CArcadeMap::Create()
{
	shared_ptr<CArcadeMap> pInstance = make_shared<CArcadeMap>();

	if (FAILED(pInstance->Initialize()))
		MSG_BOX("Failed to Created : Arcade Map");

	return pInstance;
}
