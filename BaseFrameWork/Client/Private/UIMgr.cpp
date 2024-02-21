#include "pch.h"
#include "UIMgr.h"
#include "UI.h"

#include "UIPlrHPBar.h"
#include "UIBurstGaugeBar.h"
#include "UISkillBar.h"
#include "UIBurstSkillGauge.h"

IMPLEMENT_SINGLETON(CUIMgr)

CUIMgr::CUIMgr()
{
}

void CUIMgr::AddUI(const wstring& _strUIKey, shared_ptr<class CUI> _pUI)
{
	auto iter = m_UIList.find(_strUIKey);

	if (m_UIList.end() == iter)
		m_UIList.emplace(_strUIKey, _pUI);
}

shared_ptr<class CUI> CUIMgr::FindUI(wstring& _strFindKey)
{
	auto iter = m_UIList.find(_strFindKey);

	if (m_UIList.end() != iter)
		return (*iter).second;

	return nullptr;
}

void CUIMgr::SetPlayerHP(_int _iHP)
{
	wstring FindKey = TEXT("UI_PlayerHP");
	shared_ptr<CUI> pHPBar = FindUI(FindKey);
	
	if (pHPBar) {
		dynamic_pointer_cast<CUIPlrHPBar>(pHPBar)->SetHP(_iHP);
	}
}

void CUIMgr::AddBurstGauge()
{
	wstring FindKey = TEXT("UI_BurstBar");
	shared_ptr<CUI> pBurstBar = FindUI(FindKey);

	if (pBurstBar) {
		dynamic_pointer_cast<CUIBurstGaugeBar>(pBurstBar)->AddBurstGauge();
	}

}

void CUIMgr::SetBurstGauge(_float _fCurrentBurst)
{
	wstring FindKey = TEXT("UI_BurstBar");
	shared_ptr<CUI> pBurstBar = FindUI(FindKey);

	if (pBurstBar) {
		dynamic_pointer_cast<CUIBurstGaugeBar>(pBurstBar)->SetCurrentBurstGauge(_fCurrentBurst);	
	}
	

}

void CUIMgr::SetSkillGauge(_int _iSkillGauge, _bool _isIncrease)
{
	wstring FindKey = TEXT("UI_SkillBar");
	shared_ptr<CUI> pSkillBar = FindUI(FindKey);

	if (pSkillBar) {
		dynamic_pointer_cast<CUISkillBar>(pSkillBar)->SetSkillGauge(_iSkillGauge, _isIncrease);
	}

}

void CUIMgr::SetTimeRatio(_float _fTimeRate)
{
	wstring FindKey = TEXT("UI_SkillBar");
	shared_ptr<CUI> pSkillBar = FindUI(FindKey);

	if (pSkillBar) {
		dynamic_pointer_cast<CUISkillBar>(pSkillBar)->SetCurrentGauging(_fTimeRate);
	}


}

void CUIMgr::SetBurstSkillGauge(_float _fCurrentBurst)
{

	wstring FindKey = TEXT("UI_BurstSkillBar");
	shared_ptr<CUI> pSkillBar = FindUI(FindKey);

	if (pSkillBar) {
		dynamic_pointer_cast<CUIBurstSkillGauge>(pSkillBar)->SetBurstSkillGauge(_fCurrentBurst);
	}
}
