#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class CCollider;


class CCollisionMgr
{
public:
	enum COLTYPE_GROUP {
		COL_PLAYER, //플레이어 Body
		COL_MONSTER,
		COL_BLOCK, //몬스터 Body
		COL_PLRPROJ, //플레이어 공격체
		COL_MONPROJ, //몬스터 공격체 
		COL_TRIGGER,
		COL_END
	};

//플레이어-몬스터&맵오브젝트 : 막혀서 못 지나감
//플레이어-몬스터 공격투사체 : 플레이어 공격당함
//몬스터- 플레이어 공격투사체 : 몬스터 공격당함

public:
	CCollisionMgr();
	~CCollisionMgr() = default;

public:

	void AddColliderList(COLTYPE_GROUP _eColGroup, shared_ptr<CCollider> _pCollider);

public:
	void LateTick();

private:
	void CheckPlrBlock();
	void CheckPlrProjMonster();
	void CheckPlrMonProj();
	void CheckMonsterCollide();
	void CheckPlrTrigger();
	void CheckPlrProjTrigger();


private:

	void ListClear();

private:
	list<shared_ptr<CCollider>> m_ColliderList[COL_END];

public:

	static shared_ptr<CCollisionMgr> Create();

};

END