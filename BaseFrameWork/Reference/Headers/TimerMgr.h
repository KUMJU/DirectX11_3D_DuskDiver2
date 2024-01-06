#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CTimerMgr
{
public:
	CTimerMgr();
	~CTimerMgr() = default;

public:
	_float ComputeTimeDelta(const wstring& _strTimerTag);

public:
	HRESULT AddTimer(const wstring& _strTimerTag);

private:
	shared_ptr<class CTimer> FindTimer(const wstring& _strTimerTag);

private:
	map<const wstring, shared_ptr<class CTimer>> m_Timers;

public:
	static shared_ptr<CTimerMgr> Create();

};

END