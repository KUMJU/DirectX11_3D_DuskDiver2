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

//���� ������
public:
	void ActiveMonster(list<SPAWN_INFO> _SpawnList);
	shared_ptr<class CMonster> CheckList(_uint _iMonsterType);

//�̺�Ʈ ����(��) ��������
public:
	shared_ptr<class CBear> GetBearMonster() { return m_pBear; }

private:
	//0 : enemy 1 // 1: enemy2 
	vector<shared_ptr<class CMonster>> m_Monsters[5];

	//Ư������ �ִ� ���Ͷ� ���� ����
	shared_ptr<class CBear> m_pBear = nullptr;



};

END