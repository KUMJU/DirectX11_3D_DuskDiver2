#include "pch.h"
#include "Player.h"
#include "GameInstance.h"
#include "GameMgr.h"

#include "Model.h"
#include "Shader.h"

#include "CameraMgr.h"

CPlayer::CPlayer()
{
}

HRESULT CPlayer::Initialize()
{

    CTransform::TRANSFORM_DESC TransformDesc = {};
    TransformDesc.fSpeedPerSet = 7.f;
    TransformDesc.fRotationPerSet = 45.f;

    if (FAILED(__super::Initialize(&TransformDesc)))
        return E_FAIL;

    if (FAILED(AddComponent()))
        return E_FAIL;

    m_pShader = CGameInstance::GetInstance()->GetShader(TEXT("Shader_VtxAnim"));

    m_pBattleModelCom = CGameInstance::GetInstance()->GetModel(TEXT("Hero1_BattleMode_3Anim"));
    m_pBurstModelCom = CGameInstance::GetInstance()->GetModel(TEXT("Hero1_Burst"));

    SetAnimSpeed();

    m_pModelCom = m_pBattleModelCom;

    m_Components.emplace(TEXT("Com_BattleModel"), m_pBattleModelCom);
    m_Components.emplace(TEXT("Com_BurstModel"), m_pBurstModelCom);


    m_eCurrentState = HEROSTATE::STATE_IDLE;
    m_pModelCom->ChangeAnimation(44);

    return S_OK;
}

void CPlayer::PriorityTick(_float _fTimeDelta)
{
}

void CPlayer::Tick(_float _fTimeDelta)
{
    if (HEROSTATE::STATE_BURST_TRANS == m_eCurrentState) {

        m_fransformTime += _fTimeDelta;
        
        if (m_fransformTime > 2.8f) {
            
            m_pModelCom = m_pBurstModelCom;

            m_eCurrentState = HEROSTATE::STATE_IDLE;
            m_fransformTime = 0.f;
            m_bBurstMode = true;
            
        }
        else {
            _float3 fTemp = _float3();
            m_pBurstModelCom->PlayAnimation(_fTimeDelta, false, &fTemp);
        }
    }


    //공격모션이 idle로 전환되는 과정까지 포함되어있어서 스피드한 콤보 공격을 위해 중간에 끊어 진행
    if (m_bComboAttackStart) {
        m_fComboTime += _fTimeDelta;
        if (m_bReserveCombo && m_fComboTime > m_fMinComboAnimTime) {
            _uint iNextCombo = m_NextAnimIndex.front().iNextAnimNum;
            m_NextAnimIndex.pop_front();
            ChangeAnim(iNextCombo, false);
            m_fComboTime = 0.f;
            m_bReserveCombo = false;
        }
    }

    //마우스 debounce
    if (m_IsMouseDeb) {
        m_fDebTime += _fTimeDelta;

        if (m_fDebTime > 0.2f) {
            m_fDebTime = 0.f;
            m_IsMouseDeb = false;
        }
    }
     
    //루프 애니메이션 제어
    if (m_IsLoopMotion) {

        m_fCurLoopTime += _fTimeDelta;
        if (m_fCurLoopTime > m_fLoopTotalTime) {
            m_IsLoopMotion = false;
            m_fCurLoopTime = 0.f;
            CheckReserveAnimList();
        }
    }


    //입력 장치 
    MouseInput(_fTimeDelta);
    KeyInput(_fTimeDelta);

    if (m_bComboAttackStart)
    {
        m_fLinearTime = 0.03f;
    }

    m_pModelCom->SetLinearTime(m_fLinearTime);

    //애니메이션 재생
    if(m_pModelCom->PlayAnimation(_fTimeDelta, m_isAnimLoop, &m_vCurretnAnimPos)){

        CheckReserveAnimList();
    }

    m_fLinearTime = 0.06f;

    //루트모션 거리계산
    CalcAnimMoveDistance();
}

