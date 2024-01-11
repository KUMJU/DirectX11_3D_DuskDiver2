#include "pch.h"
#include "UIMgr.h"
#include "UI.h"

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
