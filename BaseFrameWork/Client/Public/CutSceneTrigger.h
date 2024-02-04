#pragma once
#include "EventTrigger.h"

class CCutSceneTrigger : public CEventTrigger
{
public:
	CCutSceneTrigger();
	virtual ~CCutSceneTrigger() = default;

public:
	virtual HRESULT Initialize(_float3 _vTriggerPos);
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;


public:

	static shared_ptr<CCutSceneTrigger> Create(_float3 _vTriggerPos);


};

//컷씬에서 필요할 수 있는거 
//카메라 ............ 
