#pragma once

#include "Minigame.h"
#include "MonsterPool.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Tool)

class CMinigameHockey :public CMinigame
{
public:
	CMinigameHockey();
	virtual ~CMinigameHockey() = default;

public:
	virtual HRESULT Initialize(CTransform::TRANSFORM_DESC* _pDesc);

	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void GameStart();
	virtual void GameEnd();
	
private:

	shared_ptr<CCollider> m_pWallColliders[4];
	shared_ptr<class CHockeyBall> m_pHockeyBall;
	

	list<CMonsterPool::SPAWN_INFO> m_MonsterList;

	//하키공 1개 
	//몬스터 4마리->하키게임용 몬스터 따로 만들기(에너미3)
	//콜라이더 4개(벽)

public:
	static shared_ptr<CMinigameHockey> Create();

};

END