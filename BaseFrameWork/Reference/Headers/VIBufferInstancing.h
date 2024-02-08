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
		_float			fDuration;

		_float3			vDirectionMin;
		_float3			vDirectionMax;
		_float3			vStartPointMin;
		_float3			vStartPointMax;

	}INSTANCE_DESC;


public:
	CVIBufferInstancing(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);
	virtual ~CVIBufferInstancing() = default;

public:
	virtual HRESULT Initialize(INSTANCE_DESC* _desc);
	virtual HRESULT Render();
	virtual HRESULT Bind_Buffers();

public:
	void TickDrop(_float _fTimeDelta);
	void TickSpread(_float _fTimeDelta);

	void TickInstance(_float _fTimeDelta);

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


public:
	void EffectInitSetting();
	void ResetEffect();

};

END