void CPlayer::LateTick(_float _fTimeDelta)
{

    if (m_iCurrentAnimIdx == 133) {

        m_fSkillRJumpCool += _fTimeDelta;
        if (m_fSkillRJumpCool > 1.1f) {

            m_fJumpSpeed = 15.f;

            m_fTotalHeight += m_fJumpSpeed * _fTimeDelta;
            _vector vPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
            vPos.m128_f32[1] += m_fJumpSpeed * _fTimeDelta;

            m_pTransformCom->SetState(CTransform::STATE_POSITION, vPos);
        }

    }
    else if (m_iCurrentAnimIdx == 52 && !m_bJump) {

        m_fJumpSpeed = 5.f;

        m_fTotalHeight += m_fJumpSpeed * _fTimeDelta;
        _vector vPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
        vPos.m128_f32[1] += m_fJumpSpeed * _fTimeDelta;

        m_pTransformCom->SetState(CTransform::STATE_POSITION, vPos);
    }
    else if (m_iCurrentAnimIdx == 50 && !m_bJump) {

        if(m_fSkillRJumpCool == 0.f)
            m_fJumpSpeed = 10.f;
        else
            m_fJumpSpeed = 25.f;

        m_fTotalHeight -= m_fJumpSpeed * _fTimeDelta;
        _vector vPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);

        if (m_fTotalHeight < 0) {
            vPos.m128_f32[1] += ((m_fTotalHeight * -1) + m_fJumpSpeed * _fTimeDelta);
            m_fTotalHeight = 0;
            ChangeAnim(53, false);
            m_bJump = false;
            m_fSkillRJumpCool = 0.f;
        }
        else {
            vPos.m128_f32[1] -= m_fJumpSpeed * _fTimeDelta;
        }

        m_pTransformCom->SetState(CTransform::STATE_POSITION, vPos);


    }




    if (FAILED(CGameInstance::GetInstance()->AddRenderGroup(CRenderer::RENDER_NONBLEND, shared_from_this())))
        return;
}

HRESULT CPlayer::Render()
{
    if (FAILED(BindShaderResources()))
        return E_FAIL;

    _uint iNumMeshes = m_pModelCom->GetNumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++) {

        if (FAILED(m_pModelCom->BindMaterialShaderResource(m_pShader, (_uint)i, aiTextureType::aiTextureType_DIFFUSE, "g_DiffuseTexture")))
            return E_FAIL;

        if (FAILED(m_pModelCom->BindBoneMatrices(m_pShader, "g_BoneMatrices", i)))
            return E_FAIL;

        if (FAILED(m_pShader->Begin(0)))
            return E_FAIL; 

        if (FAILED(m_pModelCom->Render((_uint)i)))
            return E_FAIL;
    }

    return S_OK;
}


void CPlayer::SetAnimSpeed()
{
    m_pBattleModelCom->SetSpecificAnimSpeed(9, 1.4f);
    m_pBattleModelCom->SetSpecificAnimSpeed(11, 1.4f);
    m_pBattleModelCom->SetSpecificAnimSpeed(13, 1.4f);
    m_pBattleModelCom->SetSpecificAnimSpeed(15, 1.4f);
    m_pBattleModelCom->SetSpecificAnimSpeed(16, 1.4f);
    m_pBattleModelCom->SetSpecificAnimSpeed(0, 1.4f);
    m_pBattleModelCom->SetSpecificAnimSpeed(1, 1.4f);
    m_pBattleModelCom->SetSpecificAnimSpeed(2, 1.4f);

    m_pBurstModelCom->SetSpecificAnimSpeed(9, 2.f);
    m_pBurstModelCom->SetSpecificAnimSpeed(11, 2.f);
    m_pBurstModelCom->SetSpecificAnimSpeed(13, 2.f);
    m_pBurstModelCom->SetSpecificAnimSpeed(15, 2.f);
    m_pBurstModelCom->SetSpecificAnimSpeed(16, 2.f);
    m_pBurstModelCom->SetSpecificAnimSpeed(0, 1.4f);
    m_pBurstModelCom->SetSpecificAnimSpeed(1, 1.4f);
    m_pBurstModelCom->SetSpecificAnimSpeed(2, 1.4f);

}

HRESULT CPlayer::AddComponent()
{
    return S_OK;
}

HRESULT CPlayer::BindShaderResources()
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

