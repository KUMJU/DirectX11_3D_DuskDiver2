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
	m_fAccTime += _fTimeDelta;

	if (m_fAccTime >= m_vTotalDuration) {
		//���� ����Ʈ�϶�
		if (m_bLoop) {
			ResetEffect();
			m_fAccTime = 0.f;
		}
		else {
			//��Ȱ��ȭ 
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

}

void CEffectPreset::PlayEffect()
{
	for (auto& iter : m_Effects) {

		iter->ResetEffect();
		iter->SetEnable(true);
	}
}

void CEffectPreset::ParsingEffect()
{
	//����Ʈ ���� + �������� + ��ü duration
	//����Ʈ Ÿ�� + �ʿ��� ���ڵ�����

	for (auto& iter : m_Effects) {
		
		//iter->ParsingData

	}



}

void CEffectPreset::ResetEffect()
{




	//Effect �ȿ� �������̵� �Լ� ��������� ��ȸ�ϸ鼭 �����ֱ� 

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
