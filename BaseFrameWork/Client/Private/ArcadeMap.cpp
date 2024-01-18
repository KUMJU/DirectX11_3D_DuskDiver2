#include "pch.h"
#include "ArcadeMap.h"

#include "GameInstance.h"
#include "Terrain.h"

#include "CameraFree.h"
#include "ThirdPersonCam.h"
#include "EventCamera.h"

#include "Dummy.h"
#include "Player.h"

#include "Engine_Defines.h"
#include "GameMgr.h"

#include "MapLoader.h"
#include "CameraMgr.h"

#include "UIBackGround.h"

#include "Enemy01.h"
#include "Enemy02.h"
#include "MiddleBoss.h"
#include "FinalBoss.h"

#include "GameObject.h"


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

	//CMapLoader::GetInstance()->LoadMapData("../Bin/DataFiles/TestMap.json");


	if (FAILED(ReadyLayerPlayer(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(ReadyLayerMonster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(ReadyLayerCamera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(ReadyLayerUI(TEXT("Layer_UI"))))
		return E_FAIL;

	return S_OK;
}

void CArcadeMap::Tick(_float _fTimeDelta)
{
	if (m_bKeyDeb) {
		m_fTime += _fTimeDelta;
		if (m_fTime > 10.f) {
			m_bKeyDeb = false;
		}
	}

	KeyInput();
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

	shared_ptr<CGameObject> pCam;

	//////////////////////////////////////////////////ThirdPersonCamera//////////////////////////////////////////////////

	pCam = CThirdPersonCam::Create();
	CCameraMgr::GetInstance()->SetCamObject(CCameraMgr::ECAMERATYPE::THIRDPERSON, pCam);

	if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, _strLayerTag, pCam)))
		return E_FAIL;

	//////////////////////////////////////////////////FreeCamera//////////////////////////////////////////////////

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

	pCam = CCameraFree::Create(false , &pCamFreeDesc, &pCamDesc, &pTransDesc);
	CCameraMgr::GetInstance()->SetCamObject(CCameraMgr::ECAMERATYPE::FREE, pCam);

	if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, _strLayerTag, pCam)))
		return E_FAIL;


	//////////////////////////////////////////////////EventCamera//////////////////////////////////////////////////


	pCam = CEventCamera::Create();
	CCameraMgr::GetInstance()->SetCamObject(CCameraMgr::ECAMERATYPE::EVENT, pCam);

	if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, _strLayerTag, pCam)))
		return E_FAIL;

	return S_OK;
}

HRESULT CArcadeMap::ReadyLayerPlayer(const wstring& _strLayerTag)
{
	shared_ptr<CPlayer> pPlayer = CPlayer::Create();

	if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, _strLayerTag, pPlayer)))
		return E_FAIL;

	CGameMgr::GetInstance()->SetPlayer(pPlayer);

	shared_ptr<CGameObject> pDummy = CDummy::Create(TEXT("PlatformA"));
	if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, _strLayerTag, pDummy)))
		return E_FAIL;


	return S_OK;
}

HRESULT CArcadeMap::ReadyLayerMonster(const wstring& _strLayerTag)
{
	//shared_ptr<CGameObject> pMonster2 = CEnemy01::Create();
	//if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, _strLayerTag, pMonster2)))
	//	return E_FAIL;

	//shared_ptr<CGameObject> pMonster3 = CEnemy02::Create();
	//if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, _strLayerTag, pMonster3)))
	//	return E_FAIL;

	//pMonster3->SetPosition({ 4.f , 0.f, 0.f ,1.f });

	//-----------------Middle Boss--------------------------------//
	//shared_ptr<CGameObject> pMonster4 = CMiddleBoss::Create();
	//if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, _strLayerTag, pMonster4)))
	//	return E_FAIL;

	//-----------------Final Boss--------------------------------//

	//shared_ptr<CGameObject> pMonster5 = CFinalBoss::Create();
	//if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, _strLayerTag, pMonster5)))
	//	return E_FAIL;

	pMonster1 = CEnemy01::Create();
	if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_Monster"), pMonster1)))
		return E_FAIL;
	pMonster1->SetEnable(false);

	pMonster2 = CEnemy02::Create();
	if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_Monster"), pMonster2)))
		return E_FAIL;
	pMonster2->SetEnable(false);
	

	pMonster3 = CFinalBoss::Create();
	if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_Monster"), pMonster3)))
		return E_FAIL;
	pMonster3->SetEnable(false);


	pMonster4 = CMiddleBoss::Create();
	if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_Monster"), pMonster4)))
		return E_FAIL;
	pMonster4->SetEnable(false);

	return S_OK;
}

HRESULT CArcadeMap::ReadyLayerUI(const wstring& _strLayerTag)
{

	CUIBackGround::tagUIInfo UITagInfo;
	UITagInfo.fSizeX = 463.f * 0.85f;
	UITagInfo.fSizeY = 179.f * 0.85f;
	UITagInfo.fX = g_iWinSizeX * 0.5f - 460.f;
	UITagInfo.fY = g_iWinSizeY * 0.5f- 280.f;

	if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_LOGO, _strLayerTag, CUIBackGround::Create(UITagInfo, TEXT("hud_status_bg_He01"), 0))))
		return E_FAIL;

	UITagInfo.fSizeX = 247.f * 0.65f;
	UITagInfo.fSizeY = 100.f * 0.65f;
	UITagInfo.fX = g_iWinSizeX * 0.5f - 450.f;
	UITagInfo.fY = g_iWinSizeY * 0.5f - 310.f;

	if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_LOGO, _strLayerTag, CUIBackGround::Create(UITagInfo, TEXT("hud_neme_he01"), 1))))
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
                                                                                                                                                            
void CArcadeMap::KeyInput()
{
	if (m_bKeyDeb)
		return;


	if (GetKeyState('7') & 0x8000) {
		pMonster1->SetEnable(true);

		m_bKeyDeb = true;
	}

	if (GetKeyState('8') & 0x8000) {
		
		pMonster1->SetEnable(false);
		pMonster2->SetEnable(true);

		m_bKeyDeb = true;

	}

	if (GetKeyState('9') & 0x8000) {

		pMonster2->SetEnable(false);
		pMonster3->SetEnable(true);

		m_bKeyDeb = true;

	}


	if (GetKeyState('0') & 0x8000) {

		pMonster3->SetEnable(false);
		pMonster4->SetEnable(true);

		m_bKeyDeb = true;

	}

}

shared_ptr<CArcadeMap> CArcadeMap::Create()
{
	shared_ptr<CArcadeMap> pInstance = make_shared<CArcadeMap>();

	if (FAILED(pInstance->Initialize()))
		MSG_BOX("Failed to Created : Arcade Map");

	return pInstance;
}
