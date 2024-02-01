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

public:
	void SetTimeOffset(_float _fOffset) { m_fTimeOffset = _fOffset; }

private:
	LARGE_INTEGER			m_FrameTime;
	LARGE_INTEGER			m_FixTime;
	LARGE_INTEGER			m_LastTime;
	LARGE_INTEGER			m_CpuTick;

	_float					m_fTimeDelta;
	_float					m_fTimeOffset = 1.f;

public:

	static shared_ptr<CTimer> Create(void);

};

END