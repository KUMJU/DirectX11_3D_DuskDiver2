#include "pch.h"
#include "EffectMesh.h"

#include "Shader.h"
#include "Model.h"
#include "Texture.h"

#include "GameInstance.h"


CEffectMesh::CEffectMesh()
{
}

CEffectMesh::CEffectMesh(const CEffectMesh& _rhs)
    :m_MeshKey(_rhs.m_MeshKey),
    m_MeshDesc(_rhs.m_MeshDesc)
{
}

HRESULT CEffectMesh::Initialize( const wstring& _strModelKey, MESH_DESC* _MeshDesc, char* _strName, _bool _bLoop)
{
    m_eEffectType = EFFECT_TYPE::TYPE_MESH;
    m_MeshDesc = *_MeshDesc;

    m_MeshKey = _strModelKey;

    __super::Initialize(nullptr);
    ComputeInitSetting();

    m_pShader = CGameInstance::GetInstance()->GetShader(TEXT("Shader_VtxMesh"));


    _tchar szNoiseKey[MAX_PATH] = TEXT("");
    MultiByteToWideChar(CP_ACP, 0, m_MeshDesc.szNoiseTexKey, (_int)strlen(m_MeshDesc.szNoiseTexKey), szNoiseKey, MAX_PATH);

    _tchar szMaskKey[MAX_PATH] = TEXT("");
    MultiByteToWideChar(CP_ACP, 0, m_MeshDesc.szMaskTexKey, (_int)strlen(m_MeshDesc.szMaskTexKey), szMaskKey, MAX_PATH);


    m_pMaskTexture = CGameInstance::GetInstance()->GetTexture(szMaskKey);
    m_Components.emplace(TEXT("Com_MaskTexture"), m_pTexture);

    m_pNoiseTexture = CGameInstance::GetInstance()->GetTexture(szNoiseKey);
    m_Components.emplace(TEXT("Com_NoiseTexture"), m_pTexture);

    m_pModel = CGameInstance::GetInstance()->GetModel(_strModelKey);
    m_Components.emplace(TEXT("Com_Model"), m_pModel);
    m_pTransformCom->SetScaling(_MeshDesc->vStartScale.x, _MeshDesc->vStartScale.y, _MeshDesc->vStartScale.z);
    m_pTransformCom->SetState(CTransform::STATE_POSITION, { _MeshDesc->vCenter.x, _MeshDesc->vCenter.y, _MeshDesc->vCenter.z, 1.f });

    m_pTransformCom->RotaitionRollYawPitch(
        XMConvertToRadians(_MeshDesc->vRotation.x),
        XMConvertToRadians(_MeshDesc->vRotation.y),
        XMConvertToRadians(_MeshDesc->vRotation.z));

    m_strEffectName = _strName;

    m_vColor = _MeshDesc->vColor;
    m_bLoop = _bLoop;

    return S_OK;
}

void CEffectMesh::PriorityTick(_float _fTimeDelta)
{
}

void CEffectMesh::Tick(_float _fTimeDelta, _matrix _ParentMat)
{
    if (!m_IsEnabled)
        return;

    m_fTimeDelta = _fTimeDelta;
    m_fAccTime += _fTimeDelta;

    //루프가 아닐때
    if (!m_MeshDesc.bLoop) {

        //아직 시작 시간이 아닐때
        if (m_fAccTime < m_MeshDesc.vDuration.x) {
            return;
        }

        if (m_fAccTime >= m_MeshDesc.vDuration.y) {
            m_IsEnabled = false;
            return;
        }

        ScaleLerp();
        m_ParentMat = _ParentMat;
    }
    else {
        m_ParentMat = _ParentMat;
    }
}

void CEffectMesh::LateTick(_float _fTimeDelta)
{
    if (!m_IsEnabled)
        return;

    if (m_fAccTime < m_MeshDesc.vDuration.x) {
        return;
    }

    if (FAILED(CGameInstance::GetInstance()->AddRenderGroup(CRenderer::RENDER_NONLIGHT, shared_from_this())))
        return;

    if (FAILED(CGameInstance::GetInstance()->AddRenderGroup(CRenderer::RENDER_GLOW, shared_from_this())))
       return;

}

