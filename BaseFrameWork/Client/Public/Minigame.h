#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CMinigame abstract : public CGameObject
{
public:
	enum class EMINIGAME_TYPE {
		GAME_MOLE, GAME_HOCKEY, GAME_COMMAND, GAME_END
	};


public:
	CMinigame();
	virtual ~CMinigame() = default;

public:
	virtual HRESULT Initialize();

	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void GameStart();
	virtual void GameEnd();

	virtual void StartEventCutScene() {};
	virtual void EndEvenetCutScene() {};
		
	EMINIGAME_TYPE GetMinigameType() { return m_eMinigameType; }

public:
	virtual void SetCameraEvent() {};

protected:
	_bool m_bDone = false;
	_bool m_bProcessing = false;

	_bool m_bStartCutSceneDone = false;
	_bool m_bCutEndSceneDone = false;

	_float m_fEventProcessTime = 0.f;

	EMINIGAME_TYPE m_eMinigameType = EMINIGAME_TYPE::GAME_END;

};



END