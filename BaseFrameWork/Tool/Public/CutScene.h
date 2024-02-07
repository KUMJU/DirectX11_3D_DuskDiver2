#pragma once

#include "GameObject.h"
#include "Tool_Defines.h"

BEGIN(Tool)


class CCutScene : public CGameObject
{
public:
	CCutScene();
	virtual ~CCutScene() = default;

public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	_bool GetIsEnd() { return m_IsEnd; }

public:
	void PlayEvent();


private:
	virtual void AddCameraEvent() {};

	_bool m_IsEnd = false;
	_bool m_IsStart = false;


};

END