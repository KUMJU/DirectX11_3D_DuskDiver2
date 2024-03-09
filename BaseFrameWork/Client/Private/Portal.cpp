#include "pch.h"
#include "Portal.h"

#include "GameInstance.h"
#include "Navigation.h"

#include "UIMgr.h"

CPortal::CPortal()
{
}

HRESULT CPortal::Initialize(_float3 _vWarpPos, _float3 _vTriggerPos)
{
	CCollider::COLLIDER_DESC tColDesc = {};
	tColDesc.fRadius = 3.f;
	tColDesc.vCenter = { 0.f , 0.f, 0.f };


	__super::Initialize(tColDesc, _vTriggerPos);
	vWarpPosition = _vWarpPos;

	return S_OK;
}

void CPortal::PriorityTick(_float _fTimeDelta)
{
}

void CPortal::Tick(_float _fTimeDelta)
{

	if (m_bCollide) {
		m_fAccTime += _fTimeDelta;

		if (m_fAccTime >= 12.f) {
			CUIMgr::GetInstance()->StartQuest(TEXT("Ÿ���� �μ��� ��踦 Ǭ��"));
			m_IsActive = false;
		}

	}

	__super::Tick(_fTimeDelta);
}

void CPortal::LateTick(_float _fTimeDelta)
{
	__super::LateTick(_fTimeDelta);

}

HRESULT CPortal::Render()
{
	__super::Render();


	return S_OK;
}

void CPortal::OnCollide(CGameObject::EObjType _eObjType, shared_ptr<CCollider> _pCollider)
{
	if (!m_IsEnabled)
		return;

	if (m_bCollide)
		return;

	//�÷��̾� �浹

	if (CGameObject::EObjType::OBJ_PLAYER == _eObjType) {
		shared_ptr<CGameObject> pPlayer = _pCollider->GetOwner();

		_vector vPos = XMLoadFloat3(&vWarpPosition);
		CUIMgr::GetInstance()->StartDialog(TEXT("AfterWarp"));

		//�÷��̾� ��ġ ���� + �׺���̼� �ε��� ����
		pPlayer->SetPosition(XMVectorSetW(vPos, 1.f));
		dynamic_pointer_cast<CNavigation>(pPlayer->GetComponent(TEXT("Com_Navigation")))->CalcCurrentPos(vPos);
		
		m_bCollide = true;
	}
}

shared_ptr<CPortal> CPortal::Create(_float3 _vWarpPos, _float3 _vTriggerPos)
{
	shared_ptr<CPortal> pInstance = make_shared<CPortal>();

	if (FAILED(pInstance->Initialize(_vWarpPos, _vTriggerPos)))
		MSG_BOX("Failed to Create : CPortal");

	return pInstance;
}
