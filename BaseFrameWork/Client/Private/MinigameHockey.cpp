#include "pch.h"
#include "MinigameHockey.h"

#include "Collider.h"
#include "CollisionMgr.h"

#include "GameInstance.h"
#include "HockeyBall.h"
#include "BattleSystem.h"

#include "MonsterPool.h"

#include "UIMgr.h"


CMinigameHockey::CMinigameHockey()
{
}

HRESULT CMinigameHockey::Initialize(CTransform::TRANSFORM_DESC* _pDesc)
{
    __super::Initialize();

    m_eMinigameType = EMINIGAME_TYPE::GAME_HOCKEY;
    m_eObjType = EObjType::OBJ_WALL;

    //실질적으로 렌더에 사용할 건 아니지만 콜라이더용 + 위치잡기용
    m_pTransformCom->SetState(CTransform::STATE_POSITION, { 0.f, 30.f , -305.f, 1.f });

    //**********************************************************************************//
    /*콜라이더*/
    CCollider::COLLIDER_DESC ColDesc = {};
    shared_ptr<CCollider> pCollider;

    //왼쪽 
    ColDesc.vExtents = { 2.f, 2.f,21.f };
    ColDesc.vCenter = {43.f, 1.f, 0.f};

    pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_AABB, ColDesc);
    m_pWallColliders[0] = pCollider;
    pCollider->SetOwner(shared_from_this());
    //오른쪽
    ColDesc.vExtents = { 2.f, 2.f, 21.f };
    ColDesc.vCenter = { -43.f, 1.f, 0.f };

    pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_AABB, ColDesc);
    m_pWallColliders[1] = pCollider;
    pCollider->SetOwner(shared_from_this());

    
    //상
    ColDesc.vExtents = { 37.f, 2.f, 2.f };
    ColDesc.vCenter = { 0.f, 1.f, -22.f };

    pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_AABB, ColDesc);
    m_pWallColliders[2] = pCollider;
    pCollider->SetOwner(shared_from_this());


    //하
    ColDesc.vExtents = { 37.f, 2.f,2.f };
    ColDesc.vCenter = { 0.f, 1.f, 22.f };

    pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_AABB, ColDesc);
    m_pWallColliders[3] = pCollider;
    pCollider->SetOwner(shared_from_this());

    //**********************************************************************************//

    /*하키공*/

    //m_pHockeyBall = CHockeyBall::Create({ 10.f, 30.f , -305.f, 1.f });
    m_pHockeyBall = CHockeyBall::Create({ 10.f, 30.f , -310.f, 1.f });


    /*몬스터 소환 리스트*/

    CMonsterPool::SPAWN_INFO info = {};
    info.iMonsterType = 3;
    info.vMonsterPos = { -10.f, 30.f , -310.f, 1.f };

    m_MonsterList.push_back(info);

    info.iMonsterType = 3;
    info.vMonsterPos = { 7.f, 30.f , -300.f, 1.f };

    m_MonsterList.push_back(info);


    info.iMonsterType = 3;
    info.vMonsterPos = { -5.f, 30.f , -315.f, 1.f };

    m_MonsterList.push_back(info);


    info.iMonsterType = 3;
    info.vMonsterPos = { 13.f, 30.f , -313.f, 1.f };

    m_MonsterList.push_back(info);


    info.iMonsterType = 3;
    info.vMonsterPos = { -13.f, 30.f , -295.f, 1.f };

    m_MonsterList.push_back(info);

    return S_OK;
}

void CMinigameHockey::PriorityTick(_float _fTimeDelta)
{
    if (!m_bProcessing)
        return;

    m_pHockeyBall->PriorityTick(_fTimeDelta);

}

void CMinigameHockey::Tick(_float _fTimeDelta)
{
    if (!m_bProcessing)
        return;

    if (!CBattleSystem::GetInstance()->GetOnBattle()) {
        GameEnd();
    }


    m_pHockeyBall->Tick(_fTimeDelta);


    for (size_t i = 0; i < 4; ++i) {
        m_pWallColliders[i]->Tick(m_pTransformCom->GetWorldMatrix());
        CGameInstance::GetInstance()->AddCollider(CCollisionMgr::COL_BLOCK, m_pWallColliders[i]);
    }

}

void CMinigameHockey::LateTick(_float _fTimeDelta)
{
    if (!m_bProcessing)
        return;

    m_pHockeyBall->LateTick(_fTimeDelta);
    
    if (FAILED(CGameInstance::GetInstance()->AddRenderGroup(CRenderer::RENDER_NONBLEND, shared_from_this())))
        return;

}

HRESULT CMinigameHockey::Render()
{
    if (!m_bProcessing)
        return S_OK;

   // m_pHockeyBall->Render();
    

    for (size_t i = 0; i < 4; ++i) {
        m_pWallColliders[i]->Render();
    }


    return S_OK;
}

void CMinigameHockey::GameStart()
{
    //몬스터 소환+배틀 시작 
    CMonsterPool::GetInstance()->ActiveMonster(m_MonsterList);
    CBattleSystem::GetInstance()->BattleStart();

    CUIMgr::GetInstance()->StartDialog(TEXT("StartHockeyGame"));

    __super::GameStart();

}

void CMinigameHockey::GameEnd()
{
    __super::GameEnd();

    m_pHockeyBall->SetInActive();

}

shared_ptr<CMinigameHockey> CMinigameHockey::Create()
{
    shared_ptr<CMinigameHockey> pInstance = make_shared<CMinigameHockey>();

    if (FAILED(pInstance->Initialize(nullptr)))
        MSG_BOX("Failed to Create : CMinigameHockey");

    return pInstance;
}
