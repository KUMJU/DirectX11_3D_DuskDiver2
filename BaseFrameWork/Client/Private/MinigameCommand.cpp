#include "pch.h"
#include "MinigameCommand.h"
#include "GameInstance.h"

#include "MinigameTrigger.h"
#include "GameMgr.h"

#include "Player.h"

#include "MonsterPool.h"
#include "Bear.h"
#include "CameraMgr.h"

#include "UICommandGame.h"
#include "UIMgr.h"
#include "UI_SequenceTex.h"


CMinigameCommand::CMinigameCommand()
{
}

HRESULT CMinigameCommand::Initialize()
{
	__super::Initialize();

	m_eMinigameType = EMINIGAME_TYPE::GAME_COMMAND;
	SetModelPool();
	SetAnswerLists();

	shared_ptr<CMinigameTrigger> pTrigger = CMinigameTrigger::Create(dynamic_pointer_cast<CMinigame>(shared_from_this()), { 0.f, 25.f, -166.f });
	CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_Event"), pTrigger);

	m_pPlayer = CGameMgr::GetInstance()->GetPlayer();
	m_pMinigameUI = CUICommandGame::Create();
	m_pMinigameUI->SetEnable(false);
	
	CUI::tagUIInfo UIInfo = {};

	UIInfo.fSizeX = 250.f;
	UIInfo.fSizeY = 250.f;
	UIInfo.fX = g_iWinSizeX * 0.5f + 200.f;
	UIInfo.fY = g_iWinSizeY * 0.5f;

	CUI_SequenceTex::SequenceTexInfo SequeneceInfo = {};
	SequeneceInfo.bLoop = false;
	SequeneceInfo.fScrollTime = 0.05f;
	SequeneceInfo.iCol = 4;
	SequeneceInfo.iRow = 4;

	m_pSparkleImg =  CUI_SequenceTex::Create(&UIInfo, TEXT("T_AOG_Bling_4x4"), 2, &SequeneceInfo);
	m_pSparkleImg->SetEnable(false);

	UIInfo.fSizeX = 300.f;
	UIInfo.fSizeY = 300.f;
	UIInfo.fX = g_iWinSizeX * 0.5f;
	UIInfo.fY = g_iWinSizeY * 0.5f + 50.f;

	SequeneceInfo.bLoop = false;
	SequeneceInfo.fScrollTime = 0.1f;
	SequeneceInfo.iCol = 2;
	SequeneceInfo.iRow = 2;

	m_pSuccessImg = CUI_SequenceTex::Create(&UIInfo, TEXT("T_ToonSmoke_01"), 2, &SequeneceInfo);
	m_pSuccessImg->SetEnable(false);

	return S_OK;
}

void CMinigameCommand::PriorityTick(_float _fTimeDelta)
{
	if (!m_bProcessing)
		return;

}

void CMinigameCommand::Tick(_float _fTimeDelta)
{
	if (!m_bProcessing)
		return;

	if (m_bClearGame) {
		m_fClearAccTime += _fTimeDelta;

		if (m_fClearAccTime >= 3.f && !m_bFadeIn) {
			CUIMgr::GetInstance()->StartScreenEffect(CUIScreenEffect::TYPE_FADE);
			m_bFadeIn = true;
		}

		if (m_fClearAccTime >= 5.f) {
			GameEnd();
		}
	}


	if (!m_bStartCutSceneDone) {
		ProcessingEvent(_fTimeDelta);
		m_pSparkleImg->Tick(_fTimeDelta);
		return;
	}
	
	KeyInput();

	for (auto& iter : m_ActiveBear) {
		iter->Tick(_fTimeDelta);
	}
	m_pSuccessImg->Tick(_fTimeDelta);
	m_pMinigameUI->Tick(_fTimeDelta);


}

void CMinigameCommand::LateTick(_float _fTimeDelta)
{

	if (!m_bStartCutSceneDone && m_bProcessing) {
		m_pBear->LateTick(_fTimeDelta);
		m_pSparkleImg->LateTick(_fTimeDelta);

	}

	if (!m_bProcessing)
		return;

	if (m_bStartCutSceneDone) {
		for (auto& iter : m_ActiveBear) {
			iter->LateTick(_fTimeDelta);
		}
		
		m_pSuccessImg->LateTick(_fTimeDelta);
		m_pMinigameUI->LateTick(_fTimeDelta);
	}
}

HRESULT CMinigameCommand::Render()
{
	if (!m_bProcessing)
		return S_OK;


	return S_OK;
}

