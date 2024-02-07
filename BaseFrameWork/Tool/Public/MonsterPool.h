#pragma once

#include "Tool_Defines.h"

BEGIN(Tool)


class CMonsterPool
{

DECLARE_SINGLETON(CMonsterPool)

public:
	typedef struct tag_MonsterSpawn
	{
		_uint iMonsterType;
		_vector vMonsterPos;

	}SPAWN_INFO;

private:
	CMonsterPool();
	~CMonsterPool() = default;

public:
	void Initialize();

//몬스터 스포너
public:
	void ActiveMonster(list<SPAWN_INFO> _SpawnList);
	shared_ptr<class CMonster> CheckList(_uint _iMonsterType);

//이벤트 몬스터(곰) 가져오기
public:
	shared_ptr<class CBear> GetBearMonster() { return m_pBear; }

private:
	//0 : enemy 1 // 1: enemy2 
	vector<shared_ptr<class CMonster>> m_Monsters[5];

	//특수성이 있는 몬스터라 따로 관리
	shared_ptr<class CBear> m_pBear = nullptr;



};

END