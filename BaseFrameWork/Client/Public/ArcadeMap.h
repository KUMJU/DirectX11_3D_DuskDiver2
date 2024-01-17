#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CArcadeMap : public CLevel
{
public:
	CArcadeMap();
	virtual ~CArcadeMap();

public:
	virtual void PreInit();
	virtual HRESULT Initialize();
	virtual void Tick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	HRESULT ReadyLayerBackGround(const wstring& _strLayerTag);
	HRESULT ReadyLayerCamera(const wstring& _strLayerTag);
	HRESULT ReadyLayerPlayer(const wstring& _strLayerTag);
	HRESULT ReadyLayerMonster(const wstring& _strLayerTag);
	HRESULT ReadyLayerUI(const wstring& _strLayerTag);
	HRESULT ReadyLight();

public:
	static shared_ptr<CArcadeMap> Create();
};

END