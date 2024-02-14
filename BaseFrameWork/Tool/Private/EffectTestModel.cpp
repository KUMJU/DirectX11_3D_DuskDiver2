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


	return S_OK;
}

void CEffectTestModel::PriorityTick(_float _fTimeDelta)
{
}

void CEffectTestModel::Tick(_float _fTimeDelta)
{
}

void CEffectTestModel::LateTick(_float _fTimeDelta)
{
}

HRESULT CEffectTestModel::Render()
{
	return E_NOTIMPL;
}

shared_ptr<CEffectTestModel> CEffectTestModel::Create(shared_ptr<class CEffectPreset> _pPreset, const wstring& _strModelName)
{
	return shared_ptr<CEffectTestModel>();
}
