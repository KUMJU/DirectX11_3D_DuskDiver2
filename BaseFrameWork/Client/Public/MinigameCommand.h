#pragma once

#include "Minigame.h"

BEGIN(Client)


/*
[���� ��]
Ŀ�ǵ�� �� 6��
�� �� �� �� ����Ű 2��

[Ŀ�ǵ� ���� ġȯǥ]
��: 0
��: 1
��: 2
��: 3
�����: 4
������: 5

����1 : �����¿찭��
����2 :�ϻ���ϻ�������
����3 : ����ϻ��¿���ϻ��°�����


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
	//Initialize �ܰ迡�� �̸� ���� ����Ʈ�� �����صд�
	void SetAnswerLists();
	//�÷��̾� Ű��ǲ �ƿ� ���ƹ����� ���� Ű��ǲ���� 
	void KeyInput();

private:

	//���丮��Ʈ
	vector<_uint> m_AnswerCommand[3];
	list<_uint> m_PlayerCommand;

	//���� ���� ���� 
	int m_iCurrentRound = 0;
	//���� �Է��ϰ� �ִ� Ŀ�ǵ� ����(���� ����Ʈ�� ���ϱ� ���� ����)
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