#include "pch.h"

#include "EffectPreset.h"
#include "Effect.h"

#include "EffectMesh.h"
#include "EffectParticle.h"

CEffectPreset::CEffectPreset()
{
}

CEffectPreset::CEffectPreset(const CEffectPreset& _rhs)
	:m_bLoop(_rhs.m_bLoop),
	m_vTotalDuration(_rhs.m_vTotalDuration)
{
	shared_ptr<CEffect> pCloneEffect = nullptr;

	for (auto& pEffect : _rhs.m_Effects) {

		if (CEffect::EFFECT_TYPE::TYPE_PARTICLE == pEffect->GetEffectType()) {
			pCloneEffect = dynamic_pointer_cast<CEffectParticle>(pEffect)->CloneEffect();

		}
		else if (CEffect::EFFECT_TYPE::TYPE_TEXTURE == pEffect->GetEffectType()) {


		}
		else if (CEffect::EFFECT_TYPE::TYPE_MESH == pEffect->GetEffectType()) {
			
			pCloneEffect = dynamic_pointer_cast<CEffectMesh>(pEffect)->CloneEffect();
		}

		m_Effects.push_back(pCloneEffect);
	}
}

HRESULT CEffectPreset::Initialize(_float _fDuration, _bool _bLoop)
{
	m_vTotalDuration = _fDuration;
	m_bLoop = _bLoop;


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

	for (auto& iter : m_Effects) {
		

		if (m_pOwnerTransform) {
			iter->Tick(_fTimeDelta, m_pOwnerTransform->GetWorldMatrix());

		}else{
			iter->Tick(_fTimeDelta);

		}

	}


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

void CEffectPreset::SetEffectPosition(_vector _vPos)
{
	for (auto& pEffect : m_Effects) {
		pEffect->SetPosition(_vPos);
	}
}

void CEffectPreset::SetEffectRotation(_float fX, _float _fY, _float _fZ)
{
	for (auto& pEffect : m_Effects) {
		pEffect->SetRotation(fX, _fY, _fZ);
	}

}


void CEffectPreset::SetBillboard(_bool _bBillboard)
{
	for (auto& pEffect : m_Effects) {
		pEffect->SetBillboard(_bBillboard);
	}
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

void CEffectPreset::ResetEffect()
{
	for (auto& iter : m_Effects) {
		iter->ResetEffect();
	}
}

void CEffectPreset::DeleteAll()
{
	m_IsEnabled = false;
	m_Effects.clear();
}

shared_ptr<CEffectPreset> CEffectPreset::Create(_float _fDuration, _bool _bLoop)
{
	shared_ptr<CEffectPreset> pInstance = make_shared<CEffectPreset>();

	if (FAILED(pInstance->Initialize(_fDuration, _bLoop)))
		MSG_BOX("Failed to Create : CEffectPreset");

	return pInstance;
}

shared_ptr<CEffectPreset> CEffectPreset::Clone(shared_ptr<CEffectPreset> _rhs)
{
	shared_ptr<CEffectPreset> pInstance = make_shared<CEffectPreset>(*(_rhs.get()));
	return pInstance;
}
