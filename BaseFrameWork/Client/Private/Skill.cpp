#include "pch.h"
#include "Skill.h"

#include "Collider.h"
#include "GameInstance.h"
#include "Animation.h"

#include "EffectPreset.h"

CSkill::CSkill()
{
}

HRESULT CSkill::Initialize()
{
	
	m_pMainAnimation = m_pAnimation;
	
	return S_OK;
}

void CSkill::PriorityTick(_float _fTimeDelta)
{
}

void CSkill::Tick(_float _fTimeDelta)
{
	if (!m_IsEnabled)
		return;

	CCollisionMgr::COLTYPE_GROUP eCurrentGroup;

	//duration의 일정부분이 지나면 스킬을 중간 취소할 수 있게 코드 추가
	//스킬이 다 마지막에 idle로 돌아가서 스킬 연계를 할 수가 없다

	if ((*m_pMainAnims).size() != 1) {

		_double temp = (*m_pMainAnims)[m_iCurrentSkillOrder]->GetCurrentTrackPosition();

		if ((*m_pMainAnims)[m_iCurrentSkillOrder]->GetCurrentTrackPosition() >= m_Infos[m_iCurrentSkillOrder].iStartTrackPosition) {
			if (m_Infos[m_iCurrentSkillOrder].iEndTrackPosition <= (*m_pMainAnims)[m_iCurrentSkillOrder]->GetCurrentTrackPosition()) {
				++m_iCurrentSkillOrder;


				if (m_iCurrentSkillOrder == m_Infos.size()) {
					m_IsEnabled = false;
					m_bCancle = true;
					EndSkill();
					SkillReset();
					return;
				}
				else {

					m_bDownAtk = m_Infos[m_iCurrentSkillOrder].bDownAtk;
					m_bKnokUp = m_Infos[m_iCurrentSkillOrder].bKnockUp;
					m_fKnockUpDistance = m_Infos[m_iCurrentSkillOrder].fKnockUpDistance;
					return;
				}



			}
			eCurrentGroup = (m_eSkillOwner == EOWNER_TYPE::OWNER_PLAYER) ? CCollisionMgr::COL_PLRPROJ : CCollisionMgr::COL_MONPROJ;

			m_Collider->Tick(m_pOwnerTransform->GetWorldMatrix());
			CGameInstance::GetInstance()->AddCollider(eCurrentGroup, m_Collider);
				
		}

		if (!m_bCancle && m_iCurrentSkillOrder == (*m_pMainAnims).size()-1) {

			_double AnimRatio = (*m_pMainAnims)[m_iCurrentSkillOrder]->GetAnimRatio();

			if (AnimRatio >= m_Infos[m_iCurrentSkillOrder].CancleAbleRatio)
				m_bCancle = true;
		}

	}
	else {

		_double pp = m_pMainAnims->front()->GetCurrentTrackPosition();
		_double ss = m_Infos[m_iCurrentSkillOrder].iStartTrackPosition;
		if (m_pMainAnims->front()->GetCurrentTrackPosition() >= m_Infos[m_iCurrentSkillOrder].iStartTrackPosition) {
			if (m_Infos[m_iCurrentSkillOrder].iEndTrackPosition <= m_pMainAnims->front()->GetCurrentTrackPosition()) {
				++m_iCurrentSkillOrder;

				//현재 스킬 인덱스와 size 갯수가 같으면 순회가 끝났다는 의미. 스킬 사용을 끝낸다
				if (m_iCurrentSkillOrder == m_Infos.size()) {
					m_IsEnabled = false;
					EndSkill();
					SkillReset();
					return;
				}
				else
				{
					m_bDownAtk = m_Infos[m_iCurrentSkillOrder].bDownAtk;
					m_bKnokUp = m_Infos[m_iCurrentSkillOrder].bKnockUp;

				}
			}

			eCurrentGroup = (m_eSkillOwner == EOWNER_TYPE::OWNER_PLAYER) ? CCollisionMgr::COL_PLRPROJ : CCollisionMgr::COL_MONPROJ;

			m_Collider->Tick(m_pOwnerTransform->GetWorldMatrix());
			CGameInstance::GetInstance()->AddCollider(eCurrentGroup, m_Collider);

		}


		if (!m_bCancle) {

			_double AnimRatio = (*m_pMainAnims).front()->GetAnimRatio();

			if (AnimRatio >= m_Infos[m_iCurrentSkillOrder].CancleAbleRatio)
				m_bCancle = true;
		}


	}

	if(m_pEffectPreset)
		m_pEffectPreset->Tick(_fTimeDelta);

}

void CSkill::LateTick(_float _fTimeDelta)
{

#ifdef _DEBUG
	CGameInstance::GetInstance()->AddDebugComponent(m_Collider);
#endif // DEBUG

	
	if (m_pEffectPreset)
		m_pEffectPreset->LateTick(_fTimeDelta);

	CGameInstance::GetInstance()->AddRenderGroup(CRenderer::RENDER_UI, shared_from_this());

}

HRESULT CSkill::Render()
{
if (!m_IsEnabled)
	return S_OK;

#ifdef _DEBUG
	
	/*wstring cc = to_wstring(m_iCurrentSkillOrder);
	CGameInstance::GetInstance()->RenderFont(TEXT("Font_Default_KR"), cc, { g_iWinSizeX * 0.5f - 50.f ,g_iWinSizeY * 0.5f - 230.f }, Colors::White);


	wstring ww = to_wstring(m_pMainAnims->front()->GetCurrentTrackPosition());
	CGameInstance::GetInstance()->RenderFont(TEXT("Font_Default_KR"), ww, { g_iWinSizeX * 0.5f - 50.f ,g_iWinSizeY * 0.5f - 300.f }, Colors::White);*/

#endif // DEBUG

	return S_OK;
}


void CSkill::SetOwnerTransform(shared_ptr<CTransform> _pOwnerTransform) {

	m_pOwnerTransform = _pOwnerTransform;

	if (m_pEffectPreset) {
		m_pEffectPreset->SetParentTransform(_pOwnerTransform);
	}

	if (m_pParticlePreset) {
		m_pParticlePreset->SetParentTransform(_pOwnerTransform);
	}

}

void CSkill::ActiveSkill()
{
	if (!m_IsBasicCombat && EOWNER_TYPE::OWNER_PLAYER == m_eSkillOwner) {

		CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_PLR_SKILLTRIGER);
		CGameInstance::GetInstance()->PlayAudio(TEXT("se_ba_skill.wav"), CSoundMgr::CHANNELID::CH_PLR_SKILLTRIGER, 1.f);

	}


	if (m_pEffectPreset) {

		m_pEffectPreset->PlayEffect();

	}


}

void CSkill::SkillReset()
{
	m_fSkillActiveTime = 0.f;
	m_iCurrentSkillOrder = 0;
	m_fAccTime = 0.f;
	bVoiceDone = false;
	bSkillSeDone = false;

	if (m_pEffectPreset) {
		m_pEffectPreset->StopEffect();
		m_pEffectPreset->ResetEffect();
	}
	
}

void CSkill::OnCollide(CGameObject::EObjType _eObjType, shared_ptr<CCollider> _pCol)
{
}

_vector CSkill::GetOwnerPos()
{
	if (!m_pOwnerTransform)
		return _vector();

	return m_pOwnerTransform->GetState(CTransform::STATE_POSITION);
}
