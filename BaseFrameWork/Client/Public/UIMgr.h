#pragma once

#include "Client_Defines.h"

BEGIN(Client)

class CUIMgr
{
	DECLARE_SINGLETON(CUIMgr)

private:
	CUIMgr();
	~CUIMgr() = default;
	 
public:
	void AddUI(const wstring& _strUIKey , shared_ptr<class CUI> _pUI);

public:
	shared_ptr<class CUI> FindUI(wstring& _strFindKey);

private:

	map<wstring, shared_ptr<class CUI>> m_UIList;

};

END