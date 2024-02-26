#include "pch.h"
#include "UIMgr.h"
#include "UI.h"

#include "UIPlrHPBar.h"
#include "UIBurstGaugeBar.h"
#include "UISkillBar.h"
#include "UIBurstSkillGauge.h"
#include "UIDialog.h"
#include "UIMiniquest.h"
#include "UIMapTitle.h"
#include "UIQuest.h"

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

void CUIMgr::SetEnable(wstring& _strFindKey, _bool _bEnable)
{
	shared_ptr<CUI> pUI =  FindUI(_strFindKey);

	if (pUI) {
		pUI->SetEnable(_bEnable);
	}

}

void CUIMgr::CloseAllUI()
{
	for (auto& iter : m_UIList) {

		if (iter.first != TEXT("UI_ScreenEffect")) {
			iter.second->SetEnable(false);
		}
	}

}

void CUIMgr::HUDOn()
{
	shared_ptr<CUI> pUI;
	wstring strUIName = TEXT("UI_PlayerHP");

	FindUI(strUIName)->SetEnable(true);

	strUIName = TEXT("UI_Quest");
	FindUI(strUIName)->SetEnable(true);

	strUIName = TEXT("UI_SkillBar");
	FindUI(strUIName)->SetEnable(true);

	strUIName = TEXT("UI_BurstBar");
	FindUI(strUIName)->SetEnable(true);

	strUIName = TEXT("UI_HUDBackground1");
	FindUI(strUIName)->SetEnable(true);

	strUIName = TEXT("UI_HUDBackground2");
	FindUI(strUIName)->SetEnable(true);

}

void CUIMgr::CloseUI(wstring _strUIKey)
{
	shared_ptr<CUI> pUI = FindUI(_strUIKey);

	if (pUI) {
		pUI->SetEnable(false);
	}


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

void CUIMgr::SetBurstMode()
{
	wstring FindKey = TEXT("UI_BurstSkillBar");
	shared_ptr<CUI> pSkillBar = FindUI(FindKey);

	if (pSkillBar) {
		pSkillBar->SetEnable(true);
	}

	wstring FindKey2 = TEXT("UI_BurstBar");
	shared_ptr<CUI> pBurstBar = FindUI(FindKey2);

	if (pBurstBar) {
		dynamic_pointer_cast<CUIBurstGaugeBar>(pBurstBar)->SetBurstMode(true);
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

void CUIMgr::StartDialog(const wstring& _strDialogKey)
{
	wstring FindKey = TEXT("UI_Dialog");
	shared_ptr<CUI> pDialog = FindUI(FindKey);

	if (pDialog) {
		dynamic_pointer_cast<CUIDialog>(pDialog)->StartDialog(_strDialogKey);
	}
}

void CUIMgr::StartMoleMinigame()
{
	wstring FindKey = TEXT("UI_Miniquest");
	shared_ptr<CUI> pMiniquest = FindUI(FindKey);

	if (pMiniquest) {
		dynamic_pointer_cast<CUIMiniquest>(pMiniquest)->StartMoleGame();
	}

}

void CUIMgr::StartCoinQuest()
{
	wstring FindKey = TEXT("UI_Miniquest");
	shared_ptr<CUI> pMiniquest = FindUI(FindKey);

	if (pMiniquest) {
		dynamic_pointer_cast<CUIMiniquest>(pMiniquest)->StartCoinQuest();
	}
}

void CUIMgr::SetMiniQuestSuccessNumber(_int _iSucNum)
{
	wstring FindKey = TEXT("UI_Miniquest");
	shared_ptr<CUI> pMiniquest = FindUI(FindKey);

	if (pMiniquest) {
		dynamic_pointer_cast<CUIMiniquest>(pMiniquest)->SetSuccessNum(_iSucNum);
	}
}

void CUIMgr::AddMiniQuestSuccessNumber()
{
	wstring FindKey = TEXT("UI_Miniquest");
	shared_ptr<CUI> pMiniquest = FindUI(FindKey);

	if (pMiniquest) {
		dynamic_pointer_cast<CUIMiniquest>(pMiniquest)->AddSuccessNum();
	}

}

void CUIMgr::StartScreenEffect(CUIScreenEffect::EFFECTTYPE _eEffectType)
{
	wstring FindKey = TEXT("UI_ScreenEffect");
	shared_ptr<CUI> pScreenEffect = FindUI(FindKey);

	if (pScreenEffect) {
		dynamic_pointer_cast<CUIScreenEffect>(pScreenEffect)->SwitchScreenEffect(_eEffectType);
	}



}

void CUIMgr::StartQuest(const wstring& _strQuestInfo)
{

	wstring FindKey = TEXT("UI_QeustTitle");
	shared_ptr<CUI> pQuestTitle = FindUI(FindKey);

	if (pQuestTitle) {
		dynamic_pointer_cast<CUIMapTitle>(pQuestTitle)->StartQuest(_strQuestInfo);
		SetQuestDesc(_strQuestInfo);
	}

}

void CUIMgr::SetQuestDesc(const wstring& _strQuestInfo)
{
	wstring FindKey = TEXT("UI_Quest");
	shared_ptr<CUI> pQuest = FindUI(FindKey);

	if (pQuest) {
		dynamic_pointer_cast<CUIQuest>(pQuest)->SetQuestDesc(_strQuestInfo);
	}
}