_bool CPlayer::ChangeAnim(_uint _iAnimNum, _bool _isLoop)
{
   // 52 == _iAnimNum ||
    m_iCurrentAnimIdx;
    if (m_bJump && 51 == _iAnimNum) {
        m_IsLoopMotion = true;
    }
    else if (1 == _iAnimNum || 12 == _iAnimNum) {
        m_eCurrentState = HEROSTATE::STATE_COMBO_ATTACK2;
    }
    else if (2 == _iAnimNum || 14 == _iAnimNum) {
        m_eCurrentState = HEROSTATE::STATE_COMBO_ATTACK3;
    }
    else if (15 == _iAnimNum) {
        m_eCurrentState = HEROSTATE::STATE_COMBO_ATTACK4;
    }
    else if (16 == _iAnimNum) {
        m_eCurrentState = HEROSTATE::STATE_COMBO_ATTACK5;
    }
    else if (53 == _iAnimNum || 3 == _iAnimNum) {
        //Jump Landing 
        m_bJump = false;
    }

    if (!m_pModelCom->ChangeAnimation(_iAnimNum))
        return false;



    if (HEROSTATE::STATE_COMBO_ATTACK1 != m_eCurrentState &&
        HEROSTATE::STATE_COMBO_ATTACK2 != m_eCurrentState &&
        HEROSTATE::STATE_COMBO_ATTACK3 != m_eCurrentState &&
        HEROSTATE::STATE_COMBO_ATTACK4 != m_eCurrentState &&
        HEROSTATE::STATE_COMBO_ATTACK5 != m_eCurrentState &&
        m_iCurrentAnimIdx != 63 && m_iCurrentAnimIdx != 64 &&
        m_iCurrentAnimIdx != 44 && m_iCurrentAnimIdx != 133 &&
        m_iCurrentAnimIdx != 135 && m_iCurrentAnimIdx != 137)
        {
        m_vPrevAnimPos = { 0.f, 0.f, 0.f };
        m_vCurretnAnimPos = { 0.f, 0.f, 0.f };
    }


    m_iCurrentAnimIdx = _iAnimNum;
    m_isAnimLoop = _isLoop;

    return true;

}

//미리 예약해둔 애니메이션을 확인하고, 있으면 그 애니메이션을 돌리고 없으면 기본 idle 상태로 돌아옵니다
void CPlayer::CheckReserveAnimList()
{


    if (m_NextAnimIndex.empty()) {

        if (81 == m_iCurrentAnimIdx && m_bSprint == false) {
            m_bDash = false;
        }

        if (m_bComboAttackStart && m_bJump) {
            m_eCurrentState = HEROSTATE::STATE_JUMP;
            ChangeAnim(53, false);
        }


        if (m_bComboAttackStart) {
            ResetComboState();
            m_IsKeyDeb = true;
            m_eCurrentState = HEROSTATE::STATE_IDLE;
            ChangeAnim(44, true);
            return;
        }


        if (m_IsUsingSkill)
            m_IsUsingSkill = false;

        //달리기 중이면 달리기로 전환
        if (m_bRunning) {

            if (m_bDash) {
                m_eCurrentState = HEROSTATE::STATE_WALK;
                m_eCurrentState = HEROSTATE::STATE_WALK;
                ChangeAnim(80, true);
            }
            else {
                //IDLE로 돌아감
                m_eCurrentState = HEROSTATE::STATE_WALK;
                ChangeAnim(59, true);
            }
        }
        else {
                //IDLE로 돌아감
                m_eCurrentState = HEROSTATE::STATE_IDLE;
                ChangeAnim(44, true);
        }

    }
    else {

        _uint iNextAnim = m_NextAnimIndex.front().iNextAnimNum;
        _bool isLoop = m_NextAnimIndex.front().IsLoop;

        if (14 == iNextAnim)
            int a = 34;

        m_NextAnimIndex.pop_front();
        ChangeAnim(iNextAnim, isLoop);
    }
}

void CPlayer::FinishCombo()
{

}

