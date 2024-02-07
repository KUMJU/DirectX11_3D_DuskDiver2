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

	//��Ű�� 1�� 
	//���� 4����->��Ű���ӿ� ���� ���� �����(���ʹ�3)
	//�ݶ��̴� 4��(��)

public:
	static shared_ptr<CMinigameHockey> Create();

};

END