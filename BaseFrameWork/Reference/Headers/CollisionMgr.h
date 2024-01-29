#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class CCollider;


class CCollisionMgr
{
public:
	enum COLTYPE_GROUP {
		COL_PLAYER, //�÷��̾� Body
		COL_MONSTER,
		COL_BLOCK, //���� Body
		COL_PLRPROJ, //�÷��̾� ����ü
		COL_MONPROJ, //���� ����ü 
		COL_END
	};

//�÷��̾�-����&�ʿ�����Ʈ : ������ �� ������
//�÷��̾�-���� ��������ü : �÷��̾� ���ݴ���
//����- �÷��̾� ��������ü : ���� ���ݴ���

public:
	CCollisionMgr();
	~CCollisionMgr() = default;

public:

	void AddColliderList(COLTYPE_GROUP _eColGroup, shared_ptr<CCollider> _pCollider);

public:
	void LateTick();

private:


	void CheckCollisionGroup(COLTYPE_GROUP _eColGroup1, COLTYPE_GROUP _eColGroup2);


	void CheckPlrBlock();
	void CheckPlrProjMonster();
	void CheckPlrMonProj();
	void CheckMonsterCollide();


private:

	void ListClear();

private:
	list<shared_ptr<CCollider>> m_ColliderList[COL_END];

public:

	static shared_ptr<CCollisionMgr> Create();

};

END