HRESULT CEffectMesh::Render()
{
 
    _float4x4 ViewMat = CGameInstance::GetInstance()->GetTransformFloat4x4(CPipeLine::D3DTS_VIEW);

    if (FAILED(m_pShader->BindMatrix("g_ViewMatrix", &ViewMat)))
        return E_FAIL;

    _float4x4 ProjMat = CGameInstance::GetInstance()->GetTransformFloat4x4(CPipeLine::D3DTS_PROJ);

    if (FAILED(m_pShader->BindMatrix("g_ProjMatrix", &ProjMat)))
        return E_FAIL;

    _matrix WorldOwnMatrix = m_pTransformCom->GetWorldMatrix();
    _float4x4 FloatWorld;

    //부모 객체가 따로 없으면 0,0,0 혹은 아이덴티티가 들어가서 아무 영향 없다 
    WorldOwnMatrix.r[3] += m_vParentPos;
    WorldOwnMatrix = WorldOwnMatrix * m_ParentMat;


    XMStoreFloat4x4(&FloatWorld, WorldOwnMatrix);

    //if (FAILED(m_pTransformCom->BindShaderResource(m_pShader, "g_WorldMatrix")))
     //   return E_FAIL;

    if (m_bBillboard) {

        _float4x4 ViewCopyMat = ViewMat;

        ViewCopyMat._41 = 0.f;
        ViewCopyMat._42 = 0.f;
        ViewCopyMat._43 = 0.f;

        _matrix ViewInv = {};

        ViewInv = XMMatrixInverse(NULL, XMLoadFloat4x4(&ViewCopyMat));
        ViewInv = WorldOwnMatrix * ViewInv;
        ViewInv.r[3] = WorldOwnMatrix.r[3] + _vector({m_MeshDesc.vCenter.x , m_MeshDesc.vCenter.y, m_MeshDesc.vCenter.z, 0.f});

        _float4x4 BillboardWorld = {};
        XMStoreFloat4x4(&BillboardWorld, ViewInv);

        if (FAILED(m_pShader->BindMatrix("g_WorldMatrix", &BillboardWorld)))
            return E_FAIL;

    }
    else {
        if (FAILED(m_pShader->BindMatrix("g_WorldMatrix", &FloatWorld)))
            return E_FAIL;
    }


    if (FAILED(m_pShader->BindRawValue("g_vColor", &m_vColor, sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pShader->BindRawValue("g_vLerpColor", &m_MeshDesc.vLerpColor, sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pShader->BindRawValue("g_bMaskTex", &m_MeshDesc.bMask, sizeof(_bool))))
        return E_FAIL;

    if (FAILED(m_pShader->BindRawValue("g_fDeltaTime", &m_fTimeDelta, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShader->BindRawValue("g_bBillboard", &m_bBillboard, sizeof(_bool))))
        return E_FAIL;

    

    if (m_MeshDesc.bMask) {
        m_pMaskTexture->BindShaderResource(m_pShader, "g_MaskTexture", 0);

        if (FAILED(m_pShader->BindRawValue("g_vMaskSpeed", &m_fCurrentSpeed, sizeof(_float))))
            return E_FAIL;

        m_fCurrentSpeed += m_MeshDesc.fMaskSpeed;

        if (FAILED(m_pShader->BindRawValue("g_vMaskDirection", &m_MeshDesc.vMaskDir, sizeof(_float2))))
            return E_FAIL;

    }

    if (FAILED(m_pShader->BindRawValue("g_bNoiseTex", &m_MeshDesc.bNoise, sizeof(_bool))))
        return E_FAIL;

    if (m_MeshDesc.bNoise) {
        m_pNoiseTexture->BindShaderResource(m_pShader, "g_NoiseTexture", 0);

        if (FAILED(m_pShader->BindRawValue("g_vNoiseSpeed", &m_MeshDesc.fNoiseSpeed, sizeof(_float))))
            return E_FAIL;

        if (FAILED(m_pShader->BindRawValue("g_vNoiseDirection", &m_MeshDesc.vNoiseDir, sizeof(_float2))))
            return E_FAIL;
    }

    _uint iPassNum = 3; //Clamp

    if (m_MeshDesc.bUVLoop) {
        iPassNum = 2; //Wrap
    }


    _uint iNumMeshes = m_pModel->GetNumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++) {

        if (FAILED(m_pModel->BindMaterialShaderResource(m_pShader, (_uint)i, aiTextureType::aiTextureType_DIFFUSE, "g_DiffuseTexture")))
            return E_FAIL;

        if (FAILED(m_pShader->Begin(iPassNum)))
            return E_FAIL;

        if (FAILED(m_pModel->Render((_uint)i)))
            return E_FAIL;
    }

    return S_OK;
}


void CEffectMesh::ScaleLerp()
{
    if (m_fMiddleTime >= m_fAccTime) {
        m_vCurrentScale += XMLoadFloat3(&m_vStartScaleDiff) * m_fTimeDelta;
    }
    else {
        m_vCurrentScale += XMLoadFloat3(&m_vEndScaleDiff) * m_fTimeDelta;
    }

    // SetScaling
    m_pTransformCom->SetScaling(m_vCurrentScale.m128_f32[0],
        m_vCurrentScale.m128_f32[1],
        m_vCurrentScale.m128_f32[2]);

}


void CEffectMesh::ComputeInitSetting()
{
    m_fMiddleTime = m_MeshDesc.fScaleChangeTime;

    _float fStartProcessTime = m_fMiddleTime - m_MeshDesc.vDuration.x;
    _float fEndProcessTime = m_MeshDesc.vDuration.y - m_fMiddleTime;

    //시작~ 중간 차이 계산 
    m_vStartScaleDiff = _float3({ (m_MeshDesc.vMiddleScale.x - m_MeshDesc.vStartScale.x) / fStartProcessTime,
    (m_MeshDesc.vMiddleScale.y - m_MeshDesc.vStartScale.y) / fStartProcessTime,
    (m_MeshDesc.vMiddleScale.z - m_MeshDesc.vStartScale.z) / fStartProcessTime });


    //중간 ~ 끝 차이 계산


    m_vEndScaleDiff = _float3({ (m_MeshDesc.vEndScale.x - m_MeshDesc.vMiddleScale.x) / fEndProcessTime,
    (m_MeshDesc.vEndScale.y - m_MeshDesc.vMiddleScale.y) / fEndProcessTime,
    (m_MeshDesc.vEndScale.z - m_MeshDesc.vMiddleScale.z) / fEndProcessTime });


}

void CEffectMesh::ResetEffect()
{
    m_fCurrentSpeed = 0.f;
    m_fAccTime = 0.f;
    m_vCurrentScale = XMLoadFloat3(&m_MeshDesc.vStartScale);
}


shared_ptr<CEffectMesh> CEffectMesh::Create( const wstring& _strModelKey, MESH_DESC* _MeshDesc, char* _strName, _bool _bLoop)
{

    shared_ptr<CEffectMesh> pInstance = make_shared<CEffectMesh>();

    if (FAILED(pInstance->Initialize( _strModelKey, _MeshDesc, _strName , _bLoop)))
        MSG_BOX("Failed to Create : cEffectMesh");

    return pInstance;
}

shared_ptr<CEffect> CEffectMesh::CloneEffect()
{
    shared_ptr<CEffectMesh> pInstance = make_shared<CEffectMesh>();

    if (FAILED(pInstance->Initialize(m_MeshKey, &m_MeshDesc, m_strEffectName, m_bLoop))) {
        MSG_BOX("Failed to Clone : CModel");
    }

    return pInstance;

}
