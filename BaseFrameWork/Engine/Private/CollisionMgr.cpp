#include "CollisionMgr.h"
#include "Collider.h"
#include "Bounding.h"

#include "GameObject.h"


CCollisionMgr::CCollisionMgr()
{
}

void CCollisionMgr::AddColliderList(COLTYPE_GROUP _eColGroup, shared_ptr<CCollider> _pCollider)
{
	m_ColliderList[_eColGroup].push_back(_pCollider);
}

void CCollisionMgr::LateTick()
{
	CheckPlrProjMonster();
	CheckPlrMonProj();
	CheckPlrBlock();

	ListClear();
}

void CCollisionMgr::CheckPlrBlock()
{
	for (auto& pPlr : m_ColliderList[COL_PLAYER]) {
		for (auto& pMon : m_ColliderList[COL_MONSTER]) {

			if (pPlr->Intersect(pMon->GetColliderType(), pMon->GetBounding())) {
				//Ãæµ¹µÊ
				(pPlr->GetOwner())->OnCollide(CGameObject::OBJ_MONSTER, pMon);
				(pMon->GetOwner())->OnCollide(CGameObject::OBJ_PLAYER, pPlr);
			}
		}
	}


}

void CCollisionMgr::CheckPlrProjMonster()
{
	for (auto& pPlr : m_ColliderList[COL_PLRPROJ]) {
		for (auto& pMon : m_ColliderList[COL_MONSTER]) {

			if (pPlr->Intersect(pMon->GetColliderType(), pMon->GetBounding())) {
				//Ãæµ¹µÊ
				(pPlr->GetOwner())->OnCollide(CGameObject::OBJ_MONSTER, pMon);
				(pMon->GetOwner())->OnCollide(CGameObject::OBJ_PROJ, pPlr);
			}
		}
	}
}

void CCollisionMgr::CheckPlrMonProj()
{
}

void CCollisionMgr::ListClear()
{
	for (auto& iter : m_ColliderList) {
		iter.clear();
	}

}

shared_ptr<CCollisionMgr> CCollisionMgr::Create()
{
	return make_shared<CCollisionMgr>();
}
