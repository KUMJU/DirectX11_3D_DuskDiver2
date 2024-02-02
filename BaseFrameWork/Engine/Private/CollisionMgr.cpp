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

	CheckPlrMonster();
	CheckPlrBlock();

	CheckMonsterBlock();

	CheckPlrTrigger();
	CheckPlrProjTrigger();

	CheckBlocktoBlock();

	ListClear();
}

void CCollisionMgr::CheckPlrMonster()
{
	for (auto& pPlr : m_ColliderList[COL_PLAYER]) {
		for (auto& pMon : m_ColliderList[COL_MONSTER]) {

			if (pPlr->Intersect(pMon->GetColliderType(), pMon->GetBounding())) {
				(pPlr->GetOwner())->OnCollide(CGameObject::OBJ_MONSTER, pMon);
				(pMon->GetOwner())->OnCollide(CGameObject::OBJ_PLAYER, pPlr);
			}
		}
	}


}

void CCollisionMgr::CheckPlrBlock()
{
	for (auto& pPlr : m_ColliderList[COL_PLAYER]) {
		for (auto& pBlock : m_ColliderList[COL_BLOCK]) {

			if (pPlr->Intersect(pBlock->GetColliderType(), pBlock->GetBounding())) {
				//Ãæµ¹µÊ

				CGameObject::EObjType eObjType = pBlock->GetOwner()->GetObjectType();

				(pPlr->GetOwner())->OnCollide(eObjType, pBlock);
				(pBlock->GetOwner())->OnCollide(CGameObject::OBJ_PLAYER, pPlr);
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

	for (auto& pPlr : m_ColliderList[COL_PLAYER]) {
		for (auto& pMon : m_ColliderList[COL_MONPROJ]) {

			if (pPlr->Intersect(pMon->GetColliderType(), pMon->GetBounding())) {
				//Ãæµ¹µÊ
				(pPlr->GetOwner())->OnCollide(CGameObject::OBJ_PROJ, pMon);
				(pMon->GetOwner())->OnCollide(CGameObject::OBJ_PLAYER, pPlr);
			}
		}
	}

}

void CCollisionMgr::CheckMonsterCollide()
{

	//for (auto iter = m_ColliderList[COL_MONSTER].begin() ; i < m_ColliderList[COL_MONSTER].size() - 1; ++i) {

	//	for (size_t j = i + 1; j < m_ColliderList[COL_MONSTER].size(); ++j) {
	//	
	//		if (pPlr->Intersect(pMon->GetColliderType(), pMon->GetBounding())) {
	//			//Ãæµ¹µÊ
	//			(pPlr->GetOwner())->OnCollide(CGameObject::OBJ_PROJ, pMon);
	//			(pMon->GetOwner())->OnCollide(CGameObject::OBJ_PLAYER, pPlr);
	//		}
	//	
	//	}
	//}

}

void CCollisionMgr::CheckMonsterBlock()
{
	for (auto& pMon : m_ColliderList[COL_MONSTER]) {
		for (auto& pBlock : m_ColliderList[COL_BLOCK]) {

			CGameObject::EObjType eObjType = pBlock->GetOwner()->GetObjectType();

			if (pMon->Intersect(pBlock->GetColliderType(), pBlock->GetBounding())) {
				(pMon->GetOwner())->OnCollide(eObjType, pBlock);
			}
		}
	}
}

void CCollisionMgr::CheckPlrTrigger()
{
	for (auto& pPlr : m_ColliderList[COL_PLAYER]) {
		for (auto& pTrigger : m_ColliderList[COL_TRIGGER]) {

			if (pPlr->Intersect(pTrigger->GetColliderType(), pTrigger->GetBounding())) {
				(pTrigger->GetOwner())->OnCollide(CGameObject::OBJ_PLAYER, pPlr);
			}
		}
	}
}

void CCollisionMgr::CheckPlrProjTrigger()
{
	for (auto& pPlr : m_ColliderList[COL_PLRPROJ]) {
		for (auto& pTrigger : m_ColliderList[COL_TRIGGER]) {

			if (pPlr->Intersect(pTrigger->GetColliderType(), pTrigger->GetBounding())) {
				(pTrigger->GetOwner())->OnCollide(CGameObject::OBJ_PROJ, pPlr);
			}
		}
	}
}

void CCollisionMgr::CheckBlocktoBlock()
{

	if (m_ColliderList[COL_BLOCK].size() == 1 || m_ColliderList[COL_BLOCK].empty()) {
		return;
	}

	size_t iListSize = m_ColliderList[COL_BLOCK].size();

	auto iter = m_ColliderList[COL_BLOCK].begin();
	auto iter2 = m_ColliderList[COL_BLOCK].begin();

	for (size_t i = 0; i < iListSize -1 ; ++i) {

		auto iter3 = iter;
		iter2 = ++iter3;

		for (size_t j = 0; j < iListSize - i -1 ; ++j) {

			if ((*iter)->Intersect((*iter2)->GetColliderType(), (*iter2)->GetBounding())) {

				CGameObject::EObjType eObjType1 = (*iter)->GetOwner()->GetObjectType();
				CGameObject::EObjType eObjType2 = (*iter2)->GetOwner()->GetObjectType();

				((*iter)->GetOwner())->OnCollide(eObjType2, (*iter2));
				((*iter2)->GetOwner())->OnCollide(eObjType1, (*iter));
			}
			++iter2;
		}

		++iter;
	}

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