void CPlayer::CalcAnimMoveDistance()
{
    if (79 == m_iCurrentAnimIdx || 76 == m_iCurrentAnimIdx) {
        return;
    }


    _vector vDistance = XMLoadFloat3(&m_vCurretnAnimPos) - XMLoadFloat3(&m_vPrevAnimPos);

    //if (vDistance.m128_f32[0] != 0.f &&
    //    vDistance.m128_f32[1] != 0.f &&
    //    vDistance.m128_f32[2] != 0.f){
    //    CalcLookVector(1.f);
    //}

    _vector vLook = m_pTransformCom->GetState(CTransform::STATE_LOOK);
    _matrix WorldMat = m_pTransformCom->GetWorldMatrix();

    XMVector3TransformCoord(vDistance, WorldMat);
    _float vLength = XMVector3Length(vDistance).m128_f32[0];
    vLook = vLength* vLook;

    _vector vCurrentPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
    vCurrentPos += vLook;

    m_pTransformCom->SetState(CTransform::STATE_POSITION, vCurrentPos);

    m_vPrevAnimPos = m_vCurretnAnimPos;
       
}

void CPlayer::CalcLookVector(_float _fDir)
{
    _float4 vLook = CCameraMgr::GetInstance()->GetCamLook();
    _vector vLookVec = XMLoadFloat4(&vLook) * _fDir;

    _vector vPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
    vPos += vLookVec;
    m_pTransformCom->LookAt(vPos);

}

