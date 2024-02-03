#include "pch.h"
#include "MinigameCommand.h"
#include "GameInstance.h"

#include "MinigameTrigger.h"
#include "GameMgr.h"

#include "Player.h"


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

	KeyInput();

}

void CMinigameCommand::LateTick(_float _fTimeDelta)
{
	if (!m_bProcessing)
		return;

	CGameInstance::GetInstance()->AddRenderGroup(CRenderer::RENDER_UI, shared_from_this());

}

HRESULT CMinigameCommand::Render()
{
	if (!m_bProcessing)
		return S_OK;

	/********���� Ŀ�ǵ� ��� *********/

	wstring strBaseText = TEXT("Answer Command: ");
	wstring strCurrentCommand = TEXT("");

	for (auto& iter : m_AnswerCommand[m_iCurrentRound]) {
		
		switch (iter)
		{
		case 0 :
			strCurrentCommand = TEXT("��");
			break;
		case 1:
			strCurrentCommand = TEXT("��");
			break;
		case 2:
			strCurrentCommand = TEXT("��");
			break;
		case 3:
			strCurrentCommand = TEXT("��");
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


	/********�÷��̾� Ŀ�ǵ� ��� *********/
	strBaseText = TEXT("Player Command: ");
	strCurrentCommand = TEXT("");

	for (auto& iter : m_PlayerCommand) {

		switch (iter)
		{
		case 0:
			strCurrentCommand = TEXT("��");
			break;
		case 1:
			strCurrentCommand = TEXT("��");
			break;
		case 2:
			strCurrentCommand = TEXT("��");
			break;
		case 3:
			strCurrentCommand = TEXT("��");
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

	m_pPlayer->SetOnMinigame(true);
	
	//�÷��̾� Ű��ǲ �����ֱ� + ���콺 ��ǲ �����ֱ� 
	//�ƿ� �׳� �̴ϰ��� ������ �༭ ��¼�� ��¼�� �ѹ��� ó���ع����� 



}

void CMinigameCommand::GameEnd()
{
	__super::GameEnd();

	m_pPlayer->SetOnMinigame(false);


}

void CMinigameCommand::CheckCommandList(_uint _iNewCmd)
{

	//�����ϰ��
	if (_iNewCmd == m_AnswerCommand[m_iCurrentRound][m_iCurrentCommandIdx]) {

		++m_iCurrentCommandIdx;
		m_PlayerCommand.push_back(_iNewCmd);

		if (m_PlayerCommand.size() == m_AnswerCommand[m_iCurrentRound].size()) {

			m_pPlayer->CommandMinigameSuccess(m_iCurrentRound);

			++m_iCurrentRound;
			m_PlayerCommand.clear();
			m_iCurrentCommandIdx = 0;

			if (3 == m_iCurrentRound) {
				GameEnd();
			}
		}

	}
	//�����ϰ��
	else {
		m_PlayerCommand.clear();
		m_iCurrentCommandIdx = 0;
	}


}

void CMinigameCommand::SetAnswerLists()
{

	/********����1**********/
	vector<_uint> Answer1;

	Answer1.push_back(0);
	Answer1.push_back(1);
	Answer1.push_back(2);
	Answer1.push_back(3);

	m_AnswerCommand[0] = Answer1;

	/********����2*********/

	vector<_uint> Answer2;

	Answer2.push_back(3);
	Answer2.push_back(2);
	Answer2.push_back(1);
	Answer2.push_back(0);

	m_AnswerCommand[1] = Answer2;


	/********����3*********/

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
