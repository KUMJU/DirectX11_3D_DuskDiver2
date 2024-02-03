#pragma once

#include "Client_Defines.h"
#include "Camera.h"
 
class CEventCamera final : public CCamera
{
public:
	enum ECAMERA_FUNCTION
	{
		FUNC_ZOOMIN,
		FUNC_ZOOMOUT,
		FUNC_FOCUSING,
		FUNC_MOVING,
		FUNC_SHAKING,
		FUNC_IDLE,
		ENUM_END
	};

public:
	struct EVENT_INFO {
		ECAMERA_FUNCTION eCamFunc;
		_float fDuration;
		_float fCurrentDuration;
		shared_ptr<CTransform> pTarget;
		_float3 vStart;
		_float3 vEnd;
		_float3 fDistance;
		_float fSpeed;
	};

public:
	CEventCamera();
	virtual ~CEventCamera();

private:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:

	void AddPreset(const wstring& _strName, vector<EVENT_INFO> _info);
	void ResetPreset();

	void StartEvent(const wstring& _strEventName);
	void EndEvent();

	void SetInfoList(list<EVENT_INFO> _info);
	//데이터 파일로 프리셋 저장해두고 파싱하기 ?
	void LoadPreset(const _tchar* _szName);


public:
	void FocusObject();


private:

	void EventInitialize();

private:

	_uint m_iCurrentIdx = 0;
	_uint m_iMaxSize = 0;


	void SetPrevEvent();

private:

	_bool m_bEventStart = false;

private:
	vector<EVENT_INFO> m_EventList;
	map<wstring, vector<EVENT_INFO>> m_PresetList;

public:

	static shared_ptr<CEventCamera> Create();


};

