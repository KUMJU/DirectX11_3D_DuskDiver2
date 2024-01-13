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
    TransformDesc.fSpeedPerSet = 10.f;
    TransformDesc.fRotationPerSet = 20.f;

    if (FAILED(__super::Initialize(&TransformDesc)))
        return E_FAIL;

    if (FAILED(AddComponent()))
        return E_FAIL;

    m_pShader = CGameInstance::GetInstance()->GetShader(TEXT("Shader_VtxAnim"));

    m_pModelCom = CGameInstance::GetInstance()->GetModel(TEXT("Hero1_BattleMode_3Anim"));
    m_Components.emplace(TEXT("Com_Model"), m_pModelCom);

    m_eCurrentState = HEROSTATE::STATE_IDLE;
    m_pModelCom->SetAnimNum(17);

    return S_OK;
}

void CPlayer::PriorityTick(_float _fTimeDelta)
{
}

void CPlayer::Tick(_float _fTimeDelta)
{

    if (m_IsMouseDeb) {

        m_fDebTime += _fTimeDelta;

        if (m_fDebTime > 0.6f) {
            m_fDebTime = 0.f;
            m_IsMouseDeb = false;
        }
            

    }

    //MouseInput(_fTimeDelta);
    KeyInput(_fTimeDelta);
    if(m_pModelCom->PlayAnimation(_fTimeDelta, m_isAnimLoop, &m_vCurretnAnimPos)){

        if (HEROSTATE::STATE_COMBO_ATTACK1 == m_eCurrentState) {
            if (5 == m_CurrentCombo) {
                FinishCombo();
            }
            else if(true == m_ComboArr[m_CurrentCombo+1]) 
            {
                ++m_CurrentCombo;
                ChangeAnim(m_ComboAnimKeyArr[m_CurrentCombo], false);
            }
            else {
                FinishCombo();
            }
        }
        else {

            m_eCurrentState = HEROSTATE::STATE_IDLE;
            ChangeAnim(0, true);
        }
    }

    CalcAnimMoveDistance();
}

void CPlayer::LateTick(_float _fTimeDelta)
{
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
    if (!m_pModelCom->ChangeAnimation(_iAnimNum))
        return;

    if (HEROSTATE::STATE_COMBO_ATTACK1 != m_eCurrentState) {
        m_vPrevAnimPos = { 0.f, 0.f, 0.f };
        m_vCurretnAnimPos = { 0.f, 0.f, 0.f };
    }

    m_isAnimLoop = _isLoop;

}

void CPlayer::FinishCombo()
{
    m_IsKeyDeb = true;

    for (_uint i = 0; i < 5; ++i) {
        m_ComboArr[i] = false;
    }

    m_CurrentCombo = -1;

    m_eCurrentState = HEROSTATE::STATE_IDLE;
    ChangeAnim(0, true);
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

    if (HEROSTATE::STATE_COMBO_ATTACK1 == m_eCurrentState)
        return;

    _bool IsKeyInput = false;

    if (GetKeyState('W') & 0x8000)
    {
        _float4 vLook = CCameraMgr::GetInstance()->GetCamLook();
        _vector vLookVec = XMLoadFloat4(&vLook);

        _vector vPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
        vPos += vLookVec;
        m_pTransformCom->LookAt(vPos);

        m_pTransformCom->GoStraight(_fTimeDelta);
        m_eCurrentState = HEROSTATE::STATE_WALK;
        ChangeAnim(24, true);
        IsKeyInput = true;

    }

    if (GetKeyState('A') & 0x8000)
    {
       // m_pTransformCom->GoLeft(_fTimeDelta);
//m_eCurrentState = HEROSTATE::WALK;

      //  m_pTransformCom->GoStraight(_fTimeDelta);
        ChangeAnim(1, true);
     //   IsKeyInput = true;

    }

    if (GetKeyState('D') & 0x8000)
    {
        if (m_IsMouseDeb)
            return;


        m_IsMouseDeb = true;
        //m_pTransformCom->GoRight(_fTimeDelta);

        //m_eCurrentState = HEROSTATE::WALK;

        //m_pTransformCom->GoStraight(_fTimeDelta);
        ChangeAnim(++m_iCurrentAnimIdx, true);
      //  IsKeyInput = true;

    //    m_pModelCom->ChangeAnimation(m_iCurrentAnimIdx);

        return;

    }

    if (GetKeyState('S') & 0x8000) {

        _float4 vLook = CCameraMgr::GetInstance()->GetCamLook();
        _vector vLookVec = XMLoadFloat4(&vLook) * -1.f;

        _vector vPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
        vPos += vLookVec;

        m_pTransformCom->LookAt(vPos);

        m_pTransformCom->GoStraight(_fTimeDelta);
        m_eCurrentState = HEROSTATE::STATE_WALK;

        m_pTransformCom->GoStraight(_fTimeDelta);
        ChangeAnim(24, true);
        IsKeyInput = true;
    }

    ////////////////Camera/////////////////////////

    if (GetKeyState('1') & 0x8000) {

        CCameraMgr::GetInstance()->SwitchingCamera(CCameraMgr::ECAMERATYPE::FREE);
    }

    if (HEROSTATE::STATE_WALK == m_eCurrentState) {
            m_eCurrentState = HEROSTATE::STATE_IDLE;
            ChangeAnim(17, true);
    
    }
}


void CPlayer::MouseInput(_float _fTimeDelta)
{
   
}

shared_ptr<CPlayer> CPlayer::Create()
{
    shared_ptr<CPlayer> pInstance = make_shared<CPlayer>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CPlayer");

    return pInstance;
}
