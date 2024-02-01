#pragma once

#include "Client_Defines.h"

BEGIN(Client)


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

public:
	void ActiveMonster(list<SPAWN_INFO> _SpawnList);
	shared_ptr<class CMonster> CheckList(_uint _iMonsterType);

private:
	//0 : enemy 1 // 1: enemy2 
	vector<shared_ptr<class CMonster>> m_Monsters[4];




};

END