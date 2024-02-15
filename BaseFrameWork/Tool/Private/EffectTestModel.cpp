#include "pch.h"
#include "EffectTestModel.h"

#include "Model.h"
#include "Shader.h"
#include "GameInstance.h"

CEffectTestModel::CEffectTestModel()
{
}

HRESULT CEffectTestModel::Initialize(CTransform::TRANSFORM_DESC* _pDesc)
{
	CTransform::TRANSFORM_DESC TransformDesc = {};
	TransformDesc.fSpeedPerSet = 0.f;
	TransformDesc.fRotationPerSet = 0.f;

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	m_pModel = CGameInstance::GetInstance()->GetModel(TEXT("Hero1_BattleMode_3Anim"));
	m_pShader = CGameInstance::GetInstance()->GetShader(TEXT("Shader_VtxAnim"));
	
	m_pModel->ChangeAnimation(44);

	return S_OK;
}

void CEffectTestModel::PriorityTick(_float _fTimeDelta)
{
}

void CEffectTestModel::Tick(_float _fTimeDelta)
{
	_float3 vPos = _float3();
	m_pModel->PlayAnimation(_fTimeDelta, true, &vPos);
}

void CEffectTestModel::LateTick(_float _fTimeDelta)
{
	CGameInstance::GetInstance()->AddRenderGroup(CRenderer::RENDER_NONBLEND, shared_from_this());

}

HRESULT CEffectTestModel::Render()
{
	if (FAILED(m_pTransformCom->BindShaderResource(m_pShader, "g_WorldMatrix")))
		return E_FAIL;

	_float4x4 ViewMat = CGameInstance::GetInstance()->GetTransformFloat4x4(CPipeLine::D3DTS_VIEW);

	if (FAILED(m_pShader->BindMatrix("g_ViewMatrix", &ViewMat)))
		return E_FAIL;

	_float4x4 ProjMat = CGameInstance::GetInstance()->GetTransformFloat4x4(CPipeLine::D3DTS_PROJ);

	if (FAILED(m_pShader->BindMatrix("g_ProjMatrix", &ProjMat)))
		return E_FAIL;

	_uint iNumMeshes = m_pModel->GetNumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++) {

		if (FAILED(m_pModel->BindMaterialShaderResource(m_pShader, (_uint)i, aiTextureType::aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pModel->BindBoneMatrices(m_pShader, "g_BoneMatrices", (_uint)i)))
			return E_FAIL;

		if (FAILED(m_pShader->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModel->Render((_uint)i)))
			return E_FAIL;
	}

	return S_OK;
}

void CEffectTestModel::ChangeAnimation(_uint _iAnimNum)
{
	m_pModel->ChangeAnimation(_iAnimNum);
}

shared_ptr<CEffectTestModel> CEffectTestModel::Create()
{
	shared_ptr<CEffectTestModel> pModel = make_shared<CEffectTestModel>();

	if (FAILED(pModel->Initialize(nullptr)))
		MSG_BOX("Failed to Create : CEffectTestModel");

	return pModel;
}
