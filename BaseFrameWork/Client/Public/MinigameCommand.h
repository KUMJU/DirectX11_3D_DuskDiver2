#pragma once

#include "Minigame.h"

BEGIN(Client)


/*
[게임 룰]
커맨드는 총 6개
상 하 좌 우 공격키 2개

[커맨드 숫자 치환표]
상: 0
하: 1
좌: 2
우: 3
약공격: 4
강공격: 5

정답1 : 상하좌우강약
정답2 :하상우하상우공공공
정답3 : 우상하상좌우상하상좌강강강


*/


class CMinigameCommand : public CMinigame
{
public:
	CMinigameCommand();
	virtual ~CMinigameCommand() = default;

public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void GameStart();
	virtual void GameEnd();

public:
	virtual void StartEventCutScene();
	virtual void SetCameraEvent();
	void ProcessingEvent(_float _fTimeDelta);


private:
	void CheckCommandList(_uint _iNewCmd);

private:
	//Initialize 단계에서 미리 정답 리스트를 세팅해둔다
	void SetAnswerLists();
	//플레이어 키인풋 아예 막아버리고 여기 키인풋쓰기 
	void KeyInput();

private:

	//정답리스트
	vector<_uint> m_AnswerCommand[3];
	list<_uint> m_PlayerCommand;

	//현재 진행 라운드 
	int m_iCurrentRound = 0;
	//지금 입력하고 있는 커맨드 순서(정답 리스트와 비교하기 위한 변수)
	int m_iCurrentCommandIdx = 0;


private:
	_float m_fBearDropSpeed = 0.f;
	_float m_fCurrentHeight = 0.f;

	_vector m_vInitPos = _vector();

private:
	shared_ptr<class CPlayer> m_pPlayer = nullptr;
	shared_ptr<class CBear> m_pBear = nullptr;


public:

	static shared_ptr<CMinigameCommand> Create();

};

END