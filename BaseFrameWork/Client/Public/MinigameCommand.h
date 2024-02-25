#pragma once

#include "Minigame.h"

BEGIN(Client)


/*
[���� ��]

[Ŀ�ǵ� ���� ġȯǥ]
��: 0
��: 1
��: 2
��: 3

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

	void GameClear();

public:
	virtual void StartEventCutScene();
	virtual void SetCameraEvent();
	void ProcessingEvent(_float _fTimeDelta);


private:
	void CheckCommandList(_uint _iNewCmd);

private:
	//Initialize �ܰ迡�� �̸� ���� ����Ʈ�� �����صд�
	void SetAnswerLists();
	void SetModelPool();
	//�÷��̾� Ű��ǲ �ƿ� ���ƹ����� ���� Ű��ǲ���� 
	void KeyInput();

	void AddBearList(_int _iModelNum);

private:
	//���丮��Ʈ
	list<_uint> m_AnswerCommand;
	list<shared_ptr<class CBear>> m_ActiveBear;
	list<shared_ptr<class CBear>> m_BearPool[4];

private:
	shared_ptr<class CUICommandGame> m_pMinigameUI = nullptr;

private:
	_float m_fBearDropSpeed = 0.f;
	_float m_fCurrentHeight = 0.f;

	_vector m_vInitPos = _vector();
	_int m_iCurrentAccomplish = 0;

	_bool m_bDialogPrint = false;
	_bool m_bZoomInCamEvent = false;
	_bool m_bBearFaceChangeEvent = false;
	_bool m_bFadeIn = false;

	_bool m_bClearGame = false;
	_float m_fClearAccTime = 0.f;

private:
	_float4 m_vLinePositions[6];

private:
	shared_ptr<class CPlayer> m_pPlayer = nullptr;
	shared_ptr<class CBear> m_pBear = nullptr;


private:

	shared_ptr<class CUI_SequenceTex> m_pSuccessImg = nullptr;
	shared_ptr<class CUI_SequenceTex> m_pSparkleImg = nullptr;



public:

	static shared_ptr<CMinigameCommand> Create();

};

END