#include "pch.h"
#include "MonsterPool.h"

#include "Enemy01.h"
#include "Enemy02.h"
#include "MiddleBoss.h"
#include "FinalBoss.h"
#include "Bear.h"

#include "GameInstance.h"

#include "Navigation.h"

IMPLEMENT_SINGLETON(CMonsterPool)

CMonsterPool::CMonsterPool()
{
}

void CMonsterPool::Initialize()
{
	//enemy 1 ����
	for (_int i = 0; i < 5; ++i) {

		shared_ptr<CMonster> pMonster = CEnemy01::Create();
		pMonster->SetEnable(false);
		m_Monsters[0].push_back(pMonster);
	}

	//enemy2 ����
	for (_int i = 0; i < 3; ++i) {

		shared_ptr<CMonster> pMonster = CEnemy02::Create();
		pMonster->SetEnable(false);
		m_Monsters[1].push_back(pMonster);
	}

	for (_int i = 0; i < 1; ++i) {

		shared_ptr<CMonster> pMonster = CMiddleBoss::Create();
		pMonster->SetEnable(false);
		m_Monsters[2].push_back(pMonster);
	}

	for (_int i = 0; i < 1; ++i) {

		shared_ptr<CMonster> pMonster = CFinalBoss::Create();
		pMonster->SetEnable(false);
		m_Monsters[3].push_back(pMonster);
	}

	for (_int i = 0; i < 3; ++i) {

		shared_ptr<CMonster> pMonster = CEnemy01::Create();
		pMonster->SetHockeyMonster();
		pMonster->SetEnable(false);
		m_Monsters[4].push_back(pMonster);
	}

	m_pBear = CBear::Create();
	m_pBear->SetEnable(false);

	CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_NPC"), m_pBear);

}

void CMonsterPool::ActiveMonster(list<SPAWN_INFO> _SpawnList)
{
	for (auto& iter : _SpawnList) {

		shared_ptr<CMonster> pMonster = CheckList(iter.iMonsterType);
		
		if (pMonster) {

			pMonster->SetSpawnState();
			pMonster->SetEnable(true);
			CGameInstance::GetInstance()->AddObject(CGameInstance::GetInstance()->GetCurrentLevel(), TEXT("Layer_Monster"), pMonster);
			pMonster->SetPosition(XMVectorSetW(iter.vMonsterPos, 1.f));

			shared_ptr<CComponent> pNavi = pMonster->GetComponent(TEXT("Com_Navigation"));

			if (pNavi) {
				dynamic_pointer_cast<CNavigation>(pNavi)->CalcCurrentPos(iter.vMonsterPos);

			}
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