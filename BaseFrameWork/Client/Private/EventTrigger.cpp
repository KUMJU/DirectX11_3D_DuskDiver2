#include "pch.h"
#include "EventTrigger.h"
#include "GameInstance.h"
#include "Collider.h"


CEventTrigger::CEventTrigger()
{
}

HRESULT CEventTrigger::Initialize(CCollider::COLLIDER_DESC _desc, _float3 _vTriggerPos)
{
	__super::Initialize(nullptr);

	m_pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_SPHERE, _desc);
	m_pCollider->SetOwner(shared_from_this());
	m_Components.emplace(TEXT("Com_Collider"), m_pCollider);

	m_pTransformCom->SetState(CTransform::STATE_POSITION, XMLoadFloat3(&_vTriggerPos));


	return S_OK;
}

void CEventTrigger::PriorityTick(_float _fTimeDelta)
{
}

void CEventTrigger::Tick(_float _fTimeDelta)
{
	if (!m_IsEnabled)
		return;

	m_pCollider->Tick(m_pTransformCom->GetWorldMatrix());
}

void CEventTrigger::LateTick(_float _fTimeDelta)
{
	if (FAILED(CGameInstance::GetInstance()->AddRenderGroup(CRenderer::RENDER_NONBLEND, shared_from_this())))
		return;
	CGameInstance::GetInstance()->AddCollider(CCollisionMgr::COL_TRIGGER, m_pCollider);
}

HRESULT CEventTrigger::Render()
{
	if (!m_IsEnabled)
		return S_OK;

	m_pCollider->Render();

	return S_OK;
}
