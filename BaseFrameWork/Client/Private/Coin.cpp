#include "pch.h"
#include "Coin.h"

#include "Collider.h"
#include "GameInstance.h"
#include "UIMgr.h"

#include "EffectMgr.h"
#include "EffectPreset.h"

#include "Model.h"
#include "Shader.h"

CCoin::CCoin()
{
}

HRESULT CCoin::Initialize()
{
    CTransform::TRANSFORM_DESC desc = {};
    desc.fRotationPerSet = 1.5f;
    desc.fSpeedPerSet = 0.f;

    __super::Initialize(TEXT("GoldA"), &desc);


    CCollider::COLLIDER_DESC CoinColDesc = {};

    CoinColDesc.vCenter = _float3(0.f, 0.3f, 0.f);
    CoinColDesc.fRadius = 0.6f;

    m_pTransformCom->Rotation({ 0.f ,1.f ,0.f, 0.f }, XMConvertToRadians(0.f));

    m_pCollider = CCollider::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), CCollider::TYPE_SPHERE, CoinColDesc);
    m_pCollider->SetOwner(shared_from_this());
    m_Components.emplace(TEXT("Com_Collider"), m_pCollider);

    m_pGlowPreset = CEffectMgr::GetInstance()->FindAndCloneEffect(TEXT("CoinSparkle"));
    m_pGlowPreset->SetParentTransform(m_pTransformCom);

    //GetCoin
    m_pGetCoinEffect = CEffectMgr::GetInstance()->FindAndCloneEffect(TEXT("GetCoin"));
    m_pGetCoinEffect->SetEnable(false);
    m_pGetCoinEffect->SetBillboard(true);

    m_IsEnabled = true;
    return S_OK;
}

void CCoin::PriorityTick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;

    m_pTransformCom->Turn({ 0.f, 1.f, 0.f, 0.f }, _fTimeDelta);

}

void CCoin::Tick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;


    if (m_bCollided) {

        m_fLerpTime += _fTimeDelta;

        if (m_fLerpTime >= m_fScaleLerpTotalTime) {
            m_IsEnabled = false;
        }
        else {

            _float fRate = 1.f - (m_fLerpTime / m_fScaleLerpTotalTime);
            //_float fRate2 = 
            m_pTransformCom->SetScaling(m_vInitScale.x * fRate, m_vInitScale.y * fRate, m_vInitScale.z * fRate);

            _float fOffset = m_vInitScale.y * fRate ;

            _vector vPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
            vPos = XMVectorSetY(vPos, m_fInitYPos + 0.6f);
            m_pTransformCom->SetState(CTransform::STATE_POSITION, vPos);
        }

    }


    m_pCollider->Tick(m_pTransformCom->GetWorldMatrix());
    CGameInstance::GetInstance()->AddCollider(CCollisionMgr::COL_BLOCK, m_pCollider);

}

void CCoin::LateTick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;

    __super::LateTick(_fTimeDelta);

#ifdef _DEBUG
    CGameInstance::GetInstance()->AddDebugComponent(m_pCollider);
#endif

}

HRESULT CCoin::Render()
{
    if (FAILED(BindShaderResources()))
        return E_FAIL;

    _uint iNumMeshes = m_pModelCom->GetNumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++) {

        if (FAILED(m_pModelCom->BindMaterialShaderResource(m_pShader, (_uint)i, aiTextureType::aiTextureType_DIFFUSE, "g_DiffuseTexture")))
            return E_FAIL;

       // if (FAILED(m_pModelCom->BindMaterialShaderResource(m_pShader, (_uint)i, aiTextureType::aiTextureType_EMISSIVE, "g_EmissiveTexture")))
       //     return E_FAIL;

        if (FAILED(m_pModelCom->BindMaterialShaderResource(m_pShader, (_uint)i, aiTextureType::aiTextureType_NORMALS, "g_NormalTexture")))
            return E_FAIL;

        if (FAILED(m_pShader->Begin(6)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Render((_uint)i)))
            return E_FAIL;
    }


    return S_OK;
}

void CCoin::OnCollide(EObjType _eObjType, shared_ptr<CCollider> _pCollider)
{
    if (m_bCollided)
        return;


    if (CGameObject::EObjType::OBJ_PLAYER == _eObjType) {
        m_fInitYPos = (m_pTransformCom->GetState(CTransform::STATE_POSITION)).m128_f32[1];
        m_bCollided = true;
        m_vInitScale = m_pTransformCom->GetScaled();
        //코인먹기 

        //이펙트 종료 
        m_pGlowPreset->StopEffect();

        //코인 먹기 이벤트 시작 
        m_pGetCoinEffect->SetEffectPosition(m_pTransformCom->GetState(CTransform::STATE_POSITION));
        m_pGetCoinEffect->PlayEffect();

        

        CUIMgr::GetInstance()->AddMiniQuestSuccessNumber();
        CGameInstance::GetInstance()->StopSound(CSoundMgr::CHANNELID::CH_MAPSE);
        CGameInstance::GetInstance()->PlayAudio(TEXT("se_GetCoin.wav"), CSoundMgr::CHANNELID::CH_MAPSE, 1.6f);
    }

}

void CCoin::ScaleLerp()
{
}

shared_ptr<CCoin> CCoin::Create()
{
    shared_ptr<CCoin> pInstance = make_shared<CCoin>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Failed to Create : CCoin");

    return pInstance;
}
