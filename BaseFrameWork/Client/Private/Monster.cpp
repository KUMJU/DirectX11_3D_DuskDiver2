#include "pch.h"
#include "Monster.h"
#include "GameInstance.h"
#include "GameMgr.h"

#include "Shader.h"
#include "Player.h"

#include "Model.h"
#include "Skill.h"
#include "Collider.h"

#include "MonsterSkillSet.h"

CMonster::CMonster()
{
}

HRESULT CMonster::Initialize(CTransform::TRANSFORM_DESC* _pDesc)
{
    __super::Initialize(_pDesc);
    m_pGameInstance = CGameInstance::GetInstance();

    m_pTarget = CGameMgr::GetInstance()->GetPlayer();

    if (m_pTarget) {
        m_pTargetTransCom = dynamic_pointer_cast<CTransform>(m_pTarget->GetComponent(TEXT("Com_Transform")));
    }

    return S_OK;
}

void CMonster::PriorityTick(_float _fTimeDelta)
{
}

void CMonster::Tick(_float _fTimeDelta)
{
}

void CMonster::LateTick(_float _fTimeDelta)
{
}



HRESULT CMonster::Render()
{
    return S_OK;
}

void CMonster::SetSpawnState()
{




}

_bool CMonster::OnHitKnockUp(_float _fTimeDelta)
{

    _vector vPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
    m_fKnockUpTime += _fTimeDelta;

    if(m_fKnockUpTime < 1.f){
        m_fKnockUpSpeed = m_fKnockUpSpeed  - (_fTimeDelta * m_fGravity * m_fGweight);
        vPos.m128_f32[1] += m_fKnockUpSpeed * _fTimeDelta;
    }
    else {
        m_fDropSpeed = m_fDropSpeed + (_fTimeDelta * m_fGravity * m_fGweight);
        vPos.m128_f32[1] -= m_fDropSpeed * _fTimeDelta;
    }

    m_pTransformCom->CheckingMove(vPos, m_pNavigation, m_bJump);

    if (!m_bJump) {
        m_bKnockUp = false;
        m_bDown = true;
        m_fDownTime = 0.f;
    }
    return m_bJump;
}

_bool CMonster::OnHitDrop(_float _fTimeDelta)
{
    m_fGweight += 10.f;

    _vector vPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);

    m_fDropSpeed = m_fDropSpeed + (_fTimeDelta * m_fGravity * m_fGweight);
    vPos.m128_f32[1] -= m_fDropSpeed * _fTimeDelta;

    m_pTransformCom->CheckingMove(vPos, m_pNavigation, m_bJump);


    if (!m_bJump) {
        m_bKnockUp = false;
        m_bDown = true;
        m_bDrop = false;
        m_fDownTime = 0.f;
    }
    return m_bJump;
}

//플레이어 추적
void CMonster::ChasePlayer()
{
    if (m_pTargetTransCom) {

        _vector vPlrPos = m_pTargetTransCom->GetState(CTransform::STATE_POSITION);
        _vector vCurrentActPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
        
        _float fLength = XMVector3Length(vPlrPos - vCurrentActPos).m128_f32[0];

        if (fLength < AttackDistance) {
            m_IsNearPlr = true;
            AttackPattern(m_iCurrentAtkNum%m_iTotalAtkNum);
            ++m_iCurrentAtkNum;
        }
    }
}

void CMonster::CalcPlayerDistance()
{
    _vector vPlrPos = m_pTargetTransCom->GetState(CTransform::STATE_POSITION);
    _vector vMonPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);

    m_pTransformCom->LookAtForLandObject(vPlrPos);
}

void CMonster::CalcAnimationDistance()
{   
    
    if (CalcDistanceOption()) {

        _vector vDistance = XMLoadFloat3(&m_vCurrentAnimPos) - XMLoadFloat3(&m_vPrevAnimPos);

        _vector vDir = XMLoadFloat3(&m_vCurrentAnimPos) - XMLoadFloat3(&m_vPrevAnimPos);

        vDir = XMVector3Normalize(vDir);

        _vector vLook = m_pTransformCom->GetState(CTransform::STATE_LOOK);
        _matrix WorldMat = m_pTransformCom->GetWorldMatrix();
        _vector vHeight = { 0.f, 0.f,0.f };
        _bool m_bJump = false;

        if (vDir.m128_f32[2] < 0.f) {
            vLook = vLook * -1.f;
        }


        XMVector3TransformCoord(vDistance, WorldMat);

        if (vDistance.m128_f32[1] != 0.f) {
            _vector vHeight = vDistance.m128_f32[1] * _vector({ 0.f, 1.f, 0.f });
            _bool m_bJump = true;
        }

        XMVectorSetY(vDistance, 0.f);

        _float vLength = XMVector3Length(vDistance).m128_f32[0];
        vLook = vLength * vLook;

        _vector vCurrentPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
        vCurrentPos += vLook;
        vCurrentPos += vHeight;

        m_pTransformCom->CheckingMove(vCurrentPos, m_pNavigation, m_bJump);

        // m_pTransformCom->SetState(CTransform::                                  , vCurrentPos);
        m_vPrevAnimPos = m_vCurrentAnimPos;

    }

}
                                                                                                                                                                                                                        
_float CMonster::CalcPlayerDistanceReturn()
{
    _vector vPlrPos = m_pTargetTransCom->GetState(CTransform::STATE_POSITION);
    _vector vCurrentActPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);

    _float fLength = XMVector3Length(vPlrPos - vCurrentActPos).m128_f32[0];

    return fLength;
}

