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
}

HRESULT CMinigameCommand::Render()
{
	if (!m_bProcessing)
		return S_OK;

	/********정답 커맨드 출력 *********/

	wstring strBaseText = TEXT("Answer Command: ");
	wstring strCurrentCommand = TEXT("");

	for (auto& iter : m_AnswerCommand[m_iCurrentRound]) {
		
		switch (iter)
		{
		case 0 :
			strCurrentCommand = TEXT("상");
			break;
		case 1:
			strCurrentCommand = TEXT("하");
			break;
		case 2:
			strCurrentCommand = TEXT("좌");
			break;
		case 3:
			strCurrentCommand = TEXT("우");
			break;
		case 4:
			strCurrentCommand = TEXT("A");
			break;
		case 5:
			strCurrentCommand = TEXT("B");
			break;
		default:
			break;
		}

		strBaseText += strCurrentCommand;
	}

	if (FAILED(CGameInstance::GetInstance()->RenderFont(TEXT("Font_Default"), strBaseText, _float2(300.f, 30.f), XMVectorSet(1.f, 1.f, 0.f, 1.f))))
		return E_FAIL;


	/********플레이어 커맨드 출력 *********/
	strBaseText = TEXT("Player Command: ");
	strCurrentCommand = TEXT("");

	for (auto& iter : m_PlayerCommand) {

		switch (iter)
		{
		case 0:
			strCurrentCommand = TEXT("상");







			break;
		case 1:
			strCurrentCommand = TEXT("하");
			break;
		case 2:
			strCurrentCommand = TEXT("좌");
			break;
		case 3:
			strCurrentCommand = TEXT("우");
			break;
		case 4:
			strCurrentCommand = TEXT("A");
			break;
		case 5:
			strCurrentCommand = TEXT("B");
			break;
		default:
			break;
		}

		strBaseText += strCurrentCommand;
	}

	if (FAILED(CGameInstance::GetInstance()->RenderFont(TEXT("Font_Default"), strBaseText, _float2(300.f, 500.f), XMVectorSet(1.f, 1.f, 0.f, 1.f))))
		return E_FAIL;

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

	//정답일경우
	if (_iNewCmd == m_AnswerCommand[m_iCurrentRound][m_iCurrentCommandIdx]) {

		++m_iCurrentCommandIdx;
		m_PlayerCommand.push_back(_iNewCmd);

		if (m_PlayerCommand.size() == m_AnswerCommand[m_iCurrentRound].size()) {

		//	m_pPlayer->CommandMinigameSuccess(m_iCurrentRound);

			++m_iCurrentRound;
			m_PlayerCommand.clear();
			m_iCurrentCommandIdx = 0;

			if (3 == m_iCurrentRound) {
				GameEnd();
			}
		}

	}
	//오답일경우
	else {
		m_PlayerCommand.clear();
		m_iCurrentCommandIdx = 0;
	}


}

void CMinigameCommand::SetAnswerLists()
{

	/********정답1**********/
	vector<_uint> Answer1;

	Answer1.push_back(0);
	Answer1.push_back(1);
	Answer1.push_back(2);
	Answer1.push_back(3);

	m_AnswerCommand[0] = Answer1;

	/********정답2*********/

	vector<_uint> Answer2;

	Answer2.push_back(3);
	Answer2.push_back(2);
	Answer2.push_back(1);
	Answer2.push_back(0);

	m_AnswerCommand[1] = Answer2;


	/********정답3*********/

	vector<_uint> Answer3;

	Answer3.push_back(2);
	Answer3.push_back(1);
	Answer3.push_back(2);
	Answer3.push_back(3);

	m_AnswerCommand[2] = Answer3;


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

shared_ptr<CMinigameCommand> CMinigameCommand::Create()
{
	shared_ptr<CMinigameCommand> pInstance = make_shared<CMinigameCommand>();

	if (FAILED(pInstance->Initialize()))
		MSG_BOX("Failed to Create : CMinigameCommand");

	return pInstance;
}
