#include "Timer.h"

CTimer::CTimer()
	:m_fTimeDelta(0.f)
{
}

HRESULT CTimer::Initialize(void)
{
	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceCounter(&m_FixTime);
	QueryPerformanceCounter(&m_LastTime);
	QueryPerformanceFrequency(&m_CpuTick);

	return S_OK;
}

_float CTimer::ComputeTimeDelta(void)
{
	QueryPerformanceCounter(&m_FrameTime);

	if (m_FrameTime.QuadPart - m_FixTime.QuadPart >= m_CpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CpuTick);
		m_FixTime = m_FrameTime;
	}

	m_fTimeDelta = (float(m_FrameTime.QuadPart - m_LastTime.QuadPart) / m_CpuTick.QuadPart) * m_fTimeOffset;

	m_LastTime = m_FrameTime;

	return m_fTimeDelta;
}

shared_ptr<CTimer> CTimer::Create(void)
{
	shared_ptr<CTimer> pInstance = make_shared<CTimer>();

	if (FAILED(pInstance->Initialize())) {
		MSG_BOX("Timer Create Failed");
	}

	return pInstance;
}
