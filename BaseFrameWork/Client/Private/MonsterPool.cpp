#include "pch.h"
#include "MonsterPool.h"

#include "Enemy01.h"
#include "Enemy02.h"

#include "GameInstance.h"

IMPLEMENT_SINGLETON(CMonsterPool)

CMonsterPool::CMonsterPool()
{
}

void CMonsterPool::Initialize()
{
	//enemy 1 持失
	for (_int i = 0; i < 5; ++i) {

		shared_ptr<CMonster> pMonster = CEnemy01::Create();
		pMonster->SetEnable(false);
		m_Monsters[0].push_back(pMonster);
	}

	//enemy2 持失
	for (_int i = 0; i < 3; ++i) {

		shared_ptr<CMonster> pMonster = CEnemy02::Create();
		pMonster->SetEnable(false);
		m_Monsters[1].push_back(pMonster);
	}

}

void CMonsterPool::ActiveMonster(list<SPAWN_INFO> _SpawnList)
{
	for (auto& iter : _SpawnList) {

		shared_ptr<CMonster> pMonster = CheckList(iter.iMonsterType);
		
		if (pMonster) {

			pMonster->SetEnable(true);
			CGameInstance::GetInstance()->AddObject(CGameInstance::GetInstance()->GetCurrentLevel(), TEXT("Layer_Monster"), pMonster);
			pMonster->SetPosition(XMVectorSetW(iter.vMonsterPos, 1.f));
		}
	}
}

shared_ptr<CMonster> CMonsterPool::CheckList(_uint _iMonsterType)
{
	for (auto& iter : m_Monsters[_iMonsterType]) {

		if (!iter->IsEnabled()) {
			return iter;
		}
	}

	return nullptr;
}
