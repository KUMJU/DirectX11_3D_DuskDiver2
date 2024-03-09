#include "pch.h"
#include "SceneTriggerInsert.h"
#include "UIMgr.h"

#include "GameInstance.h"

CSceneTriggerInsert::CSceneTriggerInsert()
{
}

HRESULT CSceneTriggerInsert::Initialize(_float3 _vTriggerPos)
{
	CCollider::COLLIDER_DESC tColDesc = {};
	tColDesc.fRadius = 3.f;
	tColDesc.vCenter = { 0.f , 0.f, 0.f };

	__super::Initialize(tColDesc, _vTriggerPos);
	m_IsTouchAble = true;

	return S_OK;
}

void CSceneTriggerInsert::PriorityTick(_float _fTimeDelta)
{
}

void CSceneTriggerInsert::Tick(_float _fTimeDelta)
{
	if (!m_IsEnabled)
		return;

	if (m_bCollided) {
		m_fEventAccTime += _fTimeDelta;

		if (m_fEventAccTime >= 1.f) {

			CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_MAPSE);
			CGameInstance::GetInstance()->PlayAudio(TEXT("se_Env12_CoinMach_On.wav"), CSoundMgr::CHANNELID::CH_MAPSE, 1.5f);
			CUIMgr::GetInstance()->CloseUI(TEXT("UI_Miniquest"));

			CUIMgr::GetInstance()->SetQuestDesc(TEXT("계속 앞으로 나아간다"));
			
			m_IsActive = false;
		}
	}

	__super::Tick(_fTimeDelta);
}

void CSceneTriggerInsert::LateTick(_float _fTimeDelta)
{
	__super::LateTick(_fTimeDelta);

}

HRESULT CSceneTriggerInsert::Render()
{
    return S_OK;
}

void CSceneTriggerInsert::OnCollide(CGameObject::EObjType _eObjType, shared_ptr<CCollider> _pCollider)
{
	if (!m_IsEnabled)
		return;

	if (m_bCollided)
		return;

	//플레이어 충돌
	if (CGameObject::EObjType::OBJ_PLAYER == _eObjType) {
		CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_MAPOBJ_SE);
		CGameInstance::GetInstance()->PlayAudio(TEXT("se_Env12_InsertCoins.wav"), CSoundMgr::CHANNELID::CH_MAPOBJ_SE, 1.f);

		m_bCollided = true;
	}
}

shared_ptr<CSceneTriggerInsert> CSceneTriggerInsert::Create(_float3 _vTriggerPos)
{
	shared_ptr<CSceneTriggerInsert> pInstance = make_shared<CSceneTriggerInsert>();

	if (FAILED(pInstance->Initialize(_vTriggerPos)))
		MSG_BOX("Create to Failed : CSceneTriggerInsert");

	return pInstance;
}
