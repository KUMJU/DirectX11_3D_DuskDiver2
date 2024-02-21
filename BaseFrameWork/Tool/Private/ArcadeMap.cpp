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

#include "MiddleBoss.h"
#include "GameObject.h"
#include "MapObject.h"

#include "MonsterTower.h"
#include "Portal.h"
#include "MonsterPool.h"

#include "MinigameMole.h"
#include "MinigameCommand.h"
#include "BattleSystem.h"

#include "MonsterTrigger.h"


#include "EffectParticle.h"



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

	if (FAILED(ReadyLayerMap(TEXT("Layer_Map"))))
		return E_FAIL;

	if (FAILED(ReadyLayerPlayer(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(ReadyLayerMonster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(ReadyLayerCamera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(ReadyLayerEvent(TEXT("Layer_Event"))))
		return E_FAIL;

	if (FAILED(ReadyLayerUI(TEXT("Layer_UI"))))
		return E_FAIL;

	

	CBattleSystem::GetInstance()->Initialize();
	CGameInstance::GetInstance()->PlayBGM(TEXT("BGM_MainTheme.wav"), 1.f);


	return S_OK;
}

void CArcadeMap::Tick(_float _fTimeDelta)
{
	CBattleSystem::GetInstance()->Tick(_fTimeDelta);

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

HRESULT CArcadeMap::ReadyLayerMap(const wstring& _strLayerTag)
{
	//맵 설치 
	shared_ptr<CGameObject> pDummy = CDummy::Create(TEXT("ArcadeMap_Final"));
	if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, _strLayerTag, pDummy)))
		return E_FAIL;

	pDummy->SetPosition({ -565.f, 40.5f, -100.f , 1.f });

	//맵 네비게이션 메쉬 셀 정보 파싱
	_float4x4 worldMat;
	_matrix dd = pDummy->GetWorldMatrix();
	XMStoreFloat4x4(&worldMat, dd);
	CMapLoader::GetInstance()->SetWorldMatrix(worldMat);
	CMapLoader::GetInstance()->LoadCellData(TEXT("CellTest.dat"));

	//맵 오브젝트 파싱정보 불러오기
	CMapLoader::GetInstance()->LoadMapData("../Bin/DataFiles/ArcadeMap_Object.dat");
	CMapLoader::GetInstance()->LoadMapData("../Bin/DataFiles/CoinData.dat");

	/***포탈 테스트***/

	shared_ptr<CPortal> pPortal = CPortal ::Create({ 0.f, 40.f, -245.f }, { 0.f, 25.f, -212.f });
	if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, _strLayerTag, pPortal)))
		return E_FAIL;

	/******몬스터 트리거 테스트(중간보스)*******/

	list<CMonsterPool::SPAWN_INFO> SpawnList;

	CMonsterPool::SPAWN_INFO info1 = {};
	info1.iMonsterType = 3;
	info1.vMonsterPos = _vector({ 0.f, 40.f, -390.f });
	SpawnList.push_back(info1);

	shared_ptr<CMonsterTrigger> pTrigger = CMonsterTrigger::Create(&SpawnList, { 0.f, 40.f, -380.f });
	CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_Event"), pTrigger);


	/**파티클 테스트**/

	/*shared_ptr<CGameObject> pParticle = CEffectParticle::Create();
	CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_Event"), pParticle);*/


	return S_OK;
}

HRESULT CArcadeMap::ReadyLayerPlayer(const wstring& _strLayerTag)
{
 	shared_ptr<CPlayer> pPlayer = CPlayer::Create();

	if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, _strLayerTag, pPlayer)))
		return E_FAIL;

	CGameMgr::GetInstance()->SetPlayer(pPlayer);

	return S_OK;
}

HRESULT CArcadeMap::ReadyLayerMonster(const wstring& _strLayerTag)
{

	CMonsterPool::GetInstance()->Initialize();

	return S_OK;
}

HRESULT CArcadeMap::ReadyLayerEvent(const wstring& _strLayerTag)
{

	shared_ptr<CMinigameMole> pInstance = CMinigameMole::Create();
	if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, _strLayerTag, pInstance)))
		return E_FAIL;
	//게임 시작 트리거
	//pInstance->GameStart();

	/*******커맨드 미니게임 테스트*********/

	shared_ptr<CMinigameCommand> pMinigameCmd = CMinigameCommand::Create();
	if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, _strLayerTag, pMinigameCmd)))
		return E_FAIL;

	return S_OK;
}

HRESULT CArcadeMap::ReadyLayerUI(const wstring& _strLayerTag)
{


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





}

shared_ptr<CArcadeMap> CArcadeMap::Create()
{
	shared_ptr<CArcadeMap> pInstance = make_shared<CArcadeMap>();

	if (FAILED(pInstance->Initialize()))
		MSG_BOX("Failed to Created : Arcade Map");

	return pInstance;
}
