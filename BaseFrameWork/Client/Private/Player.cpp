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
    TransformDesc.fRotationPerSet = 20.f;

    if (FAILED(__super::Initialize(&TransformDesc)))
        return E_FAIL;

    if (FAILED(AddComponent()))
        return E_FAIL;

    m_pShader = CGameInstance::GetInstance()->GetShader(TEXT("Shader_VtxAnim"));

    m_pModelCom = CGameInstance::GetInstance()->GetModel(TEXT("Hero1_BattleMode_3Anim"));
    m_Components.emplace(TEXT("Com_Model"), m_pModelCom);

    m_eCurrentState = HEROSTATE::STATE_IDLE;
    m_pModelCom->ChangeAnimation(18);

    return S_OK;
}

void CPlayer::PriorityTick(_float _fTimeDelta)
{
}

void CPlayer::Tick(_float _fTimeDelta)
{
    //공격모션이 idle로 전환되는 과정까지 포함되어있어서 스피드한 콤보 공격을 위해 중간에 끊어 진행
    if (m_bComboAttackStart) {
        m_fComboTime += _fTimeDelta;
        if (m_bReserveCombo && m_fComboTime > 0.5f) {
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

        if (m_fDebTime > 0.1f) {
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

    MouseInput(_fTimeDelta);
    KeyInput(_fTimeDelta);

    if(m_pModelCom->PlayAnimation(_fTimeDelta, m_isAnimLoop, &m_vCurretnAnimPos)){
        CheckReserveAnimList();
    }

    //루트모션 거리계산
    CalcAnimMoveDistance();
}

void CPlayer::LateTick(_float _fTimeDelta)
{
    //if (m_eCurrentState == HEROSTATE::STATE_JUMP) {


    //}


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

void CPlayer::ChangeAnim(_uint _iAnimNum, _bool _isLoop)
{

    m_iCurrentAnimIdx;
    if (19 == _iAnimNum) {
        m_IsLoopMotion = true;
    }
    else if (1 == _iAnimNum) {
        m_eCurrentState = HEROSTATE::STATE_COMBO_ATTACK2;
    }
    else if (2 == _iAnimNum) {
        m_eCurrentState = HEROSTATE::STATE_COMBO_ATTACK3;
    }
    else if (22 == _iAnimNum) {
        //Jump Landing 
        m_bJump = false;
    }

    if (!m_pModelCom->ChangeAnimation(_iAnimNum))
        return;

    if (HEROSTATE::STATE_COMBO_ATTACK1 != m_eCurrentState&&
        HEROSTATE::STATE_COMBO_ATTACK2 != m_eCurrentState&&
        HEROSTATE::STATE_COMBO_ATTACK3 != m_eCurrentState&&
        HEROSTATE::STATE_COMBO_ATTACK4 != m_eCurrentState) {
        m_vPrevAnimPos = { 0.f, 0.f, 0.f };
        m_vCurretnAnimPos = { 0.f, 0.f, 0.f };
    }

    m_iCurrentAnimIdx = _iAnimNum;
    m_isAnimLoop = _isLoop;

}

//미리 예약해둔 애니메이션을 확인하고, 있으면 그 애니메이션을 돌리고 없으면 기본 idle 상태로 돌아옵니다
void CPlayer::CheckReserveAnimList()
{
    if (m_NextAnimIndex.empty()) {

        if (m_bComboAttackStart) {
            ResetComboState();
        }

        if (m_IsUsingSkill)
            m_IsUsingSkill = false;

        //IDLE로 돌아감
        m_eCurrentState = HEROSTATE::STATE_IDLE;
        ChangeAnim(18, true);

    }
    else {

        _uint iNextAnim = m_NextAnimIndex.front().iNextAnimNum;
        _bool isLoop = m_NextAnimIndex.front().IsLoop;

        m_NextAnimIndex.pop_front();
        ChangeAnim(iNextAnim, isLoop);
    }
}

void CPlayer::FinishCombo()
{

}

void CPlayer::CalcAnimMoveDistance()
{
    _vector vDistance =  XMLoadFloat3(&m_vCurretnAnimPos) - XMLoadFloat3(&m_vPrevAnimPos);
    _vector vLook = m_pTransformCom->GetState(CTransform::STATE_LOOK);


    _matrix WorldMat = m_pTransformCom->GetWorldMatrix();

    XMVector3TransformCoord(vDistance, WorldMat);

    vDistance = vDistance * vLook;

    _vector vCurrentPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
    vCurrentPos += vDistance;
    //vCurrentPos = XMVectorSetW(vCurrentPos, 1.f);

    m_pTransformCom->SetState(CTransform::STATE_POSITION, vCurrentPos);

    m_vPrevAnimPos = m_vCurretnAnimPos;

}

void CPlayer::KeyInput(_float _fTimeDelta)
{ 
    if (m_bComboAttackStart)
        return;

    if (m_IsUsingSkill)
        return;

    if (HEROSTATE::STATE_HEAVY_ATTACK == m_eCurrentState)
        return;

    _bool IsKeyInput = false;
    _uint FinalAnimNum = 0;
    _bool IsLoop = false;

    ////////////////////////Skill////////////////////////

    //Skill1
    if (GetKeyState('Q') & 0x8000)
    {

        m_eCurrentState = HEROSTATE::STATE_SKILL_A;
        IsKeyInput = true;
        ChangeAnim(25, false);
        m_IsUsingSkill = true;

    }

    //Skill2
    if (GetKeyState('E') & 0x8000)
    {
        m_eCurrentState = HEROSTATE::STATE_SKILL_A;
        IsKeyInput = true;
        ChangeAnim(26, false);
        m_IsUsingSkill = true;

    }

    if (GetKeyState('Z') & 0x8000)
    {
        m_eCurrentState = HEROSTATE::STATE_SKILL_A;
        IsKeyInput = true;
        ChangeAnim(27, false);
        m_IsUsingSkill = true;

    }


    ///////////////////Walk////////////////////////


    if (GetKeyState('W') & 0x8000)
    {
        _float4 vLook = CCameraMgr::GetInstance()->GetCamLook();
        _vector vLookVec = XMLoadFloat4(&vLook);

        _vector vPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
        vPos += vLookVec;
        m_pTransformCom->LookAt(vPos);

        m_pTransformCom->GoStraight(_fTimeDelta);
        FinalAnimNum = 24;
        IsLoop = true;


        IsKeyInput = true;
    }

    if (GetKeyState('A') & 0x8000)
    {
        m_pTransformCom->GoLeft(_fTimeDelta);    
        m_pTransformCom->GoStraight(_fTimeDelta);
        
        FinalAnimNum = 24;
        IsLoop = true;

        IsKeyInput = true;


    }

    if (GetKeyState('D') & 0x8000)
    {
        if (m_IsMouseDeb)
            return;


        m_IsMouseDeb = true;
        m_pTransformCom->GoRight(_fTimeDelta);
        m_pTransformCom->GoStraight(_fTimeDelta);
        IsKeyInput = true;

        FinalAnimNum = 24;
        IsLoop = true;
    }

    if (GetKeyState('S') & 0x8000) {

        _float4 vLook = CCameraMgr::GetInstance()->GetCamLook();
        _vector vLookVec = XMLoadFloat4(&vLook) * -1.f;

        _vector vPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
        vPos += vLookVec;

        m_pTransformCom->LookAt(vPos);
        m_pTransformCom->GoStraight(_fTimeDelta);
        FinalAnimNum = 24;
        IsLoop = true;

        m_pTransformCom->GoStraight(_fTimeDelta);       
        IsKeyInput = true;
    }

    if (m_eCurrentState == HEROSTATE::STATE_JUMP ||
        m_eCurrentState == HEROSTATE::STATE_DASH ||
        m_eCurrentState == HEROSTATE::STATE_HEAVY_ATTACK) {
        return;
    }


    ////////////////////////Dash + Jump ////////////////////////

    //Dash
    if (GetKeyState('R') & 0x8000)
    {
        _float4 vLook = CCameraMgr::GetInstance()->GetCamLook();
        _vector vLookVec = XMLoadFloat4(&vLook);

        _vector vPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
        vPos += vLookVec;
        m_pTransformCom->LookAt(vPos);

        m_pTransformCom->GoStraight(_fTimeDelta);
        m_eCurrentState = HEROSTATE::STATE_DASH;

        FinalAnimNum = 5;
        IsLoop = false;
        IsKeyInput = true;

    }

    //Jump
    if (GetKeyState(VK_SPACE) & 0x8000)
    {
        if (HEROSTATE::STATE_JUMP == m_eCurrentState)
            return;

        m_eCurrentState = HEROSTATE::STATE_JUMP;
         
        m_NextAnimIndex.push_back({ 19, true });
        m_NextAnimIndex.push_back({ 22, false });

        FinalAnimNum = 21;
        IsLoop = false;
        m_bJump = true;
        ChangeAnim(21, false);


    }

    if (IsKeyInput) {

        if(24 == FinalAnimNum)
            m_eCurrentState = HEROSTATE::STATE_WALK;

        ChangeAnim(FinalAnimNum, IsLoop);
        return;
    }

    ////////////////Camera/////////////////////////

    if (GetKeyState('1') & 0x8000) {

        CCameraMgr::GetInstance()->SwitchingCamera(CCameraMgr::ECAMERATYPE::FREE);
    }

    if (HEROSTATE::STATE_WALK == m_eCurrentState) {
            m_eCurrentState = HEROSTATE::STATE_IDLE;
            ChangeAnim(18, true);
    
    }
}


void CPlayer::MouseInput(_float _fTimeDelta)
{
    if (m_IsUsingSkill)
        return;

    if (m_eCurrentState == HEROSTATE::STATE_HEAVY_ATTACK ||
        m_eCurrentState == HEROSTATE::STATE_COMBO_ATTACK5 ||
        m_IsMouseDeb == true) {
        return;
    }

    //HeavyAttack
    if (CGameInstance::GetInstance()->GetDIMouseState(MOUSEKEYSTATE::DIM_RB)) {

        if (m_bJump) {
            m_eCurrentState = HEROSTATE::STATE_HEAVY_ATTACK;
            m_NextAnimIndex.clear();
            ChangeAnim(49, false);
        }
        else {
            m_eCurrentState = HEROSTATE::STATE_HEAVY_ATTACK;
            ChangeAnim(12, false);
        }
        return;
    }

    //Nomal Combo Attack
    if (CGameInstance::GetInstance()->GetDIMouseState(MOUSEKEYSTATE::DIM_LB)) {

        if (m_bReserveCombo)
            return;

        switch (m_eCurrentState)
        {
        case Client::CPlayer::HEROSTATE::STATE_COMBO_ATTACK1:
            m_NextAnimIndex.push_back({ 1 , false });
            m_bReserveCombo = true;
            break;
        case Client::CPlayer::HEROSTATE::STATE_COMBO_ATTACK2:
            m_NextAnimIndex.push_back({ 2 , false });
            m_bReserveCombo = true;
            break;
        case Client::CPlayer::HEROSTATE::STATE_COMBO_ATTACK3:
            break;
        case Client::CPlayer::HEROSTATE::STATE_COMBO_ATTACK4:

            //attack5 Call + mouseDeb 
            break;
        case Client::CPlayer::HEROSTATE::STATE_COMBO_ATTACK5:
            break;
        default:
            if (m_bComboAttackStart)
                return;

            ChangeAnim(0, false);
            m_eCurrentState = HEROSTATE::STATE_COMBO_ATTACK1;
            m_bComboAttackStart = true;
            break;
        }

        m_IsMouseDeb = true;

    }
   
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