void CPlayer::KeyInput(_float _fTimeDelta)
{ 

    if (m_pModelCom->IsLinearInterpolation())
        return;

    if (m_bComboAttackStart)
        return;

    if (m_IsUsingSkill)
        return;

    if (m_IsMouseDeb)
        return;

    if (HEROSTATE::STATE_HEAVY_ATTACK == m_eCurrentState || 
        HEROSTATE::STATE_DODGE == m_eCurrentState)
        return;

    _bool IsKeyInput = false;
    _uint FinalAnimNum = 0;
    _bool IsLoop = false;

    m_bRunning = false;

    ////////////////////////BurstMode////////////////////////

    if (GetKeyState('Z') & 0x8000)
    {
        m_eCurrentState = HEROSTATE::STATE_BURST_TRANS;
        IsKeyInput = true;
        //ChangeAnim(30, false);

        m_pModelCom->ChangeAnimation(30);
        m_pBurstModelCom->ChangeAnimation(30);
        m_isAnimLoop = false;

        //Event_Burst
        CCameraMgr::GetInstance()->StartEvent(TEXT("Event_Burst"));

        m_IsUsingSkill = true;

    }

    if (HEROSTATE::STATE_BURST_TRANS == m_eCurrentState)
        return;

    ////////////////////////Skill////////////////////////
    
    //Burst : Super1
    if (GetKeyState('X') & 0x8000)
    {   
        if (m_bBurstMode) {
            m_eCurrentState = HEROSTATE::STATE_SKILL_Q;
            ChangeAnim(91, false);
            m_IsUsingSkill = true;
        }

    }

    //Burst : Super2
    if (GetKeyState('C') & 0x8000)
    {
        if (m_bBurstMode) {
            m_eCurrentState = HEROSTATE::STATE_SKILL_Q;
            ChangeAnim(92, false);
            m_IsUsingSkill = true;
        }

    }


    //Skill1
    if (GetKeyState('Q') & 0x8000)
    {
        m_eCurrentState = HEROSTATE::STATE_SKILL_Q;

        if (m_bBurstMode) {
            ChangeAnim(63, false);
            m_NextAnimIndex.push_back({ 64 , false });
            m_NextAnimIndex.push_back({ 65 , false });
        }
        else {
            ChangeAnim(60, false);
        }
        m_IsUsingSkill = true;

    }

    //Skill2
    if (GetKeyState('E') & 0x8000)
    {
        m_eCurrentState = HEROSTATE::STATE_SKILL_E;


        if (m_bBurstMode)
            ChangeAnim(71, false);
        else
            ChangeAnim(61, false);

        m_IsUsingSkill = true;

    }

    if (GetKeyState('R') & 0x8000)
    {
        m_eCurrentState = HEROSTATE::STATE_SKILL_R;

        if (m_bBurstMode) {
            ChangeAnim(133, false);
            m_NextAnimIndex.push_back({ 136, false });
            m_NextAnimIndex.push_back({ 50 , true});

        }
        else
            ChangeAnim(62, false);

        m_IsUsingSkill = true;

    }

    ///////////////////Walk////////////////////////

    m_bSprint = false;

    m_MoveFlag = 0x0000;

    _vector vPlrPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
    _vector vDir = {0.f, 0.f, 0.f, 0.f};
    _float4 vCamFloat = CCameraMgr::GetInstance()->GetCamLook();
    _vector vCamLookDir = XMLoadFloat4(&vCamFloat);
    _vector vPlrLook = m_pTransformCom->GetState(CTransform::STATE_LOOK);

    _vector vSrc = { 0.f, 0.f ,0.f };
    _vector vDst = { 0.f, 0.f, 0.f };

    ///////////////////////부드러운 방향전환을 위한 변수/////////////////////

    if (GetKeyState('W') & 0x8000)
    {
        /*CalcLookVector(1.f);
*/
        m_bRunning = true;
        m_fCurrentDir = 1.f;

        vDst = vCamLookDir;
        vSrc = m_pTransformCom->GetState(CTransform::STATE_LOOK);
        
        m_MoveFlag |= 0x0001;
       // m_pTransformCom->GoStraight(_fTimeDelta);

        if (m_bDash) {
            m_bSprint = true;
            FinalAnimNum = 80;
            m_bDash = true;
        }
        else {
            FinalAnimNum = 59;
        }

        IsLoop = true;
        IsKeyInput = true;
    }

    if (GetKeyState('S') & 0x8000) {

        if (m_MoveFlag != 0x0001) {

            vSrc = vPlrLook;
            vDst = -1.f * vCamLookDir;

            m_bRunning = true;
            m_fCurrentDir = -1.f;
            m_MoveFlag |= 0x0010;


            if (m_bDash) {
                m_bSprint = true;
                FinalAnimNum = 80;
                m_bDash = true;
            }
            else {
                FinalAnimNum = 59;
            }

            IsLoop = true;
            IsKeyInput = true;
        }
    }

    if (GetKeyState('A') & 0x8000)
    {
        _vector vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vCamLookDir);
        _vector vLeft = vRight * -1.f;

        if (m_MoveFlag == 0x0001) {
            vSrc = vPlrLook;
            vDst = vCamLookDir + vLeft;
        }else if(m_MoveFlag == 0x0010) {
            vSrc = vPlrLook;
            vDst += vLeft;
        }
        else {
            vSrc = vPlrLook;
            vDst = vLeft;
        }

        m_MoveFlag |= 0x0100;


        if (m_bDash) {
            m_bSprint = true;
            FinalAnimNum = 80;
            m_bDash = true;
        }
        else {
            FinalAnimNum = 59;
        }

        IsLoop = true;

        IsKeyInput = true;


    }

    if (GetKeyState('D') & 0x8000)
    {
        _vector vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vCamLookDir);


        vSrc = vPlrLook;

        if (m_MoveFlag == 0x0001) {
            vSrc = vPlrLook;
            vDst = vCamLookDir + vRight;


        }else if(m_MoveFlag == 0x0010) {
            vSrc = vPlrLook;
            vDst += vRight;
        }
        else {
            vSrc = vPlrLook;
            vDst = vRight;
        }

        m_MoveFlag |= 0x1000;

        if (m_bDash) {
            m_bSprint = true;
            FinalAnimNum = 80;
            m_bDash = true;
        }
        else {
            FinalAnimNum = 59;
        }

        IsKeyInput = true;
        IsLoop = true;
    }

    if (IsKeyInput) {
        TurnLerp(vSrc, vDst, _fTimeDelta, vPlrPos, m_MoveFlag);
        m_pTransformCom->GoStraight(_fTimeDelta);
    }

    if (m_eCurrentState == HEROSTATE::STATE_JUMP ||
        m_eCurrentState == HEROSTATE::STATE_DASH ||
        m_eCurrentState == HEROSTATE::STATE_HEAVY_ATTACK) {
        return;
    }

    ////////////////////////Dash + Jump + Dodge ////////////////////////

    //Dodge 
    if (GetKeyState(VK_SHIFT) & 0x8000)
    {
        m_pTransformCom->GoStraight(_fTimeDelta);
        m_eCurrentState = HEROSTATE::STATE_DODGE;

        if (m_bJump) {
            FinalAnimNum = 28;
        }
        else {

            FinalAnimNum = 27;
        }
        IsLoop = false;
        IsKeyInput = true;

    }


    //Dash
    //대쉬 마지막 부분에 idle 전환이 포함되어있음
    // key input이 되어있다면 바로 대쉬 사용 가능 or Idle 전환 안 하고 run 가능 
    if (GetKeyState('F') & 0x8000)
    {
        m_pTransformCom->GoStraight(_fTimeDelta);
        m_eCurrentState = HEROSTATE::STATE_DASH;

        FinalAnimNum = 81;

        IsLoop = false;
        IsKeyInput = true;
        m_bDash = true;

    }

    //Jump
    if (GetKeyState(VK_SPACE) & 0x8000)
    {
        if (m_bJump)
            return;

        if (HEROSTATE::STATE_JUMP == m_eCurrentState)
            return;

        m_eCurrentState = HEROSTATE::STATE_JUMP;
         
        m_NextAnimIndex.push_back({ 51, true });
      //  m_NextAnimIndex.push_back({ 50, true });
        m_NextAnimIndex.push_back({ 53, false });

        FinalAnimNum = 52;
        IsLoop = false;
        m_bJump = true;
    }

    if (IsKeyInput) {

        //걷기 충돌 안 나게 조절 
        if (59 == FinalAnimNum && m_bJump)
            return;

        if(59 == FinalAnimNum)
            m_eCurrentState = HEROSTATE::STATE_WALK;

        ChangeAnim(FinalAnimNum, IsLoop);
        return;
    }

    ////////////////Camera/////////////////////////

    if (GetKeyState('1') & 0x8000) {

        CCameraMgr::GetInstance()->SwitchingCamera(CCameraMgr::ECAMERATYPE::FREE);
    }

    if (HEROSTATE::STATE_WALK == m_eCurrentState ||
        HEROSTATE::STATE_RUN == m_eCurrentState) {
        if (m_bDash) {
            m_eCurrentState = HEROSTATE::STATE_IDLE;
            ChangeAnim(79, false);
            m_bDash = false;
        }
        else {
            m_eCurrentState = HEROSTATE::STATE_IDLE;
            ChangeAnim(44, true);
        }
    
    }
}