_bool CMonster::CheckReserveAnimList()
{
    m_vPrevAnimPos = { 0.f, 0.f, 0.f };
    m_vCurrentAnimPos = { 0.f, 0.f, 0.f };

    if (m_NextAnimIndex.empty()) {
        //객체 별 별도 애니메이션 제어 선언
        IfEmptyAnimList();
        return false;
    }
    else
    {
        m_iAnimNum = m_NextAnimIndex.front().iNextAnimNum;
        m_bLoop = m_NextAnimIndex.front().IsLoop;

        m_NextAnimIndex.pop_front();
        m_pModelCom->ChangeAnimation(m_iAnimNum);

        return true;
    }

}

void CMonster::ChangeAnim(_uint _iAnimIdx, _bool _bloop)
{
    if (m_iAnimNum == _iAnimIdx && m_eCurrentState != EMONSTER_STATE::STATE_HIT)
        return;

    if (m_pModelCom->IsLinearInterpolation())
        return;

    m_iAnimNum = _iAnimIdx;
    m_bLoop = _bloop;
    m_pModelCom->ChangeAnimation(_iAnimIdx);
}

void CMonster::CalcSuperArmorTime(_float _fTimeDelta)
{
    m_bSuperArmorCoolTime += _fTimeDelta;

    if (m_bSuperArmorCoolTime > 0.25f) {
        m_bSuperArmorCoolTime = 0.f;
        m_bSuperArmor = false;
    }


}

void CMonster::KnockUpInfoReset()
{
    m_fKnockUpTime = 0.f;
    m_fDropSpeed = 10.f;

    m_bDown = false;
    m_fDownTime = 0.f;


}


HRESULT CMonster::BindShaderResources()
{
    if (FAILED(m_pTransformCom->BindShaderResource(m_pShader, "g_WorldMatrix")))
        return E_FAIL;

    _float4x4 ViewMat = CGameInstance::GetInstance()->GetTransformFloat4x4(CPipeLine::D3DTS_VIEW);

    if (FAILED(m_pShader->BindMatrix("g_ViewMatrix", &ViewMat)))
        return E_FAIL;

    _float4x4 ProjMat = CGameInstance::GetInstance()->GetTransformFloat4x4(CPipeLine::D3DTS_PROJ);

    if (FAILED(m_pShader->BindMatrix("g_ProjMatrix", &ProjMat)))
        return E_FAIL;

    _float4 CamPos = CGameInstance::GetInstance()->GetCamPosition();

    if (FAILED(m_pShader->BindRawValue("g_vCamPosition", &CamPos, sizeof(_float4))))
        return E_FAIL;

    const LIGHT_DESC* pLightDesc = CGameInstance::GetInstance()->GetLightDesc(0);
    if (nullptr == pLightDesc)
        return E_FAIL;

    if (FAILED(m_pShader->BindRawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShader->BindRawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShader->BindRawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShader->BindRawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
        return E_FAIL;

    return S_OK;
}

void CMonster::OnCollide(CGameObject::EObjType _eObjType, shared_ptr<CCollider> _pCollider)
{
    if (EObjType::OBJ_PROJ == _eObjType) {

        shared_ptr<CSkill> pSkill = dynamic_pointer_cast<CSkill>(_pCollider->GetOwner());

        if (m_bDie)
            return;

        if (pSkill->GetMultiAtk()) {
            if (m_bSuperArmor) {
                return;
            }
        }
        else {

            if (m_iLastHitIndex == pSkill->GetSkillIndex() && m_iCurrentSkillOrderIndex == pSkill->GetCurrentOrder())
                return;

        }

        //공격 중이었다면 공격 취소 
        if (m_eCurrentState == EMONSTER_STATE::STATE_ATTACK && EMONSTER_TYPE::MONSTER_NORMAL == m_eMonsterType) {
            m_pSkillSet->SkillCancle();
            m_bAttackCoolTime = 0.f;
        }

        ///스킬 info 받아오기/// 
        m_bKnockUp = pSkill->GetIsKnokUp();
        m_bDownAttack = pSkill->GetIsDownAttack();
        m_iLastHitIndex = pSkill->GetSkillIndex();
        m_iCurrentSkillOrderIndex = pSkill->GetCurrentOrder();
        m_fKnockUpSpeed = pSkill->GetKnokUpDistance();
        m_fGweight = pSkill->GetGravityWeight();
        m_bDrop = pSkill->GetIsDropAttack();

        CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_MON);
        CGameInstance::GetInstance()->PlayAudio(TEXT("flesh_hit_02.wav"), CSoundMgr::CHANNELID::CH_MON, 0.7f);

        _int iDamage = pSkill->GetDamage();

        m_iHP -= iDamage;

        if (m_iHP <= 0) {

            m_bDie = true;
            m_eCurrentState = EMONSTER_STATE::STATE_DIE;
        }


        m_bSuperArmor = true;

        if (m_bKnockUp) {
            KnockUpInfoReset();
        }


        if (m_bDown) {
            m_bDown = false;
            m_fDownTime = 0.f;
        }

        if (m_bDrop) {
            m_fDropSpeed = 3.f;
            KnockUpInfoReset();
            m_bKnockUp = false;
        }


        m_bCollisionCheck = true;
        //넉백 여부, 넉업 여부, 
        OnHit();
    }

}
