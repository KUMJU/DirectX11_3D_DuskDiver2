#include "pch.h"
#include "MinigameMole.h"

#include "Mole.h"

CMinigameMole::CMinigameMole()
{
}

HRESULT CMinigameMole::Initialize(CTransform::TRANSFORM_DESC* _pDesc)
{

    //mole ����Ʈ�� �̸� mole ��ü ��Ƶα� -> Ǯ����  

    //���� �δ���(ŷ�ް� ���� �ִ� ǥ��)
    for (_int i = 0; i < 3; ++i) {
        shared_ptr<CMole> pMole = CMole::Create(0);
        m_Moles[0].push_back(pMole);
        pMole->SetMinigameMgr(dynamic_pointer_cast<CMinigameMole>(shared_from_this()));
    }

    //���� �δ���(��ǥ��)
    for (_int i = 0; i < 3; ++i) {
        shared_ptr<CMole> pMole = CMole::Create(1);
        m_Moles[1].push_back(pMole);
        pMole->SetMinigameMgr(dynamic_pointer_cast<CMinigameMole>(shared_from_this()));

    }

    //���� �ٸ��� ���� ����ũ �δ��� 
    for (_int i = 0; i < 3; ++i) {
        shared_ptr<CMole> pMole = CMole::Create(2);
        m_Moles[2].push_back(pMole);
        pMole->SetMinigameMgr(dynamic_pointer_cast<CMinigameMole>(shared_from_this()));
    }

    //�δ��� ���� üũ�� �迭 �ʱ�ȭ
    for (_int i = 0; i < 9; ++i) {
        m_HoleSlot[i] = false;
    }

    
    for (_int i = 0; i < 9; ++i) {
        m_HoleSlot[i] = false;
    }

    return S_OK;
}

void CMinigameMole::PriorityTick(_float _fTimeDelta)
{
    if (m_bProcessing) {

    }


}

void CMinigameMole::Tick(_float _fTimeDelta)
{
    if (m_bProcessing) {

    }

}

void CMinigameMole::LateTick(_float _fTimeDelta)
{
    if (m_bProcessing) {
        m_fSpawnCoolTime += _fTimeDelta;
        if (m_fSpawnCoolTime >= 3.f) {
            SpawnMole();
        }
    }
}

HRESULT CMinigameMole::Render()
{
    if (m_bProcessing) {


    }

    return S_OK;
}

void CMinigameMole::GameStart()
{
    m_bProcessing = true;

}

void CMinigameMole::GameEnd()
{
}

void CMinigameMole::GetScore()
{
    ++m_iCurrentScore;

    if (m_iCurrentScore >= 10) {
        GameEnd();
    }
}

void CMinigameMole::SpawnMole()
{
}

shared_ptr<CMinigameMole> CMinigameMole::Create()
{
    shared_ptr<CMinigameMole> pInstance = make_shared<CMinigameMole>();

    if (FAILED(pInstance->Initialize(nullptr)))
        MSG_BOX("Failed to Create : CMinigameMole");

    return pInstance;
}
