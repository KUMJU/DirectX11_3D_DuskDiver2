#pragma once

#include "Tool_Defines.h"

BEGIN(Tool)

class CUIMgr
{
	DECLARE_SINGLETON(CUIMgr)

private:
	CUIMgr();
	~CUIMgr() = default;
	 
public:
	void AddUI(const wstring& _strUIKey , shared_ptr<class CUI> _pUI);

//common
public:
	shared_ptr<class CUI> FindUI(wstring& _strFindKey);


//Specific Function
public:
	void SetPlayerHP(_int _iHP);



private:

	map<wstring, shared_ptr<class CUI>> m_UIList;

};

END