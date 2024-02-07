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
	//이펙트 갯수 + 루프여부 + 전체 duration
	//이펙트 타입 + 필요한 인자들저장

	for (auto& iter : m_Effects) {
		
		//iter->ParsingData

	}



}

void CEffectPreset::ResetEffect()
{

	//Effect 안에 오버라이드 함수 만든다음에 순회하면서 돌려주기 

}

shared_ptr<CEffectPreset> CEffectPreset::Create()
{
	shared_ptr<CEffectPreset> pInstance = make_shared<CEffectPreset>();

	if (FAILED(pInstance->Initialize()))
		MSG_BOX("Failed to Create : CEffectPreset");

	return pInstance;
}
