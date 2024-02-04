#include "pch.h"
#include "MinigameMole.h"

#include "Mole.h"
#include "MapObject.h"
#include "GameInstance.h"

#include "MinigameTrigger.h"

#include "CameraMgr.h"
#include "EventCamera.h"

CMinigameMole::CMinigameMole()
{
}

HRESULT CMinigameMole::Initialize(CTransform::TRANSFORM_DESC* _pDesc)
{

    __super::Initialize();

    m_eMinigameType = EMINIGAME_TYPE::GAME_MOLE;
    //mole 리스트에 미리 mole 객체 담아두기 -> 풀링용  

    //정답 두더지(킹받게 웃고 있는 표정)
    for (_int i = 0; i < 5; ++i) {
        shared_ptr<CMole> pMole = CMole::Create(0);
        m_Moles[0].push_back(pMole);
        pMole->SetMinigameMgr(dynamic_pointer_cast<CMinigameMole>(shared_from_this()));
    }

    //오답 두더지(무표정)
    for (_int i = 0; i < 5; ++i) {
        shared_ptr<CMole> pMole = CMole::Create(1);
        m_Moles[1].push_back(pMole);
        pMole->SetMinigameMgr(dynamic_pointer_cast<CMinigameMole>(shared_from_this()));

    }

    //완전 다르게 생긴 페이크 두더지 
    for (_int i = 0; i < 5; ++i) {
        shared_ptr<CMole> pMole = CMole::Create(2);
        m_Moles[2].push_back(pMole);
        pMole->SetMinigameMgr(dynamic_pointer_cast<CMinigameMole>(shared_from_this()));
    }

    //Center : 88,40,-300
    // 0 1 2 3
    // 4 5 6 7
    // 8 9 10 11

    _vector vCenterPos = { 88.f, 40.f, -300.f };


    shared_ptr<CMinigameTrigger> pTrigger =  CMinigameTrigger::Create(dynamic_pointer_cast<CMinigame>(shared_from_this()), { 75.f, 40.f, -299.f });
    CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE, TEXT("Layer_Event"), pTrigger);


    for (_int i = 0; i < 9; ++i) {

        _uint iRow = i % 3;
        _uint iCol = i / 3;

        _float fXweight = 0.f;
        _float fZweight = 0.f;

        if (0 == iRow) {
            fZweight = -1.f;
        }
        else if (1 == iRow) {

            fZweight = 0.f;

        }
        else if (2 == iRow) {
            fZweight = 1.f;
        }
        else if (3 == iRow) {

        }


        if (0 == iCol) {
            fXweight = 1.f;
        }
        else if (1 == iCol) {
            fXweight = 0.f;
        }
        else if (2 == iCol) {
            fXweight = -1.f;
        }
        else if (3 == iCol) {

        }

        _vector vHolePos = { vCenterPos.m128_f32[0]+ (8.f* fXweight) , 39.6f, vCenterPos.m128_f32[2] + (8.f * fZweight), 1.f };
        _float4 vHolePosFloat;

        shared_ptr<CMapObject> pObject= CMapObject::Create(TEXT("Hole_WhackMole"));
       // dynamic_pointer_cast<CTransform>(pObject->GetComponent(TEXT("Com_Transform")))->SetState(CTransform::STATE_POSITION, vHolePos);
        CGameInstance::GetInstance()->AddObject(LEVEL_ARCADE,TEXT("Layer_Map"), pObject);
        pObject->SetPosition(vHolePos);


        XMStoreFloat4(&vHolePosFloat, vHolePos);
        vHolePosFloat.y -= 3.f;

        m_vMolePos[i] = vHolePosFloat;
        m_HoleSlot[i] = false;
    }

    //시작 이벤트 카메라 프리셋 세팅 
    SetCameraEvent();

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

        if (!m_bStartCutSceneDone) {
            ProcessingEvent(_fTimeDelta);
            return;
        }

        m_fSpawnCoolTime += _fTimeDelta;
        if (m_fSpawnCoolTime >= 2.f) {
            SpawnMole();
        }

        for (auto& iter : m_ActiveMoles)
            iter->Tick(_fTimeDelta);

    }

}

void CMinigameMole::LateTick(_float _fTimeDelta)
{
    if (m_bProcessing) {

        if (m_bStartCutSceneDone) {



            for (auto iter = m_ActiveMoles.begin(); iter != m_ActiveMoles.end();) {
                if (!(*iter)->IsEnabled()) {
                    _uint iSlotIdx = (*iter)->GetSlotIdx();
                    m_HoleSlot[iSlotIdx] = false;
                    iter = m_ActiveMoles.erase(iter);
                }
                else {
                    ++iter;
                }
            }

            for (auto& iter : m_ActiveMoles)
                iter->LateTick(_fTimeDelta);

        }

        CGameInstance::GetInstance()->AddRenderGroup(CRenderer::RENDER_UI, shared_from_this());
    }

}

HRESULT CMinigameMole::Render()
{
    if (m_bProcessing) {

        for (auto& iter : m_ActiveMoles)
            iter->Render();
      
        wstring strBaseText = TEXT("Score: ");
        wstring strCurrentScore = to_wstring(m_iCurrentScore);

        strBaseText = strBaseText + strCurrentScore;

        if (FAILED(CGameInstance::GetInstance()->RenderFont(TEXT("Font_Default"), strBaseText, _float2(700.f, 30.f), XMVectorSet(1.f, 1.f, 0.f, 1.f))))
            return E_FAIL;

    }

    return S_OK;
}


