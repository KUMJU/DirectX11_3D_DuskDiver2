#include "pch.h"
#include "MinigameCommand.h"
#include "GameInstance.h"

#include "MinigameTrigger.h"
#include "GameMgr.h"

#include "Player.h"

#include "MonsterPool.h"
#include "Bear.h"
#include "CameraMgr.h"


CMinigameCommand::CMinigameCommand()
{
}

HRESULT CMinigameCommand::Initialize()
{
	__super::Initialize();

	m_eMinigameType = EMINIGAME_TYPE::GAME_COMMAND;
	SetModelPool();
	SetAnswerLists();

	shared_ptr<CMinigameTrigger> pTrigger = CMinigameTrigger::Create(dynamic_pointer_cast<CMinigame>(shared_from_this()), { 0.f, 25.f, -170.f });
	CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_Event"), pTrigger);

	m_pPlayer = CGameMgr::GetInstance()->GetPlayer();

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

	if (!m_bStartCutSceneDone) {
		ProcessingEvent(_fTimeDelta);
		return;
	}


	KeyInput();

}

void CMinigameCommand::LateTick(_float _fTimeDelta)
{
	if (!m_bProcessing)
		return;

	if (m_bStartCutSceneDone) {
		CGameInstance::GetInstance()->AddRenderGroup(CRenderer::RENDER_UI, shared_from_this());
	}
	else {





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
	
	//플레이어 키인풋 막아주기 + 마우스 인풋 막아주기 
	//아예 그냥 미니게임 변수를 줘서 어쩌고 저쩌고 한번에 처리해버리기 

	



}

void CMinigameCommand::GameEnd()
{
	__super::GameEnd();
	m_pPlayer->SetOnMinigame(false);
	CCameraMgr::GetInstance()->SwitchingCamera(CCameraMgr::ECAMERATYPE::THIRDPERSON);

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

	if (m_fEventProcessTime > 1.f && m_fEventProcessTime < 2.f) {

		m_fBearDropSpeed += 0.5f * _fTimeDelta + 0.02f;
		m_vInitPos.m128_f32[1] -= m_fBearDropSpeed;

		if (m_vInitPos.m128_f32[1] < 23.f) {
			m_vInitPos.m128_f32[1] = 23.f;
		}

		m_pBear->SetPosition(m_vInitPos);

	}
	if (3.5f < m_fEventProcessTime) {

		CCameraMgr::GetInstance()->SetFreeCamPos({ m_vInitPos.m128_f32[0] -3.2f, 24.6f, -170.8f , 1.f}, { m_vInitPos.m128_f32[0], 24.6f, -170.8f , 1.f});
		CCameraMgr::GetInstance()->SwitchingCamera(CCameraMgr::ECAMERATYPE::FREE);

		m_bStartCutSceneDone = true;
	}



}

void CMinigameCommand::CheckCommandList(_uint _iNewCmd)
{
	//정답일때
	if (m_AnswerCommand.front() == _iNewCmd) {
		m_AnswerCommand.pop_front();
		m_ActiveBear.pop_front();
		++m_iCurrentAccomplish;
	}
	else {


	}

}

void CMinigameCommand::SetAnswerLists()
{
	//50개만 맞춰도 종료되게 
	for (_int i = 0; i < 100; ++i) {
		_uint iRandomNum = rand() % 4;
		m_AnswerCommand.push_back(iRandomNum);
	}

	auto iter = m_AnswerCommand.begin();

	for (_int i = 0; i < 6; ++i) {
		AddBearList(*iter);
		++iter;
	}

}

void CMinigameCommand::SetModelPool()
{
	for (_int i = 0; i < 6; ++i) {

		shared_ptr<CBear> pBear = CBear::Create();
		m_BearPool[0].push_back(pBear);
	}

	for (_int i = 0; i < 6; ++i) {

		shared_ptr<CBear> pBear = CBear::Create();
		pBear->ChangeModel(1);
		m_BearPool[1].push_back(pBear);
	}

	for (_int i = 0; i < 6; ++i) {

		shared_ptr<CBear> pBear = CBear::Create();
		pBear->ChangeModel(2);
		m_BearPool[2].push_back(pBear);
	}

	for (_int i = 0; i < 6; ++i) {

		shared_ptr<CBear> pBear = CBear::Create();
		pBear->ChangeModel(3);
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
