#include "pch.h"
#include "BattleSystem.h"
#include "GameInstance.h"
#include "GameMgr.h"
#include "Player.h"
#include "Layers.h"

#include "Monster.h"

IMPLEMENT_SINGLETON(CBattleSystem)

CBattleSystem::CBattleSystem()
{
}

void CBattleSystem::Initialize()
{
	m_pPlayer = CGameMgr::GetInstance()->GetPlayer();
}

void CBattleSystem::BattleStart()
{
	m_bOnBattle = true;
	m_bEventDone = false;
	m_pPlayer->SetOnBattle(true);
}

void CBattleSystem::BattleEnd()
{
	m_bOnBattle = false;
	m_bEventDone = false;
	m_pPlayer->SetOnBattle(false);

}

void CBattleSystem::Tick(_float _fTimeDelta)
{

	if (m_bLastOne) {
		m_pMonsterLayer = CGameInstance::GetInstance()->FindLayer(TEXT("Layer_Monster"));
		list<shared_ptr<CGameObject>>* pMonsterList = m_pMonsterLayer->GetObjectList();

		if (!(pMonsterList->front()->IsEnabled())) {
			pMonsterList->clear();
			m_bLastOne = false;
		}
	}

	if (m_bEventDone)
		return;
	

	if (m_bSlowMotion) {

		m_fSlowTIme += _fTimeDelta;

		if (m_fSlowTIme > 0.6f) {
			m_bSlowMotion = false;
			m_fSlowTIme = 0.f;
			m_bEventDone = true;
			CGameInstance::GetInstance()->SetTimerOffset(TEXT("Timer_60"), 1.f);
			BattleEnd();
			return;
		}
	}

	if (!m_bOnBattle) {
		return;
	}

	m_pMonsterLayer = CGameInstance::GetInstance()->FindLayer(TEXT("Layer_Monster"));

	if (!m_pMonsterLayer)
		return;

	list<shared_ptr<CGameObject>>* pMonsterList = m_pMonsterLayer->GetObjectList();

	for (auto iter = pMonsterList->begin(); iter != pMonsterList->end(); ) {
		//몬스터가 비활성화 되었다면 리스트에서 빼주기 
		//어차피 몬스터풀에서는 남아있기 때문에 지워도 ㄱㅊ 
		if (!((*iter)->IsEnabled())) {
			iter = pMonsterList->erase(iter);
		}
		else {
			++iter;
		}
	}



	if (1 == pMonsterList->size() && !m_bSlowMotion) {
		if (dynamic_pointer_cast<CMonster>(pMonsterList->front())->GetIsDead()) {
			m_bSlowMotion = true;
			KillLastOne();
		}
	}

	//if (pMonsterList->empty()) {

	//	BattleEnd();
	//}

	
}

void CBattleSystem::KillLastOne()
{
	CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_MONHIT);
	CGameInstance::GetInstance()->PlayAudio(TEXT("se_slowmotion_finish.wav"), CSoundMgr::CHANNELID::CH_MONHIT, 1.3f);
	CGameInstance::GetInstance()->SetTimerOffset(TEXT("Timer_60"), 0.3f);
	m_bLastOne = true;
}

void CBattleSystem::EraseMonster()
{

}