void CMinigameMole::GameStart()
{
    __super::GameStart();
    
    //Mole_StartEvent
    StartEventCutScene();
    CCameraMgr::GetInstance()->StartEvent(TEXT("Mole_StartEvent"));

}

void CMinigameMole::GameEnd()
{
    __super::GameEnd();

    for (auto& iter : m_ActiveMoles) {
        iter->SetEnable(false);
    }


}

void CMinigameMole::GetScore()
{
    ++m_iCurrentScore;

    if (m_iCurrentScore >= 5) {
        GameEnd();
    }
}

void CMinigameMole::SpawnMole()
{

    _bool bPass = false;


    while(!bPass)
    {
        _uint iPickHoleNum = rand() % 9;
        _uint iPickMoleNum = rand() % 3;

        if (m_HoleSlot[iPickHoleNum])
            continue;

        for (auto& iter : m_Moles[iPickMoleNum]) {
            //현재 활성화되고 있지 않은 두더지를 선택해서 활성화
            if (!iter->IsEnabled()) {
                iter->StateReset();
                iter->SetPosition(XMLoadFloat4(&m_vMolePos[iPickHoleNum]));
                iter->SetEnable(true);
                iter->SetSlotIdx(iPickHoleNum);

                m_ActiveMoles.push_back(iter);
                m_HoleSlot[iPickHoleNum] = true;
                bPass = true;
                m_fSpawnCoolTime = 0.f;
                break;
            }
        }

    }

}

void CMinigameMole::SetCameraEvent()
{
    vector<CEventCamera::EVENT_INFO> StartEventVector;

    CEventCamera::EVENT_INFO StartEvent = {};

    StartEvent.eCamFunc = CEventCamera::FUNC_FOCUSING;
    StartEvent.vStart = {75.f, 41.5f, -303.5f };
    StartEvent.vEnd = { 88.f, 41.5f, -299.f };
    StartEvent.fDuration = 8.f;

    StartEventVector.push_back(StartEvent);
    CCameraMgr::GetInstance()->AddEventPreset(TEXT("Mole_StartEvent"), StartEventVector);

}

void CMinigameMole::StartEventCutScene()
{


    _vector vStartMolePos1 = XMLoadFloat4(&m_vMolePos[4]) + _vector({0.f, 2.f, 0.f, 0.f});

    m_Moles[0].front()->StateReset();
    m_Moles[0].front()->SetPosition(vStartMolePos1);
    m_Moles[0].front()->SetEnable(true);

    m_ActiveMoles.push_back(m_Moles[0].front());

    vCutEventPos1 = vStartMolePos1;

    _vector vStartMolePos2 = XMLoadFloat4(&m_vMolePos[1]) + _vector({ 0.f, 2.f, 0.f, 0.f });

    m_Moles[1].front()->StateReset();
    m_Moles[1].front()->SetPosition(vStartMolePos2);
    m_Moles[1].front()->SetEnable(true);

    m_ActiveMoles.push_back(m_Moles[1].front());

    vCutEventPos2 = vStartMolePos2;

    _vector vStartMolePos3 = XMLoadFloat4(&m_vMolePos[7]) + _vector({ 0.f, 2.f, 0.f, 0.f });

    m_Moles[2].front()->StateReset();
    m_Moles[2].front()->SetPosition(vStartMolePos3);
    m_Moles[2].front()->SetEnable(true);

    m_ActiveMoles.push_back(m_Moles[2].front());
    vCutEventPos3 = vStartMolePos3;




}

void CMinigameMole::ProcessingEvent(_float _fTimeDelta)
{

    m_fEventProcessTime += _fTimeDelta;



    if (m_fEventProcessTime > 2.f && m_fEventProcessTime < 4.f) {

        _float fDropSpeed = 5.f * _fTimeDelta;

       vCutEventPos2.m128_f32[1] = vCutEventPos2.m128_f32[1] - fDropSpeed;
       vCutEventPos3.m128_f32[1] = vCutEventPos3.m128_f32[1] - fDropSpeed;

       m_Moles[1].front()->SetPosition(vCutEventPos2);
       m_Moles[2].front()->SetPosition(vCutEventPos3);

    }

    if (m_fEventProcessTime > 4.5f && m_fEventProcessTime < 7.f) {


        _float fDropSpeed = 2.f * _fTimeDelta;

        vCutEventPos1.m128_f32[1] = vCutEventPos1.m128_f32[1] - fDropSpeed;
        m_Moles[0].front()->SetPosition(vCutEventPos1);

    }

    if (m_fEventProcessTime > 7.5f) {
        //두더지 들어가기 

        m_Moles[0].front()->StateReset();
        m_Moles[0].front()->SetEnable(false);

        m_Moles[1].front()->StateReset();
        m_Moles[1].front()->SetEnable(false);

        m_Moles[2].front()->StateReset();
        m_Moles[2].front()->SetEnable(false);

        m_ActiveMoles.clear();
        m_bStartCutSceneDone = true;
    
    }





}

shared_ptr<CMinigameMole> CMinigameMole::Create()
{
    shared_ptr<CMinigameMole> pInstance = make_shared<CMinigameMole>();

    if (FAILED(pInstance->Initialize(nullptr)))
        MSG_BOX("Failed to Create : CMinigameMole");

    return pInstance;
}