void CMinigameCommand::GameStart()
{
	__super::GameStart();
	StartEventCutScene();
	m_pPlayer->SetOnMinigame(true);
	m_pMinigameUI->GameStart();

	//플레이어 키인풋 막아주기 + 마우스 인풋 막아주기 
	//아예 그냥 미니게임 변수를 줘서 어쩌고 저쩌고 한번에 처리해버리기 
}

void CMinigameCommand::GameEnd()
{
	__super::GameEnd();
	m_pPlayer->SetOnMinigame(false);

	CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_BGM);
	CGameInstance::GetInstance()->PlayBGM(TEXT("BGM_MainTheme.wav"),1.f);

	CUIMgr::GetInstance()->HUDOn();
	CCameraMgr::GetInstance()->SwitchingCamera(CCameraMgr::ECAMERATYPE::THIRDPERSON);

}

void CMinigameCommand::GameClear()
{
	m_bClearGame = true;
	m_pMinigameUI->GameClear();
	CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_EFFECT);
	CGameInstance::GetInstance()->PlayAudio(TEXT("se_Item_CrystCube_Open_Luxury.wav"), CSoundMgr::CHANNELID::CH_EFFECT, 1.f);
}

void CMinigameCommand::StartEventCutScene()
{

	_vector vPlrPos = dynamic_pointer_cast<CTransform>(m_pPlayer->GetComponent(TEXT("Com_Transform")))->GetState(CTransform::STATE_POSITION);

	m_pBear = CMonsterPool::GetInstance()->GetBearMonster();
	m_pBear->SetEnable(true);
	m_pBear->SetPosition({ 0.f, 50.f, -172.3f , 1.f });
	
	m_vInitPos = { vPlrPos.m128_f32[0], 50.f, -172.3f , 1.f};

}

void CMinigameCommand::SetCameraEvent()
{

}

void CMinigameCommand::ProcessingEvent(_float _fTimeDelta)
{
	m_fEventProcessTime += _fTimeDelta;

	//	CUIMgr::GetInstance()->StartDialog(TEXT("StartCommandGame"));


	if (m_fEventProcessTime > 1.f && m_fEventProcessTime < 2.f) {

		m_fBearDropSpeed += 0.5f * _fTimeDelta + 0.02f;
		m_vInitPos.m128_f32[1] -= m_fBearDropSpeed;

		if (m_vInitPos.m128_f32[1] < 23.f) {
			m_vInitPos.m128_f32[1] = 23.f;
		}

		m_pBear->SetPosition(m_vInitPos);

	}

	if (m_fEventProcessTime > 2.f && !m_bDialogPrint) {
		CUIMgr::GetInstance()->StartDialog(TEXT("StartCommandGame"));
		m_bDialogPrint = true;

	}

	if (m_fEventProcessTime > 5.f && !m_bZoomInCamEvent) {
		m_bZoomInCamEvent = true;

		CCameraMgr::GetInstance()->SetFreeCamPos({ m_vInitPos.m128_f32[0], 24.5f, -168.8f, 1.f},
			{ m_vInitPos.m128_f32[0], 24.5f, -170.f , 1.f });
		CCameraMgr::GetInstance()->SwitchingCamera(CCameraMgr::ECAMERATYPE::FREE);
	}


	if (m_fEventProcessTime > 6.5f && !m_bBearFaceChangeEvent) {
		m_bBearFaceChangeEvent = true;
		m_pBear->ChangeModel(1);
		m_pSparkleImg->SetEnable(true);

		CUIMgr::GetInstance()->StartScreenEffect(CUIScreenEffect::TYPE_SPEEDLINE);
		CGameInstance::GetInstance()->PlayAudio(TEXT("se_enemy_warning.wav"), CSoundMgr::CHANNELID::CH_EFFECT, 1.f);
	}

	if (m_fEventProcessTime > 8.f && !m_bFadeIn) {
		m_bFadeIn = true;
		CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_MAPSE);
		CGameInstance::GetInstance()->PlayAudio(TEXT("se_CloseView.wav"), CSoundMgr::CHANNELID::CH_MAPSE, 1.f);
		CUIMgr::GetInstance()->StartScreenEffect(CUIScreenEffect::TYPE_FADE);
	}

	if (10.f < m_fEventProcessTime) {

		//실질적인 게임 시작
		CCameraMgr::GetInstance()->SetFreeCamPos({0, 27.5f, -169.f, 1.f},
			{0, 24.6f, -171.8f , 1.f});
		CCameraMgr::GetInstance()->SwitchingCamera(CCameraMgr::ECAMERATYPE::FREE);

		m_pBear->SetEnable(false);
		m_bStartCutSceneDone = true;
		auto iter = m_AnswerCommand.begin();

		for (_int i = 0; i < 6; ++i) {
			AddBearList(*iter);
			++iter;
		}

		CUIMgr::GetInstance()->CloseAllUI();

		m_pMinigameUI->SetEnable(true);

		m_bFadeIn = false;

		CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_BGM);
		CGameInstance::GetInstance()->PlayBGM(TEXT("BGM_CommandMinigame.wav"), 1.f);

	}



}

