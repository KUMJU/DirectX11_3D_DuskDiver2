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

#include "UIMgr.h"
#include "UIPlrHPBar.h"
#include "UIQuest.h"
#include "UISkillBar.h"
#include "UIBurstGaugeBar.h"
#include "UIBurstSkillGauge.h"
#include "UIDialog.h"
#include "UIMiniquest.h"
#include "UIMapTitle.h"

#include "EffectMgr.h"
#include "EffectPreset.h"

#include "UI_SequenceTex.h"

#include "SceneTriggerStart.h"
#include "SceneTriggerCoin.h"
#include "SceneTriggerBear.h"
#include "SceneTriggerBoss.h"


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
	CEffectMgr::GetInstance()->LoadEffectPreset();

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

	CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_BGM);
	CGameInstance::GetInstance()->PlayBGM(TEXT("se_EnvLab_Ambience.wav"), 1.f);


	CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_MAPSE);
	CGameInstance::GetInstance()->PlayAudio(TEXT("se_DVEnergy_Interaction1.wav"),CSoundMgr::CHANNELID::CH_MAPSE,  1.f);

	//CUIMgr::GetInstance()->CloseAllUI();
	//CUIMgr::GetInstance()->StartDialog(TEXT("StartDialog"));

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

	/*list<CMonsterPool::SPAWN_INFO> SpawnList;
	CMonsterPool::SPAWN_INFO info1 = {};
	info1.iMonsterType = 3;
	info1.vMonsterPos = _vector({ 0.f, 39.5f, -425.f });
	SpawnList.push_back(info1);

	shared_ptr<CMonsterTrigger> pTrigger = CMonsterTrigger::Create(&SpawnList, { 0.f, 40.f, -380.f });
	CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_Event"), pTrigger);*/


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

	/*시작 이벤트 트리거*/

	//shared_ptr<CSceneTriggerStart> pTrigger2 = CSceneTriggerStart::Create({ 0.f, 10.f, -25.f });
	//CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, _strLayerTag, pTrigger2);

	/*코인 이벤트 트리거*/

	shared_ptr<CSceneTriggerCoin> pTrigger3 = CSceneTriggerCoin::Create({ 0.f, 27.f, -140.f });
	CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, _strLayerTag, pTrigger3);

	shared_ptr<CSceneTriggerBear> pTrigger4 = CSceneTriggerBear::Create({ -87.f , 43.f, -87.f });
	CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, _strLayerTag, pTrigger4);

	shared_ptr<CSceneTriggerBoss> pTrigger5 = CSceneTriggerBoss::Create({ 2.f, 40.f, -378.f });
	CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, _strLayerTag, pTrigger5);

	return S_OK;
}

HRESULT CArcadeMap::ReadyLandObjects()
{
	return E_NOTIMPL;
}

HRESULT CArcadeMap::ReadyLayerUI(const wstring& _strLayerTag)
{
	shared_ptr<CUI> pInstance;

	CUIBackGround::tagUIInfo UITagInfo;
	UITagInfo.fSizeX = 463.f * 0.85f;
	UITagInfo.fSizeY = 179.f * 0.85f;
	UITagInfo.fX = g_iWinSizeX * 0.5f - 460.f;
	UITagInfo.fY = g_iWinSizeY * 0.5f- 280.f;

	shared_ptr<CUIBackGround> pBackground = CUIBackGround::Create(UITagInfo, TEXT("hud_status_bg_He01"),0);

	if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, _strLayerTag, pBackground)))
		return E_FAIL;

	UITagInfo.fSizeX = 247.f * 0.65f;
	UITagInfo.fSizeY = 100.f * 0.65f;
	UITagInfo.fX = g_iWinSizeX * 0.5f - 450.f;
	UITagInfo.fY = g_iWinSizeY * 0.5f - 310.f;

	shared_ptr<CUIBackGround> pBackground2 = CUIBackGround::Create(UITagInfo, TEXT("hud_neme_he01"),1);

	if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, _strLayerTag, pBackground2)))
		return E_FAIL;

	CUIMgr::GetInstance()->AddUI(TEXT("UI_HUDBackground1"), pBackground);
	CUIMgr::GetInstance()->AddUI(TEXT("UI_HUDBackground2"), pBackground2);


	/**Player HP Bar**/

	pInstance = CUIPlrHPBar::Create();
	if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, _strLayerTag, pInstance)))
		return E_FAIL;
	
	CUIMgr::GetInstance()->AddUI(TEXT("UI_PlayerHP"), pInstance);

	/**Quest UI**/

	pInstance = CUIQuest::Create();
	if(FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, _strLayerTag, pInstance)))
		return E_FAIL;
	CUIMgr::GetInstance()->AddUI(TEXT("UI_Quest"), pInstance);


	/**Player SkillBar**/

	pInstance = CUISkillBar::Create();
	if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, _strLayerTag, pInstance)))
		return E_FAIL;

	CUIMgr::GetInstance()->AddUI(TEXT("UI_SkillBar"), pInstance);

	/**Player BurstBar**/

	pInstance = CUIBurstGaugeBar::Create();
	if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, _strLayerTag, pInstance)))
		return E_FAIL;

	CUIMgr::GetInstance()->AddUI(TEXT("UI_BurstBar"), pInstance);

	/**Player Burst SkillBar**/
	pInstance = CUIBurstSkillGauge::Create();
	if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, _strLayerTag, pInstance)))
		return E_FAIL;

	CUIMgr::GetInstance()->AddUI(TEXT("UI_BurstSkillBar"), pInstance);

	/**Quest UI**/

	//pInstance = CUIQuest::Create();
	//if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, _strLayerTag, pInstance)))
	//	return E_FAIL;
	//CUIMgr::GetInstance()->AddUI(TEXT("UI_Quest"), pInstance);

	/**Dialog UI**/

	pInstance = CUIDialog::Create();
	if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, _strLayerTag, pInstance)))
		return E_FAIL;
	CUIMgr::GetInstance()->AddUI(TEXT("UI_Dialog"), pInstance);


	pInstance = CUIMiniquest::Create();

	if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, _strLayerTag, pInstance)))
		return E_FAIL;
	CUIMgr::GetInstance()->AddUI(TEXT("UI_Miniquest"), pInstance);

	pInstance = CUIScreenEffect::Create();
	if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, _strLayerTag, pInstance)))
		return E_FAIL;
	CUIMgr::GetInstance()->AddUI(TEXT("UI_ScreenEffect"), pInstance);

	pInstance = CUIMapTitle::Create();
	if (FAILED(CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, _strLayerTag, pInstance)))
		return E_FAIL;
	CUIMgr::GetInstance()->AddUI(TEXT("UI_QeustTitle"), pInstance);


	return S_OK;
}

HRESULT CArcadeMap::ReadyLight()
{
	LIGHT_DESC LightDesc = {};

	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(0.f, -0.5f, -1.f, 0.f);
	LightDesc.vDiffuse = _float4(0.7f, 0.7f, 0.7f, 1.f);
	LightDesc.vAmbient = _float4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.vSpecular = _float4(0.1f, 0.5f, 0.8f, 1.f);

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
