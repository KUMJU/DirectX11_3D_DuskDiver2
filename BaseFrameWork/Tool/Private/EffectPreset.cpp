#include "pch.h"
#include "EffectPreset.h"
#include "Effect.h"

CEffectPreset::CEffectPreset()
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