void CPlayer::MouseInput(_float _fTimeDelta)
{
    if (m_pModelCom->IsLinearInterpolation())
        return;

    if (m_IsUsingSkill)
        return;

    if (m_eCurrentState == HEROSTATE::STATE_HEAVY_ATTACK ||
        m_eCurrentState == HEROSTATE::STATE_COMBO_ATTACK5 ||
        m_IsMouseDeb == true) {
        return;
    }

    if (m_eCurrentState == HEROSTATE::STATE_COMBO_ATTACK3 && m_bJump)
        return;

    //HeavyAttack
    if (CGameInstance::GetInstance()->GetDIMouseState(MOUSEKEYSTATE::DIM_RB)) {

        if (m_bDash)
            m_bDash = false;

        if (m_bJump) {
            m_NextAnimIndex.clear();
            if (ChangeAnim(3, false)) {
                m_eCurrentState = HEROSTATE::STATE_HEAVY_ATTACK;
            }

        }
        else {
            m_eCurrentState = HEROSTATE::STATE_HEAVY_ATTACK;
            if (ChangeAnim(35, false)) {
                m_eCurrentState = HEROSTATE::STATE_HEAVY_ATTACK;
            }
        }
        m_IsMouseDeb = true;
        return;
    }

    //Nomal Combo Attack
    if (CGameInstance::GetInstance()->GetDIMouseState(MOUSEKEYSTATE::DIM_LB)) {

        if (m_bDash)
            m_bDash = false;

        if (m_bReserveCombo)
            return;


        //Air Attack
        if (m_bJump) {

            switch (m_eCurrentState)
            {
            case Client::CPlayer::HEROSTATE::STATE_COMBO_ATTACK1:
                m_fMinComboAnimTime = 0.3f;
                m_NextAnimIndex.push_back({ 1 , false });
                m_bReserveCombo = true;
                break;
            case Client::CPlayer::HEROSTATE::STATE_COMBO_ATTACK2:
                m_fMinComboAnimTime = 0.3f;
                m_NextAnimIndex.push_back({ 2 , false });
                m_bReserveCombo = true;
                break;
            case Client::CPlayer::HEROSTATE::STATE_COMBO_ATTACK3:
                m_bReserveCombo = true;
                break;
            default:
                if (m_bComboAttackStart)
                    return;

                m_NextAnimIndex.clear();

                if (ChangeAnim(0, false)) {
                 m_fMinComboAnimTime = 0.3f;
                 m_eCurrentState = HEROSTATE::STATE_COMBO_ATTACK1;
                 m_bComboAttackStart = true;
                }
                break;
            }

            m_IsMouseDeb = true;

        }
        //Ground Attack
        else {
            switch (m_eCurrentState)
            {
            case Client::CPlayer::HEROSTATE::STATE_COMBO_ATTACK1:
                m_fMinComboAnimTime = 0.5f;
                m_NextAnimIndex.push_back({ 12 , false });
                m_bReserveCombo = true;
                break;
            case Client::CPlayer::HEROSTATE::STATE_COMBO_ATTACK2:
                m_fMinComboAnimTime = 0.5f;
                m_NextAnimIndex.push_back({ 14 , false });
                m_bReserveCombo = true;
                break;
            case Client::CPlayer::HEROSTATE::STATE_COMBO_ATTACK3:
                m_fMinComboAnimTime = 0.5f;
                m_NextAnimIndex.push_back({ 15 , false });
                m_bReserveCombo = true;
                break;
            case Client::CPlayer::HEROSTATE::STATE_COMBO_ATTACK4:
                m_fMinComboAnimTime = 0.7f;
                m_NextAnimIndex.push_back({ 16 , false });
                m_bReserveCombo = true;
                //attack5 Call + mouseDeb 
                break;
            case Client::CPlayer::HEROSTATE::STATE_COMBO_ATTACK5:
                m_bReserveCombo = true;
                break;
            default:
                if (m_bComboAttackStart)
                    return;

                ChangeAnim(10, false);
                m_fMinComboAnimTime = 0.5f;
                m_eCurrentState = HEROSTATE::STATE_COMBO_ATTACK1;
                m_bComboAttackStart = true;
                break;
            }

            m_IsMouseDeb = true;

        }
    }

   
}

