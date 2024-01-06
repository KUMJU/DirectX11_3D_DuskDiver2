#include "TimerMgr.h"
#include "Timer.h"

CTimerMgr::CTimerMgr()
{
}

_float CTimerMgr::ComputeTimeDelta(const wstring& _strTimerTag)
{
    shared_ptr<CTimer> pTimer = FindTimer(_strTimerTag);

    if (!pTimer)
        return 0.0f;

    return pTimer->ComputeTimeDelta();
}

HRESULT CTimerMgr::AddTimer(const wstring& _strTimerTag)
{
    shared_ptr<CTimer> pTimer = FindTimer(_strTimerTag);

    if (pTimer)
        return E_FAIL;

    pTimer = CTimer::Create();

    if (!pTimer)
        return E_FAIL;

    m_Timers.insert({ _strTimerTag , pTimer });

    return S_OK;
}

shared_ptr<class CTimer> CTimerMgr::FindTimer(const wstring& _strTimerTag)
{
    auto iter = m_Timers.find(_strTimerTag);

    if (m_Timers.end() == iter)
        return nullptr;

    return iter->second;
}

shared_ptr<CTimerMgr> CTimerMgr::Create()
{
    return make_shared<CTimerMgr>();
}
