#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBufferInstancing : public CVIBuffer
{
public:
	typedef struct
	{
		_float3			vPivot;
		_float3			vCenter;
		_float3			vRange;
		_float2			vSize;
		_float2			vSpeed;
		_float2			vLifeTime;
		_bool			isLoop;
		_float4			vColor;

		_float2			vDuration;

		_float3			vDirectionMin;
		_float3			vDirectionMax;
		_float3			vStartPointMin;
		_float3			vStartPointMax;

		_bool			bGlow;
		_float4			vGlowColor;

	}INSTANCE_DESC;


public:
	CVIBufferInstancing(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);
	virtual ~CVIBufferInstancing() = default;

public:
	virtual HRESULT Initialize(INSTANCE_DESC* _desc);
	virtual HRESULT Render();
	virtual HRESULT Bind_Buffers();

//기본 Tick
public:
	_bool TickInstance(_float _fTimeDelta); //시작점과 방향을 지정할 수 있는 function


//파티클 추가 기능 
protected:
	void ColorLerp() {};
	void AlphaLerp() {};

public:

	void ResetInstance();

protected:
	wrl::ComPtr<ID3D11Buffer> m_pVBInstance = { nullptr };
	_uint m_iInstanceStride = { 0 };
	_uint m_iNumInstance = { 0 };
	_uint m_iIndexCountPerInstance = { 0 };

	random_device m_RandomDevice;
	mt19937_64 m_RandomNumber;
	INSTANCE_DESC m_InstanceData = {};

	_float m_fTimeAcc = { 0.f };
	_bool m_isFinished = { false };


protected:
	vector<_float> m_Speeds;
	vector<_float> m_LifeTimes;

	//각 인스턴스의 초기 시작 위치+이동 방향 저장
	vector<_float3> m_InitPositions;
	vector<_float3> m_Directions;


//For Parsing
protected:
	INSTANCE_DESC* GetParticleDesc() { return &m_InstanceData; }


public:
	void EffectInitSetting();

};

END