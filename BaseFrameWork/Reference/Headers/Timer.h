#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CTimer
{
public:
	CTimer();
	~CTimer() = default;

public:
	HRESULT Initialize(void);
	_float ComputeTimeDelta(void);

private:
	LARGE_INTEGER			m_FrameTime;
	LARGE_INTEGER			m_FixTime;
	LARGE_INTEGER			m_LastTime;
	LARGE_INTEGER			m_CpuTick;

	_float					m_fTimeDelta;

public:

	static shared_ptr<CTimer> Create(void);

};

END