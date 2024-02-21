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

//common
public:
	shared_ptr<class CUI> FindUI(wstring& _strFindKey);


//Specific Function
public:
	void SetPlayerHP(_int _iHP);

public:
	void AddBurstGauge();
	void SetBurstGauge(_float _fCurrentBurst);

public:
	void SetSkillGauge(_int _iSkillGauge, _bool _isIncrease);
	void SetTimeRatio(_float _fTimeRate);

public:
	void SetBurstSkillGauge(_float _fCurrentBurst);

private:

	map<wstring, shared_ptr<class CUI>> m_UIList;

};

END