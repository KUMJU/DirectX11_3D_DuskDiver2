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
		ENUM_END
	};

public:
	struct EVENT_INFO {
		ECAMERA_FUNCTION eCamFunc;
		_float fDuration;
		shared_ptr<CGameObject> pTarget;
		_float3 vStart;
		_float3 vEnd;
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

private:


public:
	list<EVENT_INFO> m_EventList;

};

