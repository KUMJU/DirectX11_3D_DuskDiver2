#include "pch.h"
#include "HockeyBall.h"
#include "GameMgr.h"

#include "MapLoader.h"
#include "Navigation.h"
#include "Collider.h"
#include "Model.h"
#include "Shader.h"

#include "GameInstance.h"
#include "Player.h"

#include "Bounding.h"

CHockeyBall::CHockeyBall()
{
}

HRESULT CHockeyBall::Initialize( _fvector _vStartPos)
{
    CTransform::TRANSFORM_DESC desc = { };
    desc.fRotationPerSet = 0.f;
    desc.fSpeedPerSet = 8.f;

    __super::Initialize(&desc);

    m_eObjType = EObjType::OBJ_HOCKEYBALL;

    m_pTransformCom->SetState(CTransform::STATE_POSITION, _vStartPos);
    m_pTransformCom->SetYOffset(1.2f);

    m_pShader = CGameInstance::GetInstance()->GetShader(TEXT("Shader_VtxMesh"));
    m_pModel = CGameInstance::GetInstance()->GetModel(TEXT("HockeyBall"));

    CCollider::COLLIDER_DESC colDesc = {};
    colDesc.fRadius = 3.f;
    colDesc.vCenter = { 0.f, -2.f, 0.f, };
    m_fColRadius = colDesc.fRadius;

    m_pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_SPHERE, colDesc );
    m_pCollider->SetOwner(shared_from_this());

    m_pNavigation = CMapLoader::GetInstance()->GetCurrentNavi(1);
    m_pNavigation->CalcCurrentPos(_vStartPos);

    return S_OK;
}

void CHockeyBall::PriorityTick(_float _fTimeDelta)
{
    if (!m_pPlayer) {
        m_pPlayer = CGameMgr::GetInstance()->GetPlayer();
        m_pPlrTransform = dynamic_pointer_cast<CTransform>(m_pPlayer->GetComponent(TEXT("Com_Transform")));
    }

}

void CHockeyBall::Tick(_float _fTimeDelta)
{
    _bool bJump = false;
    m_pTransformCom->GoStraight(_fTimeDelta, m_pNavigation, bJump);

    m_pCollider->Tick(m_pTransformCom->GetWorldMatrix());
    CGameInstance::GetInstance()->AddCollider(CCollisionMgr::COL_BLOCK, m_pCollider);

}

void CHockeyBall::LateTick(_float _fTimeDelta)
{
    if (FAILED(CGameInstance::GetInstance()->AddRenderGroup(CRenderer::RENDER_NONBLEND, shared_from_this())))
        return;
}

HRESULT CHockeyBall::Render()
{
    if (FAILED(BindShaderResources()))
        return E_FAIL;

    m_pCollider->Render();

    _uint iNumMeshes = m_pModel->GetNumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++) {

        if (FAILED(m_pModel->BindMaterialShaderResource(m_pShader, (_uint)i, aiTextureType::aiTextureType_DIFFUSE, "g_DiffuseTexture")))
            return E_FAIL;

        if (FAILED(m_pShader->Begin(0)))
            return E_FAIL;

        if (FAILED(m_pModel->Render((_uint)i)))
            return E_FAIL;
    }

    return S_OK;
}

void CHockeyBall::OnCollide(EObjType _eObjType, shared_ptr<CCollider> _pCollider)
{
    //if monster , player -> hit 뭐 어쩌고 저쩌고 함수 
    //아니다 얘는 벽충돌만 알면됨

    if (EObjType::OBJ_WALL == _eObjType) {

        _float3 vExtents = _pCollider->GetExtents();
        _float3 vCenter = _pCollider->GetBounding()->GetBoundingAABB()->Center;


        _vector vLook = m_pTransformCom->GetState(CTransform::STATE_LOOK);
        _vector vPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
        _vector vNewPos;

        _float fminDistance;

        //z만큼 빼줘야함 
        if (vExtents.x > vExtents.z) {
            fminDistance = vExtents.z;
            _float fComputeZ = 0.f;

            //Z가 양수면 음의 방향으로 제거 
            if (vLook.m128_f32[2] > 0.f) {
                fComputeZ = vCenter.z - fminDistance- m_fColRadius;
            }
            else {
                fComputeZ = vCenter.z + fminDistance + m_fColRadius;
            }

            vNewPos = { vPos.m128_f32[0] , vPos.m128_f32[1] ,fComputeZ, 1.f };
            m_pTransformCom->SetState(CTransform::STATE_POSITION, vNewPos);

        }
        else {
            fminDistance = vExtents.x;
            _float fComputeX = 0.f;

            //Z가 양수면 음의 방향으로 제거 
            if (vLook.m128_f32[0] > 0.f) {
                fComputeX = vCenter.x - fminDistance - m_fColRadius;
            }
            else {
                fComputeX = vCenter.x + fminDistance + m_fColRadius;
            }

            vNewPos = { fComputeX , vPos.m128_f32[1] ,vPos.m128_f32[2], 1.f};
            m_pTransformCom->SetState(CTransform::STATE_POSITION, vNewPos);


        }
        
        ComputeLookVector();
    }


}

HRESULT CHockeyBall::BindShaderResources()
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

    return S_OK;
}

void CHockeyBall::ComputeLookVector()
{
    _vector vPlrPos = m_pPlrTransform->GetState(CTransform::STATE_POSITION);
    m_pTransformCom->LookAtForLandObject(vPlrPos);
}

shared_ptr<CHockeyBall> CHockeyBall::Create(_fvector _vStartPos)
{
    shared_ptr<CHockeyBall> pInstance = make_shared<CHockeyBall>();

    if (pInstance->Initialize(_vStartPos))
        MSG_BOX("Failed to Create : CHockeyBall");

    return pInstance;
}