void CPlayer::TurnLerp(_vector _vSrc, _vector _vDst , _float _fDeltaTime, _vector _vPos, unsigned char _flag)
{
    if (m_MoveFlag != m_LastMoveFlag) {

        if ((m_MoveFlag == 0x0001 && m_LastMoveFlag == 0x0010) ||
            (m_LastMoveFlag == 0x0001 && m_MoveFlag == 0x0010)) {
            
            m_fSlerpFlat = 0.5f;
        }
        else {
            m_fSlerpFlat = 0.f;

        }
    }


    if (!XMVector4Equal(_vSrc, XMVector4Normalize(_vDst))) {

        if ((m_MoveFlag != m_LastMoveFlag) || m_fSlerpFlat <= 1.f) {

            m_fSlerpFlat += 1.f * _fDeltaTime;
            if (m_fSlerpFlat > 1.f)
                m_fSlerpFlat = 1.f;
        }
            
        _vDst = XMVectorLerp(_vSrc, _vDst, m_fSlerpFlat / 1.f);
      //  _vDst = XMQuaternionSlerp(_vSrc, _vDst, m_fSlerpFlat / 1.5f);
    }

    m_LastMoveFlag = m_MoveFlag;
    _vPos += _vDst;
    m_pTransformCom->LookAt(_vPos);

}

void CPlayer::ResetComboState()  
{
    m_NextAnimIndex.clear();
    m_bReserveCombo = false;
    m_bComboAttackStart = false;
    m_fComboTime = 0.f;
}

shared_ptr<CPlayer> CPlayer::Create()
{
    shared_ptr<CPlayer> pInstance = make_shared<CPlayer>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CPlayer");

    return pInstance;
}