void CMinigameCommand::CheckCommandList(_uint _iNewCmd)
{
	//정답일때
	if (m_AnswerCommand.front() == _iNewCmd) {
		m_AnswerCommand.pop_front();
		m_ActiveBear.front()->SetEnable(false);
		m_ActiveBear.pop_front();
		++m_iCurrentAccomplish;
	
		m_pMinigameUI->SetCompleteNum(m_iCurrentAccomplish);
		m_pSuccessImg->SetEnable(true);

		if (m_iCurrentAccomplish >= 20) {

			GameClear();
			//GameEnd();
		}
		else {

			auto iter = m_AnswerCommand.begin();
			auto ActiveBearIter = m_ActiveBear.begin();

			for (_int i = 0; i < 5; ++i) {
				(*ActiveBearIter)->MovingFront(XMLoadFloat4(&m_vLinePositions[i]));

				++ActiveBearIter;
				++iter;
			}

			AddBearList(*iter);
			CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_MONHIT);
			CGameInstance::GetInstance()->PlayAudio(TEXT("SE_Command_Success.wav"), CSoundMgr::CHANNELID::CH_MONHIT, 1.7f);
		
			if (m_iCurrentAccomplish == 10) {

				for (auto& iter : m_BearPool) {
					for (auto& pBear : iter) {
						pBear->StartTurn();
					}
				}

			}
		
		}

	
	}
	else {
		m_ActiveBear.front()->SetShaking(1.4f, 0.5f);

		CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_MONHIT);
		CGameInstance::GetInstance()->PlayAudio(TEXT("SE_Command_Fail.wav"), CSoundMgr::CHANNELID::CH_MONHIT, 1.5f);
	}

}

void CMinigameCommand::SetAnswerLists()
{
	//게임 중 위치 세팅 
	for (_int i = 0; i < 6; ++i) {

		m_vLinePositions[i] = { 0.f, 23.f, -172.3f +(i * -1.5f) , 1.f};

	}

	//50개만 맞춰도 종료되게 
	for (_int i = 0; i < 100; ++i) {
		_uint iRandomNum = rand() % 4;
		m_AnswerCommand.push_back(iRandomNum);
	}

}

void CMinigameCommand::SetModelPool()
{
	for (_int i = 0; i < 6; ++i) {

		shared_ptr<CBear> pBear = CBear::Create();
		pBear->SetEnable(false);
		m_BearPool[0].push_back(pBear);
	}

	for (_int i = 0; i < 6; ++i) {

		shared_ptr<CBear> pBear = CBear::Create();
		pBear->ChangeModel(1);
		pBear->SetEnable(false);
		m_BearPool[1].push_back(pBear);
	}

	for (_int i = 0; i < 6; ++i) {

		shared_ptr<CBear> pBear = CBear::Create();
		pBear->SetEnable(false);
		pBear->ChangeModel(2);
		m_BearPool[2].push_back(pBear);
	}

	for (_int i = 0; i < 6; ++i) {

		shared_ptr<CBear> pBear = CBear::Create();
		pBear->ChangeModel(3);
		pBear->SetEnable(false);
		m_BearPool[3].push_back(pBear);
	}


}

void CMinigameCommand::KeyInput()
{
	if (CGameInstance::GetInstance()->Key_Down(VK_UP)) {
		CheckCommandList(0);
	}
	
	if (CGameInstance::GetInstance()->Key_Down(VK_DOWN)) {
		CheckCommandList(1);

	}
	
	if (CGameInstance::GetInstance()->Key_Down(VK_LEFT)) {
		CheckCommandList(2);

	}
	
	if (CGameInstance::GetInstance()->Key_Down(VK_RIGHT)) {
		CheckCommandList(3);

	}

}

void CMinigameCommand::AddBearList(_int _iModelNum)
{
	for (auto& iter : m_BearPool[_iModelNum]) {
		if (!iter->IsEnabled()) {
			m_ActiveBear.push_back(iter);
			iter->SetPosition(XMLoadFloat4(&m_vLinePositions[m_ActiveBear.size() - 1]));
			iter->SetEnable(true);
			break;
		}
	}

}

shared_ptr<CMinigameCommand> CMinigameCommand::Create()
{
	shared_ptr<CMinigameCommand> pInstance = make_shared<CMinigameCommand>();

	if (FAILED(pInstance->Initialize()))
		MSG_BOX("Failed to Create : CMinigameCommand");

	return pInstance;
}
