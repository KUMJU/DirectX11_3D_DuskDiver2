#include "pch.h"
#include "EffectPreset.h"
#include "Effect.h"

CEffectPreset::CEffectPreset()
{
}

CEffectPreset::CEffectPreset(const CEffectPreset& _rhs)
	:m_bLoop(_rhs.m_bLoop),
	m_vTotalDuration(_rhs.m_vTotalDuration),
	m_Effects(_rhs.m_Effects)
{
}

HRESULT CEffectPreset::Initialize()
{
	return S_OK;
}

void CEffectPreset::PriorityTick(_float _fTimeDelta)
{
}

void CEffectPreset::Tick(_float _fTimeDelta)
{

	if (!m_IsEnabled)
		return;

	m_fAccTime += _fTimeDelta;

	if (!m_bLoop) {
		if (m_fAccTime >= m_vTotalDuration) {
			m_IsEnabled = false;
			m_fAccTime = 0.f;
			ResetEffect();
		}
	}

	for (auto& iter : m_Effects)
		iter->Tick(_fTimeDelta);


}

void CEffectPreset::LateTick(_float _fTimeDelta)
{
	if (!m_IsEnabled)
		return;

	for (auto& iter : m_Effects)
		iter->LateTick(_fTimeDelta);
}

HRESULT CEffectPreset::Render()
{

	return S_OK;
}

void CEffectPreset::AddEffect(shared_ptr<class CEffect> _pEffect)
{
	m_Effects.push_back(_pEffect);

}

void CEffectPreset::DeleteEffect(_uint _iEffectIdx)
{
	if (_iEffectIdx > m_Effects.size())
		return;

	auto iter = m_Effects.begin();


	for (size_t i = 0; i < _iEffectIdx; ++i) {
		++iter;
	}

	m_Effects.erase(iter);

}

void CEffectPreset::PlayEffect()
{
	for (auto& iter : m_Effects) {

		iter->ResetEffect();
		iter->SetEnable(true);
	}

	m_fAccTime = 0.f;
	m_IsEnabled = true;

}

void CEffectPreset::StopEffect()
{
	m_fAccTime = 0.f;
	m_IsEnabled = false;


}

void CEffectPreset::ParsingEffect(Json::Value& _root)
{
	//이펙트 갯수 + 루프여부 + 전체 duration
	//이펙트 타입 + 필요한 인자들저장

	for (auto& iter : m_Effects) {	
		iter->ParsingData(_root);
	}

}

void CEffectPreset::ResetEffect()
{
	for (auto& iter : m_Effects) {
		iter->ResetEffect();
	}
}

shared_ptr<CEffectPreset> CEffectPreset::Create()
{
	shared_ptr<CEffectPreset> pInstance = make_shared<CEffectPreset>();

	if (FAILED(pInstance->Initialize()))
		MSG_BOX("Failed to Create : CEffectPreset");

	return pInstance;
}

shared_ptr<CEffectPreset> CEffectPreset::Clone(shared_ptr<CEffectPreset> _rhs)
{
	shared_ptr<CEffectPreset> pInstance = make_shared<CEffectPreset>(*(_rhs.get()));
	return pInstance;
}
