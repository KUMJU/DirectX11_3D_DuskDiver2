#include "pch.h"
#include "SceneTriggerBoss.h"
#include "BattleSystem.h"
#include "GameInstance.h"

#include "UIMgr.h"
#include "UIScreenEffect.h"

#include "CameraMgr.h"

#include "Bear.h"
#include "MonsterPool.h"

CSceneTriggerBoss::CSceneTriggerBoss()
{
}

HRESULT CSceneTriggerBoss::Initialize(_float3 _vTriggerPos)
{
	CCollider::COLLIDER_DESC tColDesc = {};
	tColDesc.fRadius = 3.f;
	tColDesc.vCenter = { 0.f , 0.f, 0.f };

	__super::Initialize(tColDesc, _vTriggerPos);
	m_IsTouchAble = true;

	SetCameraEvent();


	CMonsterPool::SPAWN_INFO info1 = {};
	info1.iMonsterType = 2;
	info1.vMonsterPos = _vector({ 0.f, 39.5f, -425.f });
	m_SpawnMonsterList.push_back(info1);


	pBear = CMonsterPool::GetInstance()->GetBearMonster();

	return S_OK;
}

void CSceneTriggerBoss::PriorityTick(_float _fTimeDelta)
{
}

void CSceneTriggerBoss::Tick(_float _fTimeDelta)
{
	if (m_bCollided) {
		m_fEventAccTime += _fTimeDelta;


		if (m_fEventAccTime >= 5.f && !m_bScriptDone) {
			pBear->StartWhiteLerp();
			m_bScriptDone = true;

		}

		if (m_fEventAccTime >= 7.f && !m_bScreenEffectDone) {
		
			CUIMgr::GetInstance()->StartScreenEffect(CUIScreenEffect::TYPE_WHITEOUT);
			m_bScreenEffectDone = true;
		
		}

		if (m_fEventAccTime >= 9.5f && !m_bSpawnBoss) {
			SpawnBoss();
			
			m_bSpawnBoss = true;
		}

		if (m_fEventAccTime >= 11.f && !m_bBossSpawnDone) {

			CCameraMgr::GetInstance()->SetLerpMoving({ 0.f, 47.f, -409.f, 1.f }, 0.5f);

			CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_MON_SE);
			CGameInstance::GetInstance()->PlayAudio(TEXT("EN0301_Snarl02.wav"), CSoundMgr::CHANNELID::CH_MON_SE, 1.3f);

			m_bBossSpawnDone = true;
		}

		if (m_fEventAccTime >= 11.5f && !m_bShakingEvent) {
			CGameInstance::GetInstance()->SetZoomBlurOn(30.f, 0.3f);
			CCameraMgr::GetInstance()->SetShakingMode(5.f, 1.f, false);
			m_bShakingEvent = true;
		}

		if (m_fEventAccTime >= 12.8f) {

			CCameraMgr::GetInstance()->SetLerpMovingBack(0.5f);
			m_IsActive = false;
		}

	}



	__super::Tick(_fTimeDelta);


}

void CSceneTriggerBoss::LateTick(_float _fTimeDelta)
{
	__super::LateTick(_fTimeDelta);

}

HRESULT CSceneTriggerBoss::Render()
{
    return S_OK;
}

void CSceneTriggerBoss::SetCameraEvent()
{
}

void CSceneTriggerBoss::OnCollide(CGameObject::EObjType _eObjType, shared_ptr<CCollider> _pCollider)
{
	if (m_bCollided)
		return;

	if (EObjType::OBJ_PLAYER == _eObjType) {

		pBear->SetPosition({ 0.f, 39.5f, -415.f, 1.f });
		pBear->SetEnable(true);

		CUIMgr::GetInstance()->StartDialog(TEXT("LastBoss"));

		m_bCollided = true;
	}
}

void CSceneTriggerBoss::SpawnBoss()
{

	CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_BGM);
	CGameInstance::GetInstance()->PlayBGM(TEXT("BGM_LastBoss_Orchestral.wav"), 1.f);

	CMonsterPool::GetInstance()->ActiveMonster(m_SpawnMonsterList);
	CBattleSystem::GetInstance()->BattleStart();

}

shared_ptr<CSceneTriggerBoss> CSceneTriggerBoss::Create(_float3 _vTriggerPos)
{
	shared_ptr<CSceneTriggerBoss> pTrigger = make_shared<CSceneTriggerBoss>();

	if (FAILED(pTrigger->Initialize(_vTriggerPos))) {
		MSG_BOX("Create to Failed : CSceneTriggerBoss");
	}

	return pTrigger;
}
