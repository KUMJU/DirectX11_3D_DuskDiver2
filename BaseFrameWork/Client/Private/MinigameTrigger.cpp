#include "pch.h"
#include "MinigameTrigger.h"
#include "Collider.h"
#include "Minigame.h"

CMinigameTrigger::CMinigameTrigger()
{
}

HRESULT CMinigameTrigger::Initialize(shared_ptr<class CMinigame> _pMinigame, _float3 _vTriggerPos)
{
	m_eTriggerType = ETRIGGER_TYPE::TRI_MINIGAME;
	
	CCollider::COLLIDER_DESC colDesc = {};
	colDesc.vCenter = { 0.f, 0.3f, 0.f };
	colDesc.fRadius = 2.f;

	__super::Initialize(colDesc, _vTriggerPos);
	m_pMinigame = _pMinigame;

	if (CMinigame::EMINIGAME_TYPE::GAME_HOCKEY == m_pMinigame->GetMinigameType()) {
		m_IsTouchAble = false;
	}
	else {
		m_IsTouchAble = true;
	}

	return S_OK;
}

void CMinigameTrigger::PriorityTick(_float _fTimeDelta)
{
}

void CMinigameTrigger::Tick(_float _fTimeDelta)
{
	m_pMinigame->GetMinigameType();

	__super::Tick(_fTimeDelta);

}

void CMinigameTrigger::LateTick(_float _fTimeDelta)
{
	__super::LateTick(_fTimeDelta);

}

HRESULT CMinigameTrigger::Render()
{
	__super::Render();

	return S_OK;
}

void CMinigameTrigger::OnCollide(CGameObject::EObjType _eObjType, shared_ptr<CCollider> _pCollider)
{
	if (!m_IsEnabled)
		return;

	if (CGameObject::EObjType::OBJ_PLAYER == _eObjType && m_IsTouchAble) {
		m_IsEnabled = false;
		m_pMinigame->GameStart();
	}

	if (CGameObject::EObjType::OBJ_PROJ == _eObjType && !m_IsTouchAble) {
		m_IsEnabled = false;
		m_pMinigame->GameStart();
	}

}

shared_ptr<CMinigameTrigger> CMinigameTrigger::Create(shared_ptr<class CMinigame> _pMinigame, _float3 _vTriggerPos)
{
	shared_ptr<CMinigameTrigger> pInstance = make_shared<CMinigameTrigger>();
	if(FAILED(pInstance->Initialize(_pMinigame, _vTriggerPos)))
		MSG_BOX("Failed to Create : CMinigameTrigger");

	return pInstance;
}
